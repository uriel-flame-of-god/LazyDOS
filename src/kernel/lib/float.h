#ifndef KERNEL_LIB_FLOAT_H
#define KERNEL_LIB_FLOAT_H

#include <stdint.h>

/* ---- IEEE-754 single layout ---- */
typedef float float32;
union float32_bits { float32 f; uint32_t u; };

/* ---- basic arithmetic (no libgcc) ---- */
float32 float32_add(float32 a, float32 b);
float32 float32_sub(float32 a, float32 b);
float32 float32_mul(float32 a, float32 b);
float32 float32_div(float32 a, float32 b);

/* ---- unary ---- */
float32 float32_sqrt(float32 x);
float32 float32_fabs(float32 x);
float32 float32_floor(float32 x);
float32 float32_ceil (float32 x);
float32 float32_fmod(float32 x, float32 y);

/* ---- comparison ---- */
int   float32_eq(float32 a, float32 b);
int   float32_lt(float32 a, float32 b);
int   float32_le(float32 a, float32 b);

/* ---- string conversion ---- */
float32 float32_from_string(const char *s, char **end);
void    float32_to_string(float32 val, char *out, int max_len);

#endif