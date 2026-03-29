#pragma once

#include "types.h"

SI Str concat__char(Buffer* b, char c)
{
    BufCheckCap(b, 1, NullStr); // Check capacity
    BufAt(b, 0) = c;            // Set buffer
    b->pos++;                   // Update position
    return BufToStr(b, 1);      // Return single char
}

SI Str concat__string(Buffer* b, Str s)
{
    isize len = minimum(b->len - b->pos, s.len);   // Print as much as possible
    memcpy(&BufAt(b, 0), s.buf, len);              //
    b->pos += len;                                 //
    BufCheckIf(b, len != s.len, BufToStr(b, len)); // Set error and return incomplete str
    return BufToStr(b, s.len);                     // Return full string
}

SI Str concat__quoted_string(Buffer* b, Str s)
{
    Buffer bb = BufFromBuffer(b);
    BufSafeConcat(b, (&bb), concat__char((&bb), '"')); // Starting quote
    BufSafeConcat(b, (&bb), concat__string((&bb), s)); // Print as much as possible, maybe return
    BufSafeConcat(b, (&bb), concat__char((&bb), '"')); // Ending quote
    b->pos += bb.pos;                                  // Successful, so update
    return BufToStr((&bb), bb.pos);                    // Return full string
}

SI Str concat__bool(Buffer* b, bool x)
{
    Buffer bb = BufFromBuffer(b);
    Str    s  = x ? _("true") : _("false");            // Representation
    BufSafeConcat(b, (&bb), concat__string((&bb), s)); // Print as much as possible, maybe return
    b->pos += bb.pos;                                  // Successful, so update
    return BufToStr((&bb), bb.pos);                    // Return bool literal
}

SI Str concat__unsigned_int(Buffer* b, u64 x)
{
    BufCheckCap(b, MAX_LEN_INTEGER, NullStr);                  // Harsh, as snprintf can overflow
    Buffer bb  = BufFromBuffer(b);                             //
    bb.pos    += snprintf(&BufAt((&bb), 0), bb.len, "%lu", x); // Print as much as possible
    b->pos    += bb.pos;                                       // Successful, so update
    return BufToStr((&bb), bb.pos);                            // Return printed part, we cannot check if correct
}

SI Str concat__signed_int(Buffer* b, i64 x)
{
    BufCheckCap(b, MAX_LEN_INTEGER, NullStr);                  // Harsh, as snprintf can overflow
    Buffer bb  = BufFromBuffer(b);                             //
    bb.pos    += snprintf(&BufAt((&bb), 0), bb.len, "%ld", x); // Print as much as possible
    b->pos    += bb.pos;                                       // Successful, so update
    return BufToStr((&bb), bb.pos);                            // Return printed part, we cannot check if correct
}

SI Str concat__double(Buffer* b, f64 x)
{
    BufCheckCap(b, MAX_LEN_DOUBLE, NullStr);                  // Harsh, as snprintf can overflow
    Buffer bb  = BufFromBuffer(b);                            //
    bb.pos    += snprintf(&BufAt((&bb), 0), bb.len, "%f", x); // Print as much as possible
    b->pos    += bb.pos;                                      // Successful, so update
    return BufToStr((&bb), bb.pos);                           // Return printed part, we cannot check if correct
}
