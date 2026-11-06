# rusEFI F0 wideband CAN bitrate

The rusEFI F0 module supports 500 kbit/s and 1 Mbit/s with the configurable-bitrate firmware installed in both its bootloader and application. Stock firmware and the earlier fixed-1-Mbit/s build do not implement this setting.

## Changing the bitrate in TunerStudio

Open **rusEFI Wideband Tools**, select the CAN bus and **Target device HW ID**, then use **Module bitrate after power cycle** and **Store module bitrate** with the engine stopped. This dropdown requests a setting; it is not a readout of the running module's bitrate.

1. Keep the ECU and every module at the current bitrate. Select the desired rate and store it on each module on that bus. Wait for **Done** for each module. Hardware IDs must be unique on the bus.
2. Set the matching **Primary ECU CAN bitrate** or **Secondary ECU CAN bitrate** on the same page. All other devices on that physical bus must support and use the new rate.
3. Burn the ECU settings and wait until the burn completes. Power cycle the ECU and all wideband modules together. Ping each module and check valid lambda data and heater operation before running the engine.

Saving the module setting does not change its running CAN peripheral. **Any subsequent module reset**, including entry into the CAN bootloader, activates the saved rate. Do not request firmware updates or reset individual modules between steps 1 and 3. There is no automatic ECU bitrate switch or automatic rollback.

A missing confirmation leaves status Failed. It can mean incompatible module firmware, a wrong hardware ID/bus/bitrate, failed storage, or a lost reply. Keep the bus at its current rate and retry the same request; a lost reply does not prove that the setting was not saved.

## Installation and updates

The initial installation requires the full configurable-bitrate image over SWD, including its bootloader. A normal CAN application update cannot replace a fixed-rate bootloader. The new application refuses bitrate writes unless the installed bootloader carries the matching capability marker in its reserved vector word at 0x08000020. The full image ends before the configuration page. Back up the device before programming and avoid a mass erase if existing settings are to be kept.

An erased or invalid bitrate record selects **500 kbit/s** in both application and bootloader. This also applies when migrating from the earlier fixed-1-Mbit/s image, unless a valid record has already been installed. Set the ECU/adapter to 500 kbit/s to configure such a device initially.

Compatible application updates preserve the config page and bitrate record. Stock firmware does not understand this record and can erase it during a settings write; reverting to stock firmware is not a supported way to retain 1 Mbit/s. The existing stock-firmware update buttons are not a source of the configurable-bitrate image. Use the matching image and keep the configurable bootloader installed.

An interrupted configuration-page erase/program may lose both sensor/index settings and bitrate. Both new images then fall back to 500 kbit/s; reconnect the ECU/adapter at 500 kbit/s or recover via SWD. No second config page is reserved for transactional recovery.

## Protocol and storage

Extended data frame `0x0EF80000`, DLC 2: `[hardware ID, rate]`, where rate 0 = 500 kbit/s and 1 = 1 Mbit/s. Only addressed IDs 0 through 8 are accepted; no broadcast write. After verifying the flash payload and record, the module sends extended data frame `0x0EF80001`, DLC 2, echoing the same two bytes at the still-active bitrate. The ECU requires this exact reply on the selected bus; generic legacy ACKs, other devices, and different rates cannot complete the request.

The shared header `for_rusefi/wideband_bitrate.h` in the wideband repository defines the wire format and stable record ABI. The last eight bytes of the F042 config page at **0x08007ff8** contain little-endian magic `0x57425231`, uint16 rate and uint16 bitwise inverse. The record is independent of the application's Configuration version. Every new F0 sensor/index/config write preserves it. The application and bootloader compile the same CAN selector and enforce a 48 MHz APB clock. CAN BTR is `0x001c0005` for 500 kbit/s and `0x001c0002` for 1 Mbit/s, with a sample point of 87.5% for both.

## Validation before deployment

Host tests exercise the real selector and config writer using mocked flash: both directions, normal config writes preserving pending rates, erased/corrupt/torn records, failed writes and addressed response matching. ARM builds check the linker limits and nominal clock assumption. Hardware validation is still required for initial SWD installation, both boot rates, both CAN update rates, both switch directions, multiple modules, reset before ECU switch and power loss during storage. No hardware is flashed by this change.
