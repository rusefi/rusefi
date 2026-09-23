# Engine Sniffer

rusEFI has several ways to inspect crank/cam edges and engine-synchronous
outputs. Start by identifying the capture path: their formats, controls, and
consumers differ. CAN sniffing is a separate subsystem.

## Capture paths

| Path | Producer / consumer | Use |
|------|---------------------|-----|
| Console Digital Sniffer | `WaveChart` text events -> `EngineState` -> `EngineSnifferPanel` | Display crank/cam, output-pin transitions, logic-analyzer inputs, and TDC markers together |
| Tooth/composite logger | `LogTrigger*` -> `ToothLoggerBufferPool` -> TS binary protocol or SD writer | Capture digital state changes for trigger diagnosis and replay |
| Trigger oscilloscope | `TRIGGER_SCOPE` binary commands | Separate capture facility; do not decode its payload as a composite record |
| Unit-test artifacts | `EngineTestHelper`, tooth logger, test logger | Inspect simulated scheduling and pin events without hardware |

Digital output events describe firmware pin commands; they do not prove that a
coil fired or an injector passed current. Trigger inputs describe the ECU's
digital interpretation, not the analog sensor voltage. TDC/sync markers depend
on the ECU's decoder and configuration; they do not establish mechanical timing
independently.

## Source map

Paths below are relative to the repository root.

| File | Responsibility |
|------|----------------|
| `firmware/development/engine_sniffer.{cpp,h}` | Text chart buffer, event helpers, `chartsize`, `reset_engine_chart` |
| `firmware/console/status_loop.cpp` | Publishes charts and logical-channel -> physical-pin mappings |
| `firmware/controllers/trigger/trigger_central.cpp` | Trigger/cam event producers, sync diagnostics |
| `firmware/controllers/algo/engine.cpp` | Shared sniffer RPM gate |
| `firmware/integration/rusefi_config.txt`, `ts_protocol.txt` | Configuration fields, protocol keys, units, packet size |
| `firmware/console/binary/tooth_logger.{cpp,h}` | Composite state, logger modes, SD binary/CSV writers |
| `firmware/console/binary/tooth_logger_buffer.{cpp,h}` | Buffer ownership, queues, timestamps, event-time sensor snapshots |
| `firmware/console/binary/tunerstudio.cpp` | Binary logger enable/read/disable commands |
| `java_console/ui/src/main/java/com/rusefi/ui/engine/EngineSnifferPanel.java` | Digital Sniffer display, pause, zoom, image export |
| `java_console/models/src/main/java/com/rusefi/waves/EngineChartParser.java` | Text chart triplet parser |
| `java_console/io/src/main/java/com/rusefi/binaryprotocol/BinaryProtocolLogger.java` | Automatic composite polling and host file output |
| `java_console/models/src/main/java/com/rusefi/composite/CompositeParser.java` | Java binary composite decoder |
| `java_console/mcp_ecu/src/main/java/com/rusefi/mcp/EcuMcpServer.java` | Available MCP tools and their actual schemas |

## Console Digital Sniffer

Built with `EFI_ENGINE_SNIFFER`; event formatting also requires
`EFI_TEXT_LOGGING`. `WaveChart` collects text triplets
`channel!event!time!` under the `wave_chart` protocol key. Time is relative to
the first event in that chart, in `ENGINE_SNIFFER_UNIT_US` units (10 us).
Crank events include the decoded event index (`u_<index>` / `d_<index>`);
output and cam events use up/down tokens; TDC events carry RPM.

`printOverallStatus()` calls `publishIfFull()`. Publication occurs at
`engineChartSize` events or when the chart exceeds the duration calculated as
`engineChartSize / 20` seconds. This is a bounded snapshot, not a continuous
trace. Production text storage is 5000 bytes (35000 outside production), so
increasing the event count does not increase the underlying byte capacity.

Useful controls:

| Control | Meaning |
|---------|---------|
| `chartsize <N>` | Set the chart event count; values below 5 are ignored. Prints sniffer status and size. |
| `reset_engine_chart` | Clear the chart and skip until the revolution counter advances by three, avoiding previously scheduled events after changes. |
| `engineSnifferRpmThreshold` | Shared firmware gate: enabled only while RPM is strictly below the configured threshold. Generic default is 2500; presets/boards can override it. Console command: `set engineSnifferRpmThreshold <N>`. |
| `engineSnifferFocusOnInputs` | Suppress output-pin events from the text chart. |
| `displayLogicLevelsInEngineSniffer` | Select input logic-level display versus decoder-consumed events. For a cam using only one edge, the latter can show synthetic rise/fall pairs; do not interpret those as measured pulse widths. |

Check the connected tune for actual settings. `chartsize` and the text `set`
command change runtime configuration; do not assume a durable burn occurred.
The UI's Pause button freezes display updates, not firmware acquisition. Pin
labels arrive separately through `outpin` mappings from `status_loop.cpp`.

## Tooth/composite logger and files

The binary path is built with `EFI_TOOTH_LOGGER` and also checks
`isEngineSnifferEnabled`. Full mode records primary input, up to four cams,
sync, TDC, coil/injector bitmasks, and ACR state. Primary-tooth mode selects
primary rising edges for tooth-period analysis. TDC is a toggled marker at
first-cylinder TDC; the coil/injector bytes represent the first eight outputs.

`composite_logger_s` is an 8-byte wire contract, distinct from the text chart.
The buffer writer stores microseconds relative to each buffer's start and
reverses the entire 64-bit record. Consult both the producer and the exact
consumer before writing a decoder: a generic timestamp-plus-flags parser or
an assumption of globally monotonic timestamps is insufficient. In particular,
the Java `CompositeParser` has legacy field extraction that does not mirror
all current firmware fields; do not assume its exported coil/injector labels
faithfully represent the current firmware bitmasks.

Buffers hold 250 events each. The pool uses configured static storage or the
shared BigBuffer; consumers must return buffers. Normal mode drops events when
no free buffer remains. Background circular capture can recycle old filled
buffers; DTC capture can reserve them, preventing the TS reader from taking
them. A read reporting out-of-range may mean no filled buffer, a busy logger,
or a build without tooth logging, rather than missing physical edges.

`TS_SET_LOGGER_SWITCH` handles enable, primary-tooth enable, disable, and read.
The Console's `TS_GET_COMPOSITE_BUFFER_DONE_DIFFERENTLY` request also attempts
to enable full logging. Java writes `rusEFI_trigger_log_*` files in
`FileLogger.DIR` as `.vcd`, `.logicdata`, and TS high-speed `.csv`.

Java adds a separate RPM policy: by default it stops composite polling after
10 seconds above 700 RPM and resumes at or below 700 if composite logging is
enabled in `LinkManager`. JVM properties `high_speed_logger_rpm` and
`high_speed_logger_time` override these values. Raising the firmware threshold
alone does not override this host policy.

On SD, `sdTriggerLog` selects `.teeth` instead of normal `.mlg` operating-data
logs; `sdTriggerLogCsv` chooses CSV versus binary at file creation. The CSV
writer includes event-time VBatt, coolant temperature (`ET`), InstantMAP, and
TPS snapshots and optional board-specific columns. Inspect the contents/header
instead of assuming every `.teeth` file is binary. See
[SD Card Logging](sd_card_logging.md) for ownership, file lifecycle, and writes.

## Relevant MCP tools

Use the existing `:mcp_ecu` server; setup/build/connection instructions and full
schemas are in [the ECU MCP README](../../java_console/mcp_ecu/README.md) and
[the MCP overview](../../README-mcp.md). Target the intended real ECU serial
port with a matching INI. Keep multi-step commands in one server session.

| Tool | Use during sniffer diagnosis |
|------|-----------------------------|
| `connect`, `ecu_info` | Identify the connected firmware and establish the connection. |
| `read_tune` | Save the configuration alongside the capture; inspect trigger type, edge settings, and sniffer threshold. |
| `read_output_channel` | Read context such as `RPMValue`; names must exist in the connected INI. |
| `send_command` / `command` | Queue `triggerinfo`, `chartsize 180`, or `reset_engine_chart`; optionally adjust the threshold for the investigation. |
| `read_messages`, `wait_for_message` | Collect command responses and trigger diagnostics. Use `sinceSeq` to exclude stale messages. |
| `start_data_logging`, `data_logging_status`, `stop_data_logging` | Record host `.mlg` operating data alongside edge captures. These tools do not start or return a sniffer waveform. |
| `mount_to_ecu`, `mount_to_pc` | Switch SD ownership when working with `.teeth` captures. These tools do not enable `sdTriggerLog` or download files. |

Example MCP tool calls (each object is a `tools/call` params object):

```json
{"name":"read_output_channel","arguments":{"name":"RPMValue"}}
{"name":"send_command","arguments":{"command":"triggerinfo"}}
{"name":"read_messages","arguments":{"maxLines":100}}
{"name":"send_command","arguments":{"command":"chartsize 180"}}
```

`send_command` returning `queued: true` confirms queueing, not execution.
For a fresh response, obtain the message sequence before sending the command
and pass it as `sinceSeq` to subsequent reads/waits.

There is currently no dedicated MCP tool to start/stop/export an engine-sniffer
capture or return parsed composite events. `read_messages` listens to
`MessagesCentral` (`msg` protocol entries); `wave_chart` is routed separately
through `EngineState` and is not exposed by that tool. The shared Java
connection may create automatic composite files, but MCP does not expose their
paths or lifecycle as a capture API. Use the Console/TS or SD capture path for
waveforms. `convert_log_to_csv` supports MLG/MSL operating logs, not `.teeth`,
`.logicdata`, or text `wave_chart` packets. `:mcp_can` captures CAN frames and
does not substitute for crank/cam capture.

For SD ownership changes, follow the MCP README's host-eject and reconnect
procedure: CDC and mass storage share USB, so a switch can drop the connection.

## Diagnosis and regression artifacts

1. Save the tune, firmware signature, RPM, capture source, and active thresholds.
2. For an empty chart, check build flags, the RPM gate, chart size, UI pause,
   and input activity. For missing output rows, check `engineSnifferFocusOnInputs`.
3. For missing composite data, additionally check the Java RPM policy and buffer
   ownership (SD/DTC/TS). Separate capture gaps from genuine long tooth intervals.
4. Compare trigger edges, sync transitions, and TDC before drawing conclusions
   about injection/ignition timing. Cross-check output labels against the format
   decoder; physical actuator behavior needs independent measurement.
5. Reproduce scheduling/decoder problems with `EngineTestHelper`; call
   `setUnitTestCreateLogs(true)` before constructing it. Per-test traces and
   logic-data artifacts go to `unit_tests/test_results/`; see its
   [readme](../../unit_tests/test_results/readme.md). Copy evidence elsewhere
   before another test run, which cleans that directory.

For `.teeth` replay, follow the capture-gap and recorded Sync/TDC guidance in
[CLAUDE.md](../../CLAUDE.md). See also [Scheduling & Timing](scheduling_system.md)
and [Hardware Quality Control & Direct I/O](hardware-quality-control.md).
