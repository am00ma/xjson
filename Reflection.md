# Reflection

Arguments used for reflection:

- name: Name of parent struct type, needed to define `print__##name(...)`, count `X_COUNT_##name`

- idx: Used to check if in final field `idx == X_COUNT_##name - 1`; Can be auto-generated with enums

- type: seems like it is not used
- field: Used to put/get `x->field`
- ref: Used in `print__##name(b, ref(x))` to call either `print__<>(x)` or `print__<>(&x)`
- isarray: Used to differentiate between arrays and normals, to call `print__array__<>(x)`
  - Deprecated in favour of common name for arrays

Specific to serialization

- key: Used in printing/parsing key of json member
- print_fn: Make explicit which func is used
- parse_fn: Make explicit which func is used
