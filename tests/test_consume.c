#include "consume.h"

#include "test.h"

#define SETUP_BUFFER(b, src) Buffer b = {.buf = src.buf, .len = src.len};

typedef struct
{
    Str src;
    int err;
    Str dst;

} TestConsume;

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

DECLARE_EXAMPLE_STRUCT(char, TestConsumeChar);
DECLARE_EXAMPLE_STRUCT(Str, TestConsumeLiteral);

int main(int argc, char* argv[])
{
    TEST_SUITE("consume");

    TEST_CASE("whitespace")
    {
        TestConsume tests[] = {
            {_(" "), 0, _(" ")},              // Normal case
            {_(""), 0, _("")},                // Empty case - not fail
            {_(" x"), 0, _(" ")},             // End consuming at `x`
            {_(" \n \t x"), 0, _(" \n \t ")}, // Escape characters TODO:
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
        TestConsume tests[] = {
            {_("a"), 0, _("a")}, // Normal case
            {_(" "), 0, _(" ")}, //
            {_(""), -1, _("")},  // Empty case - fail
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
        TestConsume tests[] = {
            {_("\"a\""), 0, _("\"a\"")},    // Normal case
            {_("\" \""), 0, _("\" \"")},    //
            {_(""), -1, _("")},             // Empty case 1 - fail
            {_("\""), -1, _("")},           // Empty case 2 - unterminated
            {_(" "), -1, _("")},            // Empty case 3 - no quote
            {_("\"ab\"x"), 0, _("\"ab\"")}, // Truncate after match
            {_("x\"ab\""), -1, _("")},      // Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__quoted_string(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("digit")
    {
        TestConsume tests[] = {
            {_("0"), 0, _("0")},  // Normal case
            {_(""), -1, _("")},   // Empty case - fail
            {_("5"), 0, _("5")},  // Other numbers
            {_("9"), 0, _("9")},  //
            {_("a"), -1, _("")},  // Non-numbers
            {_("\n"), -1, _("")}, // Escape chars
            {_("\t"), -1, _("")}, //
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__digit(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("digits")
    {
        TestConsume tests[] = {
            {_("0"), 0, _("0")},       // Single: Normal case
            {_(""), -1, _("")},        //   Empty case - fail
            {_("5"), 0, _("5")},       //   Other numbers
            {_("9"), 0, _("9")},       //
            {_("a"), -1, _("")},       //   Non-numbers
            {_("\n"), -1, _("")},      //   Escape chars
            {_("\t"), -1, _("")},      //
            {_("0123"), 0, _("0123")}, // Seq: Normal case
            {_("01ab"), 0, _("01")},   //   Truncated case
            {_("ab01"), -1, _("")},    //   Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__digits(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("integer")
    {
        TestConsume tests[] = {
            {_("0"), 0, _("0")},       // Single: Normal case
            {_(""), -1, _("")},        //   Empty case - fail
            {_("5"), 0, _("5")},       //   Other numbers
            {_("9"), 0, _("9")},       //
            {_("a"), -1, _("")},       //   Non-numbers
            {_("\n"), -1, _("")},      //   Escape chars
            {_("\t"), -1, _("")},      //
            {_("0123"), 0, _("0123")}, // Seq: Normal case
            {_("01ab"), 0, _("01")},   //   Truncated case
            {_("ab01"), -1, _("")},    //   Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__integer(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("fraction")
    {
        TestConsume tests[] = {
            {_(".0"), 0, _(".0")},       // Single: Normal case
            {_(""), 0, _("")},           //   Empty case 1 - pass
            {_("."), -1, _("")},         //   Empty case 2 - fail
            {_("a"), 0, _("")},          // Truncated: Non numbers
            {_("\n"), 0, _("")},         //   Escape chars
            {_("\t"), 0, _("")},         //
            {_(".5"), 0, _(".5")},       //   Other numbers
            {_(".9"), 0, _(".9")},       //
            {_(".0123"), 0, _(".0123")}, // Seq: Normal case
            {_(".01ab"), 0, _(".01")},   //   Truncated case
            {_("ab01"), 0, _("")},       //   Only from start
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__fraction(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("exponent")
    {
        TestConsume tests[] = {
            {_("e0"), 0, _("e0")},         // Single: Normal case
            {_("E0"), 0, _("E0")},         //
            {_(""), 0, _("")},             //   Empty case 1 - pass
            {_("."), 0, _("")},            //   Empty case 2 - truncated
            {_("e"), -1, _("")},           //   Empty case 2 - fail
            {_("a"), 0, _("")},            // Truncated: Non-numbers
            {_("\n"), 0, _("")},           //   Escape chars
            {_("\t"), 0, _("")},           //
            {_("e5"), 0, _("e5")},         //   Other numbers
            {_("E9"), 0, _("E9")},         //
            {_("e0123"), 0, _("e0123")},   // Seq: Normal case
            {_("E01ab"), 0, _("E01")},     //   Truncated case
            {_("abe01"), 0, _("")},        //   Only from start
            {_("e-0123"), 0, _("e-0123")}, // Seq: Integers
            {_("e+0123"), 0, _("e+0123")}, // Seq: Integers
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__exponent(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_CASE("number")
    {
        TestConsume tests[] = {
            // --- Integers
            {_("0"), 0, _("0")},       // Single: Normal case
            {_(""), -1, _("")},        //   Empty case - fail
            {_("5"), 0, _("5")},       //   Other numbers
            {_("9"), 0, _("9")},       //
            {_("a"), -1, _("")},       //   Non-numbers
            {_("\n"), -1, _("")},      //   Escape chars
            {_("\t"), -1, _("")},      //
            {_("0123"), 0, _("0123")}, // Seq: Normal case
            {_("01ab"), 0, _("01")},   //   Truncated case
            {_("ab01"), -1, _("")},    //   Only from start
            // --- Fractions
            {_(".5"), -1, _("")},          //   Other numbers
            {_(".9"), -1, _("")},          //
            {_(".0123"), -1, _("")},       // Seq: Normal case
            {_(".01ab"), -1, _("")},       //   Truncated case
            {_("0.5"), 0, _("0.5")},       //   Other numbers
            {_("0.9"), 0, _("0.9")},       //
            {_("0.0123"), 0, _("0.0123")}, // Seq: Normal case
            {_("0.01ab"), 0, _("0.01")},   //   Truncated case
            // --- Exponents
            {_("0e5"), 0, _("0e5")},         //   Other numbers
            {_("0E9"), 0, _("0E9")},         //
            {_("0e0123"), 0, _("0e0123")},   // Seq: Normal case
            {_("0E01ab"), 0, _("0E01")},     //   Truncated case
            {_("0e-0123"), 0, _("0e-0123")}, // Seq: Integers
            {_("0e+0123"), 0, _("0e+0123")}, // Seq: Integers
            // --- Combined
            {_("0.1e5"), 0, _("0.1e5")},         //   Other numbers
            {_("0.1E9"), 0, _("0.1E9")},         //
            {_("0.1e0123"), 0, _("0.1e0123")},   // Seq: Normal case
            {_("0.1E01ab"), 0, _("0.1E01")},     //   Truncated case
            {_("0.1e-0123"), 0, _("0.1e-0123")}, // Seq: Integers
            {_("0.1e+0123"), 0, _("0.1e+0123")}, // Seq: Integers
            {_("3.140000"), 0, _("3.140000")},   //
        };

        RANGE(i, countof(tests))
        {
            SETUP_BUFFER(b, tests[i].src);

            Str dst = consume__number(&b);
            EXPECT_EQ_INT(b.err, tests[i].err);
            EXPECT_EQ_STR(dst, tests[i].dst);
        }
    }

    TEST_RESULTS();
}
