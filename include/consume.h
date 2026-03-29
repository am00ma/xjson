#pragma once

#include "types.h"

SI Str consume__whitespace(Buffer* b)
{
    Buffer bb = BufFromBuffer(b);                                  // Setup local buffer
    while ((bb.pos <= bb.len) && is_ws(BufAt((&bb), 0))) bb.pos++; // Scan
    b->pos += bb.pos;                                              // Successful, so update original
    return BufToStr((&bb), bb.pos);                                // Return from local buffer
}

SI Str consume__anychar(Buffer* p)
{
    BufCheckCap(p, 1, NullStr); // Error if not long enough
    p->pos++;                   // Always succeeds, so no need for local buffer
    return BufToStr(p, 1);      // Return from main buffer
}

SI Str consume__char(Buffer* p, char c)
{
    BufCheckCap(p, 1, NullStr);                  // Error if not long enough
    BufCheckIf(p, !(BufAt(p, 0) == c), NullStr); // Check char
    p->pos++;                                    // Update buffer on success
    return BufToStr(p, 1);                       // Return from main buffer
}

SI Str consume__literal(Buffer* p, Str s)
{
    BufCheckCap(p, s.len, NullStr);                                    // Error if not long enough
    BufCheckIf(p, !str_equal((Str){&BufAt(p, 0), s.len}, s), NullStr); // Check string
    p->pos += s.len;                                                   // Update buffer on success
    return BufToStr(p, s.len);                                         // Return from main buffer
}

SI Str consume__quoted_string(Buffer* p)
{
    BufCheckCap(p, 2, NullStr);                                      // Error if not long enough
    Buffer bb = BufFromBuffer(p);                                    // Setup local buffer
    BufCheckIf(p, (BufAt((&bb), 0) != '"'), NullStr);                // Starting quote
    bb.pos++;                                                        //
    while ((bb.pos <= bb.len) && (BufAt((&bb), 0) != '"')) bb.pos++; // Hack, ignoring escapes for now
    BufCheckCap(p, 1, NullStr);                                      // Unterminated string
    BufCheckIf(p, (BufAt((&bb), 0) != '"'), NullStr);                // Ending quote
    bb.pos++;                                                        //
    p->pos += bb.pos;                                                // Successful, so update buffer
    return (Str){&bb.buf[1], bb.pos - 2};                            // Return from local buffer without quotes
}

SI Str consume__digit(Buffer* p)
{
    BufCheckCap(p, 1, NullStr);                     // Error if not long enough
    BufCheckIf(p, !is_digit(BufAt(p, 0)), NullStr); // One digit
    p->pos++;                                       // Update buffer on success
    return BufToStr(p, 1);                          // Return from main buffer
}

SI Str consume__digits(Buffer* p)
{
    Buffer bb = BufFromBuffer(p);                                     // Setup local buffer
    while ((bb.pos <= bb.len) && is_digit(BufAt((&bb), 0))) bb.pos++; // Within bounds increment on digit
    BufCheckIf(p, !(bb.pos), NullStr);                                // Empty string is error
    p->pos += bb.pos;                                                 // Update buffer on success
    return BufToStr((&bb), bb.pos);                                   // Return from local buffer
}

SI Str consume__integer(Buffer* p)
{
    Buffer bb = BufFromBuffer(p);                   // Setup local buffer
    if (is_sign(BufAt((&bb), 0))) bb.pos++;         // Optional sign
    BufSafeConsume(p, (&bb), consume__digits(&bb)); // Digits or return error
    p->pos += bb.pos;                               // Update buffer on success
    return BufToStr((&bb), bb.pos);                 // Return from local buffer
}

SI Str consume__fraction(Buffer* p)
{
    BufCheckCap(p, 1, NullStr);                        // Error if not long enough
    Buffer bb = BufFromBuffer(p);                      // Setup local buffer
    BufCheckIf(p, !(BufAt((&bb), 0) == '.'), NullStr); // Mandatory dot
    bb.pos++;                                          // Update position
    BufSafeConsume(p, (&bb), consume__digits(&bb));    // Digits or return error
    p->pos += bb.pos;                                  // Update buffer on success
    return BufToStr((&bb), bb.pos);                    // Return from local buffer
}
