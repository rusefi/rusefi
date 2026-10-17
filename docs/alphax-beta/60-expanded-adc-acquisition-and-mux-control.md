# 60 — Expanded ADC acquisition and mux control

[Back to the ECU change index](../alphax-beta-vs-master.md) · [Fork branch](https://github.com/ElDominio/rusefi/tree/alphax-beta)

## Executive summary

The branch extends STM32 slow-ADC handling with separate ADC1/ADC3 mux selection, additional ADC3 channel groups, up to 48 logical channels, inverted mux polarity, corresponding parent-channel mapping and extra conversion stages. S550 uses the ADCD3 slow path, while raw mux diagnostics and forced GPIO writes help establish mux state. ADC output-pin API changes accompany that forced write.

## ECU logic and behavior

1. Select separate ADC1/ADC3 mux arrangements and their polarity from board configuration.
2. Add ADC3 groups and extra conversion stages for up to 48 logical inputs.
3. Switch mux GPIO state and map each logical sample to the appropriate parent ADC channel.
4. Use forced GPIO writes where state must be established even when cached output state suggests no change.
5. Expose raw mux diagnostics and connect the S550 board to the ADC3 slow path.

**Illustrative behavior:** The same parent ADC pin can represent two different sensors across mux states; a wrong phase/polarity can swap valid-looking temperature or pressure readings.

## Calibration, dependencies and review limits

This is board-sensitive acquisition work, not just extra names in a sensor enum. Master has evolved its own ADC support since the common ancestor, so these changes need to be reconciled at the hardware/conversion-sequence level. The branch-side diff contains no dedicated new ADC acquisition test; host sensor tests cannot establish sampling timing or correct physical channel routing.

Verify startup GPIO state, sample ordering, settling and channel identity on hardware. Current master also evolved its ADC implementation, so these patches require sequence-level reconciliation, not just enum additions.

No feature-specific test is claimed by this report. The source/commit evidence below establishes implementation, not hardware validation.

No firmware, host-test, simulator or hardware run was performed for this documentation update. The pinned branch tip records an unresolved CAN-listener unit-test link failure; see the [comparison and validation notes](../alphax-beta-vs-master.md#validation-limits).

## Lines of code changed

**Retained source-file footprint: 363 changed lines — 326 additions and 37 deletions across 7 maintained C/C++ files.**

Baseline: common ancestor `9154d40f5732` → fork `ebb877615ee1`. For the same files, upstream master `c12cf1498315` → fork is **+371 / −110**.

These are physical `git diff --numstat` source lines, including comments and blank lines. They measure the explicitly listed file scope, not a feature-exclusive patch. Shared files contain other changes; counts overlap between reports and must not be added together. The master comparison also includes upstream work absent from the fork. Generated code, tune schemas, build files, documentation and tests are excluded from this firmware-source total.

Of this scope, 0 files are new since the common ancestor (+0 / −0); new shared modules can still contain several features.

| Measured source file | Common ancestor → fork | Master → fork |
| --- | ---: | ---: |
| [firmware/controllers/algo/rusefi_hw_adc_enums.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/algo/rusefi_hw_adc_enums.h) | +1 / −1 | +2 / −3 |
| [firmware/controllers/system/efi_output.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/controllers/system/efi_output.h) | +1 / −1 | +1 / −1 |
| [firmware/hw_layer/adc/adc_onchip_slow.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/adc/adc_onchip_slow.cpp) | +23 / −3 | +25 / −16 |
| [firmware/hw_layer/drivers/gpio/core.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/drivers/gpio/core.cpp) | +34 / −4 | +0 / −1 |
| [firmware/hw_layer/io_pins.h](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/io_pins.h) | +3 / −2 | +3 / −15 |
| [firmware/hw_layer/ports/stm32/stm32_adc.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/ports/stm32/stm32_adc.cpp) | +31 / −3 | +74 / −10 |
| [firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp) | +233 / −23 | +266 / −64 |

Reproduce these counts with `git diff --no-renames --numstat <left> <right> -- <the files above>`. Full immutable revisions are in the [index](../alphax-beta-vs-master.md#comparison-scope).

## Relevant commits

Links below open commits in the ElDominio fork. Each selected commit touches the measured source or cited tests and matches this change's history. The source delta column is restricted to the measured files, against that commit's first parent. It is historical churn, not an additional retained-size total. Mixed commits can include unrelated features; earlier implementations may be replaced by later commits.

| Date | Commit and original subject | Source delta in measured files |
| --- | --- | ---: |
| 2026-06-16 | [7aa222cc0e9](https://github.com/ElDominio/rusefi/commit/7aa222cc0e9c633a315c303597ed23c2733021dd) — AlphaX feature set rebased onto master (June 2026) **Mixed feature-set commit.** | +235 / −33 |
| 2026-08-01 | [88e36bfb459](https://github.com/ElDominio/rusefi/commit/88e36bfb45982f619a1cb1167209506ef6f38610) — Fix A/C Pressure reading wrong on power-on on alphax-s550-pnp | +57 / −0 |

Supporting configuration and source: [firmware/config/boards/alphax-s550-pnp/board.mk](https://github.com/ElDominio/rusefi/blob/ebb877615ee157feaadc9a186d31f193f488c643/firmware/config/boards/alphax-s550-pnp/board.mk).
