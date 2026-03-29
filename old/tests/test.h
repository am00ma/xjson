#pragma once

#include <regex.h> // regex_t, ...
#include <stdio.h> // fprintf, stderr

/* ---------------------------------------------------------------------------
 *  Printing
 * ------------------------------------------------------------------------- */

// Terminal ANSI codes
#define COLOR_RESET   "\033[0m"
#define COLOR_BOLD    "\033[1m"
#define COLOR_ITALIC  "\033[3m"
#define COLOR_ULINE   "\033[4m"
#define COLOR_BLACK   "\033[0;30m"
#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"
#define COLOR_WHITE   "\033[0;37m"

// Common styles
#define STYLE_TITLE COLOR_YELLOW COLOR_BOLD COLOR_ITALIC COLOR_ULINE
#define STYLE_INFO  COLOR_WHITE
#define STYLE_WARN  COLOR_YELLOW
#define STYLE_ERROR COLOR_RED

// Common print styles
#define p_reset(...)     fprintf(stderr, COLOR_RESET)
#define p_newline(...)   fprintf(stderr, "\n")
#define p_resetline(...) fprintf(stderr, COLOR_RESET "\n")

#define p_line(...)   (fprintf(stderr, __VA_ARGS__), p_newline())
#define p_inline(...) fprintf(stderr, __VA_ARGS__)

#define p_color(color, ...)     (fprintf(stderr, color __VA_ARGS__), p_reset())
#define p_colorline(color, ...) (fprintf(stderr, color __VA_ARGS__), p_resetline())

#define p_title(...) p_colorline(STYLE_TITLE, __VA_ARGS__)
#define p_info(...)  p_colorline(STYLE_INFO, __VA_ARGS__)
#define p_warn(...)  p_colorline(STYLE_WARN, __VA_ARGS__)
#define p_error(...) p_colorline(STYLE_ERROR, __VA_ARGS__)

/* ---------------------------------------------------------------------------
 * Datastructure
 * ------------------------------------------------------------------------- */
typedef struct
{
    const char* suite;
    const char* _case;

    struct
    {
        int pass;
        int total;

    } cases, checks, total;

    const char* filter;
    regex_t     regex;

} TestSuite;

static inline bool test_case_filter(TestSuite* t)
{
    if (t->filter == NULL) return true;
    return !regexec(&t->regex, t->_case, 0, NULL, 0);
}

static inline void test_suite_start(TestSuite* t, const char* name, const char* filter)
{
    t->suite  = name;
    t->filter = filter;

    if (t->filter == NULL) { p_title("%s", t->suite); }
    else
    {
        p_title("%s | %s", t->suite, t->filter);
        int reti = regcomp(&t->regex, t->filter, REG_ICASE | REG_EXTENDED);
        if (reti) t->filter = 0;
    }

    p_colorline(COLOR_YELLOW, "| %3s  | %-50s | %8s | %8s |", "No.", "Case", "Correct", "Total");
    p_colorline(COLOR_YELLOW, "| %3s  | %-50s | %8s | %8s |", "---", "----", "-------", "-----");
}

static inline void test_case_start(TestSuite* t, const char* name)
{
    t->_case        = name;
    t->checks.pass  = 0;
    t->checks.total = 0;
}

static inline void test_case_finish(TestSuite* t)
{
    bool success = t->checks.pass == t->checks.total;

    t->cases.pass += success;
    t->cases.total++;

    t->total.pass  += t->checks.pass;
    t->total.total += t->checks.total;

    p_colorline(COLOR_RESET, "%s|  %3d | %-50s | %8d | %8d |", //
                success ? "" : "" COLOR_RED,                   //
                t->cases.total, t->_case, t->checks.pass, t->checks.total);

    t->checks.pass  = 0;
    t->checks.total = 0;
}

static inline int test_suite_finish(TestSuite* t)
{
    bool success = t->cases.pass == t->cases.total;

    p_colorline(COLOR_GREEN, "| %3s  | %-50s | %8s | %8s |", "---", "----", "-------", "-----");
    p_colorline(COLOR_GREEN, "| %3s  | %37s: %4d / %4d | %8d | %8d |", //
                success ? COLOR_GREEN " ✔ " : COLOR_RED " ✗ ",         //
                "Cases", t->cases.pass, t->cases.total,                //
                t->total.pass, t->total.total);

    regfree(&t->regex);

    return success ? 0 : 1;
}

/* ---------------------------------------------------------------------------
 * Exposed API
 * ------------------------------------------------------------------------- */
#define TEST_STRUCT StzTest

#define TEST_SUITE(name)                                                                                               \
    TestSuite TEST_STRUCT = {};                                                                                        \
    test_suite_start(&TEST_STRUCT, name, argc > 1 ? argv[1] : 0);

// Filter by test filter
#define TEST_CASE(name)                                                                                                \
    test_case_start(&TEST_STRUCT, name);                                                                               \
    for (int _stz_temp = 0; _stz_temp < test_case_filter(&TEST_STRUCT); (test_case_finish(&TEST_STRUCT), _stz_temp++))

#define TEST_RESULTS() test_suite_finish(&TEST_STRUCT)

/* ---------------------------------------------------------------------------
 * Macros for checks
 * ------------------------------------------------------------------------- */
// clang-format off
#define EXPECT_CONDITION(cond, fmt, ...)                                                                               \
    do {TEST_STRUCT.checks.total++;                                                                                    \
        if ((cond)) { p_colorline(COLOR_YELLOW, "%s:%d : " fmt, __FILE_NAME__, __LINE__, __VA_ARGS__); }                   \
        else { TEST_STRUCT.checks.pass++; } } while (0)
// clang-format on

#define EXPECT_EPS 1e-9

// clang-format off

// NOTE: Cannot use X-macros to define macros :(

#define EXPECT_TRUE(cond)      EXPECT_CONDITION(!(cond)                        , "%s == false"           , #cond)
#define EXPECT_FALSE(cond)     EXPECT_CONDITION( (cond)                        , "%s == true"            , #cond)
#define EXPECT_EQ_NULL(a)      EXPECT_CONDITION(!((a) == NULL)                 , "%s (%p) != NULL"       , #a, (a))
#define EXPECT_NEQ_NULL(a)     EXPECT_CONDITION( ((a) == NULL)                 , "%s (%p) == NULL"       , #a, (a))
#define EXPECT_EQ_INT(a, b)    EXPECT_CONDITION(!((a) == (b))                  , "%s (%d) != %s (%d)"    , #a, (a), #b, (b))
#define EXPECT_NEQ_INT(a, b)   EXPECT_CONDITION( ((a) == (b))                  , "%s (%d) == %s (%d)"    , #a, (a), #b, (b))
#define EXPECT_EQ_LONG(a, b)   EXPECT_CONDITION(!((a) == (b))                  , "%s (%ld) != %s (%ld)"  , #a, (a), #b, (b))
#define EXPECT_NEQ_LONG(a, b)  EXPECT_CONDITION( ((a) == (b))                  , "%s (%ld) == %s (%ld)"  , #a, (a), #b, (b))
#define EXPECT_EQ_FLOAT(a, b)  EXPECT_CONDITION(!(fabs((a) - (b)) < EXPECT_EPS), "%s (%f) != %s (%f)"    , #a, (a), #b, (b))
#define EXPECT_NEQ_FLOAT(a, b) EXPECT_CONDITION( (fabs((a) - (b)) < EXPECT_EPS), "%s (%f) == %s (%f)"    , #a, (a), #b, (b))
#define EXPECT_EQ_STR(a, b)    EXPECT_CONDITION(!(str_equal((a), (b)))         , "%s (%.*s) != %s (%.*s)", #a, __((a)), #b, __((b)))
#define EXPECT_NEQ_STR(a, b)   EXPECT_CONDITION( (str_equal((a), (b)))         , "%s (%.*s) == %s (%.*s)", #a, __((a)), #b, __((b)))

// clang-format on
