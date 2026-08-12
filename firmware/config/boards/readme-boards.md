# Boards in this folder

This is an overview of the board directories with active builds. Completely disabled boards and
connector/.yaml-only directories are intentionally not listed here.

## Development boards - not recommended as real ECU

| Board | Notes |
| ----- | ----- |
| [f407-discovery](f407-discovery) | STM32F407 Discovery - the first platform rusEFI was ever launched on; the most hardware-agnostic binary |
| [f429-discovery](f429-discovery) | STM32F429 Discovery |
| [nucleo_f429](nucleo_f429) | STM32 Nucleo-F429 |
| [nucleo_f767](nucleo_f767) | STM32 Nucleo-F767 |
| [nucleo_h743](nucleo_h743) | STM32 Nucleo-H743 |
| [test-build-guards](test-build-guards) | Fake board used by CI to assert firmware builds without ADC and GPT - not hardware at all |
| [hellen/premium-quick-test](hellen/premium-quick-test) | Internal Hellen premium quick-test / QC target - not an end-user ECU |

## Universal boards

| Board | Notes |
| ----- | ----- |
| [microrusefi](microrusefi) | microRusEFI (MRE), F4 and F7 variants |
| [proteus](proteus) | Proteus, F4/F7/H7 variants, including a TCU variant |
| [hellen/uaefi](hellen/uaefi) | uaEFI, including Pro F7/H7 variants |
| [hellen/uaefi121](hellen/uaefi121) | uaEFI with automotive 121-pin header |
| [hellen/super-uaefi](hellen/super-uaefi) | uaefi121 variation with F7 and superseal headers |
| [hellen/alphax-2chan](hellen/alphax-2chan) | AlphaX 2chan |
| [hellen/alphax-4chan](hellen/alphax-4chan) | AlphaX 4chan, F4 and F7 variants |
| [hellen/alphax-8chan](hellen/alphax-8chan) | AlphaX 8chan, F4 and F7 variants |
| [hellen/alphax-8chan-revA](hellen/alphax-8chan-revA) | AlphaX 8chan revision A |
| [hellen/alphax-4K-GDI](hellen/alphax-4K-GDI) | AlphaX 4K GDI |
| [hellen/alphax-gold](hellen/alphax-gold) | AlphaX Gold |
| [hellen/alphax-silver](hellen/alphax-silver) | AlphaX Silver |
| [hellen/alphax-silver-revA](hellen/alphax-silver-revA) | AlphaX Silver revision A |
| [hellen/small-can-board](hellen/small-can-board) | Small CAN accessory board |

## Vehicle-specific boards

| Board | Notes |
| ----- | ----- |
| [frankenso_na6](frankenso_na6) | Frankenso configured for Mazda Miata NA6 |
| [subaru_eg33](subaru_eg33) | Subaru EG33 (SVX) |
| [hellen/hellen-gm-e67](hellen/hellen-gm-e67) | GM E67 connector |
| [hellen/hellen-honda-k](hellen/hellen-honda-k) | Honda K series |
| [hellen/hellen121nissan](hellen/hellen121nissan) | Nissan, 121-pin connector |
| [hellen/hellen121vag](hellen/hellen121vag) | VAG, 121-pin connector |
| [hellen/hellen128](hellen/hellen128) | Mercedes, 128-pin connector |
| [hellen/hellen154hyundai](hellen/hellen154hyundai) | Hyundai/Kia, 154-pin connector, F4 (revisions A/B/C) |
| [hellen/hellen154hyundai_f7](hellen/hellen154hyundai_f7) | Hyundai/Kia, 154-pin connector, F7 |
| [hellen/hellenMiataNA6](hellen/hellenMiataNA6) | Mazda Miata NA6 |
| [hellen/hellenMiataNA96](hellen/hellenMiataNA96) | Mazda Miata NA 1996 |
| [hellen/hellenMiataNB1](hellen/hellenMiataNB1) | Mazda Miata NB1 |
| [hellen/hellenMiataNB2](hellen/hellenMiataNB2) | Mazda Miata NB2 |

## Pinout language conventions

Connector yaml pin entries use these canonical `function:` strings:

| `type:` | Canonical `function:` value |
| ------- | --------------------------- |
| `gnd` | `Power/Chassis GND ground` |
| `agnd` | `GNDA Analog/Sensor Ground` |
| `inj` | `Injector N` |
| `ign` | `Smart Ignition Coil N` |
| `av` (TPS) | `Universal analog input. Suggested TPS N sensor input` |
| `av` (PPS) | `Universal analog input. Suggested PPS input N` |
| `at` (IAT) | `IAT Sensor Input` |
| `at` (CLT) | `ECT CLT Coolant Sensor Input` |
| `av` (spare) | `Auxiliary analog input N PullDown 500K` |
| `din` (flex) | `Digital Input / Flex fuel sensor` |
| `can` | `CAN bus High` / `CAN bus Low`; second bus `CAN2 bus High` / `CAN2 bus Low` |

`Smart Ignition Coil N` means a logic-level ignition output: it drives a smart coil
(built-in igniter) or an external igniter module, never a dumb coil directly. Coils are
numbered by coil, not by cylinder - on wasted-spark boards the ts_name carries the
cylinder pairing (e.g. hellenMiataNA6 `1G - Ignition 1 & 4`). Applied so far on the
universal boards (uaefi, uaefi121, super-uaefi, proteus, microrusefi, alphax-2chan,
alphax-4chan, alphax-8chan, alphax-8chan-revA, alphax-4K-GDI), hellen121nissan,
frankenso_na6 and all hellenMiata boards (NA6, NA96, NB1, NB2).

The TPS/PPS strings follow the uaefi121 style: the `Universal analog input.
Suggested ` prefix says the pin is a general-purpose analog input with a
recommended assignment, and the sensor is numbered 1/2 (never `TPS2`/`PPS2`).
Applied on all universal boards; extra information rides along where meaningful
(e.g. proteus `TPS 2 / TPS1_2`). Board-specific channel numbers and silkscreen
labels stay in `ts_name`.

`IAT Sensor Input` / `ECT CLT Coolant Sensor Input` are applied on every board
with a plain IAT/CLT signal pin (universal and plug-n-play alike); extra
information rides along in the same value where meaningful (microrusefi
` or SENT input!`, s105 pull-up specs, hellen154hyundai wire color). Some
boards declare these pins `type: av` instead of `at` (alphax-gold/silver,
hellen-gm-e67, hellen128) - existing explicit types were left as-is.

Spare ADC pins use `Auxiliary analog input N PullDown 500K` (numbered per
board). The `PullDown 500K` suffix states the hellen-family input circuit;
proteus and microrusefi use the same base name without it, keeping their own
hardware notes instead (`, for aux see R84`, `without R30, ...`) - confirm
their pulldown values before unifying further. The first CAN bus is
unnumbered (`CAN bus High`, not `CAN1 ...`); additional buses are `CAN2`,
`CAN3`, ...

Exceptions:

- alphax-4K-GDI is the single GDI board and keeps its special injector naming:
  `Port injector N` for the port-injection channels plus `GDI Injector N Positive` /
  `GDI Injector N Negative` pairs for the direct injectors.
- uaefi, uaefi121 and super-uaefi outputs can be repurposed, so coil pins use just
  `Smart Ignition Coil N`; uaefi121 intentionally has two pins per coil (current is
  split across both wires), both carrying the same function value.
