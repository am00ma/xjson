#pragma once

#include "concat.h"

Str print__char(Buffer* b, char x) { return concat__char(b, x); }
Str print__Str(Buffer* b, Str x) { return concat__quoted_string(b, x); }

Str print__bool(Buffer* b, bool x) { return concat__bool(b, x); }
Str print__u32(Buffer* b, u32 x) { return concat__unsigned_int(b, x); }
Str print__u64(Buffer* b, u64 x) { return concat__unsigned_int(b, x); }
Str print__i32(Buffer* b, i32 x) { return concat__signed_int(b, x); }
Str print__i64(Buffer* b, i64 x) { return concat__signed_int(b, x); }
Str print__f32(Buffer* b, f32 x) { return concat__double(b, x); }
Str print__f64(Buffer* b, f64 x) { return concat__double(b, x); }

#define print__array(type, p, arr, ref)                                                                                \
    {                                                                                                                  \
        concat__char(p, '[');                                                                                          \
        for (isize i = 0; i < (arr).len; i++)                                                                          \
        {                                                                                                              \
            print__##type(p, ref(arr).buf[i]);                                                                         \
            if (i != (arr).len - 1) { concat__string(p, _(", ")); }                                                    \
        }                                                                                                              \
        concat__char(p, ']');                                                                                          \
    }
