#pragma once

#include "consume.h"
#include "user.h"

static inline int parse__Struct_A(Buffer* p, Struct_A* x)
{
    (void)x;

    consume__literal(p, _("{"));

    // Str name;
    // parse__Str(p, &name);
    BufferErrorIf(p->err, -1);
    consume__literal(p, _(" : "));
    BufferErrorIf(p->err, -1);
    // parse__bool(p, &x->bool_a);
    BufferErrorIf(p->err, -1);

    consume__literal(p, _(", "));
    if (p->err)
    {
        p->err = 0;
        consume__literal(p, _("}"));
        BufferErrorIf(p->err, -1);
    }

    return 0;
}
