#!/bin/bash

cd ../java_tools
./gradlew :models:shadowJar
cd ../firmware


java -cp ../java_console/models/build/libs/models-all.jar com.rusefi.PerfTraceEnumGenerator development/perf_trace.h ../java_console/models/src/main/java/com/rusefi/tracing/EnumNames.java
