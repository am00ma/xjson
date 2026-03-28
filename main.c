#include "types.h"
#include "user.h"

#define print_str(dst) fprintf(stdout, "%.*s\n", (int)dst.len, dst.buf)

int main()
{

    Struct_A a = {
        .bool_a = true,
        .u64_a  = 10,
        .u64_b  = 20,
        .i32_a  = -30,
        .f64_a  = 3.14,
        .str_a  = _("hello"),
        .str_b  = _("hi"),
    };

    u64 u64s[3] = {};
    for (isize i = 0; i < 3; i++) u64s[i] = i;

    f32 f32s[3] = {};
    for (isize i = 0; i < 3; i++) f32s[i] = i;

    Struct_A arr_a[] = {a, a};

    Struct_B b = {
        .u64s_     = {.buf = u64s, .len = 3},
        .f32s_     = {.buf = f32s, .len = 3},
        .arr_a     = {.buf = arr_a, .len = 2},
        .struct_a_ = a,
    };

    char   buf[1024] = {};
    Buffer p         = {
                .buf = buf,
                .len = 1024,
                .pos = 0,
                .err = 0,
    };

    Str dst_a = print__Struct_A(&p, &a);
    print_str(dst_a);

    Str dst_b = print__Struct_B(&p, &b);
    print_str(dst_b);
}
