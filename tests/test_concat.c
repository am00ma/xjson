#include "concat.h"

#include "test.h"

// clang-format off

#define DECLARE_EXAMPLE_STRUCT(type, name)  \
    typedef struct                          \
    {                                       \
        Str   src;                          \
        isize buflen;                       \
        type  in;                           \
        int   err;                          \
        Str   dst;                          \
    } name

DECLARE_EXAMPLE_STRUCT(char, TestConcatChar);
DECLARE_EXAMPLE_STRUCT(Str, TestConcatString);
DECLARE_EXAMPLE_STRUCT(Str, TestConcatQuotedString);
DECLARE_EXAMPLE_STRUCT(bool, TestConcatBool);
DECLARE_EXAMPLE_STRUCT(u64, TestConcatU64);
DECLARE_EXAMPLE_STRUCT(i64, TestConcatI64);
DECLARE_EXAMPLE_STRUCT(f64, TestConcatF64);

#define SETUP_BUFFER(b, n)                 \
    char buf__[n] = {};                    \
    memset(buf__, 0, n);                   \
    Buffer b = {.buf = buf__, .len = n};

#define RUN_TEST(fn)                        \
    RANGE(i, countof(tests))                \
    {                                       \
        SETUP_BUFFER(b, tests[i].buflen);   \
        Str dst = fn(&b, tests[i].in);      \
        EXPECT_EQ_INT(b.err, tests[i].err); \
        EXPECT_EQ_STR(dst, tests[i].dst);   \
    }

// clang-format on

int main(int argc, char* argv[])
{
    TEST_SUITE("concat");

    TEST_CASE("char")
    {
        // clang-format off
        TestConcatChar tests[] = {
            {_(""), 4, ' ',  0,  _(" ")},  // Normal case
            {_(""), 0, ' ',  -1, _("")},   // No capacity left
            {_(""), 1, ' ',  0,  _(" ")},  // Only one char left
            {_(""), 1, '\n', 0,  _("\n")}, // Checking escaped stuff
        };
        // clang-format on

        RUN_TEST(concat__char);
    }

    TEST_CASE("string")
    {
        // clang-format off
        TestConcatString tests[] = {
            {_(""), 0, _(""),     0,  _("")},     // Empty string
            {_(""), 1, _(""),     0,  _("")},     // Empty string
            {_(""), 4, _(" "),    0,  _(" ")},    // Normal case
            {_(""), 0, _(" "),    -1, _("")},     // No capacity left
            {_(""), 1, _(" "),    0,  _(" ")},    // Only one char left
            {_(""), 1, _("\n"),   0,  _("\n")},   // Checking escaped stuff
            {_(""), 4, _("abcd"), 0,  _("abcd")}, // String len > 1
            {_(""), 2, _("abcd"), -1, _("ab")},   // Incomplete
        };
        // clang-format on

        RUN_TEST(concat__string);
    }

    TEST_CASE("quoted_string")
    {
        // clang-format off
        TestConcatQuotedString tests[] = {
            {_(""), 0, _(""),     -1, _("")},         // No capacity for quotes
            {_(""), 2, _(""),     0,  _("\"\"")},     // Empty string
            {_(""), 3, _(" "),    0,  _("\" \"")},    // Normal case
            {_(""), 4, _(" "),    0,  _("\" \"")},    //
            {_(""), 3, _("\n"),   0,  _("\"\n\"")},   // Checking escaped stuff
            {_(""), 6, _("abcd"), 0,  _("\"abcd\"")}, // String len > 1
            {_(""), 3, _("abcd"), -1, _("\"ab")},     // String len exceeded
        };
        // clang-format on

        RUN_TEST(concat__quoted_string);
    }

    TEST_CASE("bool")
    {
        // clang-format off
        TestConcatBool tests[] = {
            {_(""), 0, true,  -1, _("")},      // No capacity
            {_(""), 2, true,  -1, _("tr")},    // Print as much as possible
            {_(""), 3, false, -1, _("fal")},   //
            {_(""), 4, true,  0,  _("true")},  // Normal case
            {_(""), 5, false, 0,  _("false")}, //
            {_(""), 8, true,  0,  _("true")},  // Normal case
            {_(""), 8, false, 0,  _("false")}, //
        };
        // clang-format on

        RUN_TEST(concat__bool);
    }

    TEST_CASE("u64")
    {
        // clang-format off
        TestConcatU64 tests[] = {
            {_(""), 0,               10,      -1, _("")},                     // No capacity
            {_(""), MAX_LEN_INTEGER, 10,      0,  _("10")},                   // Minimum cap needed for snprintf
            {_(""), MAX_LEN_INTEGER, 100000,  0,  _("100000")},               //
            {_(""), MAX_LEN_INTEGER, -100000, 0,  _("18446744073709451616")}, // Negatives
            {_(""), MAX_LEN_INTEGER, +100000, 0,  _("100000")},               // Positives
        };
        // clang-format on

        RUN_TEST(concat__unsigned_int);
    }

    TEST_CASE("i64")
    {
        // clang-format off
        TestConcatI64 tests[] = {
            {_(""), 0,               10,      -1, _("")},        // No capacity
            {_(""), MAX_LEN_INTEGER, 10,      0,  _("10")},      // Minimum cap needed for snprintf
            {_(""), MAX_LEN_INTEGER, 100000,  0,  _("100000")},  //
            {_(""), MAX_LEN_INTEGER, -100000, 0,  _("-100000")}, // Negatives
            {_(""), MAX_LEN_INTEGER, +100000, 0,  _("100000")},  // Positives
        };
        // clang-format on

        RUN_TEST(concat__signed_int);
    }

    TEST_CASE("f64")
    {
        // clang-format off
        TestConcatF64 tests[] = {
            {_(""), 0,              3.14,  -1, _("")},          // No capacity
            {_(""), MAX_LEN_DOUBLE, 0,     0,  _("0.000000")},  // Prints 6 places after decimal
            {_(""), MAX_LEN_DOUBLE, 3.14,  0,  _("3.140000")},  // Normal
            {_(""), MAX_LEN_DOUBLE, -3.14, 0,  _("-3.140000")}, // Negatives
            {_(""), MAX_LEN_DOUBLE, +3.14, 0,  _("3.140000")},  // Positives
        };
        // clang-format on

        RUN_TEST(concat__double);
    }

    TEST_RESULTS();
}
