# Bootloader

See https://wiki.rusefi.com/Firmware-update-via-CAN

Here we combine ChibiOS with OpenBLT

Q: how do I build bootloader for specific board say proteus/meta-info-proteus_f7.env?

A:  
Option 1:
```
./firmware/bin/compile.sh config/boards/proteus/meta-info-proteus_f7.env bootloader/blbuild/openblt_proteus_f7.bin
```
Option 2:
```
cd firmware/bootloader
source ../config/boards/common_script_read_meta_env.inc ../config/boards/proteus/meta-info-proteus_f7.env
make -j$(nproc)
```


## AT32F435 M74.9 OpenBLT profile

`config/boards/m74_9/meta-info-openblt.env` selects a replacement OpenBLT
loader. It is separate from the disabled legacy board profile and from the
OEM resident-loader firmware project. Do not install this layout on an ECU
that must retain its OEM loader or data layout.

Build from the repository root:

```sh
bash firmware/bin/compile.sh config/boards/m74_9/meta-info-openblt.env bootloader -j12
bash firmware/bin/compile.sh config/boards/m74_9/meta-info-openblt.env -j12
```

The loader occupies `0x08000000..0x08007fff`; the application starts at
`0x08008000`. Application code and the flash programming API are limited to
bank 1 (2 MiB on the ZM part). Bank 2 remains settings storage. The loader and
application reserve the same 16-byte shared RAM area at `0x20000000`.

Transport is CAN1 on PG0 (RX) / PG1 (TX), 500 kbit/s, with extended IDs
`0x10667` (host to ECU) and `0x107e1` (ECU to host). There is no USB transport
on this board. The bootloader does not arm the hardware watchdog: a short
watchdog inherited by application startup caused reset loops in the donor
checkout. Full images install the bootloader; application SREC updates do not.

See `java_console/openblt_can/README.md` for the PCAN uploader. The imported
XCP core and rusEFI transport must be updated together: the core requires
`XcpSetCanBaudrateHook`. Speed changes reboot the loader using shared parameter
byte 4. At 1 Mbit/s the entry window is 6 seconds and 5 seconds without valid
CAN packets triggers a reboot to 500 kbit/s, including after a dropped session.
The host keeps 1 Mbit/s disabled by default because the donor checkout reported
checksum failures with its adapter/driver combination.

Host-only adapter regression checks:

```sh
python3 firmware/bootloader/tests/test_flash.py
python3 firmware/bootloader/tests/test_can.py
python3 firmware/bootloader/tests/test_launcher.py
# Repeat native tests with Clang:
CXX=clang++ python3 firmware/bootloader/tests/test_flash.py
CXX=clang++ python3 firmware/bootloader/tests/test_can.py
```

These compile the production flash/CAN adapters against hardware fakes and
exercise framing, error propagation, bank bounds, erase behavior, CAN filters,
reboot requests, inactivity recovery and malformed frames. They do not replace
bench flashing and power-cycle validation.
