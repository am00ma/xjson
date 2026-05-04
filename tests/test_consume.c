#include "consume.h"

#include "test.h"

// clang-format off

typedef struct
{
    Str src;
    int err;
    Str dst;

} TestConsume;

#define DECLARE_EXAMPLE_STRUCT(type, name) \
    typedef struct                         \
    {                                      \
        Str   src;                         \
        type  in;                          \
        int   err;                         \
        Str   dst;                         \
    } name

DECLARE_EXAMPLE_STRUCT(char, TestConsumeChar);
DECLARE_EXAMPLE_STRUCT(Str, TestConsumeLiteral);

#define SETUP_BUFFER(b, src) Buffer b = {.buf = src.buf, .len = src.len};

#define RUN_TEST_PLAIN(fn)                   \
    RANGE(i, countof(tests))                 \
    {                                        \
        SETUP_BUFFER(b, tests[i].src);       \
        Str dst = fn(&b);                    \
        EXPECT_EQ_INT(b.err, tests[i].err);  \
        EXPECT_EQ_STR(dst, tests[i].dst);    \
    }

#define RUN_TEST_WITH_ARG(fn)                \
    RANGE(i, countof(tests))                 \
    {                                        \
        SETUP_BUFFER(b, tests[i].src);       \
        Str dst = fn(&b, tests[i].in);       \
        EXPECT_EQ_INT(b.err, tests[i].err);  \
        EXPECT_EQ_STR(dst, tests[i].dst);    \
    }

// clang-format on

int main(int argc, char* argv[])
{
    TEST_SUITE("consume");

    TEST_CASE("whitespace")
    {
        // clang-format off
        TestConsume tests[] = {
            {_(" "),        0, _(" ")},       // Normal case
            {_(""),         0, _("")},        // Empty case - not fail
            {_(" x"),       0, _(" ")},       // End consuming at `x`
            {_(" \n \t x"), 0, _(" \n \t ")}, // Escape characters TODO:
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__whitespace);
    }

    TEST_CASE("anychar")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("a"), 0,  _("a")}, // Normal case
            {_(" "), 0,  _(" ")}, //
            {_(""),  -1, _("")},  // Empty case - fail
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__anychar);
    }

    TEST_CASE("char")
    {
        // clang-format off
        TestConsumeChar tests[] = {
            {_("a"), 'a', 0,  _("a")}, // Normal case
            {_(" "), ' ', 0,  _(" ")}, //
            {_(""),  0,   -1, _("")},  // Empty case - fail
            {_("b"), 'a', -1, _("")},  // Wrong char - fail
        };
        // clang-format on

        RUN_TEST_WITH_ARG(consume__char);
    }

    TEST_CASE("literal")
    {
        // clang-format off
        TestConsumeLiteral tests[] = {
            {_("a"),    _("a"),  0,  _("a")},  // Normal case
            {_(" "),    _(" "),  0,  _(" ")},  //
            {_(""),     _("a"),  -1, _("")},   // Empty case 1 - fail
            {_(""),     _(""),   0,  _("")},   // Empty case 2 - pass
            {_(" "),    _(""),   0,  _("")},   // Empty case 3 - pass
            {_("abcd"), _("ab"), 0,  _("ab")}, // Truncate after match
            {_("abcd"), _("cd"), -1, _("")},   // Only from start
        };
        // clang-format on

        RUN_TEST_WITH_ARG(consume__literal);
    }

    TEST_CASE("json_string")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("\"a\""),                   0,  _("\"a\"")},                   // Normal case
            {_("\" \""),                   0,  _("\" \"")},                   //
            {_(""),                        -1, _("")},                        // Empty case 1 - fail
            {_("\""),                      -1, _("")},                        // Empty case 2 - unterminated
            {_(" "),                       -1, _("")},                        // Empty case 3 - no quote
            {_("\"ab\"x"),                 0,  _("\"ab\"")},                  // Truncate after match
            {_("x\"ab\""),                 -1, _("")},                        // Only from start
            {_("\"a \\\" \""),             0,  _("\"a \\\" \"")},             // Escapes 1
            {_("\"a \\\" \\n \\t \""),     0,  _("\"a \\\" \\n \\t \"")},     // Escapes 2
            {_("\"a \\\" \\u \\t \""),     -1, _("")},                        // Escapes 3
            {_("\"a \\\" \\uffff \\t \""), 0,  _("\"a \\\" \\uffff \\t \"")}, // Escapes 4
            {_("\"a \\a \\uffff \\t \""),  -1, _("")},                        // Escapes 5
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__json_string);
    }

    TEST_CASE("quoted_string")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("\"a\""),   0,  _("\"a\"")},  // Normal case
            {_("\" \""),   0,  _("\" \"")},  //
            {_(""),        -1, _("")},       // Empty case 1 - fail
            {_("\""),      -1, _("")},       // Empty case 2 - unterminated
            {_(" "),       -1, _("")},       // Empty case 3 - no quote
            {_("\"ab\"x"), 0,  _("\"ab\"")}, // Truncate after match
            {_("x\"ab\""), -1, _("")},       // Only from start
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__quoted_string);
    }

    TEST_CASE("digit")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("0"),  0,  _("0")}, // Normal case
            {_(""),   -1, _("")},  // Empty case - fail
            {_("5"),  0,  _("5")}, // Other numbers
            {_("9"),  0,  _("9")}, //
            {_("a"),  -1, _("")},  // Non-numbers
            {_("\n"), -1, _("")},  // Escape chars
            {_("\t"), -1, _("")},  //
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__digit);
    }

    TEST_CASE("digits")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("0"),    0,  _("0")},    // Single: Normal case
            {_(""),     -1, _("")},     //   Empty case - fail
            {_("5"),    0,  _("5")},    //   Other numbers
            {_("9"),    0,  _("9")},    //
            {_("a"),    -1, _("")},     //   Non-numbers
            {_("\n"),   -1, _("")},     //   Escape chars
            {_("\t"),   -1, _("")},     //
            {_("0123"), 0,  _("0123")}, // Seq: Normal case
            {_("01ab"), 0,  _("01")},   //   Truncated case
            {_("ab01"), -1, _("")},     //   Only from start
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__digits);
    }

    TEST_CASE("integer")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("0"),    0,  _("0")},    // Single: Normal case
            {_(""),     -1, _("")},     //   Empty case - fail
            {_("5"),    0,  _("5")},    //   Other numbers
            {_("9"),    0,  _("9")},    //
            {_("a"),    -1, _("")},     //   Non-numbers
            {_("\n"),   -1, _("")},     //   Escape chars
            {_("\t"),   -1, _("")},     //
            {_("0123"), 0,  _("0123")}, // Seq: Normal case
            {_("01ab"), 0,  _("01")},   //   Truncated case
            {_("ab01"), -1, _("")},     //   Only from start
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__integer);
    }

    TEST_CASE("fraction")
    {
        // clang-format off
        TestConsume tests[] = {
            {_(".0"),    0,  _(".0")},    // Single: Normal case
            {_(""),      0,  _("")},      //   Empty case 1 - pass
            {_("."),     -1, _("")},      //   Empty case 2 - fail
            {_("a"),     0,  _("")},      // Truncated: Non numbers
            {_("\n"),    0,  _("")},      //   Escape chars
            {_("\t"),    0,  _("")},      //
            {_(".5"),    0,  _(".5")},    //   Other numbers
            {_(".9"),    0,  _(".9")},    //
            {_(".0123"), 0,  _(".0123")}, // Seq: Normal case
            {_(".01ab"), 0,  _(".01")},   //   Truncated case
            {_("ab01"),  0,  _("")},      //   Only from start
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__fraction);
    }

    TEST_CASE("exponent")
    {
        // clang-format off
        TestConsume tests[] = {
            {_("e0"),     0,  _("e0")},     // Single: Normal case
            {_("E0"),     0,  _("E0")},     //
            {_(""),       0,  _("")},       //   Empty case 1 - pass
            {_("."),      0,  _("")},       //   Empty case 2 - truncated
            {_("e"),      -1, _("")},       //   Empty case 2 - fail
            {_("a"),      0,  _("")},       // Truncated: Non-numbers
            {_("\n"),     0,  _("")},       //   Escape chars
            {_("\t"),     0,  _("")},       //
            {_("e5"),     0,  _("e5")},     //   Other numbers
            {_("E9"),     0,  _("E9")},     //
            {_("e0123"),  0,  _("e0123")},  // Seq: Normal case
            {_("E01ab"),  0,  _("E01")},    //   Truncated case
            {_("abe01"),  0,  _("")},       //   Only from start
            {_("e-0123"), 0,  _("e-0123")}, // Seq: Integers
            {_("e+0123"), 0,  _("e+0123")}, // Seq: Integers
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__exponent);
    }

    TEST_CASE("number")
    {
        // clang-format off
        TestConsume tests[] = {
            // --- Integers
            {_("0"),         0,  _("0")},         // Single: Normal case
            {_(""),          -1, _("")},          //   Empty case - fail
            {_("5"),         0,  _("5")},         //   Other numbers
            {_("9"),         0,  _("9")},         //
            {_("a"),         -1, _("")},          //   Non-numbers
            {_("\n"),        -1, _("")},          //   Escape chars
            {_("\t"),        -1, _("")},          //
            {_("0123"),      0,  _("0123")},      // Seq: Normal case
            {_("01ab"),      0,  _("01")},        //   Truncated case
            {_("ab01"),      -1, _("")},          //   Only from start
            // --- Fractions
            {_(".5"),        -1, _("")},          //   Other numbers
            {_(".9"),        -1, _("")},          //
            {_(".0123"),     -1, _("")},          // Seq: Normal case
            {_(".01ab"),     -1, _("")},          //   Truncated case
            {_("0.5"),       0,  _("0.5")},       //   Other numbers
            {_("0.9"),       0,  _("0.9")},       //
            {_("0.0123"),    0,  _("0.0123")},    // Seq: Normal case
            {_("0.01ab"),    0,  _("0.01")},      //   Truncated case
            // --- Exponents
            {_("0e5"),       0,  _("0e5")},       //   Other numbers
            {_("0E9"),       0,  _("0E9")},       //
            {_("0e0123"),    0,  _("0e0123")},    // Seq: Normal case
            {_("0E01ab"),    0,  _("0E01")},      //   Truncated case
            {_("0e-0123"),   0,  _("0e-0123")},   // Seq: Integers
            {_("0e+0123"),   0,  _("0e+0123")},   // Seq: Integers
            // --- Combined
            {_("0.1e5"),     0,  _("0.1e5")},     //   Other numbers
            {_("0.1E9"),     0,  _("0.1E9")},     //
            {_("0.1e0123"),  0,  _("0.1e0123")},  // Seq: Normal case
            {_("0.1E01ab"),  0,  _("0.1E01")},    //   Truncated case
            {_("0.1e-0123"), 0,  _("0.1e-0123")}, // Seq: Integers
            {_("0.1e+0123"), 0,  _("0.1e+0123")}, // Seq: Integers
            {_("3.140000"),  0,  _("3.140000")},  //
        };
        // clang-format on

        RUN_TEST_PLAIN(consume__number);
    }

    TEST_RESULTS();
}
