#include "types.h"
// clang-format off

// --------------- Arrays ---------------

#define DECLARE_PRINTVAR(type, fmt, ...) SI void printvar__##type(type* x) { p_inline(fmt, ##__VA_ARGS__); };

DECLARE_PRINTVAR(i32, "%d", *x);
DECLARE_PRINTVAR(Str, "%.*s", (int)(*x).len, (*x).buf);

#define DECLARE_ARRAY(name, type) typedef struct { type* buf; isize len; } name

DECLARE_ARRAY(arr_i32, i32);
DECLARE_ARRAY(arr_Str, Str);

SI void printvar__arr_i32(arr_i32* x)
{
    if (!x->len) {p_inline("[]"); return;}
    p_inline("[");
    RANGE(i, x->len-1) {printvar__i32(&x->buf[i]); p_inline(", ");}
    printvar__i32(&x->buf[x->len-1]);
    p_inline("]");
}

SI void printvar__arr_Str(arr_Str* x)
{
    if (!x->len) {p_inline("[]"); return;}
    p_inline("[");
    RANGE(i, x->len-1) {printvar__Str(&x->buf[i]); p_inline(", ");}
    printvar__Str(&x->buf[x->len-1]);
    p_inline("]");
}

// clang-format on

// --------------- Tests ---------------

int main()
{
    i32 vals_i32[3] = {-10, 20, 36};
    Str vals_Str[3] = {_("hello"), _("hi"), _("how")};

    arr_i32 a_i32 = {.buf = vals_i32, .len = countof(vals_i32)};
    printvar__arr_i32(&a_i32);
    p_newline();

    arr_Str a_Str = {.buf = vals_Str, .len = countof(vals_Str)};
    printvar__arr_Str(&a_Str);
    p_newline();

    return 0;
}
