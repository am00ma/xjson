#pragma once

#include "types.h"

SI Str concat__char(Buffer* p, char c)
{
    BufferCheckCap(1, (Str){});      // Check capacity
    BufferAt(p, 0) = c;              // Set buffer
    p->pos++;                        // Update position
    return BufferStr(p, p->pos - 1); // Return only char
}

SI Str concat__string(Buffer* p, Str c)
{
    isize len = minimum(p->len - p->pos, c.len);             // Print as much as possible
    memcpy(&BufferAt(p, 0), c.buf, len);                     //
    p->pos += len;                                           //
    BufferErrorIf(len != c.len, BufferStr(p, p->pos - len)); // Set error if incomplete
    return BufferStr(p, p->pos - c.len);                     // Return full string
}

SI Str concat__quoted_string(Buffer* p, Str c)
{
    isize start = p->pos;                       // Mark start
    BufferSafe(p, concat__char(p, '"'), start); // Starting quote
    BufferSafe(p, concat__string(p, c), start); // Print as much as possible
    BufferSafe(p, concat__char(p, '"'), start); // Ending quote
    return BufferStr(p, start);                 // Return quoted string
}

SI Str concat__bool(Buffer* p, bool x)
{
    isize start = p->pos;                       // Mark start
    Str   s     = x ? _("true") : _("false");   // Representation
    BufferSafe(p, concat__string(p, s), start); // Print as much as possible
    return BufferStr(p, start);                 // Return bool as literal
}

SI Str concat__unsigned_int(Buffer* p, u64 x)
{
    BufferCheckCap(MAX_LEN_INTEGER, (Str){});                            // Harsh, as snprintf can overflow
    isize start  = p->pos;                                               // Mark start
    p->pos      += snprintf(&BufferAt(p, 0), p->len - p->pos, "%lu", x); // Print as much as possible
    return BufferStr(p, start);                                          // Return printed part
}

SI Str concat__signed_int(Buffer* p, i64 x)
{
    BufferCheckCap(MAX_LEN_INTEGER, (Str){});                            // Harsh, as snprintf can overflow
    isize start  = p->pos;                                               // Mark start
    p->pos      += snprintf(&BufferAt(p, 0), p->len - p->pos, "%ld", x); // Print as much as possible
    return BufferStr(p, start);                                          // Return printed part
}

SI Str concat__double(Buffer* p, f64 x)
{
    BufferCheckCap(MAX_LEN_DOUBLE, (Str){});                            // Harsh, as snprintf can overflow
    isize start  = p->pos;                                              // Mark start
    p->pos      += snprintf(&BufferAt(p, 0), p->len - p->pos, "%f", x); // Print as much as possible
    return BufferStr(p, start);                                         // Return printed part
}
