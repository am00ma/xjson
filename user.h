#pragma once

#include "types.h"

// --------------- Concrete examples ---------------
typedef struct
{

    bool bool_a;
    u64  u64_a;
    u64  u64_b;
    i32  i32_a;
    f64  f64_a;
    Str  str_a;
    Str  str_b;

} Struct_A;

// --------------- Nested examples ---------------

typedef struct
{

    struct
    {
        u64*  buf;
        isize len;
    } u64s_;

    struct
    {
        f32*  buf;
        isize len;
    } f32s_;

    struct
    {
        Struct_A* buf;
        isize     len;
    } arr_a;

    Struct_A struct_a_;

} Struct_B;
