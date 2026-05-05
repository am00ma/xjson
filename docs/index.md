# Reflection using X-Macros

Use cases:

- Generics for printing
- Serialization / Deserialization
- Database api generation

## Printing

Uses 'transparent name-mangling', by assuming constraint:

> there exists a function `void printvar__##type(type* x)` for given `type`.

This function in turn calls `printvar__##field_type(field_type* x)` for each field of the parent `type`.

The examples here include `types.h`, which aliases some types and defines a string type and constraints for an array type.

### Unfolded

A simple 'unfolded' example: [source](./unfolded.c)

### Primitives

At the 'base' level of the 'recursion', we define primitives: [source](./primitives.c)

### Arrays

To support arrays, we need a further constrait on how the data buffer and array length are named.
Assuming `buf` and `len`: [source](./primitives.c)

### References

Apart from arrays, struct may have references rather than own the data themselves: : [source](./references.c)
