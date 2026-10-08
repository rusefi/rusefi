# Console troubleshooting assistant

The tab is currently disabled by the `false &&` guard in `ConsoleUI`, retained
from the development checkout. The integration and tools are packaged in the
Console JAR; the connected workflow below applies when that guard is enabled.

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
requested ECU evidence and retrieved source/wiki excerpts are sent to OpenAI
using the selected ChatGPT plan.
The assistant's tools are read-only:

| Tool | Evidence |
| --- | --- |
| `ecu_info` | Connected firmware signature and Lua field availability |
| `list_output_channels` | INI datalog channel names and labels, with a substring filter |
| `read_output_channel` | One value from a recent full poll, with its host timestamp and age |
| `read_messages` | Console/ECU messages observed since this conversation started, with sequence numbers |
| `search_knowledge` | Literal or keyword matches in cached firmware/wiki text, with paths and line numbers |
| `read_knowledge` | Bounded line ranges, file hashes and citation metadata from cached text |

Message capture uses the same global MessagesCentral stream as the Console,
including UI diagnostics. It does not retrieve historical ECU messages. Channel
reads wait up to three seconds for a sample and reject samples older than two
seconds. Timestamps describe host reception, not an ECU hardware clock.

**Stop** cancels the current turn. A turn also has a two-minute cancellation
deadline, up to eight model requests and 24 tool calls. Failed, stopped and
incomplete turns are not added to the next request. Network disconnect/read
timeouts can delay cancellation finishing. **New conversation** clears the
conversation and releases its polling/message subscriptions. Changing account
or model also starts fresh. A changed ECU connection discards an in-progress
turn; the next Send starts a fresh conversation.

Authorization registrations, the stable host UUID, and refresh tokens remain
in `~/.rusefi/llm-access/accounts.json`. Conversation history is memory-only.
Sign out clears local tokens and attempts remote revocation. Only one instance
can open an account store at a time.

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
A second MCP process/JAR is unnecessary for this tab. Tune/Lua reading,
log/sniffer capture, diagnostic export, and write tools remain follow-ups.

## Local knowledge retrieval

`TroubleshootingTools` combines the four ECU tools with `LocalKnowledgeTools`,
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
