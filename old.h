// /* ---------------------------------------------------------------------------
//  * Helpers
//  * ------------------------------------------------------------------------- */
//
// SI Str consume_whitespace(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     while (is_ws(ParserAt(len))) len++;
//     return ParserStr(p);
// }
//
// SI Str consume_char(Parser* p, char c)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     ParserErrorIf((ParserAt(len) == c), (Str){});
//     len++;
//     return ParserStr(p);
// }
//
// SI Str consume_literal(Parser* p, Str s)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos + s.len >= p->len, (Str){});
//
//     p->pos  += s.len;
//     Str dst  = ParserStr(p);
//     ParserErrorIf(!str_equal(dst, s), (Str){});
//
//     return dst;
// }
//
// SI Str consume_bool(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     switch (ParserAt(len))
//     {
//     case 't': return consume_literal(p, _("true")); break;
//     case 'f': return consume_literal(p, _("false")); break;
//     default: ParserErrorIf(true, (Str){}); break;
//     }
//
//     ParserErrorIf(true, (Str){}); // Unreachable
// }
//
// SI Str consume_digits(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     ParserErrorIf(is_digit(ParserAt(len)), (Str){});
//     while (is_digit(ParserAt(len))) len++;
//
//     return ParserStr(p);
// }
//
// SI Str consume_integer(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     if (is_sign(ParserAt(len))) len++; // Optional Sign
//     len += consume_digits(p).len;      // Mandatory digits
//     ParserErrorIf((p->err), (Str){});  //
//
//     return ParserStr(p);
// }
//
// SI Str consume_double(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//
//     len += consume_integer(p).len; // Mandatory integer
//     ParserErrorIf((p->err), (Str){});
//
//     if (ParserAt(len) == '.') // Optional fraction
//     {
//         len++;
//         len += consume_digits(p).len;
//         ParserErrorIf((p->err), (Str){});
//     }
//
//     if ((ParserAt(len) == 'e') || (ParserAt(len) == 'E')) // Optional exponent
//     {
//         len++;
//         len += consume_integer(p).len;
//         ParserErrorIf((p->err), (Str){});
//     }
//
//     return ParserStr(p);
// }
//
// SI Str consume_str(Parser* p)
// {
//     p->start = p->pos;
//     ParserErrorIf(p->pos >= p->len, (Str){});
//
//     isize len = 0;
//     ParserErrorIf((ParserAt(len) == '"'), (Str){});
//     len++;
//
//     ParserErrorIf(p->pos >= p->len, (Str){});
//     ParserErrorIf((ParserAt(len) == '"'), (Str){});
//     len++;
//
//     return ParserStr(p);
// }
//
// /* ---------------------------------------------------------------------------
//  * API
//  * ------------------------------------------------------------------------- */
//
// static inline int parse__bool(Parser* p, bool* val)
// {
//     Str x = consume_bool(p);
//     if (p->err) return p->err;
//     *val = !str_equal(x, _("true"));
//     return 0;
// }
//
// static inline int parse__u64(Parser* p, u64* val)
// {
//     Str x = consume_integer(p);
//     if (p->err) return p->err;
//     *val = strtoul(x.buf, 0, 10);
//     return 0;
// }
//
// static inline int parse__i32(Parser* p, i32* val)
// {
//     Str x = consume_integer(p);
//     if (p->err) return p->err;
//     *val = strtoimax(x.buf, 0, 10);
//     return 0;
// }
//
// static inline int parse__f64(Parser* p, f64* val)
// {
//     Str x = consume_double(p);
//     if (p->err) return p->err;
//     *val = strtod(x.buf, 0);
//     return 0;
// }
//
// static inline int parse__Str(Parser* p, Str* val)
// {
//     Str x = consume_str(p);
//     if (p->err) return p->err;
//     *val = x;
//     return 0;
// }
