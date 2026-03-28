#pragma once

#include "types.h"

SI Str consume__whitespace(Buffer* p)
{
    BufferErrorIf(p->pos >= p->len, (Str){});
    isize start = p->pos;
    while ((p->pos <= p->len) && is_ws(BufferAt(0))) p->pos++;
    return BufferStr(p, start); // Empty string is ok
}

SI Str consume__char(Buffer* p, char c)
{
    BufferErrorIf(p->pos >= p->len, (Str){});
    BufferErrorIf(BufferAt(0) != c, (Str){});
    p->pos++;
    return BufferStr(p, p->pos - 1);
}

SI Str consume__digit(Buffer* p)
{
    BufferErrorIf(p->pos >= p->len, (Str){});
    BufferErrorIf(!is_digit(BufferAt(0)), (Str){});
    p->pos++;
    return BufferStr(p, p->pos - 1);
}

SI Str consume__digits(Buffer* p)
{
    BufferErrorIf(p->pos >= p->len, (Str){});
    isize start = p->pos;
    while ((p->pos <= p->len) && is_digit(BufferAt(0))) p->pos++;
    BufferErrorIf(p->pos == start, (Str){}); // Empty string is error
    return BufferStr(p, start);
}
