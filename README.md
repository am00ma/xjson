# xjson

Goal:

- Automatic serialization and deserialization of structs.
- Support for nested structs.

## Abstract

Field:

- golang -> JSON.Marshall, JSON.UnMarshall
- python -> dataclass

Research Gap:

- No such lib for C
- No standard way of representing arrays

Novelty:

- [x] Nested parsing
- [ ] Support for arrays
- [ ] X-Macros to derive subsets

Evidence:

- [x] test_parse_nested
- [x] test_print_nested
- [ ] test_parse_array
- [ ] test_print_array

Conclusion: Works

## Functional Requirements

- [ ] Support for primitive types
- [ ] Support for pointers
- [ ] Support for arrays
- [ ] Support for counted stuff
- [ ] Nested parsing
- [ ] Pretty printing
- [ ] Various in/out formats

## Usage

- Define some custom struct
- Set reflection X-Macro table
- Call DECLARE X-Macro to define
  - struct
  - enum + count
  - print
  - parse

## Timeline

- 28th March: Beginnings - printing is working
- 4th May: Restructure - Make clear what needs to be done/demonstrated

## Sections

### concat

Supported types:

```c
SI Str concat__char(Buffer* b, char c);
SI Str concat__string(Buffer* b, Str s);
SI Str concat__quoted_string(Buffer* b, Str s);
SI Str concat__bool(Buffer* b, bool x);
SI Str concat__unsigned_int(Buffer* b, u64 x);
SI Str concat__signed_int(Buffer* b, i64 x);
SI Str concat__double(Buffer* b, f64 x);
```

### consume

Supported types:

```c
SI Str consume__whitespace(Buffer* b);
SI Str consume__anychar(Buffer* b);
SI Str consume__json_string(Buffer* b);
SI Str consume__quoted_string(Buffer* b);
SI Str consume__digit(Buffer* b);
SI Str consume__digits(Buffer* b);
SI Str consume__integer(Buffer* b);
SI Str consume__fraction(Buffer* b);
SI Str consume__exponent(Buffer* b);
SI Str consume__number(Buffer* b);

// With extra args
SI Str consume__char(Buffer* b, char c);
SI Str consume__literal(Buffer* b, Str s);
SI Str consume__quoted_literal(Buffer* b, Str s);
```

### print

Primitives:

```c
Str print__char(Buffer* b, char* x);
Str print__Str(Buffer* b, Str* x);
Str print__bool(Buffer* b, bool* x);
Str print__u32(Buffer* b, u32* x);
Str print__u64(Buffer* b, u64* x);
Str print__i32(Buffer* b, i32* x);
Str print__i64(Buffer* b, i64* x);
Str print__f32(Buffer* b, f32* x);
Str print__f64(Buffer* b, f64* x);
```

### parse

Primitives:

```c
SI Str parse__char(Buffer* b, char* x);
SI Str parse__Str(Buffer* b, Str* x);
SI Str parse__bool(Buffer* b, bool* x);
SI Str parse__u32(Buffer* b, u32* x);
SI Str parse__u64(Buffer* b, u64* x);
SI Str parse__i32(Buffer* b, i32* x);
SI Str parse__i64(Buffer* b, i64* x);
SI Str parse__f32(Buffer* b, f32* x);
SI Str parse__f64(Buffer* b, f64* x);
```

## X-Macros

Arguments used for reflection:

- name: Needed to define `print__##name(...)`, to get count `X_COUNT_##name`
- idx: Used to check if in final field `idx == X_COUNT_##name - 1`
- type: seems like it is not used
- ref: Used in `print__##name(b, ref(x))` to call either `print__(x)` or `print__(&x)`
- field: Used to put/get `x->field`
- key: Used in printing/parsing key of json member
- print_fn: Make explicit which func is used
- parse_fn: Make explicit which func is used

`__VA_ARGS__` vs listing all the args in order:

- TODO: Need experiments to find out what is happening

### print

Header file declaration:

```c
#define X_PRINT(name, idx, type, ref, field, key, print_fn, parse_fn)                                                  \
    len += concat__quoted_string(b, _(key)).len;                                                                       \
    len += concat__char(b, ':').len;                                                                                   \
    len += print_fn(b, ref(x->field)).len;                                                                             \
    len += (idx == X_COUNT_##name - 1) ? 0 : concat__string(b, _(", ")).len;

#define X_DECLARE_PRINT(name, idx, type, ref, field, key, print_fn, parse_fn)                                          \
    SI Str print__##name(Buffer* b, name* x)                                                                           \
    {                                                                                                                  \
        isize len  = 0;                                                                                                \
        len       += concat__char(b, '{').len;                                                                         \
        X_TABLE_##name(idx, type, ref, field, key, print_fn, parse_fn);                                                \
        len += concat__char(b, '}').len;                                                                               \
        return BufToStr(b, len);                                                                                       \
    }
```

Usage:

```c
typedef struct
{
    i32 idx;
    u64 len;
    Str a;

} Struct_A;

// Table and count for reflection
#define X_TABLE_Struct_A(idx, type, ref, field, key, print_fn, parse_fn)                                               \
    X(0, i32, &, idx, "idx", print__i32, NULL)                                                                         \
    X(1, u64, &, len, "len", print__u64, NULL)                                                                         \
    X(2, Str, &, a, "a", print__Str, NULL)
#define X_COUNT_Struct_A 3

// Serialize -> print__Struct_A
#define X(...) X_PRINT(Struct_A, __VA_ARGS__)
X_DECLARE_PRINT(Struct_A, idx, type, ref, field, key, print_fn, parse_fn);
#undef X
```

### parse

Header file declaration:

```c
#define X_PARSE(name, idx, type, ref, field, key, print_fn, parse_fn)                                                  \
    len += consume__quoted_literal(b, _(key)).len;                                                                     \
    len += consume__char(b, ':').len;                                                                                  \
    len += parse_fn(b, ref(x->field)).len;                                                                             \
    len += (idx == X_COUNT_##name - 1) ? 0 : consume__literal(b, _(", ")).len;

#define X_DECLARE_PARSE(name, idx, type, ref, field, key, print_fn, parse_fn)                                          \
    SI Str parse__##name(Buffer* b, name* x)                                                                           \
    {                                                                                                                  \
        isize len  = 0;                                                                                                \
        len       += consume__char(b, '{').len;                                                                        \
        X_TABLE_##name(idx, type, ref, field, key, print_fn, parse_fn);                                                \
        len += consume__char(b, '}').len;                                                                              \
        return BufToStr(b, len);                                                                                       \
    }
```

Usage:

```c
typedef struct
{
    i32 idx;
    u64 len;
    f32 f;
    Str a;

} Struct_A;

// Table and count for reflection
#define X_TABLE_Struct_A(idx, type, ref, field, key, print_fn, parse_fn)                                               \
    X(0, i32, &, idx, "idx", , parse__i32)                                                                             \
    X(1, u64, &, len, "len", , parse__u64)                                                                             \
    X(2, f32, &, f, "f", , parse__f32)                                                                                 \
    X(3, Str, &, a, "a", , parse__Str)
#define X_COUNT_Struct_A 4

// Deserialize -> parse__Struct_A
#define X(...) X_PARSE(Struct_A, __VA_ARGS__)
X_DECLARE_PARSE(Struct_A, idx, type, ref, field, key, print_fn, parse_fn);
#undef X
```
