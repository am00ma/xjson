#pragma once
// clang-format off

// --------------- Includes ---------------
#include <stddef.h> // ptrdiff_t
#include <stdint.h> // uint.., int..
#include <stdio.h>  // fprintf, stderr, ..
#include <string.h> // memset, memcpy

#include <inttypes.h> // strtoimax
#include <stdlib.h>   // strtoul, strtod

// --------------- Aliases ---------------
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

// --------------- Essential macros ---------------
#define SI static inline

#define MAX_LEN_INTEGER 32
#define MAX_LEN_DOUBLE  32

#define RANGE(i, n) for (isize i = 0; i < (n); i++)

#define countof(a)  (isize)(sizeof(a) / sizeof(*(a)))
#define lengthof(s) (countof(s) - 1)

#define maximum(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define minimum(a,b)  ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })

#define p_newline()        fprintf(stderr, "\n")
#define p_inline(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#define p_line(fmt, ...)   fprintf(stderr, fmt "\n", ##__VA_ARGS__)

// --------------- Character checks ---------------

#define is_sign(c)   ((c == '-') || (c == '+'))
#define is_exp(c)    ((c == 'e') || (c == 'E'))
#define is_digit(c)  ((c >= '0') && (c <= '9'))
#define is_hex(c)    (((c >= 'a') && (c <= 'f')) || ((c >= 'A') && (c <= 'F')) || is_digit(c))
#define is_ws(c)     ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r'))
#define is_escape(c) ((c == '"') || (c == '\\') || (c == '/') || (c == 'b') || (c == 'f') || (c == 'n') || (c == 'r') || (c == 't'))

// --------------- String ---------------
typedef struct
{
    char* buf;
    isize len;

} Str;

#define NullStr (Str){}
#define _(s)    (Str){.buf = s, .len = sizeof(s) - 1}
#define __(s)   (int)s.len, s.buf

static inline bool str_equal(Str s1, Str s2)
{
    if (s1.len != s2.len) { return false; }
    if (s1.len == 0) { return true; }
    return !memcmp(s1.buf, s2.buf, s1.len);
}

// --------------- Buffer ---------------
typedef struct
{
    char* buf;
    isize len;
    isize pos;
    int   err;

} Buffer;

#define BufAt(p, len_) (p->buf[p->pos + (len_)])

#define BufFromStr(s)      (Buffer){.buf = s.buf, .len = s.len}
#define BufFromBuffer(b)   (Buffer){.buf = &b->buf[b->pos], .len = b->len - b->pos}
#define BufFromStack(b, n) char buf__##b[(n)] = {}; Buffer b = {.buf = buf__##b, .len = (n)};

#define BufToStr(p, len_) (Str){.buf = &p->buf[p->pos - (len_)], .len = (len_)}

#define BufCheckIf(p, cond, val) if ((cond)) { p->err = -1; return val; }
#define BufCheckCap(p, cap, val) BufCheckIf(p, (p->pos + (cap) > p->len), val)

// NOTE: !! expr should modify bb !! e.g. `BufSafeConcat(b, (&bb), concat__string((&bb), s))`
#define BufSafeConcat( b, bb, expr) ({(expr); if (bb->err) {b->err = bb->err; return BufToStr(bb, bb->pos);}})
#define BufSafeConsume(b, bb, expr) ({(expr); if (bb->err) {b->err = bb->err; return (Str){};}})

// clang-format on
