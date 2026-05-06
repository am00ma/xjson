// #include <meta>
#include <stdio.h>

#define p_inline(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)

template <typename T>
void printvar(T* x);

template <typename T = bool>
void printvar(bool* x)
{ p_inline("%s", (*x) ? "true" : "false"); };

template <typename T = int>
void printvar(int* x)
{ p_inline("%d", (*x)); };

typedef struct
{
    bool f_bool;
    int  f_int;
} MyStruct;

// clang-format off
template <typename T = MyStruct>
void printvar(MyStruct* x)
{
    p_inline("{");
    p_inline("f_bool="); printvar(&x->f_bool); p_inline(", ");
    p_inline("f_int=");  printvar(&x->f_int);
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
#define PrintLn(x) (p_inline("%s = ", #x), printvar(&x), p_inline("\n"));

    PrintLn(b);
    PrintLn(i);
    PrintLn(s);

    // (void)std::meta::info;
}
