#pragma once

#include "print.h"
#include "user.h"

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
