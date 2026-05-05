#include "types.h"

// --------------- References ---------------

// clang-format off
#define DECLARE_PRINTVAR(type, fmt, ...) SI void printvar__##type(type* x) { p_inline(fmt, ##__VA_ARGS__); };

DECLARE_PRINTVAR(i32, "%d", *x);
DECLARE_PRINTVAR(Str, "%.*s", (int)(*x).len, (*x).buf);

typedef struct
{
    i32* f_ref_i32;
    Str* f_ref_Str;
} References;

SI void printvar__References(References* x)
{
    p_inline("{");
    p_inline("f_ref_i32="); printvar__i32(x->f_ref_i32); p_inline(", ");
    p_inline("f_ref_Str="); printvar__Str(x->f_ref_Str); p_inline("}");
}
// clang-format on

// --------------- Tests ---------------

int main()
{
    References p = {&(int){-10}, &_("hello")};
    printvar__References(&p);
    p_newline();

    return 0;
}
