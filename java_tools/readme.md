Main folder for rusEFI console source code, also some dev utilities.

TODO: one day we shall merge java_console and java_tools, right after we come up with an amazing combined folder name!

```
./gradlew :config_definition:shadowJar
```

``configuration_definition`` is the most valuable subproject: that code reads

gcc_map_reader reads the .map file produced by GCC and prints some details on what is RAM used for

enum2string

KiCad tools have moved to https://github.com/rusefi/KiCad-utils
