# VE-Autotune Windows BT Recorder — Design

## Intent / why

The Android BT recorder (`ve_autotune/android/`) is the normal way to log a
ride for the VE-autotune pipeline, but no Android device has ever been
attached to this dev machine, so its real-hardware path has never been
exercised end to end. The user wants a Windows desktop GUI that talks to the
same microRusEFI over the same Bluetooth classic SPP link, as a second,
independently-built client: usable on a laptop (bench testing, or next to
the bike) without a phone, and incidentally a way to validate the wire
protocol on real hardware from a machine the user actually has.

## Goals

- A Windows GUI app: pick a COM port, Start/Stop, see live RPM/MAP/TPS/CLT/AFR,
  write a CSV log.
- CSV output byte-for-byte header-compatible with the Android app's
  (`Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio`), so `ve_autotune/cli.py analyze`
  and `snapshot-tune` work on it unmodified.
- Same wire protocol and safety constraint as the Android app: the only ECU
  command ever built is the output-channel read (`'O'`). No write/burn
  capability in this tool.
- Ships as a single Windows `.exe` the user builds themselves with
  PyInstaller (this dev machine is Linux; PyInstaller does not reliably
  cross-build Windows executables from Linux).

## Non-goals / anti-goals

- No BLE support — microRusEFI's TunerStudio-over-Bluetooth is classic SPP
  (RFCOMM), confirmed by the Android app's `RusefiSpp` standard SPP UUID. On
  Windows, a paired SPP device simply becomes a COM port, so this tool talks
  to it exactly like any other serial device — no WinRT/BLE APIs needed.
- No `.msq` pairing or tune read/write — that already exists via
  `ve_autotune/cli.py snapshot-tune` on the PC side.
- No auto-pairing of the Bluetooth device itself — Windows' own Bluetooth
  settings handle pairing; this tool only opens the COM port Windows already
  exposed.
- Not a rewrite of the Android app or the existing PC CLI pipeline — a third,
  independent client reusing the same protocol knowledge.

## Constraints

- Must run on Windows without the user installing Python separately (ships
  as `.exe` via PyInstaller), but every piece of logic is built and unit
  tested on this Linux dev machine first — only the final `.exe` build step
  happens on the user's Windows machine.
- Must reuse the exact protocol constants already verified against
  `java_console/io/src/main/java/com/rusefi/binaryprotocol/IoHelper.java`,
  `ByteRange.java`, and `/home/whip/.rusEFI/ini_database/425490096.ini`
  (re-confirmed in this session): RPM U16@4 scale 1.0; coolant S16@14 scale
  0.01; TPS S16@22 scale 0.01; MAP U16@30 scale 0.03333333333333333; AFR
  U16@210 scale 0.001.
- Global constraint carried over from the Android plan, unchanged: the only
  command this tool ever builds is the output-channel read (`'O'` + LE16
  offset + LE16 count). No other outer/inner command is ever constructed.
- **No existing Python implementation of the outer packet framing exists**
  anywhere in this repo (confirmed by search — `run_mcp` in `cli.py` calls
  an external MCP tool, not a raw protocol codec). `protocol.py` is a fresh
  port of the full outer+inner framing from the same Java sources already
  cited: outer = `[2-byte BE length][inner bytes][4-byte BE CRC32 of the
  inner bytes]` (standard `zlib.crc32`, matching `IoHelper.makeCrc32Packet`);
  inner request = `['O' (0x4F)] + LE16(offset) + LE16(count)`; inner
  response = `[0x00 status byte] + raw bytes`. This mirrors
  `Frame.kt`/`Crc.kt` from the Android app exactly — same algorithm, same
  byte order, new language.
- **CSV `Time` column value:** elapsed seconds since the recording session
  started, as a plain float (matching the Android app's
  `CsvLogger.appendRow(tSeconds: Double, ...)` — e.g. `0.0`, `0.103`,
  `0.206`, ...; not a wall-clock timestamp). `ve_autotune/pipeline/csv_log.py`
  parses every column as a bare float with no fixed decimal-place
  requirement, so this matches what `analyze`/`snapshot-tune` already
  expect.
- **Serial parameters for the COM port:** baud **38400**, 8 data bits, no
  parity, 1 stop bit, confirmed from firmware source
  (`firmware/controllers/algo/engine_configuration.cpp`:
  `#define TS_DEFAULT_SPEED 38400`, which is the default
  `tunerStudioSerialSpeed` for the secondary/Bluetooth-via-TTL TunerStudio
  channel on microRusEFI — board.mk confirms this board's Bluetooth is a TTL
  module on a secondary UART, not a native BT chip). A read timeout of 2
  seconds per attempted frame (long enough for a real response, short enough
  that a dead link surfaces promptly as a bad frame rather than hanging the
  worker thread).
- **Bad-frame-threshold UI behavior:** when `BadFrameTracker` trips (5
  consecutive bad frames), the worker thread stops polling and flushes/closes
  the CSV file normally (never leaves it mid-row), then posts a "connection
  lost" status to the GUI thread. The Start button re-enables and the Stop
  button disables, identical to a normal user-initiated Stop, so the CSV file
  on disk is always left valid for `cli.py analyze` either way. Resuming
  requires the user to press Start again (opens a new CSV file — no silent
  append to the old one).
  **Post-review addendum:** the final whole-branch review found that
  "press Start again" in practice reuses the same output path still sitting
  in the entry box, which — opened with `"w"` as originally implemented —
  silently truncated the previous ride's data instead of avoiding a silent
  append. The implemented fix is a confirmation dialog ("this file already
  exists, overwrite it?") on Start whenever the chosen path already exists,
  rather than an automatic new filename, so the user's explicit choice of
  output path is never second-guessed but data loss is never silent either.
  Also found in the same review: a read `TimeoutError` must be retried
  through `BadFrameTracker` exactly like a `BadResponse` (not treated as an
  immediately-fatal transport error) — see the implementation plan's
  corresponding `Ruling` note, which supersedes this document's and the
  plan's original fatal-timeout framing. And: every request must discard
  any bytes left in the serial receive buffer before sending (via
  `reset_input_buffer()`), because the ECU's own short error-reply frame
  (on a corrupted request) otherwise leaves trailing bytes that permanently
  desync all later reads from the real frame boundaries — reproduced and
  fixed against a real pty-based fake ECU in that review's fix wave.

## Approaches considered

- **Language/runtime:** Python + tkinter (stdlib, no extra Windows
  dependencies beyond `pyserial`) vs. a compiled Windows GUI (C#/.NET) vs. a
  PySide/PyQt GUI. Chosen: **Python + tkinter**. It sits next to the existing
  Python PC pipeline, needs nothing beyond `pyserial` for the serial device,
  and the existing team pattern (this repo) is already Python-first on the
  PC side. PySide/PyQt would look nicer but adds a heavy dependency and a
  much bigger PyInstaller bundle for no functional gain here.
- **Bluetooth access on Windows:** native WinRT Bluetooth/RFCOMM APIs vs.
  treating the paired device as a plain COM port. Chosen: **COM port** via
  `pyserial`. Classic SPP devices are exposed by Windows as a COM port once
  paired — no native BT stack code needed, and it matches how the user
  already thinks about "plugging into a serial device."
- **Distribution:** editable Python script vs. PyInstaller `.exe`. Chosen:
  **PyInstaller `.exe`**, built by the user on their own Windows machine from
  a `.spec` file and one-line build instructions this plan provides, since
  this dev machine cannot reliably cross-build a Windows executable.

## What's decided vs. left to the builder

Decided: file layout, module boundaries, protocol constants and scales,
CSV header, the bad-frame-retry behavior (port of `BadFrameTracker`:
5 consecutive bad frames ends the session, any good frame resets the
counter), threading model (serial I/O on a background thread, GUI stays on
the Tk main thread, communicating through a thread-safe queue).

Left to the builder: exact tkinter widget layout/spacing, variable names
inside function bodies. The polling interval is NOT left open: the Android
app (`RecordingService.kt`) polls at 1 Hz — a 1-second delay after every
successful read AND after every retried bad frame — so this tool uses the
same 1 Hz rate, not a tight loop.
