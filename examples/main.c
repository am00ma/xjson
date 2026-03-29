#include "user-parse.h"
#include "user-print.h"

#define print_str(dst) fprintf(stdout, "%.*s\n", (int)dst.len, dst.buf)
#define print_err(err) fprintf(stdout, "err=%d\n", err)

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
    Buffer print_buf = {
        .buf = buf,
        .len = 1024,
        .pos = 0,
        .err = 0,
    };

    Str dst_a = print__Struct_A(&print_buf, &a);
    print_str(dst_a);

    Str dst_b = print__Struct_B(&print_buf, &b);
    print_str(dst_b);

    Struct_A a_ = {};

    Str src_a = _("{\"bool_a\" : true}");
    print_str(src_a);

    Buffer parse_buf = {
        .buf = src_a.buf,
        .len = src_a.len,
        .pos = 0,
        .err = 0,
    };

    char   rebuf[1024] = {};
    Buffer reprint_buf = {
        .buf = rebuf,
        .len = 1024,
        .pos = 0,
        .err = 0,
    };

    int err = parse__Struct_A(&parse_buf, &a_);
    print_err(err);

    Str re_dst_a = print__Struct_A(&reprint_buf, &a_);
    print_str(re_dst_a);
}
