#include <stz/types.h>

#include <inttypes.h> // strtoimax

#define SI static inline

// --------------- Parser ---------------
typedef struct
{
    char* buf;
    isize len;

    isize pos;
    int   err;

    isize start;

} Parser;

typedef int (*consume_type_fn)(Parser* p, void* val);
typedef int (*concat_type_fn)(Parser* p, void* val);

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

// --------------- Nested examples ---------------

typedef struct
{

    struct
    {
        u64*  buf;
        isize len;
    } u64s_;

    Struct_A struct_a_;

} Struct_B;
