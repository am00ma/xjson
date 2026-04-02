#pragma once

#include "types.h"

SI Str consume__whitespace(Buffer* b)
{
    Buffer bb = BufFromBuffer(b);                                  // Setup local buffer
    while ((bb.pos <= bb.len) && is_ws(BufAt((&bb), 0))) bb.pos++; // Scan
    b->pos += bb.pos;                                              // Successful, so update original
    return BufToStr((&bb), bb.pos);                                // Return from local buffer
}

SI Str consume__anychar(Buffer* b)
{
    BufCheckCap(b, 1, NullStr); // Error if not long enough
    b->pos++;                   // Always succeeds, so no need for local buffer
    return BufToStr(b, 1);      // Return from main buffer
}

SI Str consume__char(Buffer* b, char c)
{
    BufCheckCap(b, 1, NullStr);                  // Error if not long enough
    BufCheckIf(b, !(BufAt(b, 0) == c), NullStr); // Check char
    b->pos++;                                    // Update buffer on success
    return BufToStr(b, 1);                       // Return from main buffer
}

SI Str consume__literal(Buffer* b, Str s)
{
    BufCheckCap(b, s.len, NullStr);                                    // Error if not long enough
    BufCheckIf(b, !str_equal((Str){&BufAt(b, 0), s.len}, s), NullStr); // Check string
    b->pos += s.len;                                                   // Update buffer on success
    return BufToStr(b, s.len);                                         // Return from main buffer
}

SI Str consume__quoted_literal(Buffer* b, Str s)
{
    BufCheckCap(b, s.len + 2, NullStr);                                // Error if not long enough
    BufCheckIf(b, (BufAt(b, 0) != '"'), NullStr);                      // Starting quote
    BufCheckIf(b, !str_equal((Str){&BufAt(b, 1), s.len}, s), NullStr); // Check string
    BufCheckIf(b, (BufAt(b, s.len + 1) != '"'), NullStr);              // Starting quote
    b->pos += s.len + 2;                                               // Update buffer on success
    return BufToStr(b, s.len + 2); // Return from main buffer (what was consumed, not what was parsed)
}

SI Str consume__json_string(Buffer* b)
{
    BufCheckCap(b, 2, NullStr);                       // Error if not long enough
    Buffer bb = BufFromBuffer(b);                     // Setup local buffer
    BufCheckIf(b, (BufAt((&bb), 0) != '"'), NullStr); // Starting quote
    bb.pos++;                                         //

    while (bb.pos <= bb.len)
    {
        switch (BufAt((&bb), 0))
        {
        case '"':
            bb.pos++;
            goto __done;
            break;
        case '\\': {
            bb.pos++;
            BufCheckCap((&bb), 1, NullStr);
            if (is_escape(BufAt((&bb), 0))) { bb.pos++; }
            else if (BufAt((&bb), 0) == 'u')
            {
                bb.pos++;
                BufCheckCap((&bb), 4, NullStr);
                if (is_hex(BufAt((&bb), 0))    //
                    && is_hex(BufAt((&bb), 1)) //
                    && is_hex(BufAt((&bb), 2)) //
                    && is_hex(BufAt((&bb), 3)))
                {
                    bb.pos += 4;
                }
                else BufCheckIf(b, true, NullStr);
            }
            else
            {
                BufCheckIf(b, true, NullStr);
            }
        }
        break;
        default: bb.pos++; break;
        }
    }

__done:
    BufCheckIf(b, (BufAt(b, bb.pos - 1) != '"'), NullStr); // Last char was quote
    b->pos += bb.pos;                                      // Successful, so update buffer
    return BufToStr((&bb), bb.pos); // Return from main buffer (what was consumed, not what was parsed)
}

SI Str consume__quoted_string(Buffer* b)
{
    BufCheckCap(b, 2, NullStr);                                      // Error if not long enough
    Buffer bb = BufFromBuffer(b);                                    // Setup local buffer
    BufCheckIf(b, (BufAt((&bb), 0) != '"'), NullStr);                // Starting quote
    bb.pos++;                                                        //
    while ((bb.pos <= bb.len) && (BufAt((&bb), 0) != '"')) bb.pos++; // HACK: ignoring escapes for now
    BufCheckCap((&bb), 1, NullStr);                                  // Unterminated string
    BufCheckIf(b, (BufAt((&bb), 0) != '"'), NullStr);                // Ending quote
    bb.pos++;                                                        //
    b->pos += bb.pos;                                                // Successful, so update buffer
    return BufToStr((&bb), bb.pos); // Return from main buffer (what was consumed, not what was parsed)
}

SI Str consume__digit(Buffer* b)
{
    BufCheckCap(b, 1, NullStr);                     // Error if not long enough
    BufCheckIf(b, !is_digit(BufAt(b, 0)), NullStr); // One digit
    b->pos++;                                       // Update buffer on success
    return BufToStr(b, 1);                          // Return from main buffer
}

SI Str consume__digits(Buffer* b)
{
    Buffer bb = BufFromBuffer(b);                                     // Setup local buffer
    while ((bb.pos <= bb.len) && is_digit(BufAt((&bb), 0))) bb.pos++; // Within bounds increment on digit
    BufCheckIf(b, !(bb.pos), NullStr);                                // Empty string is error
    b->pos += bb.pos;                                                 // Update buffer on success
    return BufToStr((&bb), bb.pos);                                   // Return from local buffer
}

SI Str consume__integer(Buffer* b)
{
    Buffer bb = BufFromBuffer(b);                   // Setup local buffer
    if (is_sign(BufAt((&bb), 0))) bb.pos++;         // Optional sign
    BufSafeConsume(b, (&bb), consume__digits(&bb)); // Digits or return error
    b->pos += bb.pos;                               // Update buffer on success
    return BufToStr((&bb), bb.pos);                 // Return from local buffer
}

SI Str consume__fraction(Buffer* b)
{
    if (BufAt(b, 0) != '.') return NullStr;            // Empty string is ok
    Buffer bb = BufFromBuffer(b);                      // Setup local buffer
    BufCheckCap((&bb), 1, NullStr);                    // Error if not long enough
    BufCheckIf(b, !(BufAt((&bb), 0) == '.'), NullStr); // Mandatory dot
    bb.pos++;                                          // Update position
    BufSafeConsume(b, (&bb), consume__digits(&bb));    // Digits or return error
    b->pos += bb.pos;                                  // Update buffer on success
    return BufToStr((&bb), bb.pos);                    // Return from local buffer
}

SI Str consume__exponent(Buffer* b)
{
    if (!is_exp(BufAt(b, 0))) return NullStr;           // Empty string is ok
    Buffer bb = BufFromBuffer(b);                       // Setup local buffer
    BufCheckCap((&bb), 1, NullStr);                     // Error if not long enough
    BufCheckIf(b, !(is_exp(BufAt((&bb), 0))), NullStr); // Mandatory exponent
    bb.pos++;                                           // Update position
    BufSafeConsume(b, (&bb), consume__integer(&bb));    // Integer or return error
    b->pos += bb.pos;                                   // Update buffer on success
    return BufToStr((&bb), bb.pos);                     // Return from local buffer
}

SI Str consume__number(Buffer* b)
{
    Buffer bb = BufFromBuffer(b);                     // Setup local buffer
    BufSafeConsume(b, (&bb), consume__integer(&bb));  // Integer or return error
    BufSafeConsume(b, (&bb), consume__fraction(&bb)); // Fraction or return error
    BufSafeConsume(b, (&bb), consume__exponent(&bb)); // Exponent or return error
    b->pos += bb.pos;                                 // Update buffer on success
    return BufToStr((&bb), bb.pos);                   // Return from local buffer
}
