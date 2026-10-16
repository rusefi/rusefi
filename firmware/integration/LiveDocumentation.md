Back in 2012 rusEfi was founded with the idea of simplicity and clarity.
In 2013 the approach for clarify of what's going on inside ECU was a set of commands
like _triggerinfo_, _analoginfo_ and _tempinfo_. In 2015 this was supplimented by *debug fields*.

In 2019 it became clear that a better way to look inside the ECU is needed, both to understand the overall logic of the ECU
and explain specific output numbers. **Live Documentation** is the attempt to answer that desire.

Implementation details:

1) rusEfi runtime data structures are defined in `engine_state.txt` `thermistor.txt` etc.
1) ConfigDefinition code generator produces both C++ header and java representation of the runtime data structure.
1) C++ code is annotated using DISPLAY_TEXT(x), DISPLAY_FIELD(y) and DISPLAY_CONFIG(z) macro
1) LiveDocsMetaParser code generator reads annotated C++ code and produces rusEfi console UI code like ThermistorMeta.java
1) rusEfi console periodically reads runtime data from firmware and displays it using generated UI panels.    

https://www.youtube.com/watch?v=n_pqH-3P3Qc

TODO: Live Documentation with branching sections.

### Scaling model values for telemetry

`[tag:live_data_output_scale]` SD logging and Lua `getOutput()` automatically
apply every exported field's constant INI scale and offset. The field
definition is the single source for the conversion:
`engineering = (raw + offset) * scale`. Plain C++ fields retain
their native units; SD writes the conversion into the log header, while Lua
converts only when the value is requested. `autoscale` fields already apply
the packing scale through `scaled_channel` and receive no additional
multiplier. SD offsets remain in raw payload units; Lua adds `offset *
packingScale` to the already-unpacked value. Neither path changes stored data.

Conversions support finite constant scales and offsets, including simple
division. Dynamic expressions and non-finite constants fail generation.
Missing conversion metadata defaults to scale 1 and offset 0. For example,
a float stored in grams can declare `"mg", 1000, 0` in
its field metadata. A raw trim multiplier can declare `"%", 100, -1`, so
`0.95`, `1.00` and `1.05` display as approximately `-5`, `0` and `5` percent.
Existing Lua scripts must use the declared engineering units:
`dwellActualRatio` returns percent
deviation, and the `misfire*Us` timing outputs return milliseconds despite
their historical names. Field coverage is unchanged; a fragment still needs
the existing SD/Lua source wiring to be exported.
