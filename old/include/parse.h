#pragma once

// #include "consume.h"

// void parse__char(Buffer* b, char* x)
// {
//     Str ch = consume__anychar(b);
//     if (b->err || !x) return;
//     *x = ch.buf[0];
// }
//
// void parse__Str(Buffer* b, Str* x)
// {
//     Str s = consume__quoted_string(b);
//     if (b->err || !x) return;
//     *x = (Str){.buf = &s.buf[1], .len = s.len - 1};
// }
//
// void parse__bool(Buffer* b, bool* x)
// {
//     Str i = consume__literal(b, _("true"));
//     if (b->err) i = consume__literal(b, _("false"));
//     if (b->err || !x) return;
//     *x = str_equal(i, _("true"));
// }
//
// void parse__i32(Buffer* b, i32* x)
// {
//     Str i = consume__integer(b);
//     if (b->err || !x) return;
//     *x = strtoimax(i.buf, 0, 10);
// }
//
// void parse__i64(Buffer* b, i64* x)
// {
//     Str i = consume__integer(b);
//     if (b->err || !x) return;
//     *x = strtol(i.buf, 0, 10);
// }
//
// void parse__u32(Buffer* b, u32* x)
// {
//     Str i = consume__integer(b);
//     if (b->err || !x) return;
//     *x = strtoumax(i.buf, 0, 10);
// }
//
// void parse__u64(Buffer* b, u64* x)
// {
//     Str i = consume__integer(b);
//     if (b->err || !x) return;
//     *x = strtoul(i.buf, 0, 10);
// }
//
// void parse__f32(Buffer* b, f32* x)
// {
//     Str f = consume__number(b);
//     if (b->err || !x) return;
//     *x = strtof(f.buf, 0);
// }
//
// void parse__f64(Buffer* b, f64* x)
// {
//     Str f = consume__number(b);
//     if (b->err || !x) return;
//     *x = strtod(f.buf, 0);
// }
