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

Once sources are ready, the last-used saved account is selected if ready;
otherwise another ready saved account is selected when available. Models load
in the background, refreshing credentials when possible. Startup never opens
a browser automatically. If authorization is needed, choose **Continue with
ChatGPT**; the UI distinguishes re-login from missing ChatGPT plan access.
For a ready account, **Continue with ChatGPT** is disabled because authorization
is already usable. Refreshable token expiry alone does not require manual sign-in.
**Add account** appears only after a registration exists, for adding another
account or workspace. Select a model and describe the problem. Sign-in
opens the browser automatically; if that fails, the tab displays a copyable link.
Use **Cancel sign-in** to abandon authorization. Closing the modeless link
dialog also cancels it, and the dialog closes on every terminal sign-in path
or tab close. Closing an external browser tab cannot notify the Console;
use **Cancel sign-in** instead. Cancelled account addition keeps the current
account selected.
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
time. Client revision and its tracked-edit flag come from a build-time resource.
The `provenance.knowledge` array preserves the distinct archive provenance records
from selected knowledge evidence, including firmware-source, libfirmware and wiki
revisions and manifest/payload hashes. Legacy archives report unknown revisions;
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
answers are instructed to cite `path:Lstart-Lend`. New archives contain
`knowledge-manifest.json`, with firmware-source, libfirmware and wiki Git
revisions, tracked-edit flags, per-file SHA-256/size and a payload index hash.
The manifest's own hash identifies the exact metadata. Revisions are publisher
metadata and hashes check content integrity; they are not digital signatures.
Legacy archives without a manifest remain usable with **unknown** revisions.
Malformed manifests fail retrieval rather than supplying trusted provenance.

Each retrieved file reports `file_hash_matches_manifest`. A false value means
there is no manifest entry or the cached file changed; do not attribute that
text to the declared revision. A dirty repository contains tracked local edits,
so its revision alone does not describe the archived bytes. File hashes still
identify the actual exported content.

The ECU HELLO signature identifies its INI schema/build, not a firmware Git
revision. Matching its date, board or numeric schema hash to source metadata
cannot prove matching code. The assistant therefore keeps `ecu_match` explicitly
**unverified**, even for a valid, clean manifest. Exact compatibility would need
additional firmware build identity, which the current read-only tools do not
supply.

Markdown image references are preserved. Wiki `upstream_url` links are pinned
to `upstream_revision` only for clean wiki metadata and a matching file hash;
otherwise they refer to current master and `upstream_revision` is `unverified`.
Use these links to locate diagrams omitted from the text archive.

## Firmware source cache

`com.rusefi.core.net.FirmwareSourceCodeDownloader` downloads
`https://rusefi.com/build_server/firmware-source.zip` into
`FileUtil.RUSEFI_SETTINGS_FOLDER/llm-temp` (normally `~/.rusEFI/llm-temp`).
It refreshes a missing ZIP, one smaller than 10 MiB (10 * 1024 * 1024 bytes),
or one whose local modification time is more than 30 days old. A successful
download sets that time to the current time. Otherwise it reuses the ZIP
without a network request.

Every invocation extracts into the same cache folder, producing `firmware/`,
`rusefi_documentation/`, `docs/`, `README.md` and (for new archives)
`knowledge-manifest.json` beside `firmware-source.zip`. Extraction is staged.
For a manifest-enabled ZIP, the downloader verifies the payload index hash,
file sizes and hashes, and checks that every extracted file is indexed before
replacing existing content. Hash mismatches preserve the old cache. Legacy
ZIPs remove any old manifest so they cannot inherit unrelated revisions.
Replacing a top-level tree removes stale files within it. ZIP link entries become plain text containing their target,
not filesystem links. Failed downloads or ZIP validation preserve the old cache.
Publication backs up existing trees, the manifest and any replaced ZIP before
moving staged entries into place. A publication I/O failure rolls those changes
back, including the previous ZIP timestamp. If rollback fails, recovery data is
retained in the reported `.source-*` directory rather than deleted. Unrelated
cache files are left alone. This is exception-safe rollback, not crash- or
power-loss atomicity.
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

## Distribution and archive contents

Use the normal universal Console bundle and its launchers/native libraries;
there is no separate troubleshooting application. The source ZIP is fetched
on demand through **Start Download** and reused from the local cache. A first
installation needs network access for this download and sign-in. Users need the
normal Console Java runtime (Java 11 or newer), but no Git, Python or Gradle.
The existing Windows launcher locates an installed runtime; the shell launcher
uses `java` on PATH. Runtime installation remains the normal Console setup.

`firmware/bin/assemble_universal_bundle.sh` packages the Console JAR and native
helpers unchanged. The JAR contains the authentication client, tools and
`client-build.properties`, generated from the client repository revision and
tracked-edit status during the Gradle build. Missing/malformed build metadata
is reported as unknown; the installed client never invokes Git.

The source archive includes tracked firmware/configuration/INI inputs, board
mappings, Lua examples, the initialized libfirmware submodule, root `docs/AI/`,
selected technical guides and wiki Markdown. `docs/knowledge-index.md` is a
searchable index; `docs/licenses/` contains the rusEFI and wiki licenses, with
per-file notices retained in source. Images, PDF/CHM collections, other
submodules and untracked build output are excluded. The workflow's sparse wiki
checkout explicitly includes LICENSE. Python/Git are only archive build tools.

On 2026-10-08, hosted [archive run 37727067660](https://github.com/rusefi/rusefi/actions/runs/37727067660)
completed including server upload. The published ZIP downloaded through
`FirmwareSourceCodeDownloader` matched the workflow artifact byte-for-byte:
SHA-256 `c2ec2b4c9e638c0272d52c70af98b9622d7def392a070763c9f951b97cd984eb`.
That hosted ZIP predates the manifest changes. A locally generated manifest
archive and the normal universal bundle were also built and validated; hosted
validation of the updated workflow remains a release follow-up.

## Developer validation

From the repository root:

```sh
python3 -m unittest discover -s firmware/bin -p test_zip_firmware_source.py
./gradlew :shared_io:test --tests 'com.rusefi.core.net.*Test' :ui:test --tests 'com.rusefi.ui.llm.*' :mcp_ecu:test :ui:shadowJar --max-workers=12
```

Tests use temporary credentials, fake OAuth/inference and mocked or local TCP
ECUs. No live ChatGPT account or hardware is required. For an auth/UI sandbox:

```sh
./gradlew :ui:runLLMTabSandbox -PllmStore=/tmp/rusefi-chatgpt-dev
```

That standalone launcher opens no ECU port; use the connected Console to test
live troubleshooting. Build output is `console/rusefi_console.jar`.
