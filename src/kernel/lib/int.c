#include "int.h"

/* ================== 8-bit unsigned ================== */
uint8 uint8_div(uint8 num, uint8 den)
{
    if (den == 0) return 0xFF;
    uint8 quo = 0;
    for (int bit = 7; bit >= 0; --bit) {
        uint8 shift = (uint8)1 << bit;
        if (num >= (den << bit)) { num -= den << bit; quo |= shift; }
    }
    return quo;
}
uint8 uint8_mod(uint8 num, uint8 den)
{
    uint8 q = uint8_div(num, den);
    return num - q * den;
}

/* ================== 16-bit unsigned ================== */
uint16 uint16_div(uint16 num, uint16 den)
{
    if (den == 0) return 0xFFFF;
    uint16 quo = 0;
    for (int bit = 15; bit >= 0; --bit) {
        uint16 shift = (uint16)1 << bit;
        if (num >= (den << bit)) { num -= den << bit; quo |= shift; }
    }
    return quo;
}
uint16 uint16_mod(uint16 num, uint16 den)
{
    uint16 q = uint16_div(num, den);
    return num - q * den;
}

/* ================== 32-bit unsigned (already shown) ================== */
uint32 uint32_div(uint32 num, uint32 den)
{
    if (den == 0) return 0xFFFFFFFF;
    uint32 quo = 0;
    for (int bit = 31; bit >= 0; --bit) {
        uint32 shift = (uint32)1 << bit;
        if (num >= (den << bit)) { num -= den << bit; quo |= shift; }
    }
    return quo;
}
uint32 uint32_mod(uint32 num, uint32 den)
{
    uint32 q = uint32_div(num, den);
    return num - q * den;
}

/* ================== signed helpers (build on unsigned) ================== */
/* ---- 32-bit only signed divide (no 64-bit casts) ---- */
#define SIGNED_DIV_BODY(bits, type, uTYPE, fnUdiv, fnUmod)          \
do {                                                                  \
    if (den == 0) return (type)((1u << (bits - 1)) - 1);            \
    uTYPE uns_num = (num < 0) ? (uTYPE)(-(uint32_t)num) : (uTYPE)num; \
    uTYPE uns_den = (den < 0) ? (uTYPE)(-(uint32_t)den) : (uTYPE)den; \
    uTYPE uns_quo = fnUdiv(uns_num, uns_den);                       \
    if (((uint32_t)num ^ (uint32_t)den) >> 31) uns_quo = -uns_quo;  \
    return (type)uns_quo;                                             \
} while (0)


int8 int8_div(int8 num, int8 den) { SIGNED_DIV_BODY(8, int8, uint8, uint8_div, uint8_mod); }
int8 int8_mod(int8 num, int8 den)
{
    int8 q = int8_div(num, den);
    return num - q * den;
}

int16 int16_div(int16 num, int16 den) { SIGNED_DIV_BODY(16, int16, uint16, uint16_div, uint16_mod); }
int16 int16_mod(int16 num, int16 den)
{
    int16 q = int16_div(num, den);
    return num - q * den;
}

int32 int32_div(int32 num, int32 den) { SIGNED_DIV_BODY(32, int32, uint32, uint32_div, uint32_mod); }
int32 int32_mod(int32 num, int32 den)
{
    int32 q = int32_div(num, den);
    return num - q * den;
}

/* ================== saturated add ================== */
uint8 sadd8(uint8 a, uint8 b)  { uint16 t = (uint16)a + b; return t > 0xFF ? 0xFF : (uint8)t; }
uint16 sadd16(uint16 a, uint16 b) { uint32 t = (uint32)a + b; return t > 0xFFFF ? 0xFFFF : (uint16)t; }
uint32 sadd32(uint32 a, uint32 b) { return a + b; } /* 32-bit wrap is natural */

/* ================== int → IEEE-754 single ================== */
static float u32_to_float(uint32 abs, int sign)
{
    union { float f; uint32 u; } u;
    if (abs == 0) { u.u = 0; return u.f; }
    int expo = 31;
    while ((abs >> expo) == 0) --expo;
    int shift = expo - 23;
    uint32 mant = (shift >= 0) ? (abs >> shift) : (abs << -shift);
    mant &= 0x007FFFFF;
    u.u = (sign ? 0x80000000 : 0) | ((uint32)(expo + 127) << 23) | mant;
    return u.f;
}

float int32_to_float(int32 v) { return u32_to_float((v < 0) ? -v : v, v < 0); }
float int16_to_float(int16 v) { return int32_to_float((int32)v); }
float int8_to_float(int8 v)   { return int32_to_float((int32)v); }

/* ---- deliberate stubs: if these are called the link fails ---- */
void __divdi3(void) { asm volatile ("ud2"); }
void __moddi3(void) { asm volatile ("ud2"); }
void __umuldi3(void){ asm volatile ("ud2"); }