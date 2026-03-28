#pragma once

#include "types.h"

SI Str consume__whitespace(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    while ((p->pos <= p->len) && is_ws(BufferAt(0))) p->pos++;
    return BufferStr(p, start); // Empty string is ok
}

SI Str consume__char(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    p->pos++;
    return BufferStr(p, p->pos - 1);
}

SI Str consume__literal(Buffer* p, Str s)
{
    BufferErrorIf(p->pos + s.len > p->len, (Str){});
    isize start = p->pos;
    Str   dst   = {&p->buf[p->pos], s.len};
    BufferErrorIf(!str_equal(dst, s), (Str){});
    p->pos += s.len;
    return BufferStr(p, start);
}

SI Str consume__quoted_string(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    BufferErrorIf((BufferAt(0) != '"'), (Str){});                // Starting quote
    p->pos++;                                                    //
    while ((p->pos <= p->len) && (BufferAt(0) != '"')) p->pos++; // Hack, ignoring escapes for now
    BufferErrorIf(p->pos > p->len, (Str){});                     // Unterminated string
    BufferErrorIf((BufferAt(0) != '"'), (Str){});                // Ending quote
    p->pos++;                                                    //
    return BufferStr(p, start);
}

SI Str consume__digit(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    BufferErrorIf(!is_digit(BufferAt(0)), (Str){});
    p->pos++;
    return BufferStr(p, p->pos - 1);
}

SI Str consume__digits(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start   = p->pos;
    isize d_start = p->pos;                                       // Seq of digits
    while ((p->pos <= p->len) && is_digit(BufferAt(0))) p->pos++; //
    BufferErrorIf(p->pos == d_start, (Str){});                    // Empty string is error
    return BufferStr(p, start);
}

SI Str consume__integer(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    if (is_sign(BufferAt(0))) p->pos++; // Optional sign
    consume__digits(p);                 // Seq of digits
    BufferErrorIf(p->err, (Str){});     // Pass on error
    return BufferStr(p, start);
}

SI Str consume__fraction(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    if (BufferAt(0) == '.') p->pos++; // `.`
    else return BufferStr(p, start);  // Empty string is ok
    consume__digits(p);               // Seq of digits
    BufferErrorIf(p->err, (Str){});   // Pass on error
    return BufferStr(p, start);
}

SI Str consume__exponent(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    if ((BufferAt(0) == 'e') || (BufferAt(0) == 'E')) p->pos++; // `e/E`
    else return BufferStr(p, start);                            // Empty string is ok
    if (is_sign(BufferAt(0))) p->pos++;                         // Optional sign
    consume__digits(p);                                         // Seq of digits
    BufferErrorIf(p->err, (Str){});                             // Pass on error
    return BufferStr(p, start);
}

SI Str consume__number(Buffer* p)
{
    BufferErrorIf(p->pos > p->len, (Str){});
    isize start = p->pos;
    consume__integer(p);
    BufferErrorIf(p->err, (Str){});
    consume__fraction(p);
    BufferErrorIf(p->err, (Str){});
    consume__exponent(p);
    BufferErrorIf(p->err, (Str){});
    return BufferStr(p, start);
}
