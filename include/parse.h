#pragma once

#include "consume.h"

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
    Str    s  = consume__quoted_string((&bb));
    if (b->err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = (Str){.buf = &s.buf[1], .len = s.len - 1}; // Parsed -> without qoutes
    return BufToStr((&bb), bb.pos);                        // Quoted was what was consumed
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

SI Str parse__i64(Buffer* b, i64* x)
{
    Buffer bb = BufFromBuffer(b);
    Str    i  = consume__integer((&bb));
    if (bb.err) return NullStr;
    b->pos += bb.pos;
    if (x) *x = strtol(i.buf, 0, 10);
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

// SI Str parse__f32(Buffer* b, f32* x)
// {
//     Buffer bb = BufFromBuffer(b);
//     Str    i  = consume__integer((&bb));
//     if (bb.err) return NullStr;
//     b->pos += bb.pos;
//     if (x) *x = strtof(i.buf, 0);
//     return BufToStr((&bb), bb.pos);
// }

// SI Str parse__f64(Buffer* b, f64* x)
// {
//     Buffer bb = BufFromBuffer(b);
//     Str    i  = consume__integer((&bb));
//     if (bb.err) return NullStr;
//     b->pos += bb.pos;
//     if (x) *x = strtod(i.buf, 0);
//     return BufToStr((&bb), bb.pos);
// }
