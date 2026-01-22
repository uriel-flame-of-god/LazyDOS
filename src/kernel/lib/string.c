// src/kernel/lib/string.c
#include <stddef.h>

char* strcpy(char* restrict dst, const char* restrict src)
{
    char *d = dst;
    while ((*d++ = *src++));
    return dst;
}

int strcmp(const char* a, const char* b)
{
    while (*a && *a == *b) { ++a; ++b; }
    return *(unsigned char*)a - *(unsigned char*)b;
}


size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = ptr;
    while (num--) {
        *p++ = (unsigned char)value;
    }
    return ptr;
}

void* memcpy(void* dest, const void* src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

void* memmove(void* dest, const void* src, size_t n) {
    // Convert pointers to char pointers for byte-wise copying
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;

    // Check for overlap and copy accordingly
    if (d < s) {
        // Non-overlapping or destination is before source
        while (n--) {
            *d++ = *s++;
        }
    } else if (d > s) {
        // Overlapping case: copy from the end to avoid corruption
        d += n;
        s += n;
        while (n--) {
            *(--d) = *(--s);
        }
    }

    // Return the destination pointer
    return dest;
}

int strncmp(const char* a, const char* b, size_t n)
{
    while (n-- && *a && *a == *b) { ++a; ++b; }
    if (n == (size_t)-1) return 0;
    return *(unsigned char*)a - *(unsigned char*)b;
}