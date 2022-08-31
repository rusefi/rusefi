Here we have some tiny dev utilities - average rusEFI user would not need to use any of these!


```
cd android
./gradlew :config_definition:shadowJar
```

gcc_map_reader reads the .map file produced by GCC and prints some details on what is RAM used for

ts2c reads a piece of TunerStudio map and produces a matching C language piece of code

version2header creates a C language header containting current SVN version

enum2string

KiCad tools have moved to https://github.com/rusefi/KiCad-utils