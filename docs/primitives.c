#include "types.h"

// --------------- Primitives ---------------

// clang-format off
#define DECLARE_PRINTVAR(type, fmt, ...) SI void printvar__##type(type* x) { p_inline(fmt, ##__VA_ARGS__); };

DECLARE_PRINTVAR(bool, "%s", *x ? "true" : "false");
DECLARE_PRINTVAR(u32, "%u", *x);
DECLARE_PRINTVAR(i32, "%d", *x);
DECLARE_PRINTVAR(f32, "%f", *x);
DECLARE_PRINTVAR(u64, "%lu", *x);
DECLARE_PRINTVAR(i64, "%ld", *x);
DECLARE_PRINTVAR(f64, "%f", *x);
DECLARE_PRINTVAR(Str, "%.*s", (int)(*x).len, (*x).buf);

typedef struct
{
    bool f_bool;
    u32  f_u32;
    i32  f_i32;
    f32  f_f32;
    u64  f_u64;
    i64  f_i64;
    f64  f_f64;
    Str  f_Str;
} Primitives;

SI void printvar__Primitives(Primitives* x)
{
    p_inline("{");
    p_inline("f_bool="); printvar__bool(&x->f_bool); p_inline(", ");
    p_inline("f_u32=");  printvar__u32(&x->f_u32);   p_inline(", ");
    p_inline("f_i32=");  printvar__i32(&x->f_i32);   p_inline(", ");
    p_inline("f_f32=");  printvar__f32(&x->f_f32);   p_inline(", ");
    p_inline("f_u64=");  printvar__u64(&x->f_u64);   p_inline(", ");
    p_inline("f_i64=");  printvar__i64(&x->f_i64);   p_inline(", ");
    p_inline("f_f64=");  printvar__f64(&x->f_f64);   p_inline(", ");
    p_inline("f_Str=");  printvar__Str(&x->f_Str);   p_inline("}");
}
// clang-format on

// --------------- Tests ---------------

int main()
{
    bool f_bool = true;
    u32  f_u32  = 10;
    i32  f_i32  = -10;
    f32  f_f32  = 3.14;
    u64  f_u64  = 10;
    i64  f_i64  = -10;
    f64  f_f64  = 3.14;
    Str  f_Str  = _("hello");

    // clang-format off
    printvar__bool(&f_bool); p_newline();
    printvar__u32(&f_u32);   p_newline();
    printvar__i32(&f_i32);   p_newline();
    printvar__f32(&f_f32);   p_newline();
    printvar__u64(&f_u64);   p_newline();
    printvar__i64(&f_i64);   p_newline();
    printvar__f64(&f_f64);   p_newline();
    printvar__Str(&f_Str);   p_newline();
    // clang-format on

    Primitives p = {true, 10, -10, 3.14, 10, -10, 3.14, _("hello")};
    printvar__Primitives(&p);
    p_newline();

    return 0;
}
