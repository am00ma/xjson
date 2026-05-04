#pragma once

#include "consume.h"

// --------------- Declaration ---------------

SI Str parse__char(Buffer* b, char* x);
SI Str parse__Str(Buffer* b, Str* x);
SI Str parse__bool(Buffer* b, bool* x);
SI Str parse__u32(Buffer* b, u32* x);
SI Str parse__u64(Buffer* b, u64* x);
SI Str parse__i32(Buffer* b, i32* x);
SI Str parse__i64(Buffer* b, i64* x);
SI Str parse__f32(Buffer* b, f32* x);
SI Str parse__f64(Buffer* b, f64* x);

// --------------- Implementation ---------------

SI Str parse__char(Buffer* b, char* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    ch = consume__anychar((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = ch.buf[0];
    return BufToStr((&bb), bb.pos);
}

SI Str parse__Str(Buffer* b, Str* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    s  = consume__json_string((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = (Str){.buf = &s.buf[1], .len = s.len - 2}; // Parsed -> without qoutes
    return BufToStr((&bb), bb.pos);                        // Quoted, was what was consumed
}

SI Str parse__bool(Buffer* b, bool* x)
{
    BufCheckCap(b, 4, NullStr);
    bool   val = true;
    Buffer bb  = BufFromBuffer(b);
    consume__literal((&bb), _("true"));
    if (bb.err)
    {
        bb = BufFromBuffer(b);
        consume__literal((&bb), _("false"));
        if (!bb.err) val = false;
    }
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = val;
    return BufToStr((&bb), bb.pos);
}

SI Str parse__i32(Buffer* b, i32* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__integer((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtoimax(i.buf, 0, 10);
    return BufToStr((&bb), bb.pos);
}

SI Str parse__u32(Buffer* b, u32* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__integer((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtoumax(i.buf, 0, 10);
    return BufToStr((&bb), bb.pos);
}

SI Str parse__u64(Buffer* b, u64* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__integer((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtoul(i.buf, 0, 10);
    return BufToStr((&bb), bb.pos);
}

SI Str parse__i64(Buffer* b, i64* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__integer((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtol(i.buf, 0, 10);
    return BufToStr((&bb), bb.pos);
}

SI Str parse__f32(Buffer* b, f32* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__number((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtof(i.buf, 0);
    return BufToStr((&bb), bb.pos);
}

SI Str parse__f64(Buffer* b, f64* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__number((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtod(i.buf, 0);
    return BufToStr((&bb), bb.pos);
}

// --------------- TODO: Array ---------------

#define parse__array(type, b, arr)

// --------------- The Infamous X macros ---------------

#define X_PARSE(name, idx, type, ref, field, key, print_fn, parse_fn)                                                  \
    len += consume__quoted_literal(b, _(key)).len;                                                                     \
    len += consume__char(b, ':').len;                                                                                  \
    len += parse_fn(b, ref(x->field)).len;                                                                             \
    len += (idx == X_COUNT_##name - 1) ? 0 : consume__literal(b, _(", ")).len;

#define X_DECLARE_PARSE(name, idx, type, ref, field, key, print_fn, parse_fn)                                          \
    SI Str parse__##name(Buffer* b, name* x)                                                                           \
    {                                                                                                                  \
        isize len  = 0;                                                                                                \
        len       += consume__char(b, '{').len;                                                                        \
        X_TABLE_##name(idx, type, ref, field, key, print_fn, parse_fn);                                                \
        len += consume__char(b, '}').len;                                                                              \
        return BufToStr(b, len);                                                                                       \
    }
