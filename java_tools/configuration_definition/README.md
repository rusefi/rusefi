# Dynamic TunerStudio array dimensions

Config TXT primitive arrays can optionally attach a TunerStudio dimension
constant to each fixed storage dimension:

```text
struct_no_prefix example_s
uint8_t rows;;"", 1, 0, 8, 32, 0
uint8_t cols;;"", 1, 0, 8, 24, 0
uint16_t[24{cols}] rpm
uint16_t[32{rows}] load
uint16_t[32{rows} x 24{cols}] table
end_struct
```

The number (or integer `#define`) before each brace is the storage capacity.
The name inside braces is a single, exact INI constant identifier. References
are emitted verbatim, without adding enclosing struct prefixes. Declare the
referenced scalars before the arrays using their final generated INI names.

This generates C storage `table[32][24]` and a TS array dimension of
`[{cols}x{rows}]`. As with fixed arrays, TXT uses C dimension order and TS uses
the reverse order. Following field offsets and total storage size use the fixed
capacities. Existing array syntax retains its behavior. A fixed and a dynamic
dimension can be mixed, although TS resizing tools require single constant
references for the dimensions they resize.

Only primitive, non-`iterate` arrays in `[Constants]` support this annotation.
Capacities must be positive. Struct/custom arrays and arbitrary expressions
inside braces are unsupported. Output channel generation rejects dynamic
annotations. The parser does not resolve or validate the referenced runtime
scalars, their bounds, or defaults.

For a usable TS configuration, supply defaults and an explicit element limit
in the INI template's `[ConstantsExtensions]` section, for example:

```ini
[ConstantsExtensions]
    defaultValue = rows, 16
    defaultValue = cols, 16
    maximumElements = table, 768
```

Keep scalar bounds within the reserved capacities and defaults within those
bounds. TunerStudio requires support for INI spec 3.53. This change only adds
syntax and generation support: it does not enable any production table.
Firmware must validate active dimensions and index the active packed table
correctly when its width changes; a fixed-width C array alone is insufficient.
The rusEFI Java INI reader also needs runtime dimension support before using
such definitions with the console.

Reference: [TunerStudio resizable table documentation](https://www.tunerstudio.com/index.php/products/tuner-studio/tsarticles/140-configuring-dynamically-sized-tables-in-tunerstudio).

