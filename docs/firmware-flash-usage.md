# Firmware flash usage

## uaefi121: measured example

On 2026-10-03, revision `d942e51da6d` overflowed uaefi121's application
flash region by 104 bytes. Disabling the embedded INI disk with
`EFI_EMBED_INI_MSD=FALSE` saves 139,540 bytes and leaves 139,436 bytes
(136.2 KiB) free. This is now set in
[uaefi121/board.mk](../firmware/config/boards/hellen/uaefi121/board.mk).

Users must obtain the matching TunerStudio INI from the firmware bundle.
The ECU no longer supplies its embedded INI disk/image. USB serial tuning,
SD logging and USB access to the SD card remain compiled in. Wideband
firmware updates and Lua name lookups also remain enabled.

Measurements used Arm GNU Toolchain 14.2.Rel1 (GCC 14.2.1), matching the
version pinned in `.github/workflows/build-firmware.yaml`, release `-Os`,
default LTO, and `USE_OPENBLT=yes` from the board's `meta-info.env`.
Each alternative below starts from the original board configuration;
the savings are independent, not cumulative.

| Configuration | `flash0` used, bytes | Saved, bytes | Free, bytes |
|---|---:|---:|---:|
| Original uaefi121 | 753,768 | 0 | -104 (link fails) |
| Disable embedded INI: `EFI_EMBED_INI_MSD=FALSE` | 614,228 | 139,540 | 139,436 |
| Disable wideband updater: `EFI_WIDEBAND_FIRMWARE_UPDATE=FALSE` | 725,748 | 28,020 | 27,916 |

The original map contains a 139,264-byte `ramdisk_image` (136 KiB) and a
25,600-byte `build_wideband_image_bin` (25 KiB). Removing a feature also
removes some supporting code, so the linked savings exceed the array sizes.
The comment in `mass_storage_init.cpp` saying 128K is approximate;
`bin/gen_image_board.sh` currently defaults `INI_IMAGE_SIZE` to 136.

The overflow was reproduced twice. Both alternatives were built with fresh,
separate object, PCH and dependency directories, then invoked again with the
same settings to check for regeneration changes. These are build/link
measurements, not hardware tests. Future revisions and toolchains can differ.

## What to disable next

Prefer a feature whose loss is acceptable for the board's users, and leave
room for future growth instead of fixing only the last few overflow bytes.

1. **Embedded INI disk** (`EFI_EMBED_INI_MSD=FALSE`): the change chosen for
   uaefi121. It removes the local copy of the INI and its disk wrapper.
   `getStorageImage()` returns null and its size is zero; the INI USB LUN
   uses the null block device. It does not disable `EFI_USB_SERIAL`,
   `EFI_FILE_LOGGING`, or the separate SD-card LUN.
2. **Wideband firmware updater** (`EFI_WIDEBAND_FIRMWARE_UPDATE=FALSE`):
   the next candidate if more space is needed. It removes the embedded
   wideband payload and both ECU-driven update paths (embedded payload and
   SD-file update). Normal CAN wideband sensor communication remains.
   uaefi121 has onboard wideband controllers, so retaining this service
   capability is useful. The measured 28,020-byte saving above is relative
   to the original build; remeasure it on top of the INI change.
3. **Lua name lookups** (`EFI_LUA_LOOKUP=FALSE`): a larger, more disruptive
   option. Prior f407-discovery measurements were roughly 60 KiB; this was
   not measured on uaefi121 in this investigation. It removes generated
   config/output lookups used by Lua `getCalibration`, `setCalibration`,
   and `getOutput`, console get/set-by-name, and CAN calibration lookup.
   Lua itself and its separate sensor APIs remain. See
   [lookup tables](AI/lookup.md) for the stubs and generated-code details.
4. **Unused peripherals and optional controllers**: inspect the board's
   hardware and intended tunes before removing accelerometer, EGT, CAN GPIO,
   Bluetooth setup, LTFT or other controllers. Measure each candidate;
   source-file size and feature names do not establish the saving.
   Keep trigger diagnostics and engine protection unless their loss is
   explicitly acceptable.

uaefi121 already disables `EFI_LOGIC_ANALYZER`, `EFI_MISFIRE_DETECTION`, and
`EFI_HPFP`. Disabling an already-false flag saves nothing. In particular,
misfire detection also defaults to false in the F4 feature header.

## Measure the right memory region

The F4 linker script reserves 768 KiB for code before subtracting the 32 KiB
OpenBLT region. For this board, application `flash0` is therefore
`0x08008000..0x080BFFFF`, or 753,664 bytes (736 KiB).
The two 128 KiB settings sectors start at `0x080C0000` (backup) and
`0x080E0000` (primary) on a 1 MiB device. See
[STM32F4.ld](../firmware/hw_layer/ports/stm32/stm32f4/STM32F4.ld) and
[mpu_util.cpp](../firmware/hw_layer/ports/stm32/stm32f4/mpu_util.cpp).

- Use the linker's `--print-memory-usage` **`flash0`** row as the budget.
  Do not enlarge the linker region into the settings sectors to hide an overflow.
- `arm-none-eabi-size` is useful for comparison, but its default `dec` total
  includes BSS. BSS is RAM, not a flash payload. Initialized RAM data has a
  flash load image, so `.text` alone is also insufficient.
- ELF file size includes debug information; `.srec` is a text representation;
  the packaged `deliver/rusefi.bin` also contains the bootloader. None is a
  direct replacement for the application-region accounting.
- `ram0`/`ram4` can report 100% because linker-defined heaps claim the remaining
  space. This does not describe thread-stack headroom or Lua heap pressure.
- LTO can inline generated lookup switches into large Lua hook functions.
  Symbol names alone can misattribute their cost. Compare complete builds.

## Reproducible build and inspection

From the repository root, build with the board's metadata so CPU, identity
and bootloader reservation all match the shipped configuration:

```bash
cd firmware
source config/boards/common_script_read_meta_env.inc config/boards/hellen/uaefi121/meta-info.env
make -j12 -r clean
make -j12 -r > /tmp/uaefi121-build.log 2>&1
```

Check the make exit status and the log for link errors. A failed link can
still produce `build/rusefi.map`; an older ELF or file in `deliver/` does not
prove the new build succeeded. Invoke the same build a second time and
check whether code generation changes the result before recording a size.

```bash
rg 'flash0:|overflowed|error:' /tmp/uaefi121-build.log
arm-none-eabi-size -A build/rusefi.elf
arm-none-eabi-objdump -h build/rusefi.elf
arm-none-eabi-nm -S --size-sort --radix=d -C build/rusefi.elf | tail -40
rg -n 'ramdisk_image|build_wideband_image_bin|allParameters|rodata.*fields' build/rusefi.map
```

`objdump -h` distinguishes runtime addresses (VMA) from load addresses (LMA).
For `nm`, check the symbol type as well as size: `T/t` is code, `R/r` read-only
data, and `B/b` BSS. A large BSS symbol is not a flash-saving opportunity.

For an experiment, use fresh `BUILDDIR` **and** `DEPDIR` paths and request
only the application ELF. For example, after sourcing the metadata above:

```bash
make -j12 -r \
  BUILDDIR=/tmp/uaefi121-no-wideband \
  DEPDIR=/tmp/uaefi121-no-wideband-dep \
  EXTRA_PARAMS=-DEFI_WIDEBAND_FIRMWARE_UPDATE=FALSE \
  /tmp/uaefi121-no-wideband/rusefi.elf
```

Use new paths for each flag combination. Make does not reliably invalidate
objects when command-line flags change. The PCH lives under `BUILDDIR`,
while compiler dependencies normally live separately in `firmware/.dep`.
Do not run different board builds concurrently: several generated headers,
lookup files and RAM-disk images have shared paths. Leave regenerated files
unstaged; reverting them can make stale generated content appear current.

## Put feature switches in the right place

- Ordinary board-specific switches belong in `board.mk` as
  `DDEFS += -DEFI_FEATURE=FALSE`. Avoid changing global F4 defaults for one board.
- Lua lookup removal needs **both** the make variable and C/C++ definition:

  ```make
  EFI_LUA_LOOKUP = FALSE
  DDEFS += -DEFI_LUA_LOOKUP=$(EFI_LUA_LOOKUP)
  ```

  `controllers/lua/lua.mk` uses the make variable to select stub sources.
  A C/C++ define alone does not remove the generated lookup translation units.
- `EFI_LUA` and `EFI_LTFT_CONTROL` own TunerStudio pages. Disable these via
  `#define EFI_LUA FALSE` or `#define EFI_LTFT_CONTROL FALSE` in the board's
  `prepend.txt`, so generated INI pages and firmware agree. Follow the
  `[tag:disable_engine_module]` instructions in
  [engine_module.h](../firmware/controllers/core/engine_module.h).
- Hiding a menu with `ts_show_*` or turning a feature off in the tune normally
  does not remove its compiled code. Regenerate through `make`; do not edit
  generated INI/C++ files by hand.
- Preserve the release optimization settings while comparing features.
  The default build adds `-flto=auto` even though it assigns `USE_LTO=no`
  internally for ChibiOS compatibility. Setting `USE_LTO=no` explicitly on
  the command line bypasses that default block and changes the comparison.
- Compressing the embedded INI is another option, but the current compressed
  block device needs roughly 34 KiB RAM and a larger USB MSD thread stack.
  Check actual RAM/stack headroom before enabling
  `EFI_USE_COMPRESSED_INI_MSD` on an F407.

After selecting a change, rebuild normally with the board's metadata,
confirm the expected symbols disappeared, and check the final application
and bundle build. Hardware validation should cover the affected USB/SD or
service workflows before release.
