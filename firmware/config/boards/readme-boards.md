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

##

Canonical function strings in use:

- `Power/Chassis GND ground` for `type: gnd`
- `GNDA Analog/Sensor Ground` for `type: agnd`

- 'Injector N' for type: inj

alphax-4K-GDI is a single GDI board with special injector naming: 'Port Injector N' and Positive/Negative GDI injectors
