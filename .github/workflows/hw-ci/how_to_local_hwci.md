# requeriments:

Apart from the requirements for building firmware, you need to install ST-Util on the host:
https://github.com/stlink-org/stlink?tab=readme-ov-file#installation

If you are using a devcontainer, it has to be installed on the host, *not* on the docker image.

## wiring:

* F4-Discovery: read HwCiF4Discovery.java or https://wiki.rusefi.com/Dev-Quality-Control#stm32f407g-disc1
* F767-Nucleo: jumper between "PD2" and "PA6", it is recommended to power the board from VIN and not from the STLink USB

## compiling:

F4-Discovery:

```bash
.github/workflows/hw-ci/build_for_hw_ci.sh config/boards/f407-discovery f407-discovery
```

F767-Nucleo:

```bash
.github/workflows/hw-ci/build_for_hw_ci.sh config/boards/nucleo_f767 nucleo_f767/
```


## erase & flash, all targets:

```bash
.github/workflows/hw-ci/st_flash_wipe_and_flash.sh firmware/deliver/rusefi.bin
```

This is the same script CI uses: full erase, then write, both with `--connect-under-reset`
and a few retries. Set `HARDWARE_CI_STLINK_SERIAL` to pick one ST-LINK if several are attached.

## run CI:
```bash
.github/workflows/hw-ci/run_hw_ci.sh com.rusefi.HwCiNucleoF7 ../firmware/tunerstudio/generated/rusefi_stm32f767_nucleo.ini
```
