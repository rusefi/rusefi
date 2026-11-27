# VE Autotune — Android BT Recorder Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** A minimal Android app that connects to the already-paired rusEFI
Bluetooth module, polls RPM/MAP/AFR/coolant/TPS over rusEFI's existing
TunerStudio binary protocol, and writes them to a CSV log the PC pipeline
(other plan, same `design.md`) can consume directly.

**Architecture:** Single-activity app. A protocol layer (`Crc`, `Frame`,
`OutputChannels`) ported from the exact framing already used by
`java_console`'s desktop client, read-only by construction — it can only
build an output-channel *read* request, nothing that writes to the ECU. A
`RusefiBtConnection` opens a classic-Bluetooth SPP socket to an
already-paired device and round-trips one frame at a time. A `CsvLogger`
polls on an interval and appends rows. `MainActivity` wires device
selection + start/stop.

**Tech Stack:** Kotlin, Android Gradle Plugin, `java.util.zip.CRC32`
(standard JDK class, available on Android — confirmed below, no custom CRC
needed), plain `BluetoothSocket`. No third-party BT or UI libraries.

**Spec:** `/home/whip/rusefi/ve_autotune/design.md` (see "Addendum: Android
recorder for Stage 1")

## Global Constraints

- The app never sends a write/burn command to the ECU — only the
  output-channel *read* command exists in this codebase at all (design.md
  addendum: "never writes to the ECU"). Enforce this by construction: the
  protocol layer has no function that builds any other command frame.
- Wire format, verified against this checkout's own protocol source (not
  guessed):
  - Outer envelope, both directions: `[2 bytes big-endian length of the
    inner command][inner command bytes][4 bytes big-endian CRC32 of the
    inner command bytes]` (`java_console/io/.../IoHelper.makeCrc32Packet`).
  - CRC32 is the **standard** `java.util.zip.CRC32` algorithm — same class
    exists on Android, so it is used directly, not reimplemented
    (`IoHelper.getCrc32`).
  - Request inner command = `['O' (0x4F) = Integration.TS_OUTPUT_COMMAND]`
    + `offset` as **little-endian** 16-bit + `count` as **little-endian**
    16-bit (`ByteRange.packOffsetAndSize`, which writes big-endian bytes of
    a pre-swapped value — net effect is little-endian on the wire; this is
    the one place the inner payload's endianness differs from the outer
    envelope's, and it must not be "fixed" to match).
  - Response inner payload = `[1 byte: 0x00 = Integration.TS_RESPONSE_OK]`
    + `count` raw bytes copied from the ECU's live output-channel buffer
    starting at `offset` (`BinaryProtocolServer.getOutputCommandResponse`).
    A first byte other than `0x00` means the ECU rejected the request.
  - Use `offset = 0`, `count = 214` — covers every channel this app needs
    in one round trip (below).
- Output-channel byte offsets within that 214-byte buffer (from this
  checkout's `.ini`, `/home/whip/.rusEFI/ini_database/425490096.ini`,
  little-endian STM32 native values — copy these constants with a comment
  citing the exact `.ini` line, do not re-derive them):
  - RPM: `RPMValue = scalar, U16, 4, "RPM", 1, 0` → unsigned 16-bit at byte
    4, scale 1.0.
  - Coolant temp: `coolant = scalar, S16, 14, "deg C", 0.01, 0` → signed
    16-bit at byte 14, scale 0.01.
  - TPS: `TPSValue = scalar, S16, 22, "%", 0.01, 0` → signed 16-bit at byte
    22, scale 0.01.
  - MAP: `MAPValue = scalar, U16, 30, "kPa", 0.0333333333333333, 0` →
    unsigned 16-bit at byte 30, scale 0.0333333333333333.
  - AFR: `AFRValue = scalar, U16, 210, "AFR", 0.001, 0` → unsigned 16-bit at
    byte 210, scale 0.001 (already the ECU's computed AFR, not a raw
    voltage — no calibration math needed on the Android side).
- Output CSV header must use the same logical channel names the PC
  pipeline's `channel_map` expects: `rpm`, `map`, `tps`, `clt`, `afr`, plus
  `t` for the poll timestamp in seconds since logging started (matches
  `ve_autotune/pipeline/filters.py`'s expected row shape in the other plan).
- Pairing happens once in Android's own Bluetooth settings, outside this
  app; `MainActivity` only lists already-bonded devices.

## Review Focus

- A response whose first byte isn't `0x00` (ECU rejected the request, e.g.
  bad offset/count) — must surface as a connection error, not be silently
  parsed as if it were channel data. (Task 3)
- A response whose outer CRC32 doesn't match the received bytes (BT link
  corruption) — must be dropped/retried, never decoded. (Task 3)
- A `BluetoothSocket` read that returns a partial frame (fewer bytes than
  the declared length) — must keep reading until the full frame arrives
  (or the connection drops), not decode a short buffer as if it were
  complete. (Task 4)
- The device being out of range / connection dropped mid-session — Start
  must surface this as a visible status, and Stop must still flush and
  close whatever rows were captured so far. (Task 5/6)
- Negative/signed fields (`coolant`, `TPSValue` are `S16`) decoded as
  unsigned would silently produce wrong values for any reading below zero
  — must decode as signed. (Task 2)

---

### Task 1: Project scaffold

**Files:**
- Create: `ve_autotune/android/settings.gradle.kts`
- Create: `ve_autotune/android/build.gradle.kts`
- Create: `ve_autotune/android/app/build.gradle.kts`
- Create: `ve_autotune/android/app/src/main/AndroidManifest.xml`
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/MainActivity.kt`
  (placeholder: a single `TextView` reading "VE Autotune Recorder")
- Create: `ve_autotune/android/gradle/wrapper/...` (via `gradle wrapper`)

**Interfaces:**
- Produces: a buildable Android app module (`applicationId
  "com.rusefi.veautotune"`, `minSdk 26`, `targetSdk 34`) that later tasks add
  source files into.

- [ ] **Step 1: Scaffold the Gradle project**

Use `gradle init`-equivalent manual files (no network-dependent wizard):
root `settings.gradle.kts` including `:app`; root `build.gradle.kts` with
the Android Gradle Plugin + Kotlin plugin versions; `app/build.gradle.kts`
with `minSdk 26`/`targetSdk 34`, `BLUETOOTH_CONNECT` handling (see Step 3).
Generate the wrapper: `gradle wrapper --gradle-version 8.7` (installs a
temporary Gradle if none is present, writes `gradlew`/`gradlew.bat`).

- [ ] **Step 2: Run the build to verify the scaffold compiles**

Run: `cd ve_autotune/android && ./gradlew assembleDebug`
Expected: `BUILD SUCCESSFUL` (first run also downloads the Android SDK
platform/build-tools via the plugin — needs network and may take a while;
see design.md addendum's toolchain note).

- [ ] **Step 3: Add manifest permissions**

In `AndroidManifest.xml`: `<uses-permission android:name="android.permission.BLUETOOTH_CONNECT" />`
(API 31+) and `<uses-permission android:name="android.permission.BLUETOOTH" android:maxSdkVersion="30" />`
/ `BLUETOOTH_ADMIN` (legacy, API ≤30) — both needed since `minSdk 26` spans
the permission-model change.

- [ ] **Step 4: Re-run the build to verify it still succeeds**

Run: `cd ve_autotune/android && ./gradlew assembleDebug`
Expected: `BUILD SUCCESSFUL`

- [ ] **Step 5: Commit** (leave on disk — same "don't commit to upstream
rusEFI" note as the PC pipeline plan; this is a new, uncommitted directory)

---

### Task 2: CRC + output-channel decoding (`Crc.kt`, `OutputChannels.kt`)

**Files:**
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/protocol/Crc.kt`
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/protocol/OutputChannels.kt`
- Test: `ve_autotune/android/app/src/test/java/com/rusefi/veautotune/protocol/CrcTest.kt`
- Test: `ve_autotune/android/app/src/test/java/com/rusefi/veautotune/protocol/OutputChannelsTest.kt`

**Interfaces:**
- Produces:
  - `fun crc32(bytes: ByteArray): Long` — thin wrapper over
    `java.util.zip.CRC32` (`update(bytes)`, return `.value`).
  - `data class Channels(val rpm: Double, val map: Double, val tps: Double, val clt: Double, val afr: Double)`
  - `fun decodeChannels(payload: ByteArray): Channels` — `payload` is the
    214-byte buffer (response with its leading `0x00` status byte already
    stripped by the caller). Reads little-endian `U16`/`S16` at the exact
    byte offsets and applies the exact scales from Global Constraints.

- [ ] **Step 1: Write the failing tests**

```kotlin
// CrcTest.kt — same vectors as java_console's own CRCTest.java, ported verbatim
// so correctness is checked against an existing trusted vector, not a fresh guess.
@Test
fun `crc32 matches known vector`() {
    // read java_console/io/src/test/java/com/rusefi/binaryprotocol/test/CRCTest.java
    // before writing this test and copy its exact input bytes + expected CRC value here.
}

// OutputChannelsTest.kt
@Test
fun `decodeChannels reads little-endian fields at the documented offsets`() {
    val buf = ByteArray(214)
    // RPM = 1500 at offset 4 (U16 LE, scale 1.0)
    buf[4] = 0xDC.toByte(); buf[5] = 0x05.toByte()  // 1500 = 0x05DC
    // MAP = 45.0 kPa at offset 30 (U16 LE, raw = round(45.0 / 0.0333333333333333) = 1350)
    buf[30] = 0x46.toByte(); buf[31] = 0x05.toByte() // 1350 = 0x0546
    // coolant = -5.00 C at offset 14 (S16 LE, raw = -500)
    val cltRaw = (-500).toShort()
    buf[14] = cltRaw.toByte(); buf[15] = (cltRaw.toInt() shr 8).toByte()
    val c = decodeChannels(buf)
    assertEquals(1500.0, c.rpm, 0.001)
    assertEquals(45.0, c.map, 0.01)
    assertEquals(-5.00, c.clt, 0.01)
}
```

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*protocol*"`
Expected: FAIL

- [ ] **Step 3: Read `CRCTest.java` and fill in the real vector, then
implement `Crc.kt` and `OutputChannels.kt`**

Run: `cat /home/whip/rusefi/java_console/io/src/test/java/com/rusefi/binaryprotocol/test/CRCTest.java`
first, copy its exact input/expected-output into the Kotlin test above, then
implement both files per the Produces block (little-endian reads: `(bytes[offset].toInt() and 0xFF) or ((bytes[offset+1].toInt() and 0xFF) shl 8)`
for `U16`; same expression cast through `.toShort()` for `S16`).

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*protocol*"`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 3: Request/response framing (`Frame.kt`)

**Files:**
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/protocol/Frame.kt`
- Test: `ve_autotune/android/app/src/test/java/com/rusefi/veautotune/protocol/FrameTest.kt`

**Interfaces:**
- Consumes: `crc32` (Task 2).
- Produces:
  - `fun buildOutputChannelRequest(offset: Int, count: Int): ByteArray` —
    returns the full outer-enveloped request bytes, per Global Constraints'
    wire format (inner command `'O'` + LE16 offset + LE16 count; outer
    BE16 length + inner bytes + BE32 CRC32 of the inner bytes).
  - `class BadResponse(message: String) : Exception(message)`
  - `fun parseResponse(frame: ByteArray): ByteArray` — `frame` is the full
    outer envelope read off the socket (length already consumed to know how
    much to read — see Task 4). Verifies the trailing CRC32 against the
    inner bytes; throws `BadResponse` on mismatch. Verifies the first inner
    byte is `0x00`; throws `BadResponse("ECU rejected request: 0x%02x")`
    otherwise. Returns the remaining inner bytes (the 214-byte channel
    buffer) on success.

- [ ] **Step 1: Write the failing tests**

```kotlin
@Test
fun `buildOutputChannelRequest encodes command and little-endian offset-count`() {
    val req = buildOutputChannelRequest(0, 214)
    // inner = ['O', 0x00,0x00, 0xD6,0x00] = 5 bytes; outer length prefix = 5
    assertEquals(0, req[0].toInt()); assertEquals(5, req[1].toInt())
    assertEquals('O'.code.toByte(), req[2])
    assertEquals(0, req[3].toInt()); assertEquals(0, req[4].toInt())       // offset=0 LE
    assertEquals(0xD6.toByte(), req[5]); assertEquals(0x00.toByte(), req[6]) // count=214 LE
    assertEquals(11, req.size) // 2 (len) + 5 (inner) + 4 (crc)
}

@Test
fun `parseResponse rejects bad crc`() {
    val good = /* build a valid frame: 0x00 status + 214 zero bytes, correct crc */ TODO()
    val corrupted = good.copyOf().also { it[it.size - 1] = (it[it.size - 1] + 1).toByte() }
    assertThrows(BadResponse::class.java) { parseResponse(corrupted) }
}

@Test
fun `parseResponse rejects non-OK status byte`() {
    // build a valid-CRC frame whose inner[0] != 0x00
    assertThrows(BadResponse::class.java) { parseResponse(/* ... */ TODO()) }
}

@Test
fun `parseResponse returns 214-byte payload on success`() {
    val frame = /* valid 0x00 + 214 zero bytes + correct crc, with outer length prefix */ TODO()
    assertEquals(214, parseResponse(frame).size)
}
```

(Build the `TODO()` fixtures using `buildOutputChannelRequest`'s own inner
helper, or a small local test helper that mirrors the outer-envelope
format — the point of each test is documented in its name; fill in actual
bytes using `crc32` from Task 2 so the fixtures are self-consistent rather
than copied from elsewhere.)

- [ ] **Step 2: Run tests to verify they fail**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*FrameTest*"`
Expected: FAIL

- [ ] **Step 3: Implement `Frame.kt`** per the Produces block above.

- [ ] **Step 4: Run tests to verify they pass**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*FrameTest*"`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 4: Bluetooth connection (`RusefiBtConnection.kt`)

**Files:**
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/bt/RusefiBtConnection.kt`
- Test: `ve_autotune/android/app/src/test/java/com/rusefi/veautotune/bt/RusefiBtConnectionTest.kt`

**Interfaces:**
- Consumes: `buildOutputChannelRequest`/`parseResponse` (Task 3).
- Produces:
  - `interface ByteTransport { fun write(bytes: ByteArray); fun readFully(n: Int): ByteArray }`
    — thin seam so this class is testable without a real `BluetoothSocket`
    (a fake implementation backs the tests; the real app wires a
    `BluetoothSocket`'s input/output streams behind this interface in
    Task 5).
  - `class RusefiBtConnection(private val transport: ByteTransport)` with
    `fun readChannels(): Channels` — calls `transport.write(buildOutputChannelRequest(0, 214))`,
    then `transport.readFully(2)` for the outer length prefix (big-endian
    `Short`), `readFully(length)` for the inner bytes, `readFully(4)` for
    the CRC, concatenates length+inner+crc into one buffer, passes it to
    `parseResponse`, and `decodeChannels` (Task 2) on the result.

- [ ] **Step 1: Write the failing test**

```kotlin
class FakeTransport(private val response: ByteArray) : ByteTransport {
    val written = mutableListOf<ByteArray>()
    private var pos = 0
    override fun write(bytes: ByteArray) { written.add(bytes) }
    override fun readFully(n: Int): ByteArray {
        val chunk = response.copyOfRange(pos, pos + n); pos += n; return chunk
    }
}

@Test
fun `readChannels sends one request and decodes the response`() {
    // build a valid response frame exactly as parseResponse/Task3's fixtures do,
    // with RPM=1500 encoded at offset 4 of the 214-byte payload
    val transport = FakeTransport(validResponseFrame)
    val channels = RusefiBtConnection(transport).readChannels()
    assertEquals(1500.0, channels.rpm, 0.001)
    assertEquals(1, transport.written.size)
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*RusefiBtConnectionTest*"`
Expected: FAIL

- [ ] **Step 3: Implement `ByteTransport`/`RusefiBtConnection`** per the
Produces block. `readFully` on the real implementation (Task 5) must loop
on `InputStream.read` until `n` bytes are collected or the stream closes —
this test only has to prove `RusefiBtConnection` calls `readFully` with the
right sizes in the right order; the looping behavior itself belongs to
Task 5's real `ByteTransport`.

- [ ] **Step 4: Run test to verify it passes**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*RusefiBtConnectionTest*"`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 5: Real socket transport + CSV logger (`SocketTransport.kt`, `CsvLogger.kt`)

**Files:**
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/bt/SocketTransport.kt`
- Create: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/logger/CsvLogger.kt`
- Test: `ve_autotune/android/app/src/test/java/com/rusefi/veautotune/logger/CsvLoggerTest.kt`
  (`SocketTransport` itself needs a real `BluetoothDevice`/Android runtime
  and is exercised manually on-device per Task 6, not in a JVM unit test)

**Interfaces:**
- Consumes: `ByteTransport` (Task 4, implemented here against a real
  `BluetoothSocket`'s streams), `Channels` (Task 2).
- Produces:
  - `class SocketTransport(private val socket: BluetoothSocket) : ByteTransport`
    — `write` writes to `socket.outputStream` and flushes; `readFully(n)`
    loops on `socket.inputStream.read(buffer, offset, remaining)` until `n`
    bytes are collected, throwing `java.io.EOFException` if `read` returns
    `-1` first.
  - `object RusefiSpp { val UUID: java.util.UUID }` set to the standard SPP
    UUID `00001101-0000-1000-8000-00805F9B34FB`, used with
    `BluetoothDevice.createRfcommSocketToServiceRecord`.
  - `class CsvLogger(private val out: java.io.Writer)` with `fun writeHeader()`
    (writes `"t,rpm,map,tps,clt,afr\n"`) and `fun appendRow(tSeconds: Double, c: Channels)`
    (writes one CSV line, flushes after each row so partial logs survive a
    crash/disconnect).

- [ ] **Step 1: Write the failing test**

```kotlin
@Test
fun `writeHeader then appendRow produces expected csv lines`() {
    val sw = StringWriter()
    val logger = CsvLogger(sw)
    logger.writeHeader()
    logger.appendRow(0.5, Channels(rpm = 1500.0, map = 45.0, tps = 10.0, clt = 80.0, afr = 13.2))
    assertEquals("t,rpm,map,tps,clt,afr\n0.5,1500.0,45.0,10.0,80.0,13.2\n", sw.toString())
}
```

- [ ] **Step 2: Run test to verify it fails**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*CsvLoggerTest*"`
Expected: FAIL

- [ ] **Step 3: Implement `CsvLogger.kt` and `SocketTransport.kt`** per the
Produces block above.

- [ ] **Step 4: Run test to verify it passes**

Run: `cd ve_autotune/android && ./gradlew :app:testDebugUnitTest --tests "*CsvLoggerTest*"`
Expected: PASS

- [ ] **Step 5: Commit** (leave on disk)

---

### Task 6: Main activity — device picker, start/stop, polling loop

**Files:**
- Modify: `ve_autotune/android/app/src/main/java/com/rusefi/veautotune/MainActivity.kt`
- Modify: `ve_autotune/android/app/src/main/AndroidManifest.xml` (already has
  BT permissions from Task 1; no change expected unless the runtime
  permission request reveals a gap)

**Interfaces:**
- Consumes: `RusefiBtConnection`/`SocketTransport` (Tasks 4-5), `CsvLogger`
  (Task 5), `RusefiSpp.UUID` (Task 5).
- Produces: a UI with a dropdown/list of `BluetoothAdapter.getDefaultAdapter().bondedDevices`,
  a Start/Stop toggle button, and a status `TextView`. On Start: requests
  `BLUETOOTH_CONNECT` at runtime if not already granted, opens a socket to
  the selected device via `createRfcommSocketToServiceRecord(RusefiSpp.UUID)`,
  creates a `CsvLogger` writing to a timestamped file under the app's
  external files dir, and polls `RusefiBtConnection.readChannels()` once
  per second on a background coroutine, appending a row each time and
  updating the status text with the latest RPM. On Stop (or on a read
  failure): closes the socket, flushes/closes the CSV file, and shows its
  final path in the status text.

- [ ] **Step 1: Wire the layout**

Add a `RecyclerView`/`Spinner` (either is fine — builder's call, no test
pins this) bound to `bondedDevices`, a `Button` for Start/Stop, and a
`TextView` for status, in `activity_main.xml` + `MainActivity.kt`.

- [ ] **Step 2: Implement the Start path**

Runtime-permission check → socket open → `CsvLogger` creation → polling
coroutine loop (`kotlinx.coroutines`, add the dependency in
`app/build.gradle.kts`) calling `readChannels()` + `appendRow()` once per
second, catching `BadResponse`/`IOException` per iteration and surfacing
the message in the status `TextView` without crashing the app.

- [ ] **Step 3: Implement the Stop path**

Cancels the polling coroutine, closes the socket, closes the `CsvLogger`'s
writer, shows the final file path.

- [ ] **Step 4: Manual on-device verification**

Run: `cd ve_autotune/android && ./gradlew installDebug` with a phone
connected via `adb` and already Bluetooth-paired with the rusEFI module
(per design.md, pairing is done in Android Settings, not by this app).
Expected: app installs, Start connects and the status text shows live RPM
changing, Stop produces a non-empty CSV file on the phone matching the
header from Task 5. This step cannot be run from this machine without the
user's phone and bike present — note its result in the task's commit
message once performed, or leave it explicitly unchecked with a note if
deferred.

- [ ] **Step 5: Commit** (leave on disk)

---

## Not covered by this plan (explicitly deferred per design.md)

- Any write/burn command — by design this app only ever sends
  `TS_OUTPUT_COMMAND` reads (Global Constraints).
- Pulling the CSV off the phone onto the PC for Stage 3 (`analyze`) — plain
  file transfer (USB-MTP, a cloud folder, email to self); not application
  logic, no task needed.
- Real on-bike verification beyond Task 6 Step 4's manual check — full
  confidence in the live protocol round-trip only comes from a real ride,
  which depends on the engine running on fuel (still outstanding per
  design.md "Current state").
