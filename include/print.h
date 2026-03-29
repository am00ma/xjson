#pragma once

#include "concat.h"

// --------------- Primitives ---------------

Str print__char(Buffer* b, char* x) { return concat__char(b, *x); }
Str print__Str(Buffer* b, Str* x) { return concat__quoted_string(b, *x); }

Str print__bool(Buffer* b, bool* x) { return concat__bool(b, *x); }
Str print__u32(Buffer* b, u32* x) { return concat__unsigned_int(b, *x); }
Str print__u64(Buffer* b, u64* x) { return concat__unsigned_int(b, *x); }
Str print__i32(Buffer* b, i32* x) { return concat__signed_int(b, *x); }
Str print__i64(Buffer* b, i64* x) { return concat__signed_int(b, *x); }
Str print__f32(Buffer* b, f32* x) { return concat__double(b, *x); }
Str print__f64(Buffer* b, f64* x) { return concat__double(b, *x); }

// --------------- Array ---------------

#define print__array(type, b, arr)                                                                                     \
    {                                                                                                                  \
        concat__char(b, '[');                                                                                          \
        for (isize i = 0; i < (arr).len; i++)                                                                          \
        {                                                                                                              \
            print__##type(b, (arr).buf[i]);                                                                            \
            if (i != (arr).len - 1) { concat__string(b, _(", ")); }                                                    \
        }                                                                                                              \
        concat__char(b, ']');                                                                                          \
    }

// --------------- The Infamous X macros ---------------

#define X_PRINT(name, idx, type, ref, field, key, print_fn, parse_fn)                                                  \
    len += concat__quoted_string(b, _(key)).len;                                                                       \
    len += concat__char(b, ':').len;                                                                                   \
    len += print_fn(b, ref(x->field)).len;                                                                             \
    len += (idx == X_COUNT_##name - 1) ? 0 : concat__string(b, _(", ")).len;

#define X_DECLARE_PRINT(name, idx, type, ref, field, key, print_fn, parse_fn)                                          \
    SI Str print__##name(Buffer* b, name* x)                                                                           \
    {                                                                                                                  \
        isize len  = 0;                                                                                                \
        len       += concat__char(b, '{').len;                                                                         \
        X_TABLE_##name(idx, type, ref, field, key, print_fn, parse_fn);                                                \
        len += concat__char(b, '}').len;                                                                               \
        return BufToStr(b, len);                                                                                       \
    }
