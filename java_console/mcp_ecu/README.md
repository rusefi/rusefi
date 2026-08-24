# rusEFI ECU MCP server

An [MCP (Model Context Protocol)](https://modelcontextprotocol.io/) server that lets an
LLM client (Claude Desktop, JetBrains AI, Cursor, etc.) iterate on rusEFI Lua scripts:
write a candidate script, upload it to the ECU, reset Lua, and observe the resulting
`print(...)` / `efiPrintf` output.

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
| `read_messages` | Pull recent ECU messages (Lua `print` included). |
| `wait_for_message` | Block until a message matches a regex. |
| `read_tune` | Save the complete ECU tune as a `.msq` file. |
| `reboot` | Reboot the ECU. |
| `reboot_to_blt` | Reboot the ECU into the OpenBLT bootloader. |

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
is wrong.

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
console and TunerStudio use for tune save/load. The tune is built from the controller
configuration image fetched over this connection (read in full at connect and kept in
sync by writes made through this connection).

Returns `path` (absolute), `constantCount`, `fileSize` (bytes) and `signature`. The XML
can easily run to hundreds of kilobytes — read the file with your own file tools
(ideally selectively) instead of trying to pull it through an MCP response.

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
