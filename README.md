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

Evidence: test_parse, test_print

Conclusion: Works

## Functional Requirements

For consume and concat, perhaps returning error is enough.
That way, it can start to resemble `sprintf/scanf` combo.
Perhaps, we can even support `%v` like golang.

## Timeline

- 28th March: Beginnings - printing is working
