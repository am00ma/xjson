#include "print.h" // IWYU pragma: keep

#include "test.h"

// --------------- Struct_A ---------------

typedef struct
{
    i32 idx;
    u64 len;
    Str a;

} Struct_A;

// Table and count for reflection
#define X_TABLE_Struct_A(idx, type, ref, field, key, print_fn, parse_fn)                                               \
    X(0, i32, &, idx, "idx", print__i32, NULL)                                                                         \
    X(1, u64, &, len, "len", print__u64, NULL)                                                                         \
    X(2, Str, &, a, "a", print__Str, NULL)
#define X_COUNT_Struct_A 3

// Serialize -> print__Struct_A
#define X(...) X_PRINT(Struct_A, __VA_ARGS__)
X_DECLARE_PRINT(Struct_A, idx, type, ref, field, key, print_fn, parse_fn);
#undef X

// --------------- Main ---------------

int main(int argc, char* argv[])
{
    TEST_SUITE("print");

    TEST_CASE("Struct_A")
    {
        BufFromStack(b, 128);

        Struct_A x = {
            .idx = 10,
            .len = 345,
            .a   = _("hello"),
        };

        Str dst      = print__Struct_A(&b, &x);
        Str expected = _("{\"idx\":10, \"len\":345, \"a\":\"hello\"}");
        EXPECT_EQ_STR(dst, expected);
    }

    TEST_RESULTS();
}
