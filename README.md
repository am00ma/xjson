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

Novelty:

- X-Macros to derive subsets
- Support for arrays
- Nested parsing

Evidence:

```c
typedef struct
{

    bool bool_a;
    u64  u64_a;
    u64  u64_b;
    i32  i32_a;
    f64  f64_a;
    Str  str_a;
    Str  str_b;

} Struct_A;

typedef struct
{

    struct
    {
        u64*  buf;
        isize len;
    } u64s_;

    struct
    {
        f32*  buf;
        isize len;
    } f32s_;

    struct
    {
        Struct_A* buf;
        isize     len;
    } arr_a;

    Struct_A struct_a_;

} Struct_B;
```

Conclusion:

## Functional Requirements

## Timeline

- 28th March: Beginnings - printing is working
