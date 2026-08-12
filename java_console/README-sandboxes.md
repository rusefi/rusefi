# Java sandboxes

A "sandbox" is a small stand-alone class with a `main()` method used as a manual dev harness:
to poke at hardware, exercise a protocol, render a single UI widget, or run a code generator in
isolation. Sandboxes are **not** part of the automated test suite — you launch them by hand
(`Run 'main()'` in the IDE).

This file lists every sandbox in `java_console` and `java_tools`, grouped by category. Each
sandbox class also carries a brief comment on top describing what it does.

## Connectivity & protocol

| Sandbox | Purpose |
| --- | --- |
| `BinaryProtocolServerSandbox` | Starts a fake device exposing the TS binary protocol over the default TCP proxy port. |
| `SerialSandbox` | Basic binary-protocol client over a serial port. |
| `SerialWriteTuneBurnCrcSandbox` | Real-ECU round-trip: write a tune field -> burn -> read back page CRC and assert it matches the sent tune. |
| `TcpClientSandbox` | Connects to a (fake or real) ECU over TCP and pulls its configuration. |
| `TcpServerSandbox` | Fully self-contained fake TCP/IP "ECU" side of the TS protocol. |
| `Elm327Sandbox` | Talks to an ECU over an ELM327 adapter (body currently disabled). |
| `SandboxCommon` | Shared helpers used by the binaryprotocol sandboxes (no `main`). |
| `InspectPortSandbox` | Continuously polls all serial ports and prints their detected type. |
| `PortDetectorSandbox` | Continuously polls and prints detected serial ports and ECU signatures. |
| `ReconnectSandbox` | Exercises ECU auto-reconnect logic against real hardware. |
| `HeadlessConnectivitySandbox` | Headless serial auto-scan + auto-reconnect harness printing INI/gauge presence, refresh rate, and ECU messages. |
| `PlainSerialPortScannerSandbox` | Scans serial ports for an ECU and resolves its update URL. |

## CAN / UDS

| Sandbox | Purpose |
| --- | --- |
| `PCanSandbox` | Reads an ECU signature over a PCAN (Peak CAN) adapter. |
| `SocketCANSandbox` | Reads an ECU signature over Linux SocketCAN. |
| `PCanIoProxySandbox` | Bridges a PCAN stream through the CAN connector startup. |
| `M74_9_UdsSandbox` | Exercises UDS communication (incl. security access) with a BMW M74.9 ECU. |

## Flashing & bootloader

| Sandbox | Purpose |
| --- | --- |
| `DfuFlasherSandbox` | Probes for DFU/ST-Link flashing hardware. |
| `OpenBltSandbox` | Clears flash memory over OpenBLT XCP-on-Ethernet. |
| `OpenBltSerialSandbox` | Flashes/erases firmware over OpenBLT via a serial transport. |
| `BltSwitchSandbox` | Switches an ECU into the OpenBLT bootloader and reboots it. |

## Auto-update & firmware bundle

| Sandbox | Purpose |
| --- | --- |
| `AutoupdateSandbox` | Downloads and unzips the autoupdate bundle. |
| `ScanAndUpdateBoardSandbox` | Full scan-and-update cycle: auto-detects an ECU on serial, and if its board is on the board_compatibility allowlist downloads that board's bundle, reboots the ECU to OpenBLT and flashes the fresh .srec. |
| `TsPluginInstallerSandbox` | Installs the TunerStudio plugin. |
| `FirmwareUpdateTabSandbox` | Visual sandbox for the firmware update tab. |
| `HwSandbox` | Runs hardware-in-the-loop tests against a real ECU. |

## TunerStudio: .ini, tunes & projects

| Sandbox | Purpose |
| --- | --- |
| `IniFileModelSandbox` | Parses an `.ini` file into an `IniFileModel` and prints gauge metadata. |
| `MsqSandbox` | Reads a TunerStudio `.msq` tune file and prints its signature. |
| `PrimeTunerStudioCacheSandbox` | Primes the local TunerStudio cache from a generated `.ini` file. |
| `TsCreateProjectSandbox` | Scans for an ECU and creates a TunerStudio project on disk. |
| `TsProjectUpdaterSandbox` | Exercises TunerStudio tune migration. |
| `RetrieveMsqSandbox` | Rebuilds an `.msq` tune from a stored calibrations image + `.ini`. |
| `TuneManifestSandbox` | Downloads the online tunes manifest. |
| `TsTuneReaderSandbox` | Reads a TunerStudio tune for a given ECU project. |

## UI: gauges, tables & panels

| Sandbox | Purpose |
| --- | --- |
| `TopLevelMenuSandbox` | Top-level calibration menu tree. |
| `WizardSandbox` | The real console setup wizard (`WizardContainer`) offline: generated `.ini` + blank config image behind a mocked `BinaryProtocol`. |
| `GaugesPanelSandbox` | Gauges panel driven by an `.ini` file and simulated sensors. |
| `GaugesPanelMockedSandbox` (`ui`) | Gauges panel using mocked ini/sensor data. |
| `GaugesPanelMocked2Sandbox` (`mocked`) | Gauges panel using fully mocked data. |
| `TableTracerSandbox` | Table-tracer overlay fed from a live ECU. |
| `TableTracerSandboxMocked` | Table-tracer overlay using fully mocked data. |
| `CurveTracerSandbox` | Curve-tracer overlay over a calibration table fed from a live ECU. |
| `LiveDataPaneSandbox` | Live-data pane. |
| `LiveDataParserSandbox` | Live-data parser panel using mocked variable values. |
| `MsgPanelSandbox` | Messages pane. |
| `WavePanelSandbox` | Engine-sniffer wave panel. |
| `UpDownSandbox` | Engine-sniffer up/down scaling control. |
| `JSplitButtonSandbox` | The `JSplitButton` widget. |
| `LafSandbox` | Custom Swing look-and-feel. |
| `ProgressViewSandbox` | The `ProgressView` dialog. |
| `KnockCanvasSandbox` | The `KnockCanvas` with simulated spectrogram data. |
| `RemoteTabSandbox` | The TS plugin "remote" tab. |
| `LogUploadSelectorSandbox` | Log-upload selector with a mocked `ControllerAccess`. |

## Logging & sensor data

| Sandbox | Purpose |
| --- | --- |
| `BinarySensorLogSandbox` | Writes a sample binary sensor log. |
| `LogicdataStreamFileSandbox` | Generates a synthetic Saleae Logic `.logicdata` file. |
| `LogicdataStreamFileCsvSandbox` | Converts a CSV composite-log into a Saleae Logic `.logicdata` file. |

## Code generators & build tooling

| Sandbox | Purpose |
| --- | --- |
| `PageToolSandbox` | Runs the config page generator (PageTool). |
| `FieldsApiGeneratorSandbox` | Runs the fields-API header generator. |
| `FindFileHelperSandbox` | Exercises `FindFileHelper` `.srec` lookup. |

## Online services & plugin host

| Sandbox | Purpose |
| --- | --- |
| `PanamaSandbox` | Uploads a file to the rusEFI Panama files API. |
| `JsonUserDetailsResolverSandbox` | Exercises the JSON user-details resolver. |
| `TakesServerSandbox` | Starts a small Takes HTTP server. |
| `PluginBodySandbox` | TS plugin body. |
| `PluginLauncherSandbox` | TS plugin auto-update launcher. |
