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
st-flash --area=main erase
st-flash --format=binary --reset --connect-under-reset write firmware/deliver/rusefi.bin 0x08000000
```

## run CI:
```bash
.github/workflows/hw-ci/run_hw_ci.sh com.rusefi.HwCiNucleoF7 ../firmware/tunerstudio/generated/rusefi_stm32f767_nucleo.ini
```
