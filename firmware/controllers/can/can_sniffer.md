# CAN Sniffer (slcan)

The rusEFI CAN sniffer implements the [slcan](https://github.com/torvalds/linux/blob/master/drivers/net/can/slcan.c) (Lawicel ASCII) protocol. It allows using the ECU as a CAN interface for PC-based tools like SavvyCAN, `can-utils`, or custom scripts.

## Hardware Setup

On rusEFI boards built with a second USB CDC channel (`HAL_USE_USB_CDC_2`, e.g. uaefi pro (F7), purple-gateway (H7)), the CAN sniffer is exposed via the second Virtual COM Port (`SDU[1]`, see `can_hw.cpp`).

- **Primary VCP**: rusEFI Console / TunerStudio.
- **Secondary VCP**: CAN Sniffer.

Both CDC interfaces belong to one composite USB device and carry the same product string and serial number, so the host cannot tell them apart from USB descriptors alone (on Linux they typically enumerate as `/dev/ttyACM0` = console, `/dev/ttyACM1` = sniffer, but ordering is not guaranteed). Identify by probing: the console port answers the TunerStudio HELLO with the rusEFI signature, the sniffer port answers the SLCAN `V` command. That is exactly what the console's `SlcanTab` (behind the `show_slcan_sniffer` flag) does when "Scanning for SLCAN port".

## Configuration

The sniffer must be enabled in TunerStudio:

1. Go to **Connectivity -> CAN Bus**.
2. Enable **CAN Sniffer** for the desired bus (CAN1, CAN2, etc.).
3. (Optional) Set **Sniffer Tx CAN bus** if you intend to transmit messages via the sniffer.

## Protocol Support

The sniffer supports the standard Lawicel ASCII protocol. Commands are terminated with a carriage return (`\r`).

### Supported Commands

| Command | Description | Example |
| :--- | :--- | :--- |
| `Sx` | Set CAN bit rate (where `x` is 0-8). **(Value ignored — but sending it is REQUIRED before `O`/`L`, see below)** | `S6` (500k) |
| `O` | Open CAN channel in normal mode. **(Soft-open only, hardware controlled by ECU)** | `O` |
| `L` | Open CAN channel in listen-only mode. **(Soft-open only, hardware controlled by ECU)** | `L` |
| `C` | Close CAN channel. **(Soft-close only, hardware controlled by ECU)** | `C` |
| `tIIILD...` | Transmit standard ID frame. | `t1232AABB` |
| `TIIIIIIIILD...` | Transmit extended ID frame. | `T000001232AABB` |
| `V` | Get hardware/firmware version. | `V` |
| `N` | Get serial number. | `N` |
| `Z0` / `Z1` | Disable/Enable timestamps. | `Z1` |
| `F` | Read status flags. | `F` |

### Bit Rate Codes (`Sx`)

| Code | Bit Rate |
| :--- | :--- |
| `S0` | 10k |
| `S1` | 20k |
| `S2` | 50k |
| `S3` | 100k |
| `S4` | 125k |
| `S5` | 250k |
| `S6` | 500k |
| `S7` | 800k |
| `S8` | 1M |

*Note: The actual baud rate used by the ECU is pre-configured in TunerStudio. The `S` command's value is ignored because the ECU manages the CAN hardware bit rate.*

### Open sequence: `S` is mandatory before `O`

Although the bit rate value is ignored, `O`/`L` are **refused with BELL (`\a`) unless an `S0`..`S8` was accepted since the last close** (`baudrate_configured` in `executeCommand`), and `C` clears that state again. So the canonical (re)open sequence is always:

```
C       (close — BELL if already closed, harmless)
S6      (any code; arms baudrate_configured)
O       (open, streaming starts)
```

A bare `O` on a freshly closed channel silently gets you nothing but BELLs — verified on hardware 2026-08-23. This matches what `SlcanTab`/`SlcanClient` sends.

### Reception Format

When the sniffer is open and receives a frame, it outputs:

- Standard ID: `tIIILDD...[TTTT]\r`
- Extended ID: `TIIIIIIIILDD...[TTTT]\r`

Where:
- `t`/`T`: Standard/Extended frame.
- `I`: Identifier (Hex).
- `L`: DLC (Data Length Code).
- `D`: Data bytes (Hex).
- `TTTT`: Optional 16-bit timestamp (if enabled with `Z1`).

## Implementation Details

### Configuration Fields

The sniffer behavior is controlled by the following fields in `engineConfiguration`:

- `canSniffer[index].read`: Enable/Disable listening to the specified CAN bus (index 0 for CAN1, 1 for CAN2, etc.). **Default: off** (gates the RX path in `can_hw.cpp`).
- `canSniffer[index].listenOurs`: If enabled, the sniffer will also report messages transmitted by the ECU itself on this bus. **Default: on for every bus** (`engine_configuration.cpp`, gates the TX echo in `can_msg_tx.cpp`).
- `canSniffer[index].handleInjected`: If enabled, messages transmitted from the PC via the sniffer will be treated by the ECU as if they were received from the CAN bus (useful for simulation/testing).
- `canSnifferTxBus`: Specifies which CAN bus to use for transmitting messages sent from the PC. **Default: None**, in which case `t`/`T` transmit commands are refused with BELL.

Consequence of the defaults (a common surprise, verified on hardware 2026-08-23): **out of the box the sniffer streams only the ECU's own transmitted frames** — the bus can be fully alive with other nodes and the SLCAN stream will not show a single frame from them until `canSnifferN_read` is enabled for that bus in the tune. If the ID inventory you see is suspiciously identical to what the ECU sends, check the `read` flags before suspecting wiring.

### ECU Control

The rusEFI CAN sniffer is a "passive" implementation of `slcan`. It does not have direct control over the CAN hardware's state or bitrate:

- **Bitrate**: The `S` command is ignored. Bitrate must be configured in TunerStudio under **Connectivity -> CAN Bus**.
- **Interface State**: The `O`, `L`, and `C` commands only open/close the sniffer's internal terminal state. The actual CAN hardware is initialized and kept active by the ECU based on its main configuration.

The implementation is located in:
- `firmware/controllers/can/can_sniffer.cpp`
- `firmware/controllers/can/can_sniffer.h`

Host-side counterparts in the Java console:
- `java_console/ui/src/main/java/com/rusefi/ui/SlcanTab.java` — sniffer tab (port auto-scan, record to file)
- `com.rusefi.io.can.slcan.SlcanClient` — the SLCAN client the tab is built on
