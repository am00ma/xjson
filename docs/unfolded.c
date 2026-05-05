#include <stdio.h>

#define p_inline(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

void printvar__bool(bool* x) { p_inline("%s", (*x) ? "true" : "false"); };
void printvar__int(int* x) { p_inline("%d", (*x)); };

typedef struct
{
    bool f_bool;
    int  f_int;
} MyStruct;

// clang-format off
void printvar__MyStruct(MyStruct* x)
{
    p_inline("{");
    p_inline("f_bool="); printvar__bool(&x->f_bool); p_inline(", ");
    p_inline("f_int=");  printvar__int(&x->f_int);
    p_inline("}");
}
// clang-format on

int main()
{
    // Variable to print
    bool     b = true;
    int      i = -10;
    MyStruct s = {b, i};

// Cannot use `typeof()` as it is compiler macro, not preprocessor macro
#define PrintType(x, type) (p_inline("%s = ", #x), printvar__##type(&x), p_inline("\n"));

    PrintType(b, bool);
    PrintType(i, int);
    PrintType(s, MyStruct);

// Can use _Generic, but limited to known types
#define PrintVar(x) _Generic((x), bool: printvar__bool, int: printvar__int, MyStruct: printvar__MyStruct)(&x)
#define PrintLn(x)  (p_inline("%s = ", #x), PrintVar(x), p_inline("\n"));

    PrintLn(b);
    PrintLn(i);
    PrintLn(s);
}
