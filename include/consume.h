#pragma once

#include "types.h"

SI Str consume__whitespace(Buffer* p)
{
    Buffer bb = BufferLocal(p);                                       // Setup local buffer
    while ((bb.pos <= bb.len) && is_ws(BufferAt((&bb), 0))) bb.pos++; // Scan
    p->pos += bb.pos;                                                 // Successful, so update original
    return BufferStr((&bb), 0);                                       // Return from local buffer
}

SI Str consume__anychar(Buffer* p)
{
    BufferCheckCap(1, (Str){});      // Error if not long enough
    p->pos++;                        // Always succeeds, so no need for local buffer
    return BufferStr(p, p->pos - 1); // Return from main buffer
}

SI Str consume__char(Buffer* p, char c)
{
    BufferCheckCap(1, (Str){});                     // Error if not long enough
    BufferErrorIf(!(BufferAt(p, 0) == c), (Str){}); // Check exact equality
    p->pos++;                                       // Update buffer on success
    return BufferStr(p, p->pos - 1);                // Return from main buffer
}

SI Str consume__literal(Buffer* p, Str s)
{
    BufferCheckCap(s.len, (Str){});                                       // Error if not long enough
    BufferErrorIf(!str_equal((Str){&BufferAt(p, 0), s.len}, s), (Str){}); // Check exact equality
    p->pos += s.len;                                                      // Update buffer on success
    return BufferStr(p, p->pos - s.len);                                  // Return from main buffer
}

SI Str consume__quoted_string(Buffer* p)
{
    BufferCheckCap(2, (Str){});                                         // Error if not long enough
    Buffer bb = BufferLocal(p);                                         // Setup local buffer
    BufferErrorIf((BufferAt((&bb), 0) != '"'), (Str){});                // Starting quote
    bb.pos++;                                                           //
    while ((bb.pos <= bb.len) && (BufferAt((&bb), 0) != '"')) bb.pos++; // Hack, ignoring escapes for now
    BufferErrorIf(bb.pos + 1 > bb.len, (Str){});                        // Unterminated string
    BufferErrorIf((BufferAt((&bb), 0) != '"'), (Str){});                // Ending quote
    bb.pos++;                                                           //
    p->pos += bb.pos;                                                   // Successful, so update buffer
    return (Str){&bb.buf[1], bb.pos - 2};                               // Return from local buffer
}

// SI Str consume__digit(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     BufferErrorIf(!is_digit(BufferAt(0)), (Str){});
//     p->pos++;
//     return BufferStr(p, p->pos - 1);
// }

// SI Str consume__digits(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     isize start   = p->pos;
//     isize d_start = p->pos;                                       // Seq of digits
//     while ((p->pos <= p->len) && is_digit(BufferAt(0))) p->pos++; //
//     BufferErrorIf(p->pos == d_start, (Str){});                    // Empty string is error
//     return BufferStr(p, start);
// }

// SI Str consume__integer(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     isize start = p->pos;
//     if (is_sign(BufferAt(0))) p->pos++; // Optional sign
//     consume__digits(p);                 // Seq of digits
//     BufferErrorIf(p->err, (Str){});     // Pass on error
//     return BufferStr(p, start);
// }

// SI Str consume__fraction(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     isize start = p->pos;
//     if (BufferAt(0) == '.') p->pos++; // `.`
//     else return BufferStr(p, start);  // Empty string is ok
//     consume__digits(p);               // Seq of digits
//     BufferErrorIf(p->err, (Str){});   // Pass on error
//     return BufferStr(p, start);
// }

// SI Str consume__exponent(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     isize start = p->pos;
//     if ((BufferAt(0) == 'e') || (BufferAt(0) == 'E')) p->pos++; // `e/E`
//     else return BufferStr(p, start);                            // Empty string is ok
//     if (is_sign(BufferAt(0))) p->pos++;                         // Optional sign
//     consume__digits(p);                                         // Seq of digits
//     BufferErrorIf(p->err, (Str){});                             // Pass on error
//     return BufferStr(p, start);
// }

// SI Str consume__number(Buffer* p)
// {
//     BufferErrorIf(p->pos > p->len, (Str){});
//     isize start = p->pos;
//     consume__integer(p);
//     BufferErrorIf(p->err, (Str){});
//     consume__fraction(p);
//     BufferErrorIf(p->err, (Str){});
//     consume__exponent(p);
//     BufferErrorIf(p->err, (Str){});
//     return BufferStr(p, start);
// }
