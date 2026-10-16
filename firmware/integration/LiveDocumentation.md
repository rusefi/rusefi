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

`[tag:live_data_output_scale]` A `LiveData.yaml` entry can set
`applyOutputScale: true` to apply its fields' constant INI scale to SD logging
and Lua `getOutput()`. The field definition remains the single source for the
scale. Plain C++ fields retain their native units; SD writes the scale into
the log header, while Lua multiplies only when the value is requested.
`autoscale` fields already convert through `scaled_channel` and receive no
additional multiplier.

This option supports constant scales (including simple division) with zero
offset. Unsupported offsets or dynamic scale expressions fail generation.
It defaults to false to preserve existing SD/Lua unit contracts. Enable it
when introducing or migrating a fragment whose outputs should match the INI
units. For example, a float stored in grams can declare `"mg", 1000, 0` in
its field metadata. No existing fragment is opted in by default.
