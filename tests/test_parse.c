#include "parse.h" // IWYU pragma: keep

#include "test.h"

typedef struct
{

    i32 idx;
    u64 len;
    Str a;

} Struct_A;

// Table and count for reflection
#define X_TABLE_Struct_A(idx, type, ref, field, key, print_fn, parse_fn)                                               \
    X(0, i32, &, idx, "idx", , parse__i32)                                                                             \
    X(1, u64, &, len, "len", , parse__u64)                                                                             \
    X(2, Str, &, a, "a", , parse__Str)
#define X_COUNT_Struct_A 3

// Deserialize -> parse__Struct_A
#define X(...) X_PARSE(Struct_A, __VA_ARGS__)
X_DECLARE_PARSE(Struct_A, idx, type, ref, field, key, print_fn, parse_fn);
#undef X

// --------------- Main ---------------

int main(int argc, char* argv[])
{
    TEST_SUITE("print");

    TEST_CASE("Struct_A")
    {
        Str src = _("{\"idx\":10, \"len\":345, \"a\":\"hello\"}");

        Buffer b = BufFromStr(src);

        Struct_A expected = {
            .idx = 10,
            .len = 345,
            .a   = _("hello"),
        };
        Struct_A x = {};

        Str dst = parse__Struct_A(&b, &x);

        EXPECT_EQ_STR(src, dst);
        EXPECT_EQ_INT(x.idx, expected.idx);
        EXPECT_EQ_LONG(x.len, expected.len);
        EXPECT_EQ_STR(x.a, expected.a);
    }

    TEST_RESULTS();
}
