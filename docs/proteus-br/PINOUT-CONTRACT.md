# PROTEUS-BR — CONTRATO DE PINOUT (target `proteus_f4`)

**Status:** auditado contra este repositório (fork rusEFI), commit da branch
`claude/proteus-br-pinout-contract-iwnwux`.
**Regra:** nada neste documento pode ser alterado na placa Proteus-BR enquanto
o firmware não for modificado. Toda linha cita `arquivo:linha` deste repo como
evidência. O que não tem evidência está na seção
[NÃO RESOLVIDO / PERGUNTAS](#não-resolvido--perguntas).

Legenda de criticidade:

| Código | Significado |
|---|---|
| **BOOT** | Hardcoded em tempo de compilação (board.mk / board.h / knock_config.h / funções C++). Mudar exige recompilar firmware. Obrigatório respeitar na placa. |
| **DEFAULT** | Default de configuração gravado pela board config (`board_configuration.cpp`). Pode ser remapeado via TunerStudio sem reflash, mas a placa DEVE seguir o default para ser "plug-and-play" com o firmware Proteus inalterado. |
| **SILÍCIO** | Restrição do próprio STM32 (canal ADC, timer, periférico fixo no pino). Inviolável. |
| **HW** | Pino de hardware do MCU sem envolvimento de firmware (strap/reset/cristal). |

---

## A. MCU compilado pelo target `proteus_f4`

| Item | Valor | Evidência |
|---|---|---|
| CPU do target | `PROJECT_CPU=ARCH_STM32F4` | `firmware/config/boards/proteus/meta-info-proteus_f4.env:2` |
| Família selecionada | `IS_STM32F429 = yes` quando `PROJECT_CPU=ARCH_STM32F4` | `firmware/config/boards/proteus/board.mk:4-6` |
| Defines de chip | `-DSTM32F429xx -DSTM32F429_MCUCONF -DEFI_IS_F42x` + `STM32F4_HAS_SRAM3=1` | `firmware/hw_layer/ports/stm32/stm32f4/hw_ports.mk:21-24` |
| Core/FPU | Cortex-M4, FPU hard | `firmware/hw_layer/ports/stm32/stm32f4/hw_ports.mk:7-8` |
| Flash exigida | bootloader 32k @ 0x08000000 + imagem (default `flash_size = 768k`) → **mínimo 768 KB; chip de 1 MB (sufixo "G") atende** | `firmware/hw_layer/ports/stm32/stm32f4/STM32F4.ld:33-37` |
| Bootloader | OpenBLT habilitado (`USE_OPENBLT=yes`) | `firmware/config/boards/proteus/meta-info-proteus_f4.env:3` |
| RAM | `ram0 = 128k + SRAM3 (64k)` = 192 KB linkados (exige família F42x com SRAM3) | `firmware/hw_layer/ports/stm32/stm32f4/STM32F4.ld:26` e `:54` |
| HSE | **Auto-detectado em runtime.** Firmware finge 25 MHz no boot via HSI, mede o cristal real e reprograma PLLM = N para cristal de N MHz (valor inteiro de MHz obrigatório) | `firmware/hw_layer/ports/stm32/mcuconf_common_f4_f7.h:439-450`; `firmware/hw_layer/ports/stm32/osc_detector.cpp:1-15,115-140` |
| LSE | Não montado; RTC desabilitado no F4 (`EFI_RTC=FALSE`) | `firmware/hw_layer/ports/stm32/stm32f4/board.h:55-58`; `meta-info-proteus_f4.env:5` |
| Option bytes F427 | arquivo de referência existe no repo | `firmware/hw_layer/ports/stm32/stm32f4/option-bytes-f427.json` |

**Conclusão:** o build `proteus_f4` é compilado para a família **STM32F42x**
(`STM32F429xx` cobre F427/F429 — mesmo die, F429 só adiciona LTDC). O
**STM32F427ZGT6 (LQFP144, 1 MB flash, 256 KB SRAM incl. SRAM3)** satisfaz todos
os requisitos do linker e dos defines. Pacote "Z" (144 pinos) é obrigatório:
o firmware usa portas F e G (ex.: PF4/PF5 knock, PG2–PG14 saídas), que não
existem em pacotes menores.

---

## B/C. Tabela-contrato de pinos

### B.1 Saídas lowside LS1–LS16

Defaults de injeção: LS1–LS12 = injetores 1–12
(`firmware/config/boards/proteus/board_configuration.cpp:13-26,43-45`).
Relés default: main = LS12, fan = LS11, bomba = LS10
(`board_configuration.cpp:176-180`). Pinos GPIO comuns (sem periférico
obrigatório); LS em B4/B5/B6/B7/B8/B9 têm hardware-PWM possível via
TIM3/TIM4 (`firmware/hw_layer/ports/stm32/stm32_pwm.cpp:113-125`).

| Função | Pino STM32 | Periférico | Evidência (mapeamento) | Criticidade |
|---|---|---|---|---|
| LS1 (Inj 1) | PD7 | GPIO | `firmware/config/boards/proteus_meta.h:15` | DEFAULT |
| LS2 (Inj 2) | PG9 | GPIO | `firmware/config/boards/proteus_meta.h:17` | DEFAULT |
| LS3 (Inj 3) | PG10 | GPIO | `firmware/config/boards/proteus_meta.h:20` | DEFAULT |
| LS4 (Inj 4) | PG11 | GPIO | `firmware/config/boards/proteus_meta.h:23` | DEFAULT |
| LS5 (Inj 5) | PG12 | GPIO | `firmware/config/boards/proteus_meta.h:26` | DEFAULT |
| LS6 (Inj 6) | PG13 | GPIO | `firmware/config/boards/proteus_meta.h:29` | DEFAULT |
| LS7 (Inj 7) | PG14 | GPIO | `firmware/config/boards/proteus_meta.h:32` | DEFAULT |
| LS8 (Inj 8) | PB4 | GPIO (TIM3_CH1 possível) | `firmware/config/boards/proteus_meta.h:33` | DEFAULT |
| LS9 (Inj 9) | PB5 | GPIO (TIM3_CH2 possível) | `firmware/config/boards/proteus_meta.h:34` | DEFAULT |
| LS10 (Inj 10 / bomba) | PB6 | GPIO (TIM4_CH1 possível) | `firmware/config/boards/proteus_meta.h:37`; `board_configuration.cpp:179` | DEFAULT |
| LS11 (Inj 11 / fan) | PB7 | GPIO (TIM4_CH2 possível) | `firmware/config/boards/proteus_meta.h:38`; `board_configuration.cpp:178` | DEFAULT |
| LS12 (Inj 12 / main relay) | PB8 | GPIO (TIM4_CH3 possível) | `firmware/config/boards/proteus_meta.h:39`; `board_configuration.cpp:177` | DEFAULT |
| LS13 | PB9 | GPIO (TIM4_CH4 possível) | `firmware/config/boards/proteus_meta.h:42` | DEFAULT |
| LS14 | PE0 | GPIO | `firmware/config/boards/proteus_meta.h:44` | DEFAULT |
| LS15 | PE1 | GPIO | `firmware/config/boards/proteus_meta.h:46` | DEFAULT |
| LS16 | PE2 | GPIO | `firmware/config/boards/proteus_meta.h:48` | DEFAULT |

### B.2 Saídas de ignição IGN1–IGN12

Defaults de ignição: IGN1–IGN12 = cilindros 1–12
(`board_configuration.cpp:28-41,47-49`). GPIO push-pull lógico (5V via buffer
na placa original).

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| IGN1 | PD4 | GPIO | `firmware/config/boards/proteus_meta.h:62` | DEFAULT |
| IGN2 | PD3 | GPIO | `firmware/config/boards/proteus_meta.h:65` | DEFAULT |
| IGN3 | PC9 | GPIO (TIM8_CH4 possível) | `firmware/config/boards/proteus_meta.h:68` | DEFAULT |
| IGN4 | PC8 | GPIO (TIM8_CH3 possível) | `firmware/config/boards/proteus_meta.h:69` | DEFAULT |
| IGN5 | PC7 | GPIO (TIM3_CH2 possível) | `firmware/config/boards/proteus_meta.h:70` | DEFAULT |
| IGN6 | PG8 | GPIO | `firmware/config/boards/proteus_meta.h:71` | DEFAULT |
| IGN7 | PG7 | GPIO | `firmware/config/boards/proteus_meta.h:72` | DEFAULT |
| IGN8 | PG6 | GPIO | `firmware/config/boards/proteus_meta.h:73` | DEFAULT |
| IGN9 | PG5 | GPIO | `firmware/config/boards/proteus_meta.h:74` | DEFAULT |
| IGN10 | PG4 | GPIO | `firmware/config/boards/proteus_meta.h:75` | DEFAULT |
| IGN11 | PG3 | GPIO | `firmware/config/boards/proteus_meta.h:76` | DEFAULT |
| IGN12 | PG2 | GPIO | `firmware/config/boards/proteus_meta.h:77` | DEFAULT |

Nota: com `HARDWARE_CI` definido (apenas builds de CI), PG3/PG2 são
emprestados como trigger simulator (`board_configuration.cpp:183-186`).

### B.3 Saídas highside HS1–HS4

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| HS1 | PA9 | GPIO (TIM1_CH2 possível) | `firmware/config/boards/proteus_meta.h:52` | DEFAULT |
| HS2 | PA8 | GPIO (TIM1_CH1 possível) | `firmware/config/boards/proteus_meta.h:55` | DEFAULT |
| HS3 | PD15 | GPIO (TIM4_CH4 possível) | `firmware/config/boards/proteus_meta.h:57` | DEFAULT |
| HS4 | PD14 | GPIO (TIM4_CH3 possível) | `firmware/config/boards/proteus_meta.h:59` | DEFAULT |

PA9 só está livre para HS1 porque o sensing de VBUS do USB OTG está
desabilitado (`BOARD_OTG_NOVBUSSENS TRUE`,
`firmware/hw_layer/ports/stm32/stm32f4/board.h:33-34`). **A placa nova NÃO
deve ligar VBUS em PA9.**

### B.4 ETB1/ETB2 (driver TLE9201: PWM, DIR, DIS)

Defaults em `board_configuration.cpp:77-102`. O pino PWM usa hardware PWM se o
pino tiver timer mapeado, senão cai para PWM por software
(`firmware/controllers/system/timer/pwm_generator_logic.cpp:374-386`). PD12 e
PD13 têm mapeamento de hardware → TIM4 (AF2)
(`firmware/hw_layer/ports/stm32/stm32_pwm.cpp:121-131`; TIM4 habilitado em
`firmware/hw_layer/ports/stm32/mcuconf_common_f4_f7.h:223-225`). DIR e DIS são
GPIO simples (`firmware/controllers/actuators/dc_motors.cpp:30-32,66-67`).
Modo: `etb_use_two_wires = false` (PWM+DIR, `board_configuration.cpp:101`).

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| ETB1 PWM | PD12 | TIM4_CH1 (AF2), fallback SW-PWM | `board_configuration.cpp:86`; `stm32_pwm.cpp:127` | DEFAULT |
| ETB1 DIR | PD10 | GPIO | `board_configuration.cpp:88` | DEFAULT |
| ETB1 DIS | PD11 | GPIO | `board_configuration.cpp:90` | DEFAULT |
| ETB2 PWM | PD13 | TIM4_CH2 (AF2), fallback SW-PWM | `board_configuration.cpp:94`; `stm32_pwm.cpp:128` | DEFAULT |
| ETB2 DIR | PD9 | GPIO | `board_configuration.cpp:96` | DEFAULT |
| ETB2 DIS | PD8 | GPIO | `board_configuration.cpp:98` | DEFAULT |

PD8–PD13 também são expostos como saídas nomeáveis no TS
(`firmware/config/boards/proteus/connectors/black23.yaml`, entradas
`pin: PD8`…`pin: PD13`).

### B.5 Entradas analógicas AV1–AV11

Mapeamento canal ADC ↔ pino é restrição de silício
(`firmware/controllers/algo/rusefi_hw_adc_enums.h:4-19`;
`firmware/hw_layer/ports/stm32/stm32_adc.cpp:19-44`). Funções default do board
em `board_configuration.cpp:104-121`. Nome lógico ↔ canal em
`firmware/config/boards/proteus_meta.h:93-134` e
`connectors/white35.yaml`. ADC1/ADC2 habilitados em
`mcuconf_common_f4_f7.h:26-32`.

| Função | Pino STM32 | Periférico | Default do firmware | Evidência | Criticidade |
|---|---|---|---|---|---|
| AV1 | PC0 | ADC123_IN10 (`EFI_ADC_10`) | MAP | `proteus_meta.h:93-95`; `board_configuration.cpp:116` | DEFAULT + SILÍCIO |
| AV2 | PC1 | ADC123_IN11 (`EFI_ADC_11`) | TPS1_1 | `proteus_meta.h:97-99`; `board_configuration.cpp:115` | DEFAULT + SILÍCIO |
| AV3 | PC2 | ADC123_IN12 (`EFI_ADC_12`) | (TPS1_2 sugerido) | `proteus_meta.h:101-103` | DEFAULT + SILÍCIO |
| AV4 | PC3 | ADC123_IN13 (`EFI_ADC_13`) | (TPS2_1 sugerido) | `proteus_meta.h:105-107` | DEFAULT + SILÍCIO |
| AV5 | PA0 | ADC123_IN0 (`EFI_ADC_0`) | — | `proteus_meta.h:109-110` | DEFAULT + SILÍCIO |
| AV6 | PA1 | ADC123_IN1 (`EFI_ADC_1`) | — (uso digital alternativo `Gpio::A1`) | `proteus_meta.h:112-114` | DEFAULT + SILÍCIO |
| AV7 | PA2 | ADC123_IN2 (`EFI_ADC_2`) | (PPS2 sugerido) | `proteus_meta.h:116-118` | DEFAULT + SILÍCIO |
| AV8 | PA3 | ADC123_IN3 (`EFI_ADC_3`) | — (uso digital alternativo `Gpio::A3`) | `proteus_meta.h:120-122` | DEFAULT + SILÍCIO |
| AV9 | PA4 | ADC12_IN4 (`EFI_ADC_4`) | (PPS1 sugerido) | `proteus_meta.h:124-126`; `stm32_adc.cpp:23` | DEFAULT + SILÍCIO |
| AV10 | PA5 | ADC12_IN5 (`EFI_ADC_5`) | AFR/WBO analógico | `proteus_meta.h:128-130`; `board_configuration.cpp:118-120` | DEFAULT + SILÍCIO |
| AV11 | PA6 | ADC12_IN6 (`EFI_ADC_6`) | — (uso digital alternativo `Gpio::A6`) | `proteus_meta.h:132-134` | DEFAULT + SILÍCIO |

Atenção: PA4/PA5/PA6/PA7 e PB0/PB1, PC4/PC5 **não** chegam ao ADC3
(`stm32_adc.cpp:23-35` — bitmask sem `BIT(2)`); apenas ADC1/ADC2. Irrelevante
para uso normal (fast/slow ADC usam ADC1/2), mas relevante se algum dia se
quiser mover knock.

### B.6 Entradas de temperatura AT1–AT4

| Função | Pino STM32 | Periférico | Default do firmware | Evidência | Criticidade |
|---|---|---|---|---|---|
| AT1 | PC4 | ADC12_IN14 (`EFI_ADC_14`) | — | `proteus_meta.h:79-80` | DEFAULT + SILÍCIO |
| AT2 | PC5 | ADC12_IN15 (`EFI_ADC_15`) | IAT | `proteus_meta.h:82-84`; `board_configuration.cpp:114` | DEFAULT + SILÍCIO |
| AT3 | PB0 | ADC12_IN8 (`EFI_ADC_8`) | CLT | `proteus_meta.h:86-88`; `board_configuration.cpp:113` | DEFAULT + SILÍCIO |
| AT4 | PB1 | ADC12_IN9 (`EFI_ADC_9`) | — | `proteus_meta.h:90-91` | DEFAULT + SILÍCIO |

### B.7 Triggers digitais HALL/Digital 1–6 e VR 1–2

Captura por **EXTI/PAL, ambas as bordas** — não há exigência de canal de timer
(`HAL_TRIGGER_USE_PAL TRUE` em
`firmware/config/stm32f4ems/efifeatures.h:139-141`;
`firmware/hw_layer/digital_input/trigger/trigger_input_exti.cpp:60`).
Restrição EXTI do STM32: apenas UM pino por número de linha (ex.: C6 ocupa a
linha 6 — nenhum outro Px6 pode gerar interrupção simultânea).
Trigger primário default = Digital 1 (PC6), demais não atribuídos
(`board_configuration.cpp:104-110`).

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| Digital 1 (trigger primário default) | PC6 | GPIO + EXTI6 | `proteus_meta.h:139`; `board_configuration.cpp:107` | DEFAULT |
| Digital 2 (CAM sugerido) | PE11 | GPIO + EXTI11 | `proteus_meta.h:140`; `connectors/black23.yaml:2-8` | DEFAULT |
| Digital 3 (VSS sugerido) | PE12 | GPIO + EXTI12 | `proteus_meta.h:141`; `connectors/black23.yaml:10-16` | DEFAULT |
| Digital 4 | PE14 | GPIO + EXTI14 | `proteus_meta.h:142`; `connectors/black23.yaml:18-24` | DEFAULT |
| Digital 5 | PE13 | GPIO + EXTI13 | `proteus_meta.h:143`; `connectors/black23.yaml:60-66` | DEFAULT |
| Digital 6 | PE15 | GPIO + EXTI15 | `proteus_meta.h:144`; `connectors/black23.yaml:75-81` | DEFAULT |
| VR1 (saída do condicionador VR, canal 1) | PE7 | GPIO + EXTI7 | `proteus_meta.h:136`; `connectors/black23.yaml:40-42` | DEFAULT |
| VR2 (saída do condicionador VR, canal 2) | PE8 | GPIO + EXTI8 | `proteus_meta.h:137`; `connectors/black23.yaml:31-33` | DEFAULT |

Nota: adaptadores Proteus com decodificador VR discreto são alimentados por
saída de ignição de 5 V (`VR_SUPPLY_VOLTAGE=5`,
`firmware/config/boards/proteus/board.mk:20-21`).

### B.8 Knock 1–2

Hardcoded em tempo de compilação — **a placa nova DEVE usar PF4/PF5**.
Software knock habilitado por default
(`board_configuration.cpp:174`; `board.mk:24-27` define
`STM32_ADC_USE_ADC3=TRUE` e `EFI_SOFTWARE_KNOCK=TRUE`).

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| Knock 1 | PF4 | ADC3_IN14 (`KNOCK_ADC=ADCD3`, `KNOCK_ADC_CH1=ADC_CHANNEL_IN14`) | `firmware/config/boards/proteus/knock_config.h:8-11,27-28`; `stm32_adc.cpp:40` | BOOT + SILÍCIO |
| Knock 2 | PF5 | ADC3_IN15 (`KNOCK_ADC_CH2=ADC_CHANNEL_IN15`) | `firmware/config/boards/proteus/knock_config.h:13-15,29`; `stm32_adc.cpp:42` | BOOT + SILÍCIO |

### B.9 CAN1 e CAN2

Periféricos bxCAN habilitados em `mcuconf_common_f4_f7.h:52-58`.
**Atenção dupla-natureza do CAN1:** os pinos são default de configuração no
firmware principal (`board_configuration.cpp:148-149`), mas são **hardcoded no
bootloader OpenBLT** (`board.mk:37-42`) e no wake-up de low-power
(`board_configuration.cpp:189-192`, PD0 fixo). Logo, para manter
reflash-por-CAN e wake, **PD0/PD1 são imutáveis na prática**.

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| CAN1 RX | PD0 | CAN1_RX (AF9) | `board_configuration.cpp:149`; OpenBLT: `board.mk:39-40`; wake: `board_configuration.cpp:191` | BOOT |
| CAN1 TX | PD1 | CAN1_TX (AF9) | `board_configuration.cpp:148`; OpenBLT: `board.mk:41-42` | BOOT |
| CAN2 RX | PB12 | CAN2_RX (AF9) | `board_configuration.cpp:150` | DEFAULT |
| CAN2 TX | PB13 | CAN2_TX (AF9) | `board_configuration.cpp:151` | DEFAULT |

### B.10 USB, SWD, BOOT0, NRST

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| USB D− | PA11 | OTG_FS_DM (AF10) | `firmware/hw_layer/ports/stm32/stm32f4/board.h:29-31`; `firmware/hw_layer/ports/stm32/serial_over_usb/usbconsole.cpp:29-30` | BOOT |
| USB D+ | PA12 | OTG_FS_DP (AF10) | idem | BOOT |
| SWDIO | PA13 | SYS (AF0), modo alternate por default | `firmware/hw_layer/ports/stm32/stm32f4/board.h:182-199` | BOOT (recuperação/debug) |
| SWCLK | PA14 | SYS (AF0), modo alternate por default | idem | BOOT (recuperação/debug) |
| BOOT0 | BOOT0 (pino dedicado) | strap de boot ST | pino de silício; sem referência de firmware (ver NÃO RESOLVIDO) | HW |
| NRST | NRST (pino dedicado) | reset | pino de silício; sem referência de firmware | HW |
| VBUS sensing | **não usar PA9** | desabilitado (`BOARD_OTG_NOVBUSSENS TRUE`) | `firmware/hw_layer/ports/stm32/stm32f4/board.h:33-34` | BOOT |

### B.11 12V_SENSE (VBatt)

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| 12V_SENSE / VBatt | PA7 | ADC12_IN7 (`EFI_ADC_7`) | `board_configuration.cpp:71-72`; `connectors/black23.yaml:117-122` (pino 18 do Black23) | DEFAULT + SILÍCIO |

Divisor esperado: ver [seção E](#e-expectativas-de-hardware-analógico).

### B.12 SD/SPI, UART, AUX_SPI, I2C do baro, LEDs, 5V_SENSOR_PG

| Função | Pino STM32 | Periférico | Evidência | Criticidade |
|---|---|---|---|---|
| SD SCK | PC10 | SPI3_SCK (AF6) | `board_configuration.cpp:128` | DEFAULT |
| SD MISO | PC11 | SPI3_MISO (AF6) | `board_configuration.cpp:129` | DEFAULT |
| SD MOSI | PC12 | SPI3_MOSI (AF6) | `board_configuration.cpp:130` | DEFAULT |
| SD CS | PD2 | GPIO | `board_configuration.cpp:125` | DEFAULT |
| SD habilitado por default | — | `isSdCardEnabled=true`, `sdCardSpiDevice=SPI_DEVICE_3` | `board_configuration.cpp:124,169` | DEFAULT |
| Console UART TX | PD5 | USART2_TX (AF7) — **somente no build F4** | `board.mk:29-35` | BOOT (F4) |
| Console UART RX | PD6 | USART2_RX (AF7) — somente F4 | `board.mk:29-35` | BOOT (F4) |
| AUX SPI5 SCK | PF7 | SPI5_SCK — **somente F7** (`#ifndef STM32F4XX`) | `board_configuration.cpp:132-138`; `board.mk:55` | DEFAULT (F7) — reservar na placa |
| AUX SPI5 MISO | PF8 | SPI5_MISO — somente F7 | idem | DEFAULT (F7) — reservar |
| AUX SPI5 MOSI | PF9 | SPI5_MOSI — somente F7 | idem | DEFAULT (F7) — reservar |
| Baro LPS25 SCL | PB10 | I2C2_SCL | `board_configuration.cpp:153` | DEFAULT |
| Baro LPS25 SDA | PB11 | I2C2_SDA | `board_configuration.cpp:154` | DEFAULT |
| LED erro crítico | PE3 | GPIO | `board.mk:8-9` (`LED_CRITICAL_ERROR_BRAIN_PIN=Gpio::E3`) | BOOT |
| LED comms (TS) | PE4 | GPIO | `board_configuration.cpp:52-54` | BOOT |
| LED running | PE5 | GPIO | `board_configuration.cpp:56-58` | BOOT |
| LED warning | PE6 | GPIO | `board_configuration.cpp:60-62` | BOOT |
| 5V_SENSOR_PG | — | **sem referência no firmware** | ver NÃO RESOLVIDO | — |

---

## D. Pinos do STM32 NÃO usados pelo firmware (candidatos a expansão)

Universo: STM32F427ZGT6, LQFP144 = 114 GPIOs (PA0–15, PB0–15, PC0–15, PD0–15,
PE0–15, PF0–15, PG0–15, PH0–PH1). Usados pelo contrato acima (build F4):
86 GPIOs. **Livres: 28**, com ressalvas anotadas:

| Pino | Ressalva |
|---|---|
| PA10 | Livre. TIM1_CH3 e USART1_RX possíveis (`stm32_pwm.cpp:100`). |
| PA15 | Livre. JTDI (irrelevante com SWD); TIM2_CH1 possível (`stm32_pwm.cpp:107`). |
| PB2 | Livre, **mas é BOOT1** — manter strap para GND no hardware. |
| PB3 | Livre. SWO/JTDO — recomendável reservar para trace de debug; TIM2_CH2 possível (`stm32_pwm.cpp:108`). |
| PB14 | Livre. |
| PB15 | Livre. |
| PC13 | Livre (só corrente baixa — domínio VBAT). |
| PC14 | Livre — **OSC32_IN**; LSE "not fitted" e `EFI_RTC=FALSE` no F4 (`stm32f4/board.h:55-58`; `meta-info-proteus_f4.env:5`). Reservar se quiser RTC futuro. |
| PC15 | Livre — OSC32_OUT, idem PC14. |
| PE9 | Livre. TIM1_CH1 possível (`stm32_pwm.cpp:102`). |
| PE10 | Livre. |
| PF0 | Livre (I2C2_SDA alternativo). |
| PF1 | Livre (I2C2_SCL alternativo). |
| PF2 | Livre. |
| PF3 | Livre — **ADC3_IN9** (`EFI_ADC_37`, `stm32_adc.cpp:34`): ótimo para entrada analógica extra. |
| PF6 | Livre — **ADC3_IN4** (`EFI_ADC_32`, `stm32_adc.cpp:24`). |
| PF7 | Livre no F4, **reservado**: SPI5_SCK no build F7 (`board_configuration.cpp:135`). Também ADC3_IN5. |
| PF8 | Livre no F4, **reservado**: SPI5_MISO no F7. Também ADC3_IN6. |
| PF9 | Livre no F4, **reservado**: SPI5_MOSI no F7. Também ADC3_IN7. |
| PF10 | Livre — **ADC3_IN8** (`EFI_ADC_36`, `stm32_adc.cpp:32`). |
| PF11 | Livre. |
| PF12 | Livre. |
| PF13 | Livre. |
| PF14 | Livre. |
| PF15 | Livre. |
| PG0 | Livre. |
| PG1 | Livre. |
| PG15 | Livre. |

(PH0/PH1 = cristal HSE, contabilizados como usados.)

---

## E. Expectativas de hardware analógico

| Item | Valor esperado pelo firmware | Evidência |
|---|---|---|
| Tensão de referência/alimentação do ADC | **3,3 V** (`adcVcc = 3.3f`) | `board_configuration.cpp:74` |
| Divisor das entradas AV/AT (`analogInputDividerCoefficient`) | **1,56** — 5,6 kΩ (high side) / 10 kΩ (low side): (5,6+10)/10 | `board_configuration.cpp:64-66` |
| Divisor do VBatt (`vbattDividerCoeff`) | **9,2** — 82 kΩ (high side) / 10 kΩ (low side): (82+10)/10 | `board_configuration.cpp:68-69` |
| Pullup de temperatura (CLT/IAT, `bias_resistor`) | **2 700 Ω** (`PROTEUS_DEFAULT_AT_PULLUP`) | `proteus_meta.h:13`; `board_configuration.cpp:145-146` |
| Tensão do pullup de termistor assumida no cálculo | **5,0 V** | `firmware/init/sensor/init_thermistors.cpp:48` |
| Fundo de escala resultante das AV | 3,3 V × 1,56 ≈ **5,15 V** no conector | derivado das linhas acima |
| Fundo de escala do VBatt | 3,3 V × 9,2 ≈ **30,4 V** no conector | derivado |
| Knock | entrada AC acoplada centrada para ADC3 em PF4/PF5; taxa de amostragem `STM32_PCLK2/(4·96)` | `knock_config.h:30-31` |
| Cristal HSE | valor **inteiro em MHz** (PLLM=N); qualquer valor dentro da faixa do F4 funciona — auto-detecção | `osc_detector.cpp:13-15`; `mcuconf_common_f4_f7.h:439-450` |

---

## Resumo executivo para o projeto Proteus-BR

1. **Imutáveis (recompilação proibida nesta etapa):** PF4/PF5 (knock),
   PD0/PD1 (CAN1 + OpenBLT + wake), PA11/PA12 (USB), PA13/PA14 (SWD),
   PE3/PE4/PE5/PE6 (LEDs), PD5/PD6 (console UART F4), cristal em PH0/PH1,
   BOOT0/NRST, VBUS fora de PA9.
2. **Defaults que a placa deve copiar 1:1** para funcionar sem retune:
   todas as tabelas B.1–B.7, B.9 (CAN2), B.11 e B.12.
3. **Expansão em etapas:** popular menos canais NÃO muda o contrato — pinos
   de canais não populados ficam flutuando no driver, mas o roteamento
   MCU→footprint deve existir desde a v1.

---

## NÃO RESOLVIDO / PERGUNTAS

1. **`5V_SENSOR_PG` (power-good do regulador 5 V dos sensores):** nenhuma
   referência em `firmware/config/boards/proteus/` nem no restante de
   `firmware/` (busca por `PG`/`pgood`/`5V` em código de board). Conclusão:
   é uma rede apenas de hardware no projeto KiCad do Proteus, invisível ao
   firmware. Confirmar no repo `./proteus` (fora deste repo) qual pino/rede é
   e decidir se a Proteus-BR mantém. **Nenhum pino do MCU é consumido por
   isso segundo o firmware.**
2. **Acelerômetro onboard:** `ts_show_onboard_accelerometer true` em
   `firmware/config/boards/proteus/prepend.txt:24-25`, mas o próprio comentário
   diz "just for a test does not mean EFI_ONBOARD_MEMS" e nenhum pino SPI/CS
   de acelerômetro é atribuído na board config. Tratar como inexistente no
   contrato.
3. **Valor do cristal HSE da placa original (25 MHz?):** o firmware
   auto-detecta e portanto não documenta o valor; a escolha (8/16/25 MHz,
   inteiro) é livre para a Proteus-BR. Confirmar no KiCad do Proteus apenas
   por fidelidade histórica.
4. **BOOT0/NRST:** sem evidência em firmware (são pinos de silício). Strap
   BOOT0→GND via resistor e acesso a NRST no conector de debug são prática
   obrigatória de hardware, não exigência rastreável neste repo.
5. **EGT/MAX31855 e SPI5:** habilitados apenas no build F7
   (`board.mk:53-55`). Se a Proteus-BR um dia migrar para F7, PF7/PF8/PF9
   (SPI5) passam a ser usados — por isso constam como "reservados" na lista
   de livres. CS de EGT não tem default na board config (não resolvido por
   falta de evidência).
6. **Pinos sugeridos mas não atribuídos por default** (TPS1_2=PC2,
   TPS2_1=PC3, PPS=PA4, PPS2=PA2): os aliases existem em
   `proteus_meta.h:101-126`, porém `board_configuration.cpp` não os grava na
   configuração default. São convenção de documentação/presets de veículo,
   não contrato de boot.
