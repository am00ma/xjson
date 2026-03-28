#include "concat.h"

#include "test.h"

// clang-format off

#define DECLARE_EXAMPLE_STRUCT(type, name) \
    typedef struct                         \
    {                                      \
        Str   src;                         \
        isize buflen;                      \
        type  in;                          \
        int   err;                         \
        Str   dst;                         \
    } name

#define SETUP_BUFFER(b, n)                 \
    char buf__[n] = {};                    \
    memset(buf__, 0, n);                   \
    Buffer b = {.buf = buf__, .len = n};

// clang-format on

DECLARE_EXAMPLE_STRUCT(char, TestConcatChar);
DECLARE_EXAMPLE_STRUCT(Str, TestConcatString);
DECLARE_EXAMPLE_STRUCT(Str, TestConcatQuotedString);
DECLARE_EXAMPLE_STRUCT(bool, TestConcatBool);
DECLARE_EXAMPLE_STRUCT(u64, TestConcatU64);
DECLARE_EXAMPLE_STRUCT(i64, TestConcatI64);
DECLARE_EXAMPLE_STRUCT(f64, TestConcatF64);

int main(int argc, char* argv[])
{
    TEST_SUITE("concat");

    TEST_CASE("char")
    {
        TestConcatChar tests[] = {
            {_(""), 4, ' ', 0, _(" ")},   // Normal case
            {_(""), 0, ' ', -1, _("")},   // No capacity left
            {_(""), 1, ' ', 0, _(" ")},   // Only one char left
            {_(""), 1, '\n', 0, _("\n")}, // Checking escaped stuff
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__char(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("string")
    {
        TestConcatString tests[] = {
            {_(""), 0, _(""), 0, _("")},         // Empty string
            {_(""), 1, _(""), 0, _("")},         // Empty string
            {_(""), 4, _(" "), 0, _(" ")},       // Normal case
            {_(""), 0, _(" "), -1, _("")},       // No capacity left
            {_(""), 1, _(" "), 0, _(" ")},       // Only one char left
            {_(""), 1, _("\n"), 0, _("\n")},     // Checking escaped stuff
            {_(""), 4, _("abcd"), 0, _("abcd")}, // String len > 1
            {_(""), 2, _("abcd"), -1, _("ab")},  // Incomplete
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__string(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("quoted_string")
    {
        TestConcatQuotedString tests[] = {
            {_(""), 0, _(""), -1, _("")},            // No capacity for quotes
            {_(""), 2, _(""), 0, _("\"\"")},         // Empty string
            {_(""), 3, _(" "), 0, _("\" \"")},       // Normal case
            {_(""), 4, _(" "), 0, _("\" \"")},       //
            {_(""), 3, _("\n"), 0, _("\"\n\"")},     // Checking escaped stuff
            {_(""), 6, _("abcd"), 0, _("\"abcd\"")}, // String len > 1
            {_(""), 3, _("abcd"), -1, _("\"ab")},    // String len exceeded
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__quoted_string(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("bool")
    {
        TestConcatBool tests[] = {
            {_(""), 0, true, -1, _("")},      // No capacity
            {_(""), 2, true, -1, _("tr")},    // Print as much as possible
            {_(""), 3, false, -1, _("fal")},  //
            {_(""), 4, true, 0, _("true")},   // Normal case
            {_(""), 5, false, 0, _("false")}, //
            {_(""), 8, true, 0, _("true")},   // Normal case
            {_(""), 8, false, 0, _("false")}, //
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__bool(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("u64")
    {
        TestConcatU64 tests[] = {
            {_(""), 0, 10, -1, _("")},                                       // No capacity
            {_(""), MAX_LEN_INTEGER, 10, 0, _("10")},                        // Minimum cap needed for snprintf
            {_(""), MAX_LEN_INTEGER, 100000, 0, _("100000")},                //
            {_(""), MAX_LEN_INTEGER, -100000, 0, _("18446744073709451616")}, // Negatives
            {_(""), MAX_LEN_INTEGER, +100000, 0, _("100000")},               // Positives
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__unsigned_int(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("i64")
    {
        TestConcatI64 tests[] = {
            {_(""), 0, 10, -1, _("")},                          // No capacity
            {_(""), MAX_LEN_INTEGER, 10, 0, _("10")},           // Minimum cap needed for snprintf
            {_(""), MAX_LEN_INTEGER, 100000, 0, _("100000")},   //
            {_(""), MAX_LEN_INTEGER, -100000, 0, _("-100000")}, // Negatives
            {_(""), MAX_LEN_INTEGER, +100000, 0, _("100000")},  // Positives
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__signed_int(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("f64")
    {
        TestConcatF64 tests[] = {
            {_(""), 0, 3.14, -1, _("")},                       // No capacity
            {_(""), MAX_LEN_DOUBLE, 0, 0, _("0.000000")},      // Prints 6 places after decimal
            {_(""), MAX_LEN_DOUBLE, 3.14, 0, _("3.140000")},   // Normal
            {_(""), MAX_LEN_DOUBLE, -3.14, 0, _("-3.140000")}, // Negatives
            {_(""), MAX_LEN_DOUBLE, +3.14, 0, _("3.140000")},  // Positives
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].buflen);

            Str dst = concat__double(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_RESULTS();
}
