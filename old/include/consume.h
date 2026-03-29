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
    BufferCheckCap(p, 1, (Str){});   // Error if not long enough
    p->pos++;                        // Always succeeds, so no need for local buffer
    return BufferStr(p, p->pos - 1); // Return from main buffer
}

SI Str consume__char(Buffer* p, char c)
{
    BufferCheckCap(p, 1, (Str){});                     // Error if not long enough
    BufferErrorIf(p, !(BufferAt(p, 0) == c), (Str){}); // Check exact equality
    p->pos++;                                          // Update buffer on success
    return BufferStr(p, p->pos - 1);                   // Return from main buffer
}

SI Str consume__literal(Buffer* p, Str s)
{
    BufferCheckCap(p, s.len, (Str){});                                       // Error if not long enough
    BufferErrorIf(p, !str_equal((Str){&BufferAt(p, 0), s.len}, s), (Str){}); // Check exact equality
    p->pos += s.len;                                                         // Update buffer on success
    return BufferStr(p, p->pos - s.len);                                     // Return from main buffer
}

SI Str consume__quoted_string(Buffer* p)
{
    BufferCheckCap(p, 2, (Str){});                                      // Error if not long enough
    Buffer bb = BufferLocal(p);                                         // Setup local buffer
    BufferErrorIf(p, (BufferAt((&bb), 0) != '"'), (Str){});             // Starting quote
    bb.pos++;                                                           //
    while ((bb.pos <= bb.len) && (BufferAt((&bb), 0) != '"')) bb.pos++; // Hack, ignoring escapes for now
    BufferErrorIf(p, bb.pos + 1 > bb.len, (Str){});                     // Unterminated string
    BufferErrorIf(p, (BufferAt((&bb), 0) != '"'), (Str){});             // Ending quote
    bb.pos++;                                                           //
    p->pos += bb.pos;                                                   // Successful, so update buffer
    return (Str){&bb.buf[1], bb.pos - 2};                               // Return from local buffer
}

SI Str consume__digit(Buffer* p)
{
    BufferCheckCap(p, 1, (Str){});                        // Error if not long enough
    BufferErrorIf(p, !is_digit(BufferAt(p, 0)), (Str){}); // One digit
    p->pos++;                                             // Update buffer on success
    return BufferStr(p, p->pos - 1);                      // Return from main buffer
}

SI Str consume__digits(Buffer* p)
{
    Buffer bb = BufferLocal(p);                                          // Setup local buffer
    while ((bb.pos <= bb.len) && is_digit(BufferAt((&bb), 0))) bb.pos++; // Within bounds increment on digit
    BufferErrorIf(p, !(bb.pos), (Str){});                                // Empty string is error
    p->pos += bb.pos;                                                    // Update buffer on success
    return BufferStr((&bb), 0);                                          // Return from local buffer
}

SI Str consume__integer(Buffer* p)
{
    Buffer bb = BufferLocal(p);                        // Setup local buffer
    if (is_sign(BufferAt((&bb), 0))) bb.pos++;         // Optional sign
    BufferSafeConsume(p, (&bb), consume__digits(&bb)); // Digits or return error
    p->pos += bb.pos;                                  // Update buffer on success
    return BufferStr((&bb), 0);                        // Return from local buffer
}

SI Str consume__fraction(Buffer* p)
{
    BufferCheckCap(p, 1, (Str){});                           // Error if not long enough
    Buffer bb = BufferLocal(p);                              // Setup local buffer
    BufferErrorIf(p, !(BufferAt((&bb), 0) == '.'), (Str){}); // Mandatory dot
    bb.pos++;                                                // Update position
    BufferSafeConsume(p, (&bb), consume__digits(&bb));       // Digits or return error
    p->pos += bb.pos;                                        // Update buffer on success
    return BufferStr((&bb), 0);                              // Return from local buffer
}

// SI Str consume__exponent(Buffer* p)
// {
//     BufferErrorIf(p, p->pos > p->len, (Str){});
//     isize start = p->pos;
//     if ((BufferAt(0) == 'e') || (BufferAt(0) == 'E')) p->pos++; // `e/E`
//     else return BufferStr(p, start);                            // Empty string is ok
//     if (is_sign(BufferAt(0))) p->pos++;                         // Optional sign
//     consume__digits(p);                                         // Seq of digits
//     BufferErrorIf(p, p->err, (Str){});                             // Pass on error
//     return BufferStr(p, start);
// }

// SI Str consume__number(Buffer* p)
// {
//     BufferErrorIf(p, p->pos > p->len, (Str){});
//     isize start = p->pos;
//     consume__integer(p);
//     BufferErrorIf(p, p->err, (Str){});
//     consume__fraction(p);
//     BufferErrorIf(p, p->err, (Str){});
//     consume__exponent(p);
//     BufferErrorIf(p, p->err, (Str){});
//     return BufferStr(p, start);
// }
