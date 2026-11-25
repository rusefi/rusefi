# Connectivity Notes

MCP servers, serial, SLCAN, USB MSD/CDC and WeAct adapter behaviour.

## MCP Servers

rusEFI provides two MCP (Model Context Protocol) servers for LLM-driven tooling over stdio JSON-RPC:

- **`:mcp_ecu`** (`java_console/mcp_ecu`) — `EcuMcpServer`: connect to an ECU, upload/download Lua scripts, send commands, and capture ECU messages. Entry point: `com.rusefi.mcp.EcuMcpServer`.
- **`:mcp_can`** (`java_console/mcp_can`) — `CanSnifferMcp`: read-only CAN bus sniffing via PCAN (default) or built-in SLCAN (`--backend slcan`, optional `--port`), with connect, read packets, wait for packet, and status tools. Entry point: `com.rusefi.mcp.CanSnifferMcp`.

## Serial Connectivity

`LinkManager.close()` disconnects but is also used before reconnecting the same
manager; it is not terminal disposal. Discovery creates disposable managers
through `BinaryProtocolExecutor`, so per-manager workers must release idle
threads without shutting down the reusable communication executor. A daemon
thread still consumes native resources and can exhaust macOS's thread limit.
See `LinkManagerThreadLifecycleTest` for repeated-probe and worker-restart coverage.

Console output channels are polled selectively: only names subscribed through
`SensorCentral.addListener` (active tokens) are requested. Subscribe with the
output channel name (`GaugeModel.getChannel()`), never the gauge name; a
gauge-name subscription requests nothing and the widget only appears to work
while another widget polls the same channel. `GaugesPanelLayoutTest.hasDemand`
checks the requested set.

All rusEFI serial connections use the USB CDC (Communications Device Class) profile. Baud rate is irrelevant and never a concern — the USB serial profile handles throughput natively regardless of any baud rate setting in host software or code.

### SLCAN CAN sniffer on the second VCP

Boards built with `HAL_USE_USB_CDC_2` (uaefi pro, purple-gateway) expose an SLCAN (Lawicel ASCII) CAN sniffer on the **second** USB VCP (`CanSniffer` on `SDU[1]`) — full doc: `firmware/controllers/can/can_sniffer.md`. Non-obvious traps (each verified on hardware 2026-08-23):
- Both VCPs share one composite USB identity — identify the sniffer port by probing (`V` command), like the console's `SlcanTab` does; do not trust `/dev/ttyACM*` ordering.
- `O` (open) is refused with BELL unless an `S0`..`S8` was accepted since the last close, and `C` clears that state — the (re)open sequence is always `C`/`S6`/`O`.
- Defaults stream **only the ECU's own TX** (`canSnifferN_listenOurs` on, `canSnifferN_read` off, `canSnifferTxBus` None): a live bus shows zero foreign frames until `canSnifferN_read` is enabled in the tune. An ID inventory identical to the ECU's own TX schedule means the `read` flag is off, not that the bus is dead.

### USB Mass Storage SCSI (known Wireshark false-positive)

When sniffing the ECU's USB link, Wireshark flags the SCSI `Mode Sense(6)` (opcode 0x1a) replies as *"Malformed Packet: SCSI: length of contained item exceeds length of containing item."* This is **not** bad wire data. The reply is a valid but *short* Caching mode page (page code 0x08, `PageLength = 0x0a`) instead of the SBC-2 mandated 0x12; Wireshark's dissector decodes the full 20-byte caching-page layout, overruns the buffer, and raises the exception. Windows accepts the reply and the device works. The response is hardcoded in the ChibiOS-Contrib USB-MSD SCSI target (`os/hal/src/hal_usb_msd.c`, a submodule usually not checked out), used by `firmware/hw_layer/mass_storage/mass_storage_device.cpp`. Treat it as cosmetic unless a host actually rejects it.

### CDC console and MSD share ONE composite USB device — SD mode switch can drop the console link

The USB serial console (CDC) and the SD-card USB mass storage are **interfaces on a single composite `USBD1` device**, not separate devices. The config descriptor is fixed at 3 interfaces — MSD IF0 + CDC-control IF1 + CDC-data IF2 (`firmware/hw_layer/ports/stm32/serial_over_usb/usbcfg.cpp`, `NUM_INTERFACES`/`DESCRIPTOR_SIZE`); MSD is always enumerated whenever `HAL_USE_USB_MSD` is built in. Switching the SD card between PC/MSD and ECU/logging does **not** re-enumerate USB — `attachMsdSdCard`/`deattachMsdSdCard` (`mass_storage_init.cpp`) only hot-swap LUN1's backing block device (real SD card ↔ null device `ND1`) on the already-running MSD controller.

Consequence (observed, `SdEcuPcCycleSandbox`): switching **PC/MSD → ECU** yanks the mounted mass-storage medium out from under the host. Windows' usbstor stack recovers by resetting/re-enumerating the whole composite device; the firmware then takes `USB_EVENT_RESET`/`SUSPEND`, whose handler calls `sduSuspendHookI(&SDU1)` (`usbcfg.cpp`), tearing down the CDC channel. The console link drops host-side (`write failed: wrote 0 but expected 11`, port closes) even though the SD switch itself succeeded firmware-side. So a console-driven SD-mode soak cannot span multiple cycles over one connection — either address it firmware-side (return SCSI "medium not present"/unit-attention for an orderly host eject instead of swapping to a dead LUN) or reconnect the host link after each switch.

**MSD thread wedge → periodic CDC disconnects (confirmed on hardware + USBPcap, 2026-07)**: if the host abandons a mass-storage command mid-data-phase (cancels its IN URB without sending a Bulk-Only Mass Storage Reset — Windows usbstor does exactly this), the MSD thread blocks forever: `lib_scsi` `data_read10` → `scsi_transport_transmit(_wait)` → `usbTransmit`/`usbTransmitWait`, which is `osalThreadSuspendS` with **no timeout** (`firmware/ChibiOS/os/hal/src/hal_usb.c`). The existing `m_botResetPending`/`transportAbandoned()` escape hatch only fires on the class-specific BOT reset request, which Windows does not send in this scenario. A wedged MSD thread never re-arms the bulk-OUT endpoint, so every subsequent CBW NAKs forever; usbstor then resets the whole composite device on a ~20 s timeout cycle, cancelling the CDC IRPs each time — the user-visible symptom is *recurring CDC console/TS disconnects*, with MSD as the hidden culprit. Diagnose with console `sdinfo`: `MSD: executing opcode 0x28 for <huge> ms` = wedged (diagnostics from `MassStorageController::printDiagnostics`). Wire-level signature (USBPcap): all-endpoint `USBD_STATUS_CANCELED` storms, a CBW submit whose IRP survives ~20 s then cancels, no CSW/STALL ever returned.

### WeAct SLCAN discovery after binary HELLO

Live WeAct V1.0.0.6_4fa52575 can discard the V query when CR/V/CR follows
binary TS HELLO in a single write, returning only BELL. Separate the CR
and V writes with a bounded separator-response read; this restored live
detection in 8/8 scans versus 0/8 for the combined write.

### WeAct console ISO-TP arbitration retries

WeAct USB2CANFDV1 defaults to one-shot CAN TX. On a live M74.9 bus, a
request or flow-control frame could disappear despite a complete USB write;
the console then received only four TS body bytes and timed out. Configure
`A1` while CAN is closed, before `O`, for a recognized WeAct version. Keep
this vendor command out of generic Lawicel setup. A fixed 20 ms host delay
improved but did not eliminate failures. Do not replay whole tune-write or
burn requests to compensate: controller arbitration retries address the
unsent CAN frame. WeAct can acknowledge setup commands even though CANable
2 omits them; both support the existing V-response setup barrier.
