# Off-chip ADC inputs

`EFI_ADC_40` through `EFI_ADC_47` are the eight reserved external ADC channels.
Their numeric values remain 41 through 48 in saved configurations.
Channels 32 through 39 are reserved for MCU ADC3 inputs.

## ADS7128

Register both the GPIO pins and ADC channels during board hardware init:

```cpp
ads7128_add(Gpio::EXTIOCHIP_0_IO_1, 0, &ads7128_cfg, EFI_ADC_40);
```

The config must outlive the driver. Its `vref` is the ADS7128 reference voltage,
independent of the MCU's `adcVcc`. Omitting the ADC base preserves GPIO-only
registration. The current enum allocation has room for one eight-channel ADC.

The premium quick-test board registers AIN0 through AIN7 this way and exposes
them as `ADS7128 AIN0` through `ADS7128 AIN7` in TunerStudio sensor pin lists.
The driver polls every 100 ms, so these inputs are suitable for slow sensors.
They do not support fast MAP averaging, trigger sampling, or knock sampling.

Normal ADC subscriptions claim the corresponding extender GPIO in analog mode.
`adcGetRawVoltage()` uses the cached ADS7128 sample and its reference voltage.
`adcGetScaledVoltage()` then applies the normal board input divider and voltage
adjustment hooks. Boards should supply the appropriate divider for their
external analog input circuitry, just as they do for MCU inputs.

Before the first successful read, after a failed I2C read, or while a pin is
configured as digital GPIO, its ADC reading is invalid. Sensor subscriptions
stop refreshing that sensor, allowing its existing timeout handling to run.
The `adc` console command and ADC reports use the same voltage path.

## Adding another driver

Implement `AdcChip::readAdc()` and call `adcchipRegister()` during board hardware
init. Supply the native raw count and voltage together. Reads must use cached
data, since the sensor loop must not wait for SPI or I2C transactions.

Registration rejects overlaps and ranges outside the reserved external slots.
Several smaller devices can share those slots. Each registration maps its
zero-based local ADC channels to consecutive GPIO pins for pin ownership and
analog-mode setup. The chip object and registration remain valid for the
lifetime of the firmware.
