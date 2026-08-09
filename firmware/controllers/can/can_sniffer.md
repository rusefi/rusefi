# CAN Sniffer (slcan)

The rusEFI CAN sniffer implements the [slcan](https://github.com/torvalds/linux/blob/master/drivers/net/can/slcan.c) (Lawicel ASCII) protocol. It allows using the ECU as a CAN interface for PC-based tools like SavvyCAN, `can-utils`, or custom scripts.

## Hardware Setup

On some F7-based rusEFI boards (like uaefi pro), the CAN sniffer is exposed via a second USB CDC (Virtual COM Port) channel.

- **Primary VCP**: rusEFI Console / TunerStudio.
- **Secondary VCP**: CAN Sniffer.

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
| `Sx` | Set CAN bit rate (where `x` is 0-8). **(Not supported, controlled by ECU)** | `S6` (500k) |
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

*Note: The actual baud rate used by the ECU is pre-configured in TunerStudio. The `S` command in the sniffer is ignored because the ECU manages the CAN hardware bit rate.*

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

- `canSniffer[index].read`: Enable/Disable listening to the specified CAN bus (index 0 for CAN1, 1 for CAN2, etc.).
- `canSniffer[index].listenOurs`: If enabled, the sniffer will also report messages transmitted by the ECU itself on this bus.
- `canSniffer[index].handleInjected`: If enabled, messages transmitted from the PC via the sniffer will be treated by the ECU as if they were received from the CAN bus (useful for simulation/testing).
- `canSnifferTxBus`: Specifies which CAN bus to use for transmitting messages sent from the PC.

### ECU Control

The rusEFI CAN sniffer is a "passive" implementation of `slcan`. It does not have direct control over the CAN hardware's state or bitrate:

- **Bitrate**: The `S` command is ignored. Bitrate must be configured in TunerStudio under **Connectivity -> CAN Bus**.
- **Interface State**: The `O`, `L`, and `C` commands only open/close the sniffer's internal terminal state. The actual CAN hardware is initialized and kept active by the ECU based on its main configuration.

The implementation is located in:
- `firmware/controllers/can/can_sniffer.cpp`
- `firmware/controllers/can/can_sniffer.h`
