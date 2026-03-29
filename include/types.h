#pragma once

/* ---------------------------------------------------------------------------
 *  Prerequisites
 * ------------------------------------------------------------------------- */

// --------------- Common Includes ---------------
#include <stddef.h> // ptrdiff_t
#include <stdint.h> // uint.., int..
#include <stdio.h>  // fprintf, stderr, ..
#include <string.h> // memset, memcpy

#include <inttypes.h> // strtoimax
#include <stdlib.h>   // strtoul, strtod

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

#define countof(a)  (isize)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#define SI static inline

#define RANGE(i, n) for (isize i = 0; i < (n); i++)

// clang-format off
#define maximum(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define minimum(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
// clang-format on

// --------------- String basics ---------------
typedef struct
{
    char* buf;
    isize len;

} Str;

#define _(s)  (Str){.buf = s, .len = sizeof(s) - 1}
#define __(s) (int)s.len, s.buf

static inline bool str_equal(Str s1, Str s2)
{
    if (s1.len != s2.len) { return false; }
    if (s1.len == 0) { return true; }
    return !memcmp(s1.buf, s2.buf, s1.len);
}

/* ---------------------------------------------------------------------------
 *  Meat
 * ------------------------------------------------------------------------- */

// --------------- Parser ---------------
typedef struct
{

    char* buf;
    isize len;

    isize pos;
    int   err;

    void* ctx;

} Buffer;

// --------------- Helpers ---------------
// clang-format off

#define MAX_LEN_INTEGER 32
#define MAX_LEN_DOUBLE  32

#define BufferAt(p, len)                   (p->buf[p->pos + (len)])
#define BufferLocal(p)                     (Buffer){.buf = &p->buf[p->pos], .pos = 0, .len = (p->len - (p->pos)), .err = 0}
#define BufferStr(p, start)                (Str){.buf = &p->buf[(start)], .len = (p->pos - (start))}
#define BufferErrorIf(cond, val)           if ((cond)) { p->err = -1; return val; }
#define BufferCheckCap(cap, val)           if ((p->pos + cap > p->len)) { p->err = -1; return val; }
#define BufferSafe(p, expr, start)         expr; if (p->err)  { return BufferStr(p, start); }
#define BufferSafeConsume(p, bb, expr)     expr; if (bb->err) { p->err = bb->err; return (Str){}; }

#define is_sign(c)   ((c == '-') || (c == '+'))
#define is_exp(c)    ((c == 'e') || (c == 'E'))
#define is_digit(c)  ((c >= '0') && (c <= '9'))
#define is_hex(c)    (((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) || is_digit(c))
#define is_ws(c)     ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
#define is_escape(c) ((c == '"') || (c == '\\') || (c == '/') || (c == 'b') || (c == 'f') || (c == 'n') || (c == 'r') || (c == 't'))

// clang-format on
