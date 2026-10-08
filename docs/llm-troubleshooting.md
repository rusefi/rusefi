# Console troubleshooting assistant

The tab is available in online Console sessions for acceptance testing. It is
hidden in offline and log-viewer modes. The integration and tools are packaged
in the Console JAR.

The **Troubleshooting** tab uses ChatGPT with the Console's existing ECU
connection. Connect to an ECU and open the tab. It first checks and extracts
the cached source ZIP in the background without making a network request.
If the ZIP is missing, too small, expired or corrupt, the panel shows a source
download prompt, **Start Download**, a progress bar and status text. All chat
controls stay hidden until ZIP validation and extraction finish successfully.
Download failures keep this screen visible and allow another attempt.

Once sources are ready, select a saved account or choose
**Continue with ChatGPT**, select a model, and describe the problem. Sign-in
opens the browser automatically; if that fails, the tab displays a copyable link.
The app identifies itself as **rusEFI Updater**.

The terminal shows streamed answers and tool names as they run. User messages,
requested ECU evidence (including selected tune values, Lua source and captures)
and retrieved source/wiki excerpts are sent to OpenAI
using the selected ChatGPT plan.
ECU access is read-only; case export writes a local report:

| Tool | Evidence |
| --- | --- |
| `ecu_info` | Connected firmware signature and Lua field availability |
| `list_output_channels` | INI datalog channel names, labels, descriptions and units, with a substring filter |
| `read_output_channel` | One value from a recent full poll, with its host timestamp and age |
| `read_live_values` | Up to 32 selected channels from one full poll, with a shared sample ID, timestamp and age |
| `diagnostic_snapshot` | Selected live channels and raw warning/error channels from the same full poll |
| `read_tune_fields` | Selected scalar, enum/bitfield and small-array calibrations, read freshly from their ECU pages |
| `get_lua` | Bounded Lua source lines from fresh ECU RAM reads, with citations and a source hash |
| `capture_live_log` | A short in-memory series of selected channels from new full polls |
| `capture_engine_sniffer` | The next received chart, with bounded events and channel summaries |
| `read_messages` | Console/ECU messages observed since this conversation started, with sequence numbers |
| `search_knowledge` | Literal or keyword matches in cached firmware/wiki text, with paths and line numbers |
| `read_knowledge` | Bounded line ranges, file hashes and citation metadata from cached text |
| `export_diagnostic_case` | Save findings, hypotheses, next measurements and selected retained evidence to local JSON |

Message capture uses the same global MessagesCentral stream as the Console,
including UI diagnostics. It does not retrieve historical ECU messages. Channel
reads wait up to three seconds for a sample and reject samples older than two
seconds. Timestamps describe host reception, not an ECU hardware clock.

For correlated readings, `read_live_values` accepts `{"names":["RPMValue","VBatt"]}`.
`diagnostic_snapshot` accepts the same optional list; omitting it selects
`RPMValue`, `VBatt`, `isCranking`, `MAPValue`, `TPSValue`, `coolant` and `intake`.
Names are case-insensitive, unique and limited to 32 per request. Discover the
connected firmware's available names with `list_output_channels`.

Channel discovery returns up to 100 datalog entries. Its case-insensitive
`filter` matches names, labels, returned descriptions and known units. Descriptions
use a literal gauge title when available, otherwise the datalog label;
`descriptionSource` identifies which. These are INI display descriptions, not
additional diagnostic guidance. If several gauges name the channel, the first
literal title in gauge-name order is used.

Units come from the output-channel definition first. If it supplies no units
metadata, agreeing literal gauge units are used as a fallback. `unitsSource`
identifies the source. `unitsStatus` is `known`, `unknown`, `dynamic` or
`conflicting`; only `known` returns a units string. A known empty string means
the INI specifies no unit. Dynamic unit expressions are not evaluated, and
disagreeing gauge units are not guessed. No tune data is needed for discovery.
Labels and descriptions are bounded to 256 and 512 characters respectively;
overlong units are marked unknown. `metadataTruncated` identifies shortened
metadata, while result-level `truncated` means to narrow the filter because
the entry-count or JSON size limit was reached.

Both tools return a `values` array and shared `sampleId`, `sampleTimestampMs`,
`sampleAgeMs` and firmware `signature`. Sample IDs increase within a conversation;
successive reads may reuse the same recent poll. All returned values come from
that completed host poll even if another poll arrives during the call. A host
poll can read multiple ECU chunks, so this is not an atomic hardware measurement.
Unavailable or non-finite channels have `found: false` and no numeric value.

The snapshot also returns a `faults` array containing `checkEngine`,
`hasCriticalError`, `isWarnNow`, `isTriggerError`, `warningCounter`, `lastErrorCode`
and `recentErrorCode1` through `recentErrorCode8`. These are raw channel values,
not decoded DTC descriptions or a complete fault inventory. Counters and
last/recent codes can describe past events. Missing channels never imply that
the ECU is fault-free; interpret codes using documentation for its firmware.
Console messages have their own timestamps and are read separately.

`read_tune_fields` accepts `{"names":["cranking_rpm","displacement"]}` with
1-32 unique, case-insensitive INI calibration names. Names can be found in the
local source/INI knowledge; they are different from live output-channel names.
Only the selected field ranges are read on the Console communication thread,
including fields on secondary pages. The tool does not read the entire tune,
use the editable Console tune cache, change settings, burn, or clear faults.

Supported fields are scalars, enums/bitfields and numeric arrays of at most
64 elements each, with a total budget of 512 values (at most 2 KiB of field
data) per call. Strings, including Lua scripts, and larger arrays are excluded.
Arrays return rows and columns in INI order. Scalars and arrays use the
Console's parsed INI scaling; enums return their numeric value and an INI label
when one exists. Unit expressions are not evaluated. Non-finite values, unknown
fields, unsupported types, invalid page ranges and failed reads have explicit
per-field errors; `complete: false` means some requested fields failed.

Results include firmware signature, `source: ecu`, field page/offset and host
read timestamps. These are fresh sequential ECU RAM reads, separate from live
poll samples, not an atomic tune snapshot or proof of flash persistence. Reads
have a ten-second overall deadline. Stop, timeout or connection replacement
discards the result and stops further chunks; an already-running wire transaction
finishes normally so cancellation does not interrupt the shared connection.

`get_lua` accepts optional `start_line` (default 1) and `max_lines` (default 80,
maximum 120). It reads the declared `LUASCRIPT` field freshly from its main or
secondary page, up to 64 KiB, with the same ten-second deadline and chunk-level
cancellation as calibration reads. Results contain at most 6,000 source characters
and 2,000 characters per line, truncation flags, a next-line hint, citations such
as `ecu:LUASCRIPT:L1-L20`, and a SHA-256 hash of the ASCII source before its NUL
terminator. Compare hashes across paged reads to detect edits. RAM source does
not establish which script is running in the Lua VM or saved in flash. The tool
does not resolve includes, execute scripts, write, burn or reset Lua.

`capture_live_log` requires `names` (1-16 channel names). Optional `duration_ms`
defaults to 2,000 and ranges from 100 to 10,000; `max_samples` defaults to 20 and
ranges from 1 to 20. It observes only full polls completed after capture starts,
returning at most one sample per 100 ms. Each sample includes its ID, timestamp,
age and selected values. Reaching the sample or JSON-size cap marks the result
partial. No new polls gives an explicit error. This is downsampled trend evidence,
not a complete or high-rate recording, and it writes no MLG/CSV or tune files.

`capture_engine_sniffer` accepts `timeoutMs` (default 5,000, maximum 10,000) and
`max_events` (default 100, maximum 128). It observes the next nonempty chart using
the existing firmware settings, alongside the Console UI. No acquisition settings
are changed and no enable/reset command is sent. It returns at most 32 channel
summaries and omits raw chart text. Event count and duration describe the full
chart even when returned events or summaries are truncated. Event times are
chart-relative; `receivedAt` is host time. Invalid/oversized charts and timeouts
are explicit errors. Completion, Stop and connection changes release only the
assistant's observer, leaving the Console listener intact.

**Stop** cancels the current turn. A turn also has a two-minute cancellation
deadline, up to eight model requests and 24 tool calls. Failed, stopped and
incomplete turns are not added to the next request. Network disconnect/read
timeouts can delay cancellation finishing. **New conversation** clears the
conversation and releases its polling/message subscriptions. Changing account
or model also starts fresh. A changed ECU connection discards an in-progress
turn; the next Send starts a fresh conversation.

Authorization registrations, the stable host UUID, and refresh tokens remain
in `~/.rusefi/llm-access/accounts.json`. Conversation history is memory-only except for explicitly exported cases.
Sign out clears local tokens and attempts remote revocation. Only one instance
can open an account store at a time.

## Diagnostic case export

Ask the assistant to "export a diagnostic case" after collecting evidence. The
`export_diagnostic_case` tool saves a UTF-8 JSON file under
`~/.rusefi/llm-access/diagnostic-cases/` (or `diagnostic-cases/` inside a custom
account-store directory). The terminal displays the saved path independently of
the model's final answer. Each export creates a new file and returns its SHA-256,
byte count and evidence count; existing cases are never overwritten.

Arguments are `findings`, `hypotheses` and `next_measurements` (each 1-4000
characters; use "None established" when appropriate), plus `evidence_ids`
(1-64 unique IDs returned by prior tools). No destination path or arbitrary
attachment is accepted. Evidence IDs are added to ECU and knowledge tool results;
the exporter copies the original retained results, not model-supplied evidence.
This includes selected tune values, Lua excerpts, live-log samples, sniffer
events, messages and source/wiki passages with their timestamps, hashes,
citations, errors, missing values, truncation flags and provenance intact.

The versioned report separates model-authored analysis from observations. It
includes the firmware identity, Console version, conversation ID and export
time. Source, libfirmware, wiki and client revisions remain explicitly unknown;
source/ECU compatibility remains unverified. A case is selected bounded evidence,
not a full tune, MLG recording or raw sniffer capture. It contains neither the
credential store nor opaque model reasoning. Review findings and included
ECU/Lua data before sharing a case.

Retention is limited to 128 results and 2 MiB per conversation. Reaching a limit
returns `evidence_retained: false`; older evidence is not silently evicted. Start
a new conversation to collect more exportable evidence. Completed turns retain
evidence for later export; failed/stopped turns do not add to the next turn's
retention. New conversations and changed connections cannot reference old IDs.
Cancellation or connection changes detected during export remove incomplete
files. A successfully saved case remains on disk even if a later model request
fails or the conversation is cleared; delete unwanted cases manually.

## Implementation

`LLMTab` is production UI code. `ChatGptClient` implements the existing browser
OAuth flow and streaming Responses transport. `ChatGptAgent` retains complete
response output items (including opaque reasoning), executes only completed
function calls, and sends `function_call_output` items with the matching
`call_id`. Requests use `store=false`, `stream=true`, and serial tool execution.
The history, arguments, text and tool results have explicit size limits.

The Java reshuffle lets `:ui` depend on `:mcp_ecu`, which uses `:ecu_shared`
instead of depending on UI. `ConsoleEcuSession` adapts MCP schemas and read
handlers in process, pins the Console's `BinaryProtocol`, and owns only its own
subscriptions. It never scans ports, reconnects, submits commands, or closes the
borrowed `LinkManager`. The standalone MCP entry point remains available for
external clients with its existing tool catalog.

The Console shadow JAR includes authentication, ECU and knowledge tool code.
A second MCP process/JAR is unnecessary for this tab. ECU write tools remain
follow-ups. Case export is a Console-local tool, not a standalone MCP tool.

## Local knowledge retrieval

`TroubleshootingTools` combines the ten ECU tools with `LocalKnowledgeTools`,
using the exact cache directory returned by source preparation. Both stay bound
to the current Console connection. The model can search case-insensitively for
a literal substring or all whitespace-separated keywords on one line, then
read the surrounding lines. `path_prefix` limits a search to a directory/file.

Reads are restricted to non-hidden text files under `firmware/`,
`rusefi_documentation/` and `docs/` when present. Supported extensions include
Markdown, plain text, C/C++/assembly, INI, Lua, YAML, make fragments and config
text. JSON credentials, unrelated cache files, absolute/traversing paths and
filesystem symlinks are excluded. Extracted ZIP link entries remain plain text.

Files must be valid UTF-8 without NUL bytes and at most 2 MiB. Searches have a
five-second work budget, at most 12,000 traversal visits, 4,000 scanned files,
64 MiB of input and 20 returned matches. Partial results are marked; narrow
the query or path prefix when a limit is reached. Reads return at most 120
lines, 6,000 text characters total and 2,000 characters per line, with truncation
flags and a next-line hint when applicable. Stop cancels retrieval too.

Results identify files with relative paths, line numbers and SHA-256 hashes;
answers are instructed to cite `path:Lstart-Lend`. The ZIP has no revision
manifest, so source revision is explicitly **unknown** and the ECU match is
**unverified**. A file hash is not proof that its source matches the ECU.
Markdown image references are preserved; wiki results include a current-upstream
page URL to help locate omitted diagrams. That URL is not a version-pinned
copy of the cached passage.

## Firmware source cache

`com.rusefi.core.net.FirmwareSourceCodeDownloader` downloads
`https://rusefi.com/build_server/firmware-source.zip` into
`FileUtil.RUSEFI_SETTINGS_FOLDER/llm-temp` (normally `~/.rusEFI/llm-temp`).
It refreshes a missing ZIP, one smaller than 10 MiB (10 * 1024 * 1024 bytes),
or one whose local modification time is more than 30 days old. A successful
download sets that time to the current time. Otherwise it reuses the ZIP
without a network request.

Every invocation extracts into the same cache folder, producing `firmware/`
and `rusefi_documentation/` beside `firmware-source.zip`. Extraction is staged
and checked before replacing the corresponding top-level trees, removing stale
files within them. ZIP link entries become plain text containing their target,
not filesystem links. Failed downloads or ZIP validation preserve the old cache.
A per-folder lock prevents overlapping preparations.

The blocking `download(DownloadProgressListener)` method returns the prepared
folder and reports monotonic 0-100 percent progress on the calling thread.
Run it on a worker and marshal the callback to Swing's event thread for a UI
progress bar. Download occupies 0-70 percent; extraction and publishing finish
the operation. When the server omits Content-Length, download progress is
indeterminate until it finishes. Cached extraction uses the full progress range.

The CLI sandbox prints progress as text:

```sh
./gradlew :shared_io:runFirmwareSourceCodeDownloaderSandbox
# Optional alternate cache directory:
./gradlew :shared_io:runFirmwareSourceCodeDownloaderSandbox --args="/tmp/rusefi-source-cache"
```

The CLI prepares files on disk; the LLM panel exposes them through the knowledge
tools. `LLMTab` uses `prepareCached(...)` on opening and waits for a user
click before calling `downloadFresh(...)`. Account-store initialization also
waits until sources are ready. Closing the panel cancels source preparation
and ignores late progress/completion callbacks.

## Developer validation

From the repository root:

```sh
./gradlew :ui:test --tests 'com.rusefi.ui.llm.*' :mcp_ecu:test :ui:shadowJar --max-workers=12
```

Tests use temporary credentials, fake OAuth/inference and mocked or local TCP
ECUs. No live ChatGPT account or hardware is required. For an auth/UI sandbox:

```sh
./gradlew :ui:runLLMTabSandbox -PllmStore=/tmp/rusefi-chatgpt-dev
```

That standalone launcher opens no ECU port; use the connected Console to test
live troubleshooting. Build output is `console/rusefi_console.jar`.
