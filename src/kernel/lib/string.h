#ifndef STRING_H
#define STRING_H 1
#pragma once

#include <stddef.h>

void* memmove(void* dest, const void* src, size_t n);
void* memcpy(void* dest, const void* src, size_t n);
void* memset(void* ptr, int value, size_t num);
size_t strlen(const char* str);
char* strcpy(char* restrict dst, const char* restrict src);
int   strcmp(const char* a, const char* b);
#endif /* STRING_H */