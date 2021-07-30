#
# this tool might have enough flexibility to generate both C# and java class depending on specified "top line" and "stringClassName" parameters?
#
java -jar ../java_console_binary/rusefi_console.jar ptrace_enums development/perf_trace.h ../java_console/models/src/main/java/com/rusefi/tracing/EnumNames.java "package com.rusefi.tracing;" "public static final String"

