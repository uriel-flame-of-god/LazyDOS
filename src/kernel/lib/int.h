#ifndef KERNEL_LIB_INT_H
#define KERNEL_LIB_INT_H

#include <stdint.h>

/* exact-width names */
typedef int8_t   int8;
typedef uint8_t  uint8;
typedef int16_t  int16;
typedef uint16_t uint16;
typedef int32_t  int32;
typedef uint32_t uint32;

/* ---------- unsigned helpers (base for signed) ---------- */
uint8  uint8_div (uint8 num, uint8 den);
uint8  uint8_mod (uint8 num, uint8 den);
uint16 uint16_div(uint16 num, uint16 den);
uint16 uint16_mod(uint16 num, uint16 den);
uint32 uint32_div(uint32 num, uint32 den);
uint32 uint32_mod(uint32 num, uint32 den);

/* ---------- signed helpers ---------- */
int8  int8_div (int8 num, int8 den);
int8  int8_mod (int8 num, int8 den);
int16 int16_div(int16 num, int16 den);
int16 int16_mod(int16 num, int16 den);
int32 int32_div(int32 num, int32 den);
int32 int32_mod(int32 num, int32 den);

/* ---------- saturated add (optional but handy) ---------- */
uint8 sadd8(uint8 a, uint8 b);
uint16 sadd16(uint16 a, uint16 b);
uint32 sadd32(uint32 a, uint32 b);

/* ---------- int → float (IEEE-754 single) ---------- */
float int32_to_float(int32 v);
float int16_to_float(int16 v);
float int8_to_float(int8 v);

#endif