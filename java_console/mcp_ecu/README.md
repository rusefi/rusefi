# rusEFI ECU MCP server

An [MCP (Model Context Protocol)](https://modelcontextprotocol.io/) server that lets an
LLM client (Claude Desktop, JetBrains AI, Cursor, etc.) iterate on rusEFI Lua scripts:
write a candidate script, upload it to the ECU, reset Lua, and observe the resulting
`print(...)` / `efiPrintf` output. It also reads live ECU values and records operating
data to host-side `.mlg` files using the Java frontend's binary log format. It can
also read and write complete TunerStudio-compatible tunes, and convert existing
binary MLG and text TunerStudio MSL logs to CSV offline. The `update_firmware` tool
updates firmware via OpenBLT while backing up and migrating the ECU configuration.

## Architecture

```
LLM client  <-- stdio JSON-RPC (MCP) -->  EcuMcpServer
                                              |
                                              +-- LuaService (in :ecu_io)
                                              |       |
                                              |       +-- LinkManager / BinaryProtocol
                                              |       +-- BurnCommand
                                              |       +-- StringIniField (LUASCRIPT)
                                              |
                                              +-- CalibrationsHelper / CalibrationsUpdater
                                              |       +-- MSQ compatibility migration
                                              |       +-- page-aware write, burn, readback
                                              |
                                              +-- MessagesCentral listener
                                                      (same hook as Swing MessagesView,
                                                       captures Lua print() + efiPrintf)
```

`LuaService` (in `:ecu_io`) is the headless reusable core that `SetLuaTool` and
`EcuMcpServer` both depend on. It owns connecting to the ECU, locating the `LUASCRIPT`
ini field, writing/burning, and `luareset`.

## Prerequisite: a matching .ini file

Connecting requires a TunerStudio `.ini` file matching the ECU's signature hash. For
released firmware it is downloaded automatically from rusefi.com and cached under
`~/.rusEFI/ini_database/`. For custom/dev boards you must supply it yourself — see
[README-mcp.md](../../README-mcp.md) at the repo root for the full resolution chain and
troubleshooting of `Connection failed: Failed to located .ini`.

## Tools exposed over MCP

Behavior common to all tools:

- Every result is returned twice: as a JSON string in the text content part, and as
  `structuredContent` for clients that prefer it.
- Failures carry `success: false` plus an `error` string.
- Any tool that touches the ECU connects implicitly first (autodetect, 60 s connect
  timeout) — calling `connect` explicitly is optional.
- Integer arguments also accept numeric strings (`"5000"`); a non-numeric value silently
  falls back to the default.

| Tool | Purpose |
|---|---|
| `connect` | Connect to the ECU (autodetect or explicit port). |
| `ecu_info` | Signature + `LUASCRIPT` offset / size budget. |
| `set_lua` | Upload Lua: write `LUASCRIPT` + burn + `luareset`. |
| `get_lua` | Read currently-flashed Lua. |
| `lua_reset` | Restart the Lua VM. |
| `send_command`, `command` | Queue any text command. |
| `read_output_channel` | Latest gauge value by name. |
| `mount_to_ecu`, `mount_to_pc` | Switch SD-card ownership and confirm the reported mount mode. |
| `start_data_logging` | Record ECU operating data to a new `.mlg` file. |
| `stop_data_logging` | Stop recording and close the file. |
| `data_logging_status` | Recording state, file path, sample count, and errors. |
| `convert_log_to_csv` | Convert a host-side MLG/MSL log to CSV without connecting to an ECU. |
| `read_messages` | Pull recent ECU messages (Lua `print` included). |
| `wait_for_message` | Block until a message matches a regex. |
| `read_tune` | Save the complete ECU tune as a `.msq` file. |
| `write_tune` | Merge, burn, and verify a host-side `.msq` tune on the ECU. |
| `update_firmware` | Update via OpenBLT, back up and migrate the configuration, then reconnect. |
| `reboot` | Reboot the ECU. |
| `reboot_to_blt` | Reboot the ECU into the OpenBLT bootloader. |

### `convert_log_to_csv`

| Argument | Type | Required | Description |
|---|---|---|---|
| `inputPath` | string | yes | Existing binary MLVLG v2 or text TunerStudio MSL file on the MCP server host. |
| `outputPath` | string | no | New CSV file; defaults to the input path with its extension replaced by `.csv`. Parent directory must exist. |

Relative paths resolve against the server's working directory. No ECU connection
or INI file is needed. Stop recording before converting the log. Existing output
files are never overwritten. Conversion errors do not publish a partial CSV.

Returns `success`, absolute `path`, `inputFormat` (`mlg` or `msl`), `recordCount`,
and `fieldCount`. Example tool call:

```json
{"name":"convert_log_to_csv","arguments":{"inputPath":"/tmp/engine-run.mlg"}}
```

CSV columns follow the C++ `msl2csv` helper: one column per logged field, with
units appended to the column name. Binary values use `(raw + transform) * scale`
and the field's declared decimal precision (clamped to 0..12); NaN becomes an
empty cell. No timestamp column is synthesized from the wrapping record header;
a logged `Time` field is exported like any other field. Text MSL input must have
a tab-separated field-name row, units row, and data rows with matching widths.
Binary checksum mismatches, truncated records, unsupported scalar/block types,
and inconsistent headers fail conversion. Marker blocks are not supported.

The converter lives in `:mcp_ecu` as `com.rusefi.mcp.MslToCsv`, adapted from
`misc/mlg2csv/MlgToCsv.java` on `at32-vovansss`. It streams records and can also run
as a CLI after building `./gradlew :mcp_ecu:fatJar` (substitute the built jar path):

```bash
java -cp /path/to/mcp_ecu-all.jar com.rusefi.mcp.MslToCsv input.mlg output.csv
```

The output argument is optional. The existing C++ converter remains available
for unit-test tooling.

### `connect`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `port` | string | no | autodetect | Serial port name (e.g. `/dev/ttyACM0`, `COM5`). Overrides startup `--port`. Ignored if a connection is already active. |

Returns `connected` (boolean) and, once the binary protocol is up, `signature`.

### `ecu_info`

No arguments. Returns `ready` (false if the binary protocol is not established yet —
other fields are then absent), `signature`, and `luascript` with `offset` and `maxSize`.
`maxSize` is the script size budget in bytes; `set_lua` fails beyond it. The `luascript`
object is absent if the connected `.ini` has no `LUASCRIPT` field (Lua-less firmware).

### `set_lua`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `script` | string | one of `script`/`path` | — | Inline Lua source. ASCII only. Takes precedence if `path` is also given. |
| `path` | string | one of `script`/`path` | — | Path to a `.lua` file **on the MCP-server host** (not the client). `--include <file>` directives are expanded relative to the file's directory; a missing include fails the call. |
| `timeoutMs` | integer | no | 120000 | Write + burn timeout in milliseconds. |

Returns `success`, `bytesWritten`, `fieldSize` (same as `ecu_info`'s `maxSize`),
`burnSucceeded`, and `error` on failure.

### `get_lua`

No arguments. Returns `script` and `length`. Old firmware (script on the main config
page): reads the cached controller image — what was last written/fetched over this
connection. New firmware (script on the dedicated Lua page): fetched live from the ECU
on every call.

### `lua_reset`

No arguments. Returns `queued: true` — the `luareset` command was accepted into the
queue, not proof the ECU executed it.

### `send_command` (alias: `command`)

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `command` | string | yes | — | Command text, e.g. `lua 1+2`, `rpm`, `help`. |

Returns `queued: true` and echoes `command`. Fire-and-forget: any output arrives as ECU
messages — read it back with `read_messages` / `wait_for_message`.

### `read_output_channel`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `name` | string | yes | — | Output-channel (gauge) name, case-insensitive, e.g. `RPMValue`. |

Returns `found` plus `value` when found; `found: false` means the channel does not
exist **or** no data has arrived yet — retry after a moment before concluding the name
is wrong. The console's output-channel polling is subscription based (it fetches only
the byte ranges of channels somebody subscribed to), so the server holds a full-frame
lease for the lifetime of the ECU connection: every channel of the `.ini` is polled,
like before that change.

### `mount_to_ecu` and `mount_to_pc`

Switch the SD card to ECU ownership (logging and ECU-side file access) or PC
ownership (USB mass storage). The request uses the same binary command as the
TunerStudio SD Card dialog. The selection lasts until power-off or `sdmode auto`.

Optional `timeoutMs` defaults to 20000 and must be 1..120000. It covers command
acknowledgement and mount confirmation; implicit connection has its separate timeout.
These tools require firmware and a matching `.ini` exposing `sdCardMode` and
`sd_present`. Older firmware returns an error before sending a mount command.

Success requires a fresh output poll after acknowledgement reporting a present card
in the requested mode. Logging may be paused or suppressed in ECU mode; its activity
bit is not used to determine ownership. Results include `success`, `mounted`,
`requestedMode`, and, when a fresh status arrived, `sdCardMode`, `mode`, `sd_present`.
Timeouts and disconnects return an error with completion unconfirmed.

Safely eject the PC drive before calling `mount_to_ecu`. Switching ownership can
drop the shared USB console connection. Reconnect and use `read_output_channel`
with `name: "sdCardMode"` to check the result before retrying. Mode values are
0 idle, 1 ECU, 2 PC, 3 unmounted, 4 formatting. `sd_present` must also be set for
the card to be usable. A PC-mode report confirms firmware ownership, not that
the host OS has mounted a drive letter.

```json
{"name":"mount_to_ecu","arguments":{"timeoutMs":20000}}
{"name":"mount_to_pc","arguments":{}}
```

### `start_data_logging`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `path` | string | no | Temporary `rusefi_data_*.mlg` file | New file on the MCP server host; parent directory must exist. Relative paths resolve against the server's working directory. |
| `saveTune` | boolean | no | `true` | Save a tune snapshot in the same folder as the data log. |

Connects if necessary, creates the file and writes its header before returning.
Existing files are never overwritten. Starting while already recording fails and
leaves the active recording intact. Records all supported numeric and enum output
channels from the connected ECU's `.ini`, using the same field selection and binary
MLG format as the frontend. Computed expression channels are not included.

By default, start reads the ECU's configuration pages afresh on the communication
thread and saves a TunerStudio `.msq` tune beside the data log before recording
begins. The filename uses the server's local date: `YYYY-MM-DD.msq`. An identical
existing tune is reused; a changed tune uses `YYYY-MM-DD_1.msq`, then `_2`, etc.
Existing files are preserved, including invalid MSQ files. Comparison includes
firmware signature and calibration constants on each page, ignoring comments and
writer metadata. Unchanged numbered snapshots are reused too.

Set `"saveTune": false` to record only data. If the required tune read/save fails,
start fails without activating recording; any newly created data log is removed.
The snapshot describes the tune at logging start; edits during a recording do not
create further tune snapshots.

Each complete poll supplies one row; the sampling rate is the connection's polling
rate, not a separate high-speed ECU or SD-card logger. Recording continues between
MCP requests. Partial polls and snapshots predating the recording's full-output
subscription are excluded. Zero samples means no qualifying poll has arrived yet.

### `stop_data_logging` and `data_logging_status`

Neither takes arguments or requires an ECU connection. Stop closes the file and
releases recording resources; repeated stops are safe. Both return the same status
fields as start:

| Field | Meaning |
|---|---|
| `success` | False if recording encountered an encoding, write, or close error. |
| `logging` | Whether the recorder is active. |
| `path` | Absolute file path, or null before the first successful start. |
| `format` | `mlg`. |
| `tunePath` | Absolute path of the saved/reused tune, or null when tune saving is disabled or no recording has started. |
| `sampleCount` | Rows successfully written in this recording. |
| `channelCount` | Fields in the log, including the frontend's MAP compatibility alias. |
| `error` | Failure description, when present. |

After stopping, status retains the last recording's details. A write/encoding error
stops recording and remains visible until another recording starts successfully;
a partially written file may remain. A disconnected ECU supplies no new samples.
Reconnecting through an ECU tool stops the old recording to avoid mixing channel
layouts; start a new recording afterwards. Normal server shutdown or stdin failure
also closes the file. An abrupt process kill may leave an incomplete file.

Example tool calls:

```json
{"name":"start_data_logging","arguments":{"path":"/tmp/engine-run.mlg"}}
{"name":"data_logging_status","arguments":{}}
{"name":"stop_data_logging","arguments":{}}
```

### `read_messages`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `sinceSeq` | integer | no | -1 (all) | Only return messages with `seq` strictly greater than this. |
| `maxLines` | integer | no | 200 | Maximum number of messages to return. |
| `sourceFilter` | string | no | — | Substring filter on the source class name (e.g. `Lua`). |

Returns `messages` — each with `seq`, `timestamp` (ms epoch), `source`, `message` — and
`latestSeq`, the newest captured sequence number. Messages come **oldest-first** from a
10 000-entry ring buffer, and when more than `maxLines` match, the **newest are cut
off**: page forward by passing the `seq` of the last returned message as `sinceSeq`
(when truncated, `latestSeq` is further ahead than the last message returned).

### `wait_for_message`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `regex` | string | yes | — | Java regex, matched with `find()` semantics — succeeds if it matches **anywhere inside** the message text. Only the message text is matched, not `source`. |
| `timeoutMs` | integer | no | 10000 | Wait timeout in milliseconds. |
| `sinceSeq` | integer | no | -1 (all) | Only consider messages with `seq` strictly greater than this. |

Returns `success: true` with `match` (same shape as a `read_messages` entry), or
`success: false` with `error: "timeout"` and `latestSeq`. Already-buffered messages are
checked first, so without `sinceSeq` a stale message from minutes ago can satisfy the
wait — pass `sinceSeq` captured before triggering the action you're waiting on.

### `read_tune`

| Argument | Type | Required | Default | Description |
|---|---|---|---|---|
| `path` | string | no | temp file | Output `.msq` file path **on the MCP-server host** (not the client). Default: a temp file named `rusefi_tune_*.msq` in the system temp directory. |

Reads the complete tune (every calibration constant defined by the matching `.ini`) and
writes it as a TunerStudio-compatible `.msq` XML file — the same format the rusEFI
console and TunerStudio use for tune save/load. Every page declared by the matching
`.ini` is freshly read from the ECU when the tool is called, including secondary
calibration pages such as the dedicated Lua page on newer firmware.

Returns `path` (absolute), `constantCount`, `fileSize` (bytes) and `signature`. The XML
can easily run to hundreds of kilobytes — read the file with your own file tools
(ideally selectively) instead of trying to pull it through an MCP response.

### `write_tune`

| Argument | Type | Required | Description |
|---|---|---|---|
| `path` | string | yes | Existing input `.msq` path **on the MCP-server host** (not the client). |

Loads the MSQ and resolves the `.ini` named by its firmware signature. It freshly reads
all tune pages from the connected ECU, compatibility-migrates fields from the input tune
onto that current image, writes and burns only changed pages, then freshly reads all
pages again and requires an exact byte-for-byte match with the intended merged result.
The ECU's `vinNumber` is deliberately preserved, matching the console tune-import path.
Fields incompatible with the connected firmware are skipped and reported rather than
causing compatible fields to be discarded.

Returns `success`, absolute `path`, `constantCount`, `fileSize`, `sourceSignature`,
`ecuSignature`, `changed`, `verified`, `failedFields`, and `warnings`. `changed: false`
is a successful no-op when the ECU already contains the requested calibration. A write,
burn, or readback mismatch returns `success: false` and `verified: false`.

```json
{"name":"write_tune","arguments":{"path":"/tmp/edited-tune.msq"}}
```

### `update_firmware`

Runs the Console's automatic OpenBLT update with configuration migration. Start with a
live ECU connection over serial or SocketCAN; do not call `reboot_to_blt` first, since
running firmware is needed to back up the tune. TCP/simulator connections are unsupported.

| Argument | Type | Required | Description |
|---|---|---|---|
| `firmwarePath` | string | no | Existing S-record firmware file on the MCP server host. Omit to select the bundle firmware for the connected board (including its normal download behavior). |

```json
{"name":"update_firmware","arguments":{"firmwarePath":"/path/to/rusefi_update.srec"}}
```

The call blocks for several minutes while it backs up the current tune and INI,
flashes firmware, reads the new configuration, migrates compatible settings, writes
and burns the migrated tune, and reconnects. Allow a sufficiently long MCP client
request timeout. Data logging stops when the update job takes over the connection.
Explicit firmware files are parsed before connecting; known board mismatches and
firmware eligibility failures are rejected without dialogs.

The result includes `success`, `handoffStarted`, `connected`, `port`, firmware
`previousSignature`/`signature`, `warnings`, and `messages` (including backup paths).
Fields that cannot be migrated remain at firmware defaults and are reported in the
warnings/messages. A backup failure aborts before flashing. A later failure can leave
new firmware installed without the restored tune: inspect `messages` and the saved
backup before retrying. `connected` reports the connection separately from update
success; reconnect manually if the ECU did not reappear.

### `reboot`

No arguments. Queues the `reboot` command. Returns `queued: true`. The serial link
drops while the ECU restarts; the next ECU-touching tool call reconnects implicitly
(give the ECU a few seconds to re-enumerate).

### `reboot_to_blt`

No arguments. Queues the `reboot_openblt` command, rebooting the ECU into the OpenBLT
bootloader for firmware update. Returns `queued: true`. The firmware stops and the
device re-enumerates as an OpenBLT device — normal tools will not work again until the
ECU is power-cycled or new firmware is flashed.

### Reading output incrementally with `sinceSeq`

Every captured message gets a monotonically increasing `seq`. To avoid re-reading (or
matching stale) output, thread `latestSeq` through the loop:

1. `read_messages {}` → note `latestSeq` (call it `N`).
2. `set_lua { script: "..." }` (or `send_command`).
3. `wait_for_message { regex: "MY_MARKER:", sinceSeq: N }` — only matches messages
   produced *after* step 1.
4. `read_messages { sinceSeq: N }` to collect everything new. Iterate.

A typical LLM session:
1. `ecu_info` to confirm signature and script size budget.
2. `set_lua { script: "..." }` to upload.
3. `wait_for_message { regex: "MY_MARKER:" }` or `read_messages` to observe output.
4. Iterate.

## Run

```
./gradlew :mcp_ecu:runMcp          # run on stdio
./gradlew :mcp_ecu:fatJar          # produce build/libs/mcp_ecu-*-all.jar
java -jar build/libs/mcp_ecu-all.jar [--port /dev/ttyACM0]
```

Always target a real ECU over serial; the simulator is not reliable for MCP workflows.
Serial is USB CDC, so baud rate is irrelevant.

### Example client config (Claude Desktop)

```json
{
  "mcpServers": {
    "rusefi-lua": {
      "command": "java",
      "args": ["-jar", "/abs/path/to/mcp_ecu-all.jar"]
    }
  }
}
```

A Python client (`McpClient` in `java_console/mcp_python/`) is also available for
scripted/CI use — see [README-mcp.md](../../README-mcp.md).

## Gotchas

- **stdio is the transport.** All logging must go to file (`Logging`) or stderr, never
  stdout.
- **One ECU connection at a time.** Don't run this alongside the rusEFI console
  pointed at the same port. All ECU access is serialized via `LinkManager#submit(...)`.
- **ASCII Lua only.** The controller's `LUASCRIPT` field is ASCII; non-ASCII characters
  are not transmitted.
- **`send_command` / `lua_reset` only queue.** A `queued: true` reply means the command
  was accepted into the queue, not that the ECU executed it — confirm effects via
  `read_messages` / `wait_for_message`.
- **`MessagesCentral` runs on the AWT EDT.** A headless JRE is fine, but you can pass
  `-Djava.awt.headless=true` to be explicit.


## Field notes from real usage (2026-07)

### Client-side

- `read_messages` returns the **oldest** `maxLines` entries from the ring buffer,
  not the newest. Page with `sinceSeq` (using the returned `latestSeq`) or request
  a large `maxLines`; otherwise you'll re-read stale history and miss new output.
- `send_command` only queues the command; the result is observable solely through
  the message stream (`read_messages` / `wait_for_message`). The queue echoes
  `confirmation_<cmd>:<n>` when the ECU accepts it.
- A fresh `connect` spends up to ~60 s fetching the controller image, and every
  queued command waits behind that. Send a cheap marker command
  (e.g. `lua print('warm')`) and wait for it before trusting short timeouts.
- MCP servers registered with an LLM client are typically only picked up by
  sessions started afterwards. The server also speaks plain newline-delimited
  JSON-RPC on stdio, so a ~50-line script can drive it directly.

### ECU-side Lua

- The interactive `lua <snippet>` console command runs inside the RUNNING script
  VM but is capped at ~99 chars (`interactiveCmd[100]`), does not tolerate
  spaces reliably, and allows only one pending snippet at a time — expose work
  as no-arg global functions (`myHelper()`) and call those.
- **The VM is heavily trimmed: there is no Lua base library.** `string`, `table`,
  `type`, `tostring` are all nil (`string.format`, `table.concat` included) — an
  error like `attempt to index a nil value (global 'string')` aborts the whole
  calling snippet/function mid-way, leaving globals partially updated. Stick to
  core syntax (`..`, `#`, tables) plus rusEFI builtins: `print`, `math.floor`,
  `toHexString`, `byteToHex`, `arrayToString`, ...
- `print()` requires a string (numbers coerce fine): `print(nil)` and
  `print(true)` fail with "bad argument #1 to 'print' (string expected)" — and
  with no `tostring`, the fix is concatenation onto a literal or `x and'y'or'z'`.
- The ECU Lua number parser rejected `1e9` ("malformed number") — use plain
  integer literals (`1000000000`).
- `LUASCRIPT` is ASCII-only: non-ASCII characters (em-dashes in comments, etc.)
  are silently mangled to `???` on the controller. Sanitize before upload.
- Budget check: the field is 20-50K depending on controller; stripping
  full-line comments/indentation from a commented source tree is often enough
  to fit.

### Paging (new firmware)

- New firmware stores the script as ini field `luaScript` on a dedicated TS page
  (identifier 0x0400) instead of the main config page. `LuaService` therefore
  uses `findIniField` (searches secondary pages), page-aware
  `writeInBlocks`/`readFromPage`, and a page-aware burn — the firmware routes a
  Lua-page burn to `burnExtraFlashPage`, which is what makes scripts survive
  power cycles (`Flash: Writing storage ID 5` in the messages confirms it).
- Any live page read must run on the LinkManager thread
  (`linkManager.submit(...)`), or you get "Communication on wrong thread".
