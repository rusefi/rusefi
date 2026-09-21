# STM32H7 two-bank ADC mux

Implemented in `firmware/hw_layer/ports/stm32/stm32_adc_v4.cpp` when the board
defines `ADC_MUX_PIN`. Select low samples logical `EFI_ADC_0..15`; select high
samples `EFI_ADC_16..31` through the same physical inputs. The existing H7 physical scan
order is retained. This supports
the same single-select, two-bank wiring as the F4/F7 driver.

## Timing and publication

Mux mode uses TIM3's periodic interrupt to start one non-circular `EFI_SLOW_ADC` scan (ADC1 by default) with
DMA and hardware oversampling. It deliberately disables ADC hardware TRGO:
before starting a scan, the callback checks `ADC_READY` and elapsed settling
time. An active conversion or a bank that has not settled causes that tick to
be skipped. No ISR sleeps or waits for the analog circuit.

ChibiOS stops non-circular DMA/ADC before calling the completion handler. Only
then does the handler switch the select pin and timestamp it. The next scan
cannot start until the handler returns and ChibiOS restores `ADC_READY`.

Two 16-sample banks occupy a 64-byte `.nocache` DMA staging buffer. This explicit
section is required because H7's `NO_CACHE` macro currently selects `.ram0`,
which is outside the H743 non-cacheable MPU region. The H743 and H723 linker
scripts map `.nocache` to their respective non-cacheable regions. Once both
are complete, the handler copies all 32 samples to the existing slow ADC output
buffer, then calls `onFastAdcComplete()`. Fast MAP/ADC-trigger consumers see
one callback per complete pair; an unfinished pair never replaces published
data. ADC errors discard the partial pair, increment the slow ADC error counters
(including overrun when applicable), and restart at bank zero on an eligible tick.

At `H7_ADC_SPEED=10000`, the nominal per-bank and fast-callback rate is 5 kHz.
The existing conversion-time estimate is 64 us for 16 inputs with 4x oversampling;
the rest of the 100 us tick must accommodate interrupt latency and settling.
Skipped ticks reduce the rate. ADC-trigger users must account for the halved
callback rate and the time separation between the two banks.

`H7_ADC_MUX_SETTLE_US` specifies the minimum elapsed time from switching the GPIO
to starting the next scan; it defaults to 10 us and must be positive. Set it for
the actual mux, source impedance and input RC network, then verify on hardware.
It may exceed one timer period; the driver will skip ticks until it has elapsed.

Without `ADC_MUX_PIN`, the original 16-channel, hardware-triggered circular DMA
path remains in use. Fast tokens are limited to the available 16 or 32 channels,
and reads before startup or outside that range return zero. Failed ADC startup
returns false and remains retryable.

## Validation

Run the host tests (the actual driver compiled against a small mock HAL):

```bash
python3 unit_tests/hardware/adc_v4/test_adc_v4.py
```

The 18 scenarios cover mux/no-mux operation, first-pair publication, callback data freshness,
bank mapping, incomplete callbacks, busy ADCs, delayed completion, settling times
of 10 and 150 us, error recovery, failed-start retry, an `EFI_SLOW_ADC` override,
the preserved `ADC_CFGR_OVRMOD` erratum setting, and fast-token bounds under
AddressSanitizer and UndefinedBehaviorSanitizer. Register encodings and HAL ABI
still require an ARM firmware build; the mock does not validate analog behavior.

Compile-only examples from `firmware/` (PF2 is an example select pin, not a new
Proteus board default). Use distinct build and dependency directories when
changing compiler flags, since make does not track them:

```bash
EXTRA_2_PARAMS='-DADC_MUX_PIN=Gpio::F2' bash bin/compile.sh config/boards/proteus/meta-info-proteus_h7.env -j12 BUILDDIR=/tmp/rusefi-h7-adc-mux-build DEPDIR=/tmp/rusefi-h7-adc-mux-dep
bash bin/compile.sh config/boards/proteus/meta-info-proteus_h7.env -j12 BUILDDIR=/tmp/rusefi-h7-adc-plain-build DEPDIR=/tmp/rusefi-h7-adc-plain-dep
```

No board is automatically opted into mux mode. A real board should define its
select GPIO and settling time in its board configuration.

Before hardware use, feed distinct known voltages to both banks, verify all 32
logical readings and MAP averaging, and scope the select transitions against
conversion timing under interrupt load. Check that ADC errors recover without
mixing bank data, and that the separate ADC3 knock path still works. Host tests
and builds do not establish the analog settling time or worst-case sample rate.
