# OpenBLT CAN uploader

Standalone Java 17 CLI for the rusEFI OpenBLT XCP transport. Requires a PCAN
adapter and the matching PCAN JNI/native driver libraries in `java_console`
(or on `java.library.path` when launching Java directly). It uses extended CAN
IDs 0x10667/0x107e1 at 500 kbit/s. This is not an uploader for the M74.9 OEM
resident loader.

From the repository root:

```sh
./gradlew :openblt_can:test :openblt_can:fatJar
bash java_console/bin/openblt_can.sh --help
bash java_console/bin/openblt_can.sh --probe
bash java_console/bin/openblt_can.sh firmware/build/rusefi.srec
```

Use an application-only SREC built for the target board and installed OpenBLT
layout. `--probe` connects and then requests application startup; it does not
erase or program flash. File arguments are resolved from the caller's working
directory, including paths containing spaces. The launcher runs the incremental
Gradle build before starting Java.

The default mode uses the rusEFI 2 KiB deferred-ACK programming extension. For
an older OpenBLT loader, use `--no-batch`; add `--no-pipeline` for one request
and acknowledgement at a time. All erase requests precede programming, and each
segment is finalized before the next segment or a baud-rate-switch reboot.
Verification uses the target's ADD11 checksum in 32 KiB chunks, followed by an
application restart. ADD11 is an 8-bit additive checksum, not a byte-for-byte
verification or a CRC; the bootloader also validates the application's image
CRC before startup.

`--1mbit` keeps the normal 500 kbit/s behavior. `--1mbit-force` enables the
experimental reboot-based switch; the donor checkout recorded checksum failures
at this speed on its MacCAN/PCAN-USB setup. Batch programming is disabled at
1 Mbit/s. Use `--verbose` for frame logging and `--help` for all options.

The host tests simulate the transport, dropped replies, rate-switch recovery,
word-buffered flash and shared erase sectors. Hardware flashing has not been
validated as part of this port.
