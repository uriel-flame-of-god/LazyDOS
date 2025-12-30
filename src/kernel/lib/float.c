#include "../lib/float.h"
#include "../lib/string.h"
#include "../lib/int.h"
#include <stdbool.h>

/* ---------- helpers ---------- */
static inline int32_t clamp_shift(int32_t s) { return (s < -150) ? -150 : (s > 150) ? 150 : s; }

static uint32_t u32_div_u32(uint32_t num, uint32_t den)   /* bit-by-bit, 32→32 */
{
    if (den == 0) return 0xFFFFFFFFU;
    uint32_t quo = 0;
    for (int bit = 31; bit >= 0; --bit) {
        uint32_t sh = 1U << bit;
        if (num >= (den << bit)) { num -= den << bit; quo |= sh; }
    }
    return quo;
}

/* ---------- add / sub ---------- */
float32 float32_add(float32 a, float32 b)
{
    union float32_bits ua = { .f = a }, ub = { .f = b };
    uint32_t sa = ua.u & 0x80000000U, sb = ub.u & 0x80000000U;
    uint32_t ea = (ua.u >> 23) & 0xFFU, eb = (ub.u >> 23) & 0xFFU;
    uint32_t ma = (ua.u & 0x007FFFFFU) | ((ea == 0) ? 0 : 0x00800000U);
    uint32_t mb = (ub.u & 0x007FFFFFU) | ((eb == 0) ? 0 : 0x00800000U);

    if (ea == 0xFFU || eb == 0xFFU) return (ea == 0xFFU) ? a : b; /* NaN / inf */
    if (sa != sb) return float32_sub(a, -b);                      /* different sign → sub */

    /* align mantissas */
    int32_t shift = (int32_t)ea - (int32_t)eb;
    if (shift > 0) mb = (shift < 24) ? (mb >> shift) : 0;
    if (shift < 0) { shift = -shift; ma = (shift < 24) ? (ma >> shift) : 0; }
    uint32_t res_m = ma + mb;
    uint32_t res_e = (shift > 0) ? ea : eb;

    /* carry? */
    if (res_m & 0x01000000U) { res_m >>= 1; res_e += 1; }
    if (res_e >= 0xFFU) { res_e = 0xFFU; res_m = 0; } /* overflow → inf */

    union float32_bits ur = { .u = (sa | (res_e << 23) | (res_m & 0x007FFFFFU)) };
    return ur.f;
}

float32 float32_sub(float32 a, float32 b) { return float32_add(a, -b); }
/* ---------- 32×32→HI/LO multiply (bit-by-bit, no 64-bit intermediate) ---------- */
static void u32_x_u32(uint32_t a, uint32_t b, uint32_t *lo, uint32_t *hi)
{
    uint32_t l = 0, h = 0;
    for (int bit = 0; bit < 32; ++bit) {
        if (b & (1U << bit)) {
            uint32_t addl = a << bit;
            uint32_t addh = (bit == 0) ? 0 : (a >> (32 - bit));
            l += addl;
            if (l < addl) h++;          /* carry */
            h += addh;
        }
    }
    *lo = l; *hi = h;
}

/* ---------- multiply (rewritten, 64-bit free) ---------- */
float32 float32_mul(float32 a, float32 b)
{
    union float32_bits ua = { .f = a }, ub = { .f = b };
    uint32_t sa = ua.u >> 31, sb = ub.u >> 31;
    int32_t  ea = (int32_t)((ua.u >> 23) & 0xFFU) - 127;
    int32_t  eb = (int32_t)((ub.u >> 23) & 0xFFU) - 127;
    uint32_t ma = (ua.u & 0x007FFFFFU) | 0x00800000U;
    uint32_t mb = (ub.u & 0x007FFFFFU) | 0x00800000U;

    if (ea == 128 || eb == 128) return (ea == 128) ? a : b; /* NaN / inf */

    uint32_t lo, hi;
    u32_x_u32(ma, mb, &lo, &hi);              /* 48-bit product in hi:lo */
    uint32_t prod = (hi << 8) | (lo >> 24);   /* keep top 24 bits */
    int32_t  exp  = ea + eb + 23;
    if (prod & 0x01000000U) { prod >>= 1; exp++; } /* carry */
    if (exp >= 127)  { exp = 127;  prod = 0; }     /* overflow → inf */
    if (exp < -126)  { exp = -126; }                 /* underflow → denorm */

    uint32_t res_e = (uint32_t)(exp + 127);
    union float32_bits ur = { .u = ((sa ^ sb) << 31) | (res_e << 23) | (prod & 0x007FFFFFU) };
    return ur.f;
}
/* ---------- divide ---------- */
float32 float32_div(float32 a, float32 b)
{
    union float32_bits ua = { .f = a }, ub = { .f = b };
    uint32_t sa = ua.u >> 31, sb = ub.u >> 31;
    int32_t  ea = (int32_t)((ua.u >> 23) & 0xFFU) - 127;
    int32_t  eb = (int32_t)((ub.u >> 23) & 0xFFU) - 127;
    uint32_t ma = (ua.u & 0x007FFFFFU) | 0x00800000U;
    uint32_t mb = (ub.u & 0x007FFFFFU) | 0x00800000U;

    if (eb == 128 || mb == 0) return (eb == 128) ? b : a; /* div-by-zero / NaN */
    uint32_t quo_m = u32_div_u32(ma << 8, mb);            /* 24Q8 → 24-bit quotient */
    int32_t  exp   = ea - eb;
    if (quo_m & 0x01000000U) { quo_m >>= 1; exp++; }      /* carry */
    exp = clamp_shift(exp);
    uint32_t res_e = (uint32_t)(exp + 127);
    union float32_bits ur = { .u = ((sa ^ sb) << 31) | (res_e << 23) | (quo_m & 0x007FFFFFU) };
    return ur.f;
}

/* ---------- fabs ---------- */
float32 float32_fabs(float32 x) { union float32_bits u = { .f = x }; u.u &= 0x7FFFFFFFU; return u.f; }

/* ---------- comparisons ---------- */
int float32_eq(float32 a, float32 b) { union float32_bits ua = { .f = a }, ub = { .f = b }; return ua.u == ub.u; }
int float32_lt(float32 a, float32 b) { return (a < b) && !float32_eq(a, b); }
int float32_le(float32 a, float32 b) { return (a < b) || float32_eq(a, b); }

/* ---------- floor / ceil ---------- */
float32 float32_floor(float32 x)
{
    union float32_bits u = { .f = x };
    int32_t e = (int32_t)((u.u >> 23) & 0xFFU) - 127;
    if (e < 0) return (x >= 0.0f) ? 0.0f : -1.0f;
    if (e >= 23) return x;                 /* integer part only */
    uint32_t mask = 0x007FFFFFU >> (e + 1);
    u.u &= ~mask;
    return u.f;
}
float32 float32_ceil(float32 x) { return -float32_floor(-x); }

/* ---------- fmod ---------- */
float32 float32_fmod(float32 x, float32 y)
{
    if (y == 0.0f) return x;
    uint32_t count = 0;
    while (x >= y && count++ < 100) x = float32_sub(x, y); /* lazy loop */
    return x;
}

/* ---------- sqrt (Newton, 4 iterations) ---------- */
float32 float32_sqrt(float32 x)
{
    if (x <= 0.0f) return 0.0f;
    union float32_bits u = { .f = x };
    /* first guess: 1.0f */
    float32 g = 1.0f;
    for (int i = 0; i < 4; ++i)
        g = float32_mul(0.5f, float32_add(g, float32_div(x, g)));
    return g;
}

/* ---------- string ←→ float32 ---------- */
float32 float32_from_string(const char *s, char **end)
{
    bool neg = false;
    if (*s == '-') { neg = true; ++s; }
    uint32_t intp = 0, fracp = 0, div = 1;
    const char *p = s;
    while (*p >= '0' && *p <= '9') intp = intp * 10 + (*p++ - '0');
    if (*p == '.') {
        ++p;
        while (*p >= '0' && *p <= '9') { fracp = fracp * 10 + (*p - '0'); div *= 10; ++p; }
    }
    if (end) *end = (char *)p;
    /* inside float32_from_string: remove / replace */
    float32 v = float32_add(int32_to_float(intp), float32_div(int32_to_float(fracp), int32_to_float(div)));
    return neg ? float32_sub(0.0f, v) : v;
}

void float32_to_string(float32 val, char *out, int max_len)
{
    if (max_len <= 0) return;
    bool neg = false;
    if (val < 0) { neg = true; val = -val; }
    uint32_t ip = (uint32_t)val;
    uint32_t fp = (uint32_t)((val - ip) * 1e6f + 0.5f);
    char tmp[24], *p = tmp;
    do { *p++ = '0' + ip % 10; ip /= 10; } while (ip);
    if (neg) *p++ = '-';
    *p-- = '\0';
    char *b = out;
    while (p >= tmp && b < out + max_len - 1) *b++ = *p--;
    *b++ = '.';
    uint32_t div = 100000;
    while (div && b < out + max_len - 1) { *b++ = '0' + fp / div; fp %= div; div /= 10; }
    *b = '\0';
}