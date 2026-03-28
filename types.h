#pragma once

// --------------- Common Includes ---------------
#include <stdalign.h> // alignof
#include <stdarg.h>   // va_list, va_start, va_end
#include <stdbool.h>  // bool
#include <stddef.h>   // ptrdiff_t
#include <stdint.h>   // uint.., int..
#include <stdio.h>    // fprintf, stderr, ..
#include <stdlib.h>   // strtoul, strtod

#include <inttypes.h> // strtoimax
#include <string.h>   // memset, memcpy

// --------------- Primitives ---------------
typedef uintptr_t uptr;
typedef ptrdiff_t isize;
typedef size_t    usize;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;

typedef struct
{
    char* buf;
    isize len;

} Str;

#define _(s) (Str){.buf = s, .len = sizeof(s) - 1}

static inline bool str_equal(Str s1, Str s2)
{
    if (s1.len != s2.len) { return false; }
    if (s1.len == 0) { return true; }
    return !memcmp(s1.buf, s2.buf, s1.len);
}

// --------------- Parser ---------------
typedef struct
{
    char* buf;
    isize len;
    isize pos;
    int   err;

    int indent;

} Buffer;

typedef int (*parse_type_fn)(Buffer* b, void* val);
typedef int (*print_type_fn)(Buffer* b, void* val);

// --------------- helpers ---------------

#define SI static inline

#define MAX_LEN_INTEGER 32
#define MAX_LEN_DOUBLE  32

// clang-format off
#define BufferAt(len)            (p->buf[p->pos + (len)])
#define BufferErrorIf(cond, val) if ((cond)) { p->err = -1; return val; }
#define BufferStr(p, start)      (Str){.buf = &p->buf[start], .len = (p->pos - start)}

#define is_sign(c)   ((c == '-') || (c == '+'))
#define is_exp(c)    ((c == 'e') || (c == 'E'))
#define is_digit(c)  ((c >= '0') && (c <= '9'))
#define is_hex(c)    (((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) || is_digit(c))
#define is_ws(c)     ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
#define is_escape(c) ((c == '"') || (c == '\\') || (c == '/') || (c == 'b') || (c == 'f') || (c == 'n') || (c == 'r') || (c == 't'))
// clang-format on
