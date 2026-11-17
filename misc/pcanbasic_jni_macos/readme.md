# PCAN-USB on macOS: PCANBasic JNI bridge over MacCAN

PEAK ships no macOS driver, so the console's `peak.can.basic.PCANBasic` Java
class cannot use `PCANBasic_JNI.dll`/`PCANBasic.dll` there. This directory
holds `PCANBasic_JNI_macos.c`, a drop-in JNI library with the same exported
`Java_peak_can_basic_PCANBasic_*` symbols that forwards to MacCAN-Core's
user-space `libPCBUSB` (<https://www.mac-can.com>), which implements the
PCANBasic C API for PCAN-USB adapters.

`PCANBasic.java` already tries `System.loadLibrary("pcanbasic_jni")` before
`PCANBasic_JNI`, so the only runtime requirements on a Mac are:

1. MacCAN installed: `brew tap mac-can/maccan && brew install pcbusb`
   (the bridge `dlopen()`s `libPCBUSB.dylib` from `/opt/homebrew/lib` or
   `/usr/local/lib`; it is not linked at build time, so the bridge loads
   even without MacCAN and `initializeAPI()` reports `false` instead).
2. `libpcanbasic_jni.dylib` on `java.library.path`. The prebuilt lives in
   `java_console/libpcanbasic_jni.dylib`; `firmware/bundle.mk` copies it
   into the bundle's `console/` folder when `BUNDLE_PCAN=yes`, and
   `misc/console_launcher/rusefi_updater.sh` starts java with
   `-Djava.library.path=.` from that folder. From a source tree:

   ```sh
   java -Djava.library.path=java_console -jar java_console/console/build/libs/rusefi_console.jar
   ```

## Rebuilding

```sh
misc/pcanbasic_jni_macos/build.sh      # universal arm64 + x86_64
ARCHS="-arch arm64" misc/pcanbasic_jni_macos/build.sh   # single arch
```

Rebuild whenever the bridge source or the `PCANBasic.java` native method
set changes. The checked-in dylib was built from this source on
Apple Silicon (arm64 only, 2026-09); Intel Macs need the universal rebuild.

## Non-obvious traps (all hit on hardware)

- `DWORD` must be `unsigned int`. `unsigned long` is 8 bytes on LP64 macOS,
  which shifts `LEN` in `TPCANMsg` by four bytes: every TX frame leaves the
  adapter with DLC=0 while RX keeps working by accident (the 64-bit ID read
  is truncated to the correct low 32 bits). Symptom: the ECU floods the bus
  but never answers the console's 0x710 ISO-TP hello.
- MacCAN's `CAN_Read` does not block. It returns `PCAN_ERROR_QRCVEMPTY`
  immediately (Windows PCANBasic blocks), so a tight re-poll burns a full
  core; `PCanRawPort.receive()` sleeps 1 ms between empty polls.
- MacCAN is strictly single-client. A reconnect that skips `CAN_Uninitialize`
  makes every later `CAN_Initialize` in the same process fail with
  `PCAN_ERROR_INITIALIZE`; `PCanRawPort.close()` always uninitializes.
  Likewise close the console before running the C harness below.
- Plugging the adapter in after the first `CAN_Initialize` failed
  (`PCAN_ERROR_ILLHW`) has been seen to leave MacCAN unable to claim the
  device until the process restarts: plug the adapter in first.
- `InitializeFD`/`ReadFD`/`WriteFD` and the receive-event functions are
  stubs returning `PCAN_ERROR_ILLPARAMTYPE`; the console only uses
  Initialize/FilterMessages/Read/Write/Uninitialize.
- `GetValue`/`SetValue` marshal the Java buffer like PEAK's Windows JNI:
  `peak.can.MutableInteger`/`MutableLong` (public `value` field), `byte[]`
  and `StringBuffer` are read before `SetValue` and filled after a
  successful `GetValue`. A bridge built before this change forwarded the
  query but never copied the result back, so `PCAN_CHANNEL_CONDITION`
  always read as its Java default and adapters could not be enumerated;
  callers that pre-seed the buffer with an impossible value can detect that
  older dylib. Rebuild with `build.sh` after updating this source.

## Diagnostics

`pcan_mac_test.c` is a stand-alone `dlopen` harness (no Java) that
initializes USBBUS1 at 500 kbit/s, enables the MacCAN wire trace, sends
the console hello on 0x710 and waits 3 s for the ECU's 0x720 reply:

```sh
cc -O2 -o pcan_mac_test pcan_mac_test.c
./pcan_mac_test /usr/local/lib/libPCBUSB.dylib /tmp
```

Exit code 0 means the ECU answered; the trace file in the given directory
shows the DLC of every TX frame (`Tx 0710 8 ...` is correct, `Tx 0710 0`
is the DWORD bug above).
