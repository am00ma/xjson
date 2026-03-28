#include "print.h"
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

static inline Str print__Struct_A(Buffer* p, Struct_A* x)
{
    isize start = p->pos;

    concat__char(p, '{');

    // clang-format off
    print__Str(p, _("bool_a")); concat__string(p, _(" : ")); print__bool(p, x->bool_a); concat__string(p, _(", "));
    print__Str(p, _("u64_a")) ; concat__string(p, _(" : ")); print__u64(p,  x->u64_a) ; concat__string(p, _(", "));
    print__Str(p, _("u64_b")) ; concat__string(p, _(" : ")); print__u64(p,  x->u64_b) ; concat__string(p, _(", "));
    print__Str(p, _("i32_a")) ; concat__string(p, _(" : ")); print__i32(p,  x->i32_a) ; concat__string(p, _(", "));
    print__Str(p, _("f64_a")) ; concat__string(p, _(" : ")); print__f64(p,  x->f64_a) ; concat__string(p, _(", "));
    print__Str(p, _("str_a")) ; concat__string(p, _(" : ")); print__Str(p,  x->str_a) ; concat__string(p, _(", "));

    print__Str(p, _("str_b")) ; concat__string(p, _(" : ")); print__Str(p, x->str_b);
    // clang-format on

    concat__char(p, '}');

    return BufferStr(p, start);
}

static inline int parse__Struct_A(Buffer* p, Struct_A* x)
{
    (void)p;
    (void)x;

    return 0;
}

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

static inline Str print__Struct_B(Buffer* p, Struct_B* x)
{
    isize start = p->pos;

    concat__char(p, '{');

    // clang-format off
    print__Str(p, _("u64s_"))    ; concat__string(p, _(" : ")); print__array(u64, p, x->u64s_,)       ; concat__string(p, _(", "));
    print__Str(p, _("f32s_"))    ; concat__string(p, _(" : ")); print__array(f32, p, x->f32s_,)       ; concat__string(p, _(", "));
    print__Str(p, _("arr_a"))    ; concat__string(p, _(" : ")); print__array(Struct_A, p, x->arr_a, &); concat__string(p, _(", "));
    print__Str(p, _("struct_a")) ; concat__string(p, _(" : ")); print__Struct_A(p, &x->struct_a_);
    // clang-format on

    concat__char(p, '}');

    return BufferStr(p, start);
}
