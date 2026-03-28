#pragma once

#include "types.h"

SI Str concat__char(Buffer* p, char c)
{
    BufferErrorIf(p->pos + 1 >= p->len, (Str){});
    BufferAt(0) = c;
    p->pos++;
    return BufferStr(p, p->pos - 1);
}

SI Str concat__string(Buffer* p, Str c)
{
    BufferErrorIf(p->pos + c.len >= p->len, (Str){});
    memcpy(&BufferAt(0), c.buf, c.len);
    p->pos += c.len;
    return BufferStr(p, p->pos - c.len);
}

SI Str concat__quoted_string(Buffer* p, Str c)
{
    BufferErrorIf(p->pos + c.len + 2 >= p->len, (Str){});

    isize start = p->pos;
    BufferAt(0) = '"';
    memcpy(&BufferAt(1), c.buf, c.len);
    BufferAt(c.len + 1) = '"';

    p->pos += c.len + 2;
    return BufferStr(p, start);
}

SI Str concat__bool(Buffer* p, bool x) { return x ? concat__string(p, _("true")) : concat__string(p, _("false")); }

SI Str concat__signed_int(Buffer* p, i64 x)
{
    BufferErrorIf(p->pos + MAX_LEN_INTEGER >= p->len, (Str){});
    isize start  = p->pos;
    p->pos      += snprintf(&BufferAt(0), MAX_LEN_INTEGER, "%ld", x);
    return BufferStr(p, start);
}

SI Str concat__unsigned_int(Buffer* p, u64 x)
{
    BufferErrorIf(p->pos + MAX_LEN_INTEGER >= p->len, (Str){});
    isize start  = p->pos;
    p->pos      += snprintf(&BufferAt(0), MAX_LEN_INTEGER, "%lu", x);
    return BufferStr(p, start);
}

SI Str concat__double(Buffer* p, f64 x)
{
    BufferErrorIf(p->pos + MAX_LEN_DOUBLE >= p->len, (Str){});
    isize start  = p->pos;
    p->pos      += snprintf(&BufferAt(0), MAX_LEN_DOUBLE, "%f", x);
    return BufferStr(p, start);
}
