#include "consume.h"

#include "test.h"

#define SETUP_BUFFER(b, src) Buffer b = {.buf = src.buf, .len = src.len};

// clang-format off
#define DECLARE_EXAMPLE_STRUCT(type, name) \
    typedef struct                         \
    {                                      \
        Str   src;                         \
        type  in;                          \
        int   err;                         \
        Str   dst;                         \
    } name
// clang-format on

DECLARE_EXAMPLE_STRUCT(char, TestConsumeWhitespace);
DECLARE_EXAMPLE_STRUCT(char, TestConsumeAnyChar);
DECLARE_EXAMPLE_STRUCT(char, TestConsumeChar);
DECLARE_EXAMPLE_STRUCT(Str, TestConsumeLiteral);
DECLARE_EXAMPLE_STRUCT(Str, TestConsumeQuotedString);

int main(int argc, char* argv[])
{
    TEST_SUITE("consume");

    TEST_CASE("whitespace")
    {
        TestConsumeWhitespace tests[] = {
            {_(" "), 0, 0, _(" ")},              // Normal case
            {_(""), 0, 0, _("")},                // Empty case - not fail
            {_(" x"), 0, 0, _(" ")},             // End consuming at `x`
            {_(" \n \t x"), 0, 0, _(" \n \t ")}, // Escape characters TODO:
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__whitespace(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("anychar")
    {
        TestConsumeAnyChar tests[] = {
            {_("a"), 0, 0, _("a")}, // Normal case
            {_(" "), 0, 0, _(" ")}, //
            {_(""), 0, -1, _("")},  // Empty case - fail
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__anychar(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("char")
    {
        TestConsumeChar tests[] = {
            {_("a"), 'a', 0, _("a")}, // Normal case
            {_(" "), ' ', 0, _(" ")}, //
            {_(""), 0, -1, _("")},    // Empty case - fail
            {_("b"), 'a', -1, _("")}, // Wrong char - fail
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__char(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("literal")
    {
        TestConsumeLiteral tests[] = {
            {_("a"), _("a"), 0, _("a")},      // Normal case
            {_(" "), _(" "), 0, _(" ")},      //
            {_(""), _("a"), -1, _("")},       // Empty case 1 - fail
            {_(""), _(""), 0, _("")},         // Empty case 2 - pass
            {_(" "), _(""), 0, _("")},        // Empty case 3 - pass
            {_("abcd"), _("ab"), 0, _("ab")}, // Truncate after match
            {_("abcd"), _("cd"), -1, _("")},  // Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__literal(&b, tests[i].in);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("quoted_string")
    {
        TestConsumeQuotedString tests[] = {
            {_("\"a\""), {}, 0, _("a")},    // Normal case
            {_("\" \""), {}, 0, _(" ")},    //
            {_(""), {}, -1, _("")},         // Empty case 1 - fail
            {_("\""), {}, -1, _("")},       // Empty case 2 - unterminated
            {_(" "), {}, -1, _("")},        // Empty case 3 - no quote
            {_("\"ab\"x"), {}, 0, _("ab")}, // Truncate after match
            {_("x\"ab\""), {}, -1, _("")},  // Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__quoted_string(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_RESULTS();
}
