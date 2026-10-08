# Console troubleshooting assistant

The tab is currently disabled by the `false &&` guard in `ConsoleUI`, retained
from the development checkout. The integration and tools are packaged in the
Console JAR; the connected workflow below applies when that guard is enabled.

The **Troubleshooting** tab uses ChatGPT with the Console's existing ECU
connection. Connect to an ECU, open the tab, select a saved account or choose
**Continue with ChatGPT**, select a model, and describe the problem. Sign-in
opens the browser automatically; if that fails, the tab displays a copyable link.
The app identifies itself as **rusEFI Updater**.

The terminal shows streamed answers and tool names as they run. User messages
and requested ECU evidence are sent to OpenAI using the selected ChatGPT plan.
The assistant's tools are read-only:

| Tool | Evidence |
| --- | --- |
| `ecu_info` | Connected firmware signature and Lua field availability |
| `list_output_channels` | INI datalog channel names and labels, with a substring filter |
| `read_output_channel` | One value from a recent full poll, with its host timestamp and age |
| `read_messages` | Console/ECU messages observed since this conversation started, with sequence numbers |

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

The Console shadow JAR includes authentication and ECU tool code. A second MCP
process/JAR is unnecessary for this tab. Local wiki/source retrieval, tune/Lua
reading, log/sniffer capture, diagnostic export, and write tools are follow-ups;
this implementation covers the model/tool loop and initial Console connection
integration. Merely extracting documentation beside the JAR does not yet make
it searchable by the assistant.

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

This prepares local content only; it does not yet expose source/wiki search to
the model.

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
