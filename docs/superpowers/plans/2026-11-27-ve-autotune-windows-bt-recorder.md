# VE-Autotune Windows BT Recorder Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A Windows-buildable tkinter GUI that connects to a microRusEFI
over a Bluetooth-SPP COM port, polls output channels at 1 Hz, and writes a
CSV log that `ve_autotune/cli.py analyze`/`snapshot-tune` can consume
unmodified.

**Architecture:** Pure-Python port of the already-shipped Android app's
protocol/connection/logger layers (same CRC32 framing, same output-channel
offsets, same CSV header, same bad-frame-retry policy), wired to a real
serial port via `pyserial` instead of a `BluetoothSocket`, with a tkinter
GUI replacing the Android Activity/Service. The polling loop is a plain
function taking injected connection/logger/sleep/clock so it is unit
testable without a display, a real serial port, or real timing; only the
tkinter wiring and the serial-port open itself are untested by pytest.

**Tech Stack:** Python 3 standard library (`tkinter`, `threading`, `queue`,
`zlib`, `struct`, `dataclasses`), `pyserial` (already present in this dev
venv at 3.5), `pytest` (matches the rest of `ve_autotune/tests`),
PyInstaller (Windows-side build only, not run on this dev machine).

**Spec:** `docs/superpowers/specs/2026-11-27-ve-autotune-windows-bt-recorder-design.md`

## Global Constraints

- The only ECU command this tool ever builds is the output-channel read:
  outer envelope `[2B BE inner length][inner][4B BE CRC32 of inner]`, inner
  request `['O' (0x4F)] + LE16(offset) + LE16(count)`. `offset = 0`,
  `count = 214` (verbatim from `RusefiBtConnection.kt`'s
  `OUTPUT_CHANNELS_OFFSET`/`OUTPUT_CHANNELS_COUNT`). No other command byte,
  outer wrapper, or inner payload is ever constructed anywhere in this tool.
- Expected inner response length is always exactly `1 + 214 = 215` bytes (1
  status byte + 214 channel bytes). A length prefix read off the wire that
  isn't exactly 215 is a `BadResponse`, never trusted enough to drive a
  `read_fully` call with an attacker/noise-controlled size.
- CRC32 is the standard algorithm (`zlib.crc32`, equivalent to Java's
  `java.util.zip.CRC32`), computed over the inner bytes only, compared
  against the trailing 4-byte big-endian value.
- Output channel offsets/scales (bytes into the 214-byte payload, status
  byte already stripped), copied verbatim, not re-derived: RPM U16 @ 4,
  scale `1.0`; CLT S16 @ 14, scale `0.01`; TPS S16 @ 22, scale `0.01`; MAP
  U16 @ 30, scale `0.0333333333333333` (yes, one fewer `3` than the raw
  `.ini` value — matches `OutputChannels.kt` exactly, do not "fix" the
  digit count); AFR U16 @ 210, scale `0.001`.
- Decoding the channel payload must guard `len(payload) < 212` and raise
  `BadResponse` rather than let a short payload raise an uncaught
  `IndexError`/`struct.error` (AFR's U16 read needs bytes 210-211, so 212 is
  the minimum safe length) — port of `OutputChannels.kt`'s explicit guard.
- CSV header is exactly `Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio` (verbatim,
  same column order as `CsvLogger.kt`). The `Time` value written per row is
  elapsed seconds since the recording session started (a float, e.g.
  `0.0`, `1.0`, `2.0`, ...), never a wall-clock timestamp.
- Poll rate is 1 Hz: a 1-second delay after every successful read and after
  every retried bad frame (verbatim from `RecordingService.kt`'s two
  `delay(1000)` call sites) — never a tight loop.
- `MAX_CONSECUTIVE_BAD_FRAMES = 5`: a `BadResponse` (CRC mismatch or
  non-OK status) **or a read `TimeoutError`** is logged as a skip-and-retry
  up to 5 times in a row; the 5th consecutive one ends the session as a
  connection error, and any intervening successful read resets the counter
  to 0.
  **Ruling (added during the final-review fix wave, superseding this
  plan's original text):** the first version of this plan treated a read
  timeout as an immediately-fatal transport error, matching Android's
  `IOException` catch — but that contradicted this same plan's read-timeout
  constraint below ("a dead link surfaces promptly as a bad frame"), and in
  practice would end a ride on one dropped byte or on firmware staying
  silent while briefly out of sync. A `TimeoutError` is now retried exactly
  like a `BadResponse`. A deeper `OSError` (port physically gone, access
  denied — something a retry cannot fix) is NOT retried through this
  counter and still ends the session immediately, same as
  `RecordingService.kt`'s separate `IOException` catch block. Cost if this
  ruling is wrong: a truly dead link now takes up to ~5 seconds longer to
  report, instead of ending on the first missed read — judged an acceptable
  trade for not killing a ride over one bit of line noise.
- Serial port parameters: baud **38400**, 8 data bits, no parity, 1 stop
  bit, 2.0 second read timeout (confirmed from
  `firmware/controllers/algo/engine_configuration.cpp`'s
  `TS_DEFAULT_SPEED 38400`, the default speed of microRusEFI's
  Bluetooth-via-TTL secondary TunerStudio channel).
- When the session ends for any reason (user-requested Stop, bad-frame
  limit, transport error), the CSV file is always flushed and closed before
  the GUI re-enables Start — never left mid-row or still open.

## Review Focus

- **Serial port that never responds at all** (wrong port picked, device
  not actually paired/SPP, or device present but ECU not powered): the
  very first `read_fully` call must hit the 2-second timeout and raise a
  clean, caught error — not hang forever, and not crash with a raw
  `serial.SerialException` traceback in the GUI. Covered by Task 5's
  timeout test and Task 8's "Start with a port that never sends data"
  manual check.
- **User clicks Start twice in a row** (e.g. a slow port open): the second
  click must be a no-op while a session is already running, not start a
  second worker thread/second open file handle. Covered by Task 8's step
  disabling Start immediately on click, before the worker thread exists.
- **User clicks Stop before Start has finished opening the port**: must
  not crash trying to signal a session/thread that doesn't exist yet, and
  must still leave the GUI in a clean, restartable state. Covered by
  Task 8's state-guarded Stop handler.
- **Output path the user picks is not writable** (e.g. a read-only
  location, or the parent directory vanished): opening the CSV file must
  raise a caught, reported error before any serial I/O happens, not a
  half-open file plus a silently abandoned serial connection. Covered by
  Task 8's worker-thread step that opens the CSV file and reports failure
  before opening the port.
- **A response arrives with the right length but a flipped CRC or
  non-zero status byte** (real Bluetooth bit noise over a long session):
  must be treated as one retryable `BadResponse`, not crash the poll loop
  and not get silently written to the CSV as a bogus row. Covered by
  Task 6's CRC-mismatch/non-OK-status tests and Task 7's retry-loop test.
- **Exactly 5 consecutive bad frames, the boundary condition**: the 5th
  one must end the session (not the 4th, not require a 6th); one good
  frame anywhere in a streak of 4 must reset the counter so the next bad
  frame starts counting from 1, not 5. Covered by Task 3's boundary tests
  (ported directly from `BadFrameTrackerTest.kt`'s own cases).

## File Structure

No `__init__.py` files: the rest of `ve_autotune` uses implicit namespace
packages (confirmed in this session — `ve_autotune/` and `ve_autotune/tests/`
have none), and both pytest and PyInstaller handle that without one.

```
ve_autotune/windows_recorder/
    protocol.py          # Task 1: CRC32 + outer/inner framing + BadResponse
    channels.py           # Task 2: Channels dataclass + decode_channels
    bad_frame_tracker.py  # Task 3: BadFrameTracker
    csv_logger.py          # Task 4: CsvLogger
    serial_transport.py   # Task 5: ByteTransport protocol, SerialTransport, open_port
    connection.py          # Task 6: RusefiBtConnection
    recorder.py            # Task 7: run_session() poll loop
    gui.py                 # Task 8: tkinter App, entry point (if __name__ == "__main__")
    requirements.txt       # Task 9: pyserial + pyinstaller (Windows-side build deps)
    windows_recorder.spec  # Task 9: PyInstaller spec
    BUILD_WINDOWS.md       # Task 9: exact build steps for the user's own Windows machine
ve_autotune/tests/windows_recorder/
    test_protocol.py
    test_channels.py
    test_bad_frame_tracker.py
    test_csv_logger.py
    test_serial_transport.py
    test_connection.py
    test_recorder.py
```

---

### Task 1: Protocol framing (CRC32 + outer/inner envelope)

**Files:**
- Create: `ve_autotune/windows_recorder/protocol.py`
- Test: `ve_autotune/tests/windows_recorder/test_protocol.py`

**Interfaces:**
- Produces: `class BadResponse(Exception)`;
  `def crc32(data: bytes) -> int`;
  `def build_output_channel_request(offset: int, count: int) -> bytes`;
  `def parse_response(frame: bytes) -> bytes` (returns the payload with the
  status byte stripped, raises `BadResponse` on CRC mismatch or non-OK
  status).

- [ ] **Step 1: Write failing tests** in `test_protocol.py`:
  - `test_crc32_matches_known_value`: `crc32(b"123456789")` equals the
    well-known CRC-32 test vector `0xCBF43926`.
  - `test_build_output_channel_request_frames_correctly`: call
    `build_output_channel_request(0, 214)` and assert byte-for-byte: first
    2 bytes `b"\x00\x05"` (inner length 5, big-endian), next 5 bytes
    `b"\x4f\x00\x00\xd6\x00"` (`'O'`, offset=0 as LE16, count=214 as LE16),
    last 4 bytes equal `struct.pack(">I", crc32(inner))` for that same
    5-byte inner.
  - `test_parse_response_returns_payload_on_ok_status`: build a frame with
    inner = `b"\x00" + bytes(214)` (status OK + 214 zero bytes), correct
    length prefix and CRC; assert `parse_response(frame) == bytes(214)`.
  - `test_parse_response_raises_on_crc_mismatch`: same frame but with one
    CRC byte corrupted; assert `pytest.raises(BadResponse)`.
  - `test_parse_response_raises_on_non_ok_status`: inner's first byte set
    to `0x01` instead of `0x00` (with a correctly recomputed CRC for that
    inner); assert `pytest.raises(BadResponse)`.

- [ ] **Step 2: Run tests, verify they fail** with `ModuleNotFoundError`/`AttributeError`

  Run: `pytest ve_autotune/tests/windows_recorder/test_protocol.py -v`

- [ ] **Step 3: Implement `protocol.py`**

  Port `Crc.kt` + `Frame.kt` verbatim: `crc32` wraps `zlib.crc32(data) & 0xFFFFFFFF`
  (always non-negative, matching Java's unsigned `CRC32.value`).
  `build_output_channel_request` packs the 5-byte inner with `struct.pack`
  (`">B"` for the command byte, `"<H"` for offset and count — note the
  endianness split: outer/length/CRC are big-endian, the inner
  offset/count fields are little-endian, per `Frame.kt`'s own comment —
  do not unify them), then wraps with a 2-byte big-endian length prefix and
  a 4-byte big-endian CRC32 trailer. `parse_response` reads the inner
  length from the first 2 bytes, slices out the inner bytes and the
  trailing 4-byte CRC, recomputes CRC32 over the inner bytes and compares,
  then checks the inner's first byte is `0x00` before returning the rest.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_protocol.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/protocol.py ve_autotune/tests/windows_recorder/test_protocol.py
git commit -m "feat(windows_recorder): port CRC32/outer-inner framing from Android"
```

---

### Task 2: Output channel decoding

**Files:**
- Create: `ve_autotune/windows_recorder/channels.py`
- Test: `ve_autotune/tests/windows_recorder/test_channels.py`

**Interfaces:**
- Consumes: `protocol.BadResponse` (Task 1)
- Produces: `@dataclass class Channels: rpm: float; map: float; tps: float; clt: float; afr: float`;
  `def decode_channels(payload: bytes) -> Channels`

- [ ] **Step 1: Write failing tests** in `test_channels.py`:
  - `test_decode_channels_reads_known_values`: build a 214-byte
    `bytearray`, write `struct.pack_into("<H", buf, 4, 3500)` (RPM raw
    3500 → expect `rpm == 3500.0`), `struct.pack_into("<h", buf, 14, 8500)`
    (CLT raw 8500 → expect `clt == 85.0`), `struct.pack_into("<h", buf, 22, 2550)`
    (TPS raw 2550 → expect `tps == 25.5`), `struct.pack_into("<H", buf, 30, 3000)`
    (MAP raw 3000 → expect `map == pytest.approx(100.0, abs=0.01)`),
    `struct.pack_into("<H", buf, 210, 14700)` (AFR raw 14700 → expect
    `afr == 14.7`); assert all five fields on the decoded `Channels`.
  - `test_decode_channels_raises_on_short_payload`: call with
    `bytes(211)` (one byte short of the 212-byte minimum); assert
    `pytest.raises(BadResponse)`.

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_channels.py -v`

- [ ] **Step 3: Implement `channels.py`**

  Port `OutputChannels.kt` verbatim, including its comments citing the
  `.ini` source lines (RPM U16@4 scale 1.0; CLT S16@14 scale 0.01; TPS
  S16@22 scale 0.01; MAP U16@30 scale 0.0333333333333333; AFR U16@210
  scale 0.001). Use `struct.unpack_from("<H", payload, offset)` for
  unsigned 16-bit reads and `struct.unpack_from("<h", payload, offset)`
  for signed. Guard `len(payload) < 212` first, raising `BadResponse`.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_channels.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/channels.py ve_autotune/tests/windows_recorder/test_channels.py
git commit -m "feat(windows_recorder): port output-channel decoding from Android"
```

---

### Task 3: Bad-frame tracker

**Files:**
- Create: `ve_autotune/windows_recorder/bad_frame_tracker.py`
- Test: `ve_autotune/tests/windows_recorder/test_bad_frame_tracker.py`

**Interfaces:**
- Produces: `class BadFrameTracker: def __init__(self, max_consecutive: int); def record_failure(self) -> bool; def record_success(self) -> None; consecutive_failures: int (property)`

- [ ] **Step 1: Write failing tests** (port of `BadFrameTrackerTest.kt`'s
  own three cases) in `test_bad_frame_tracker.py`:
  - `test_not_dead_until_threshold`: `BadFrameTracker(5)`, call
    `record_failure()` 4 times, assert each returns `False` and
    `consecutive_failures` is 1, 2, 3, 4 in turn; the 5th call returns `True`.
  - `test_reset_on_success`: after 3 failures, call `record_success()`,
    assert `consecutive_failures == 0`; the next `record_failure()` returns
    `False` and `consecutive_failures == 1` (not 4).
  - `test_single_failure_does_not_trip`: fresh tracker,
    `record_failure()` once returns `False`.

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_bad_frame_tracker.py -v`

- [ ] **Step 3: Implement `bad_frame_tracker.py`** — direct port of the
  Kotlin class: an internal counter starting at 0, `record_failure`
  increments then returns `counter >= max_consecutive`, `record_success`
  resets the counter to 0.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_bad_frame_tracker.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/bad_frame_tracker.py ve_autotune/tests/windows_recorder/test_bad_frame_tracker.py
git commit -m "feat(windows_recorder): port BadFrameTracker from Android"
```

---

### Task 4: CSV logger

**Files:**
- Create: `ve_autotune/windows_recorder/csv_logger.py`
- Test: `ve_autotune/tests/windows_recorder/test_csv_logger.py`

**Interfaces:**
- Consumes: `channels.Channels` (Task 2)
- Produces: `class CsvLogger: def __init__(self, out: TextIO); def write_header(self) -> None; def append_row(self, t_seconds: float, c: Channels) -> None`

- [ ] **Step 1: Write failing tests** in `test_csv_logger.py` using
  `io.StringIO()` as the fake file:
  - `test_write_header_writes_exact_header_line`: assert the buffer's
    value is exactly `"Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio\n"`.
  - `test_append_row_writes_comma_joined_values`: call with
    `t_seconds=1.5` and a `Channels(rpm=3500.0, map=100.0, tps=25.5, clt=85.0, afr=14.7)`;
    assert the buffer's value is exactly
    `"1.5,3500.0,100.0,25.5,85.0,14.7\n"`.
  - `test_append_row_flushes` (use a fake `TextIO`-like object whose
    `.flush()` sets a flag, since `StringIO.flush()` is a no-op): assert
    the flag is set after `append_row`.

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_csv_logger.py -v`

- [ ] **Step 3: Implement `csv_logger.py`** — direct port of
  `CsvLogger.kt`: `write_header` writes the literal header string then
  calls `self.out.flush()`; `append_row` writes
  `f"{t_seconds},{c.rpm},{c.map},{c.tps},{c.clt},{c.afr}\n"` then flushes.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_csv_logger.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/csv_logger.py ve_autotune/tests/windows_recorder/test_csv_logger.py
git commit -m "feat(windows_recorder): port CsvLogger from Android"
```

---

### Task 5: Serial transport

**Files:**
- Create: `ve_autotune/windows_recorder/serial_transport.py`
- Test: `ve_autotune/tests/windows_recorder/test_serial_transport.py`

**Interfaces:**
- Produces: `class ByteTransport(Protocol): def write(self, data: bytes) -> None; def read_fully(self, n: int) -> bytes`;
  `class SerialTransport(ByteTransport): def __init__(self, ser)` (accepts
  anything exposing pyserial's `.write(bytes)`/`.read(n) -> bytes`
  interface — duck-typed, so tests inject a fake instead of a real
  `serial.Serial`);
  `def open_port(port_name: str) -> serial.Serial` (the only place
  `serial.Serial(...)` is constructed with real parameters).

- [ ] **Step 1: Write failing tests** in `test_serial_transport.py`,
  using a hand-written fake with the same `.write`/`.read` shape as
  `serial.Serial` (no real `pyserial` I/O in these tests):
  ```python
  class FakeSerial:
      def __init__(self, chunks: list[bytes]):
          self._chunks = list(chunks)
          self.written = b""
      def write(self, data: bytes) -> None:
          self.written += data
      def read(self, n: int) -> bytes:
          return self._chunks.pop(0) if self._chunks else b""
  ```
  - `test_write_passes_bytes_through`: `SerialTransport(FakeSerial([])).write(b"abc")`
    then assert the fake's `.written == b"abc"`.
  - `test_read_fully_accumulates_across_short_reads`: fake chunks
    `[b"ab", b"cde"]`, `read_fully(5)` returns `b"abcde"`.
  - `test_read_fully_raises_timeout_on_empty_read`: fake chunks `[b"ab", b""]`,
    `read_fully(5)` raises `TimeoutError` (pyserial's `read(n)` returning
    `b""` means the configured timeout elapsed with no data — treat that
    as fatal rather than looping forever or returning a short buffer).

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_serial_transport.py -v`

- [ ] **Step 3: Implement `serial_transport.py`**

  `SerialTransport.write` calls `self._ser.write(data)`. `read_fully(n)`
  loops accumulating `self._ser.read(n - len(buf))` into a `bytearray`
  until it has `n` bytes, raising `TimeoutError(f"serial read timed out
  after {len(buf)} of {n} requested bytes")` the moment a `.read()` call
  returns an empty `bytes`. `open_port(port_name)` returns
  `serial.Serial(port=port_name, baudrate=38400, bytesize=serial.EIGHTBITS, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, timeout=2.0)`
  — the only place these constants appear.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_serial_transport.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/serial_transport.py ve_autotune/tests/windows_recorder/test_serial_transport.py
git commit -m "feat(windows_recorder): add pyserial-backed ByteTransport"
```

---

### Task 6: ECU connection

**Files:**
- Create: `ve_autotune/windows_recorder/connection.py`
- Test: `ve_autotune/tests/windows_recorder/test_connection.py`

**Interfaces:**
- Consumes: `protocol.build_output_channel_request`, `protocol.parse_response`,
  `protocol.BadResponse` (Task 1); `channels.decode_channels`,
  `channels.Channels` (Task 2); `serial_transport.ByteTransport` (Task 5,
  structural — this module only type-hints against it, any object with
  `write`/`read_fully` works, same fake-friendly pattern as the Kotlin
  `ByteTransport` interface)
- Produces: `class RusefiBtConnection: def __init__(self, transport: ByteTransport); def read_channels(self) -> Channels`

- [ ] **Step 1: Write failing tests** in `test_connection.py`, with a fake
  transport whose `read_fully` returns from a pre-built, pre-queued list of
  byte chunks and whose `write` just records the bytes sent:
  - `test_read_channels_sends_exact_request`: build a `Channels`-decodable
    214-byte zero payload, queue up a correctly-framed 215-byte inner +
    valid length-prefix/CRC response across the fake's `read_fully(2)`,
    `read_fully(215)`, `read_fully(4)` calls (in that order); call
    `read_channels()`; assert the transport's recorded `write` call equals
    `protocol.build_output_channel_request(0, 214)` exactly.
  - `test_read_channels_decodes_response`: same setup with a non-zero
    payload (reuse Task 2's known-value byte layout); assert the returned
    `Channels` matches.
  - `test_read_channels_raises_on_wrong_length_prefix`: fake's first
    `read_fully(2)` call returns a length prefix encoding `99` instead of
    `215`; assert `pytest.raises(BadResponse)` and that `read_fully` is
    never called again with that bogus value (i.e. the fake only needs to
    supply the first chunk).
  - `test_read_channels_propagates_crc_mismatch_as_bad_response`: fully
    framed response but with a corrupted CRC trailer; assert
    `pytest.raises(BadResponse)`.

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_connection.py -v`

- [ ] **Step 3: Implement `connection.py`**

  Port `RusefiBtConnection.kt` verbatim: module-level constants
  `OUTPUT_CHANNELS_OFFSET = 0`, `OUTPUT_CHANNELS_COUNT = 214`,
  `EXPECTED_INNER_LENGTH = 1 + OUTPUT_CHANNELS_COUNT`. `read_channels`
  writes the built request, reads exactly 2 bytes for the length prefix,
  validates it equals `EXPECTED_INNER_LENGTH` (raising `BadResponse`
  otherwise, without calling `read_fully` with the untrusted value), reads
  `EXPECTED_INNER_LENGTH` bytes for the inner, reads 4 bytes for the CRC,
  concatenates `length_prefix + inner + crc` into one frame, and calls
  `protocol.parse_response(frame)` then `channels.decode_channels(payload)`.

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_connection.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/connection.py ve_autotune/tests/windows_recorder/test_connection.py
git commit -m "feat(windows_recorder): port RusefiBtConnection from Android"
```

---

### Task 7: Poll loop (`run_session`)

**Files:**
- Create: `ve_autotune/windows_recorder/recorder.py`
- Test: `ve_autotune/tests/windows_recorder/test_recorder.py`

**Interfaces:**
- Consumes: `connection.RusefiBtConnection` (Task 6, structural — tests
  inject a fake with the same `read_channels() -> Channels` shape, raising
  `protocol.BadResponse` or `OSError`/`TimeoutError` as needed);
  `csv_logger.CsvLogger` (Task 4, structural fake with `append_row`);
  `bad_frame_tracker.BadFrameTracker` (Task 3, used internally, not
  injected)
- Produces:
  ```python
  def run_session(
      connection,              # has .read_channels() -> Channels, raises BadResponse/OSError/TimeoutError
      logger,                  # has .append_row(t_seconds: float, c: Channels)
      *,
      stop_event: threading.Event,
      on_status: Callable[[str], None],
      on_channels: Callable[[Channels], None],
      sleep: Callable[[float], None] = time.sleep,
      clock: Callable[[], float] = time.monotonic,
      max_consecutive_bad_frames: int = 5,
  ) -> str | None:  # error reason, or None for a clean stop
  ```

- [ ] **Step 1: Write failing tests** in `test_recorder.py`, with a fake
  connection whose `read_channels()` is driven by a pre-built list of
  "next action" values (a `Channels` instance to return, or an exception
  instance to raise), a fake logger recording `append_row` calls, a fake
  `sleep` recording its calls (so the test doesn't actually wait), and a
  real `threading.Event` for `stop_event`:
  - `test_clean_stop_returns_none`: connection always succeeds; set
    `stop_event` after the fake sleep is called twice; assert
    `run_session(...)` returns `None` and `logger`'s `append_row` was
    called at least twice with increasing `t_seconds` values (driven by a
    fake `clock` returning increasing values).
  - `test_bad_response_is_retried_not_fatal`: connection raises
    `protocol.BadResponse("noise")` once then succeeds on the next call,
    then `stop_event` is set; assert no exception propagates, `on_status`
    was called with a message mentioning the retry count, and `append_row`
    was still called for the successful read.
  - `test_fifth_consecutive_bad_response_ends_session`: connection raises
    `BadResponse` five times in a row with `max_consecutive_bad_frames=5`;
    assert `run_session` returns a non-`None` string mentioning "bad
    response" and `append_row` was never called.
  - `test_one_good_frame_resets_bad_counter`: connection raises
    `BadResponse` 4 times, succeeds once, raises `BadResponse` 4 more
    times, then `stop_event` set; assert the session does NOT end on the
    8th call (counter reset by the intervening success) — i.e. the method
    returns `None` once `stop_event` is observed, not an error string.
  - `test_transport_error_ends_session_immediately_not_retried`:
    connection raises `OSError("port gone")` on the very first call;
    assert `run_session` returns a non-`None` string mentioning the error,
    and this does NOT go through the bad-frame retry path (only 1 call to
    the connection total, not up to 5).

- [ ] **Step 2: Run tests, verify they fail**

  Run: `pytest ve_autotune/tests/windows_recorder/test_recorder.py -v`

- [ ] **Step 3: Implement `recorder.py`**

  Port `RecordingService.kt`'s polling coroutine as a plain loop: track
  `start_time = clock()`; loop while `not stop_event.is_set()`; call
  `connection.read_channels()` inside a `try`; on `BadResponse`, call
  `bad_frames.record_failure()` — if it returns `True`, set the error
  reason and `break`, else call `on_status(...)` with the retry count,
  `sleep(1.0)`, and `continue`; on `OSError`/`TimeoutError` (the transport
  failure case), set the error reason (unless this was a user-requested
  stop — check `stop_event.is_set()` before overwriting with an error
  message, mirroring `RecordingService.kt`'s `if (!userRequestedStop)`
  guard on its `IOException` branch) and `break`; on success, call
  `bad_frames.record_success()`, compute `t_seconds = clock() - start_time`,
  call `logger.append_row(t_seconds, channels)`, call `on_channels(channels)`,
  call `on_status(...)` with an RPM summary, then `sleep(1.0)`. Return the
  error reason (or `None`).

- [ ] **Step 4: Run tests, verify they pass**

  Run: `pytest ve_autotune/tests/windows_recorder/test_recorder.py -v`

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/recorder.py ve_autotune/tests/windows_recorder/test_recorder.py
git commit -m "feat(windows_recorder): add run_session poll loop ported from RecordingService"
```

---

### Task 8: Tkinter GUI

**Files:**
- Create: `ve_autotune/windows_recorder/gui.py`

**Interfaces:**
- Consumes: `serial_transport.open_port`, `serial_transport.SerialTransport`
  (Task 5); `connection.RusefiBtConnection` (Task 6); `csv_logger.CsvLogger`
  (Task 4); `recorder.run_session` (Task 7)
- Produces: `class App(tk.Tk)` and a `if __name__ == "__main__": App().mainloop()`
  entry point. No other module imports `gui.py` (it is the executable
  entry point PyInstaller targets in Task 9).

No unit tests for this task (tkinter event wiring and real serial I/O
aren't meaningfully unit-testable without a display/hardware — the logic
they wire together is already covered by Tasks 1-7). Verification is
manual, against the running app on this dev machine (which has a display
and `tkinter`/`pyserial` already available, confirmed before writing this
plan), exercising every path the lack of real hardware still allows:

- [ ] **Step 1: Implement the window layout**

  A combobox populated from `serial.tools.list_ports.comports()` (plus a
  "Refresh" button that re-populates it), a file-path entry with a
  "Browse..." button (`tkinter.filedialog.asksaveasfilename`, default
  extension `.csv`), a status label, five labels for live RPM/MAP/TPS/CLT/AFR
  values (blank until the first successful read), and Start/Stop buttons.
  Stop starts disabled.

- [ ] **Step 2: Implement the Start handler**

  On click: if no port or no output path is selected, show an error
  dialog (`tkinter.messagebox.showerror`) and return without starting
  anything. Otherwise immediately disable Start and enable Stop (before
  any I/O, so a second click can't start a second session), create a
  `threading.Event` as `stop_event` and a `queue.Queue` for
  worker-to-GUI messages, and start a daemon `threading.Thread` running a
  worker function that: opens the output CSV file (reporting a caught
  error through the queue and returning early if this fails); opens the
  serial port via `open_port` (same error-and-return pattern on failure,
  closing the CSV file first); builds the `SerialTransport`/
  `RusefiBtConnection`/`CsvLogger`; writes the CSV header; calls
  `recorder.run_session(...)` with `on_status`/`on_channels` callbacks that
  just put messages on the queue (never touch Tk widgets directly from the
  worker thread); closes the CSV file and serial port in a `finally`; puts
  a final "stopped" message (carrying the error reason, or `None`) on the
  queue.

- [ ] **Step 3: Implement the Stop handler**

  On click: if no session is running (`stop_event` is `None`), no-op.
  Otherwise call `stop_event.set()` and disable Stop immediately (Start
  stays disabled until the worker thread's "stopped" message arrives, so
  the GUI can't start a second session while the first is still tearing
  down).

- [ ] **Step 4: Implement the queue-polling loop**

  `self.after(100, self._poll_queue)`, re-scheduling itself, draining the
  queue each tick: a status message updates the status label; a channels
  message updates the five value labels; a "stopped" message re-enables
  Start, disables Stop, and shows an error dialog if the reason is not
  `None`.

- [ ] **Step 5: Manual verification on this dev machine**

  Run: `python3 -m ve_autotune.windows_recorder.gui` and confirm, by
  interacting with the real window: the port combobox populates (may be
  empty — no Bluetooth hardware here — but must not crash) and Refresh
  re-queries it; clicking Start with no port selected shows the validation
  error and does not start a thread; clicking Browse opens a real file
  save dialog; selecting a nonexistent/bogus port name (e.g. typing one
  into the combobox if it's editable, or picking any entry if the list is
  non-empty) and clicking Start shows a clean "could not open port" status
  within a couple seconds, not a crash or a hang; Stop is disabled until a
  session is running and disables itself immediately on click; closing the
  window while a session is running does not hang the process (add a
  window-close handler that calls the same stop logic before destroying
  the window if a session is active).

- [ ] **Step 6: Commit**

```bash
git add ve_autotune/windows_recorder/gui.py
git commit -m "feat(windows_recorder): add tkinter GUI wiring the recorder together"
```

---

### Task 9: Windows packaging

**Files:**
- Create: `ve_autotune/windows_recorder/requirements.txt`
- Create: `ve_autotune/windows_recorder/windows_recorder.spec`
- Create: `ve_autotune/windows_recorder/BUILD_WINDOWS.md`

**Interfaces:**
- Consumes: `gui.py` (Task 8) as the PyInstaller entry point. Nothing
  downstream consumes this task's files — it is the end of the chain.

- [ ] **Step 1: Write `requirements.txt`**

  ```
  pyserial==3.5
  pyinstaller>=6.0
  ```

- [ ] **Step 2: Write `windows_recorder.spec`**

  A standard one-file PyInstaller spec (`Analysis`/`PYZ`/`EXE` blocks)
  targeting `gui.py` as the entry script, `console=False` (it's a GUI
  app — no console window), producing `VeAutotuneRecorder.exe`. No hidden
  imports should be needed (`tkinter` and `serial` are both detected
  automatically by PyInstaller's hooks), but note in a comment that if the
  built `.exe` fails to start with a missing-module error, add
  `hiddenimports=["serial.tools.list_ports"]` to the `Analysis(...)` call
  as the first thing to try.

- [ ] **Step 3: Write `BUILD_WINDOWS.md`**

  Exact steps for the user to run on their own Windows machine: install
  Python 3 (link to python.org), open a terminal in this directory, run
  `pip install -r requirements.txt`, then
  `pyinstaller windows_recorder.spec`, then find
  `dist/VeAutotuneRecorder.exe`. Also document: pair the microRusEFI's
  Bluetooth module in Windows' own Bluetooth settings first, note the COM
  port Windows assigns it (Settings → Bluetooth & devices → Devices → More
  Bluetooth settings → COM Ports tab), and pick that port in the app.

- [ ] **Step 4: Verify by reading, not running**

  This dev machine cannot run PyInstaller for a Windows target. Verify
  instead: `windows_recorder.spec`'s entry script path resolves to the
  real `gui.py` file (a relative-path typo here would silently build an
  empty/broken exe); `requirements.txt`'s pyserial version matches what
  this dev venv already uses (`3.5`, confirmed earlier in this session) so
  the user isn't building against an untested version; `BUILD_WINDOWS.md`'s
  commands are copy-pasteable as written (no placeholder paths left in).

- [ ] **Step 5: Commit**

```bash
git add ve_autotune/windows_recorder/requirements.txt ve_autotune/windows_recorder/windows_recorder.spec ve_autotune/windows_recorder/BUILD_WINDOWS.md
git commit -m "feat(windows_recorder): add PyInstaller packaging and Windows build instructions"
```
