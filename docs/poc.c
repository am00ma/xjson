#include "types.h"
// clang-format off

// --------------- Primitives ---------------

#define X_TABLE_PRIMITIVES(type, fmt, ...) \
X(bool,"%s"  , *x ? "true" : "false") \
X(char,"%d"  , *x )                   \
X(u32, "%u"  , *x )                   \
X(i32, "%d"  , *x )                   \
X(f32, "%f"  , *x )                   \
X(u64, "%lu" , *x )                   \
X(i64, "%ld" , *x )                   \
X(f64, "%f"  , *x )                   \
X(Str, "%.*s", (int)(x)->len, (x)->buf)

#define X(type, fmt, ...) SI void printvar__##type(type* x) { p_inline(fmt, ##__VA_ARGS__); }
X_TABLE_PRIMITIVES(type, fmt, ...);
#undef X

// --------------- Arrays: Declaration ---------------

#define DECLARE_ARRAY(name, type) typedef struct { type* buf; isize len; } name

#define X(type, fmt, ...) DECLARE_ARRAY(arr_##type, type);
X_TABLE_PRIMITIVES(type, fmt, ...);
#undef X

// --------------- Arrays: Printing ---------------

#define DECLARE_PRINTVAR_ARRAY(name, type) \
SI void printvar__##name(name* x) { p_inline("["); RANGE(i, x->len) {printvar__##type(&x->buf[i]); p_inline(", ");} p_inline("]"); }

#define X(type, fmt, ...) DECLARE_PRINTVAR_ARRAY(arr_##type, type);
X_TABLE_PRIMITIVES(type, fmt, ...);
#undef X

// --------------- Definition of struct using X-Table ---------------

/*
* Needs to be declared by user
*/
#define X_TABLE_Primitives(type, field, isref1, isref2) \
X(bool,     f_bool,     &, ) \
X(char,     f_char,     &, ) \
X(u32,      f_u32,      &, ) \
X(i32,      f_i32,      &, ) \
X(f32,      f_f32,      &, ) \
X(u64,      f_u64,      &, ) \
X(i64,      f_i64,      &, ) \
X(f64,      f_f64,      &, ) \
X(Str,      f_Str,      &, ) \
X(bool,     f_ref_bool, ,  *) \
X(char,     f_ref_char, ,  *) \
X(u32,      f_ref_u32,  ,  *) \
X(i32,      f_ref_i32,  ,  *) \
X(f32,      f_ref_f32,  ,  *) \
X(u64,      f_ref_u64,  ,  *) \
X(i64,      f_ref_i64,  ,  *) \
X(f64,      f_ref_f64,  ,  *) \
X(Str,      f_ref_Str,  ,  *) \
X(arr_bool, f_arr_bool, &, ) \
X(arr_char, f_arr_char, &, ) \
X(arr_u32,  f_arr_u32,  &, ) \
X(arr_i32,  f_arr_i32,  &, ) \
X(arr_f32,  f_arr_f32,  &, ) \
X(arr_u64,  f_arr_u64,  &, ) \
X(arr_i64,  f_arr_i64,  &, ) \
X(arr_f64,  f_arr_f64,  &, ) \
X(arr_Str,  f_arr_Str,  &, ) \

// --------------- X-Macros: Struct ---------------

#define DECLARE_DEFINE_STRUCT_FIELD(type, field, isref1, isref2) type isref2 field;

#define DECLARE_DEFINE_STRUCT(name, type, field, isref1, isref2) \
typedef struct                                                   \
{                                                                \
    X_TABLE_##name(type, field, isref1, isref2);                 \
} name;

/*
* Needs to be declared by user
*/
#define X(type, field, isref1, isref2) DECLARE_DEFINE_STRUCT_FIELD(type, field, isref1, isref2)
DECLARE_DEFINE_STRUCT(Primitives, type, field, isref1, isref2)
#undef X

// --------------- X-Macros: Print ---------------

#define DECLARE_PRINT_FIELD(type, field, isref1, isref2) p_inline(#field "="); printvar__##type(isref1(x->field)); p_inline(", ");

#define DECLARE_PRINT_STRUCT(name, type, field, isref1, isref2) \
    SI void printvar__##name(name* x)                           \
    {                                                           \
        p_inline("{");                                          \
        X_TABLE_##name(type, field, isref1, isref2);            \
        p_inline("}");                                          \
    }

/*
* Needs to be declared by user
*/
#define X(type, field, isref1, isref2) DECLARE_PRINT_FIELD(type, field, isref1, isref2)
DECLARE_PRINT_STRUCT(Primitives, type, field, isref1, isref2)
#undef X

// --------------- X-Macros: Experimental: Pretty Print (TODO: Need indent for nesting) ---------------

#define DECLARE_PRINT_FIELD_PRETTY(type, field, isref1, isref2) p_inline("  " #field "="); printvar__##type(isref1(x->field)); p_inline(",\n");

#define DECLARE_PRINT_FIELD_PRETTY_FN(name, type, field, isref1, isref2)  \
    SI void pretty_printvar__##name(name* x)                              \
    {                                                                     \
        p_inline("{\n");                                                  \
        X_TABLE_##name(type, field, isref1, isref2);                      \
        p_inline("}\n");                                                  \
    }

/*
* Needs to be declared by user
*/
#define X(type, field, isref1, isref2) DECLARE_PRINT_FIELD_PRETTY(type, field, isref1, isref2)
DECLARE_PRINT_FIELD_PRETTY_FN(Primitives, type, field, isref1, isref2)
#undef X

// clang-format on

// --------------- Proof of concept ---------------

int main()
{
    bool v_bool = true;
    char v_char = 'a';
    u32  v_u32  = 10;
    i32  v_i32  = -10;
    f32  v_f32  = 3.14;
    u64  v_u64  = 10;
    i64  v_i64  = -10;
    f64  v_f64  = 3.14;
    Str  v_Str  = _("hello");

    bool a_bool[] = {true, false};
    char a_char[] = {'a', 'b'};
    u32  a_u32[]  = {10, 20};
    i32  a_i32[]  = {-10, -20};
    f32  a_f32[]  = {3.14, 5.14};
    u64  a_u64[]  = {10, 20};
    i64  a_i64[]  = {-10, -20};
    f64  a_f64[]  = {3.14, 5.14};
    Str  a_Str[]  = {_("hello"), _("hi")};

    Primitives p = {
        true,        'a',         10,         -10,        3.14,       10,         -10,        3.14,       _("hello"),
        &v_bool,     &v_char,     &v_u32,     &v_i32,     &v_f32,     &v_u64,     &v_i64,     &v_f64,     &v_Str,
        {a_bool, 2}, {a_char, 2}, {a_u32, 2}, {a_i32, 2}, {a_f32, 2}, {a_u64, 2}, {a_i64, 2}, {a_f64, 2}, {a_Str, 2},
    };

    printvar__Primitives(&p);
    p_newline();

    pretty_printvar__Primitives(&p);

    return 0;
}
