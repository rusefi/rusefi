# Java sandboxes

A "sandbox" is a small stand-alone class with a `main()` method used as a manual dev harness:
to poke at hardware, exercise a protocol, render a single UI widget, or run a code generator in
isolation. Sandboxes are **not** part of the automated test suite — you launch them by hand
(`Run 'main()'` in the IDE).

This file lists every sandbox in `java_console` and `java_tools`, grouped by category. Each
sandbox class also carries a brief comment on top describing what it does.

## Connectivity & protocol

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `BinaryProtocolServerSandbox` | `io/.../binaryprotocol/test` | Starts a fake device exposing the TS binary protocol over the default TCP proxy port. |
| `SerialSandbox` | `io/.../binaryprotocol/test` | Basic binary-protocol client over a serial port. |
| `TcpClientSandbox` | `io/.../binaryprotocol/test` | Connects to a (fake or real) ECU over TCP and pulls its configuration. |
| `TcpServerSandbox` | `io/.../binaryprotocol/test` | Fully self-contained fake TCP/IP "ECU" side of the TS protocol. |
| `Elm327Sandbox` | `io/.../binaryprotocol/test` | Talks to an ECU over an ELM327 adapter (body currently disabled). |
| `SandboxCommon` | `io/.../binaryprotocol/test` | Shared helpers used by the binaryprotocol sandboxes (no `main`). |
| `InspectPortSandbox` | `ui/.../com/rusefi` | Continuously polls all serial ports and prints their detected type. |
| `PortDetectorSandbox` | `ui/.../autodetect` | Continuously polls and prints detected serial ports and ECU signatures. |
| `ReconnectSandbox` | `ui/.../autodetect` | Exercises ECU auto-reconnect logic against real hardware. |
| `PlainSerialPortScannerSandbox` | `ts_plugin/.../com/rusefi` | Scans serial ports for an ECU and resolves its update URL. |

## CAN / UDS

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `PCanSandbox` | `io/.../binaryprotocol/test` | Reads an ECU signature over a PCAN (Peak CAN) adapter. |
| `SocketCANSandbox` | `io/.../binaryprotocol/test` | Reads an ECU signature over Linux SocketCAN. |
| `PCanIoProxySandbox` | `ui/.../binaryprotocol/test` | Bridges a PCAN stream through the CAN connector startup. |
| `M74_9_UdsSandbox` | `io/.../uds` | Exercises UDS communication (incl. security access) with a BMW M74.9 ECU. |

## Flashing & bootloader

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `DfuFlasherSandbox` | `ui/.../maintenance` | Probes for DFU/ST-Link flashing hardware. |
| `OpenBltSandbox` | `ui/.../test` | Clears flash memory over OpenBLT XCP-on-Ethernet. |
| `OpenBltSerialSandbox` | `ui/.../test` | Flashes/erases firmware over OpenBLT via a serial transport. |
| `BltSwitchSandbox` | `ui/.../test` | Switches an ECU into the OpenBLT bootloader and reboots it. |

## Auto-update & firmware bundle

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `AutoupdateSandbox` | `autoupdate/.../autoupdate` | Downloads and unzips the autoupdate bundle. |
| `TsPluginInstallerSandbox` | `autoupdate/.../autoupdate` | Installs the TunerStudio plugin. |
| `FirmwareUpdateTabSandbox` | `ui/.../ui/basic` | Visual sandbox for the firmware update tab. |
| `HwSandbox` | `autotest/.../com/rusefi` | Runs hardware-in-the-loop tests against a real ECU. |

## TunerStudio: .ini, tunes & projects

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `IniFileModelSandbox` | `io/.../opensr5/ini` | Parses an `.ini` file into an `IniFileModel` and prints gauge metadata. |
| `MsqSandbox` | `io/.../opensr5/ini` | Reads a TunerStudio `.msq` tune file and prints its signature. |
| `PrimeTunerStudioCacheSandbox` | `io/.../opensr5/ini` | Primes the local TunerStudio cache from a generated `.ini` file. |
| `TsCreateProjectSandbox` | `ui/.../ts` | Scans for an ECU and creates a TunerStudio project on disk. |
| `TsProjectUpdaterSandbox` | `ui/.../ts` | Exercises TunerStudio tune migration. |
| `RetrieveMsqSandbox` | `ui/.../maintenance/migration/default_migration` | Rebuilds an `.msq` tune from a stored calibrations image + `.ini`. |
| `TuneManifestSandbox` | `ui/.../tune_manifest` | Downloads the online tunes manifest. |
| `TsTuneReaderSandbox` | `ts_plugin/.../ts_plugin` | Reads a TunerStudio tune for a given ECU project. |

## UI: gauges, tables & panels

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `GaugesPanelSandbox` | `ui/.../ui` | Gauges panel driven by an `.ini` file and simulated sensors. |
| `GaugesPanelMockedSandbox` (`ui`) | `ui/.../ui` | Gauges panel using mocked ini/sensor data. |
| `GaugesPanelMocked2Sandbox` (`mocked`) | `ui/.../ui/mocked` | Gauges panel using fully mocked data. |
| `TableTracerSandbox` | `ui/.../ui` | Table-tracer overlay fed from a live ECU. |
| `TableTracerSandboxMocked` | `ui/.../ui/mocked` | Table-tracer overlay using fully mocked data. |
| `CurveTracerSandbox` | `ui/.../ui` | Curve-tracer overlay over a calibration table fed from a live ECU. |
| `TopLevelMenuSandbox` | `ui/.../ui` | Top-level calibration menu tree. |
| `LiveDataPaneSandbox` | `ui/.../ui` | Live-data pane. |
| `LiveDataParserSandbox` | `ui/.../livedata` | Live-data parser panel using mocked variable values. |
| `MsgPanelSandbox` | `ui/.../test` | Messages pane. |
| `WavePanelSandbox` | `ui/.../ui/test` | Engine-sniffer wave panel. |
| `UpDownSandbox` | `ui/.../ui/engine/test` | Engine-sniffer up/down scaling control. |
| `WizardSandbox` | `ui/.../ui/wizard` | Setup wizard driven by a test `.ini` file. |
| `JSplitButtonSandbox` | `ui/.../ui` | The `JSplitButton` widget. |
| `LafSandbox` | `ui/.../ui/laf` | Custom Swing look-and-feel. |
| `ProgressViewSandbox` | `core_ui/.../core/ui` | The `ProgressView` dialog. |
| `KnockCanvasSandbox` | `ts_plugin/.../ts_plugin/knock` | The `KnockCanvas` with simulated spectrogram data. |
| `RemoteTabSandbox` | `ts_plugin/.../ts_plugin` | The TS plugin "remote" tab. |
| `LogUploadSelectorSandbox` | `ts_plugin/.../ts_plugin` | Log-upload selector with a mocked `ControllerAccess`. |

## Logging & sensor data

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `BinarySensorLogSandbox` | `ui/.../sensor_logs` | Writes a sample binary sensor log. |
| `LogicdataStreamFileSandbox` | `models/.../stream` | Generates a synthetic Saleae Logic `.logicdata` file. |
| `LogicdataStreamFileCsvSandbox` | `models/.../stream` | Converts a CSV composite-log into a Saleae Logic `.logicdata` file. |

## Code generators & build tooling

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `PageToolSandbox` | `java_tools/configuration_definition/.../com/rusefi` | Runs the config page generator (PageTool). |
| `FieldsApiGeneratorSandbox` | `java_tools/configuration_definition/.../com/rusefi` | Runs the fields-API header generator. |
| `FindFileHelperSandbox` | `shared_io/.../core` | Exercises `FindFileHelper` `.srec` lookup. |

## Online services & plugin host

| Sandbox | Location | Purpose |
| --- | --- | --- |
| `PanamaSandbox` | `ui/.../panama` | Uploads a file to the rusEFI Panama files API. |
| `JsonUserDetailsResolverSandbox` | `java_tools/proxy_server/.../server` | Exercises the JSON user-details resolver. |
| `TakesServerSandbox` | `java_tools/proxy_server/.../server` | Starts a small Takes HTTP server. |
| `PluginBodySandbox` | `ts_plugin/.../ts_plugin` | TS plugin body. |
| `PluginLauncherSandbox` | `ts_plugin_launcher/.../ts_plugin` | TS plugin auto-update launcher. |
