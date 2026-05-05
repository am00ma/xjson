#include "types.h"

// --------------- Primitives ---------------

// clang-format off
#define DECLARE_PRINTVAR(type, fmt, ...) SI void printvar__##type(type* x) { p_inline(fmt, ##__VA_ARGS__); };

DECLARE_PRINTVAR(i32, "%d", *x);
DECLARE_PRINTVAR(Str, "%.*s", (int)(*x).len, (*x).buf);

typedef struct
{
    i32  f_i32;
    Str  f_Str;
} Inside;

typedef struct
{
    i32     f_i32;
    Str     f_Str;
    Inside  f_Inside;
    Inside* f_ref_Inside;
} Outside;

SI void printvar__Inside(Inside* x)
{
    p_inline("{");
    p_inline("f_i32=");  printvar__i32(&x->f_i32);   p_inline(", ");
    p_inline("f_Str=");  printvar__Str(&x->f_Str);   p_inline("}");
}

SI void printvar__Outside(Outside* x)
{
    p_inline("{");
    p_inline("f_i32=");        printvar__i32(&x->f_i32);          p_inline(", ");
    p_inline("f_Str=");        printvar__Str(&x->f_Str);          p_inline(", ");
    p_inline("f_Inside=");     printvar__Inside(&x->f_Inside);    p_inline(", ");
    p_inline("f_ref_Inside="); printvar__Inside(x->f_ref_Inside); p_inline("}");
}
// clang-format on

// --------------- Tests ---------------

int main()
{
    Inside inside = {-10, _("hello")};
    printvar__Inside(&inside);
    p_newline();

    Outside outside = {-10, _("hello"), inside, &inside};
    printvar__Outside(&outside);
    p_newline();

    return 0;
}
