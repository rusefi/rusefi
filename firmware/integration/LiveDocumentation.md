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