# Work Report

## 2026-08-09 - m74_9 watchdog round 2: extended TIM5 snapshot with RCC gate + 100 ms delayed re-read

What was done:
- User report of the instrumented firmware: `cnt=80159147..80159147 dier=0x2 sr=0x0 cr1=0x485`
  at 4 MHz -> freeze at ~20 s after boot. cr1=0x485 = CEN+URS+ARPE+PMEN (exactly what
  pwm_lld_start writes on AT32), DIER bit 1 (CC1IE) armed, no pending compare, isr=28523 /
  setHw=28525 -> the ISR->reschedule chain ran perfectly until the freeze. Conclusion: timer
  registers are intact, TIM5 counter stops counting at runtime - classic signature of the
  peripheral clock being gated (RCC APB1ENR.TIM5EN cleared, e.g. by stray
  pwmStop()/gptStop()/icuStop() on PWMD5/GPTD5/ICUD5 -> rccDisableTIM5 in the TIMv1 LLD), or a
  deeper AT32 clock issue. The two identical adjacent CNT reads are only weak proof (a few
  cycles can elapse between reads)
- Extended the diagnostic in microsecond_timer.cpp MicrosecondTimerWatchdogController::PeriodicTask:
  - re-read CNT after chThdSleepMilliseconds(100) (thread context, no locks held) - prints
    cnt2 and the delta cnt2-cnt1, so cnt2==cnt1 after 100 ms now PROVES a frozen counter
  - added RCC APB1 clock-enable snapshot: apb1enr + tim5en bit (RCC->APB1ENR + RCC_APB1ENR_TIM5EN
    on AT32F4XX/STM32F4XX/STM32F7XX, RCC->APB1LENR + RCC_APB1LENR_TIM5EN on STM32H7XX - the H7
    register name differs; verified all four families' CMSIS headers). Zeroed default keeps the
    message safe if a future family defines SCHEDULER_TIMER_DEVICE without matching the #if
  - added SMCR/PSC/ARR/CCR1/CCMR1 so a stale compare window or slave-mode gate is visible
  - cypress/kinetis GPT port still gets the old message via the #else branch
- Clock-gate culprit sweep (rusEFI tree, outside ChibiOS): no pwmStop() callers at all; the
  only stop call is icuStop() in sent_hw_icu.cpp (SENT input, but STM32_ICU_USE_TIM5=FALSE);
  HAL_USE_ONEWIRE=FALSE on AT32 so ChibiOS-Contrib hal_onewire.c pwmStop(pwmd) is not built;
  direct RCC-> writes in-tree touch only AHB1ENR (USB disable at boot, stm32_common.cpp),
  APB2ENR (osc_detector.cpp, H7-only) and BDCR (F7 RTC) - none can clear TIM5EN at runtime

Validation:
- Edit is type-checked by hand (%d/%u/%x specifiers match the casted arguments, including the
  new (unsigned)(cnt2-cnt1) and (unsigned)tim5en); no firmware build possible on the macOS
  host (no arm-none-eabi toolchain) - user builds m74_9 on Windows

Follow-ups:
- User: rebuild m74_9 on Windows, flash, reproduce (~20 s), paste the new error text; also
  capture the last console lines right before the error (any Storage/burn/MFS/CAN-reconnect
  message, any `local freeze cnt=`)
- Reading apb1enr/tim5en: bit 3 (0x8) clear -> someone gated TIM5 clock -> correlate ~20 s
  wall-time with the log to find the culprit; bit still set but cnt2==cnt1 after 100 ms ->
  deeper AT32 clock issue, next print RCC->CR (PLLON/PLLRDY) and RCC->CFGR (SW) and consider
  sampling CNT every 1 s from status_loop to find the exact freeze moment
- cnt2 != cnt1 but watchdog still latched -> the freeze was transient; re-think executor/queue
  and the hasFirmwareError() early-return interplay

## 2026-08-09 - m74_9 watchdog critical: instrument MstWatchdog with TIM5 register snapshot

What was done:
- m74_9 (AT32F435, CAN-only) runs ~10-20 s after boot, then latches
  `CRITICAL error: Watchdog: no events for 2 seconds!` (firmwareError RUNTIME_CRITICAL_TIMER_WATCHDOG)
- The MstWatchdog thread (only started under EFI_EMULATE_POSITION_SENSORS, inherited TRUE from
  stm32f4ems/efifeatures.h) fires when setHardwareSchedulerTimer() has not updated
  lastSetTimerTimeNt for 2 s. The watchDogBuddy 1 s self-rescheduling event keeps that chain
  alive, so a trip means the TIM5 compare-interrupt -> executor loop died at runtime. Console
  CAN and status_loop keep running after the error, so it is NOT a global IRQ stall - something
  specific to the TIM5 CC1 path on the AT32 port
- Static verification of the chain: PWM driver is the STM32 TIMv1 LLD compiled for AT32
  (AT32 platform.mk reuses STM32/LLD/TIMv1/driver.mk), STM32_TIM5_HANDLER=Vector108 (IRQ 50,
  vector table offset 0x108), STM32_PWM_TIM5_IRQ_PRIORITY=4 from the new at32 interrupt_priority.h,
  AT32_TIM_CR1_PMEN 32-bit mode set in pwm_lld_start. Nothing else on this board uses PWMD5/TIM5
  (A0-A3 -> PWMD5 mapping in stm32_pwm.cpp is unused by the m74_9 default config)
- Added diagnostics to firmware/hw_layer/microsecond_timer/microsecond_timer.cpp
  MicrosecondTimerWatchdogController::PeriodicTask: the error message now snapshots
  timerCallbackCounter (ISR count), setHwTimerCounter, timerFreezeCounter, isTimerPending and
  raw TIM5 CNT x2 / DIER / SR / CR1 at the moment of detection. Distinguishes timer clock dead
  (CNT frozen), compare IRQ dead (CNT counting, DIER bit 1 / CC1IE cleared), notification never
  re-armed (pending=0) vs executor/queue wedged (counters still growing). Guarded by
  #ifdef SCHEDULER_TIMER_DEVICE so the cypress/kinetis GPT port keeps the old message

Validation:
- Edit is type-checked by hand (%d/%u/%x specifiers match the casted arguments); no firmware
  build possible on the macOS host (no arm-none-eabi toolchain) - user builds m74_9 on Windows

Follow-ups:
- User: rebuild m74_9 on Windows, flash, reproduce (~20 s), capture the new watchdog error text
  with isr=/setHw=/freeze=/pending=/cnt=/dier=/sr=/cr1= values
- Reading the values: CNT frozen -> TIM5 clock stopped; DIER without CC1IE (bit 1) -> compare
  interrupt never re-armed; counters still growing -> executor/queue issue, not the timer

## 2026-08-09 - PCAN ISO-TP decode root cause: decodePacket got the 127-byte buffer instead of the DLC

What was done:
- Fixed java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java readOnePacket:
  `canDecoder.decodePacket(rx.getData())` -> `decodePacket(rx.getData(), rx.getLength())`
- Root cause of the m74_9 (AT32F435, CAN-only, no USB) "No signature returned by
  PCanIoStream{PCAN_USBBUS1}" blocker: TPCANMsg(Byte.MAX_VALUE) allocates a 127-byte data
  buffer (workaround for issue #4370) and the PCAN driver fills only rx.getLength() (DLC=8)
  bytes; the deprecated decodePacket(byte[]) overload passed data.length (127) instead, so the
  ISO-TP multi-frame state machine and the CRC check read garbage zero padding. Symptom seen in
  the log: `CRC mismatch on recv packet for [hello]: got 309f7bb9 but expected 0` (CRC field all
  zeros) even though the 0x720 frame itself was delivered fine (`isotp rate 1`). The size-aware
  overload exists exactly for this - its javadoc names PCAN as the larger-data-buffer example
- Checked the other decoders: SocketCANHelper.read allocates `new byte[rx.getDataLength()]` so
  SocketCANIoStream.payload() is exact-length and unaffected; Elm327IoStream gets real-length
  arrays from the ELM327 chip. Only the PCAN path was broken

Validation:
- ./gradlew :ecu_io:test BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...)

Follow-ups:
- User rebuilds java_console on Windows and connects to m74_9 over PCAN-USB: expect
  `Got [rusEFI ...] signature` / Connection established instead of the CRC-mismatch loop

## 2026-08-09 - PCanIoStream log flood fix (INFO_SKIP_RATE typo)

What was done:
- Fixed INFO_SKIP_RATE typo in java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java:
  `3-00` evaluated to 3 (three minus zero), so the "Skipping non 720 packet" log fired every
  3rd skipped frame - at ~3600 pps on the m74_9 test bus that is ~1200 lines/sec of noise,
  making the PCAN debug log unreadable. Now 300 (log every 300th skipped frame)

Validation:
- ./gradlew :ecu_io:test BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...)

Follow-ups:
- PCAN ISO-TP diagnosis continues: need "Skipping non 720 packet:" ID lines or PCAN-View
  capture to tell whether the m74_9 ECU announces 0x770017 (extended) on the probed bus

## 2026-08-09 - java_console PCAN detection fix + merge conflict resolution

What was done:
- Fixed java_console not detecting PCAN adapters on Windows: the PowerShell WMI query
  (`powershell -NoProfile -Command "Get-CimInstance ... -Filter \"Caption like '%PCAN-USB%'\" ..."`)
  was executed via Runtime.exec(String), whose tokenizer splits on whitespace and keeps quotes as
  literal characters, so the script arrived shredded and the query silently matched nothing
  (PCANConnected always false -> "OpenBLT CAN" menu item missing)
- Added ExecHelper.executeCommand(List<String>, ...) overload that runs commands through
  ProcessBuilder with an explicit argument list (platform-correct quoting, no shell tokenization)
- Converted all PowerShell hardware probes to the list form: PCAN (MaintenanceUtil), DFU
  (DfuFlasher, both H7 and F4 variants), ST-Link (StLinkFlasher), plus DfuFlasher.getDevicesReport
- Added MaintenanceUtilTest.pcanQueryIsSingleIntactPowerShellScriptArgument regression test
- Resolved the remaining merge conflicts (HEAD vs 075e600f): can_sniffer.cpp kept HEAD guard
  structure + theirs' explicit template instantiations; VariableRegistryValues.java took HEAD values

Key decisions:
- Root-cause fix at ExecHelper instead of patching the query string: the same broken
  Runtime.exec(String) quoting pattern was shared by PCAN/DFU/ST-Link detection
- Used Arrays.asList (not List.of): java_console still compiles with sourceCompatibility 8
- Generated VariableRegistryValues.java resolves to HEAD side; regenerated at next build anyway

Validation:
- Empirical check on macOS: a fake powershell argv dump showed Runtime.exec(String) splits the
  script into 15 broken tokens, while ProcessBuilder passes it as one intact argument
- ./gradlew :ui:test --tests MaintenanceUtilTest: BUILD SUCCESSFUL, 14 tests passed (JDK 11
  toolchain via -Porg.gradle.java.installations.paths=/opt/homebrew/opt/openjdk@11/...)

Follow-ups:
- WMIC_* constant names now hold PowerShell arg lists; renaming to PS_* would be cosmetic

## 2026-07-29 - m74_9 (AT32F435) firmware build fixes

What was done:
- Fixed build of m74_9 board (previously disabled, meta-info.disabled_env -> meta-info.env)
- Fixed gcc_version_check.c to support GCC 16 (limit 1700)
- Created AT32 interrupt_priority.h with EFI_IRQ_* priority macros
- Fixed SPI v1 struct field names (end_cb instead of slave/data_cb/error_cb) used by old AT32 port
- Fixed usbcfg.h to guard USB type declarations with #if HAL_USE_USB
- Disabled EFI_STORAGE_INT_FLASH for m74_9 (no flash storage driver for AT32)
- Added stub implementations for assertInterruptPriority, getFlashAddr*, printWRPBits, etc. in AT32 port
- Wrapped can_sniffer in #if EFI_USB_SERIAL

Build result:
- Output files: build/rusefi.bin (632516 bytes), build/rusefi.srec
- text: 389516, data: 1025, rodata: 219860, bss: 135901
- Compiler: arm-none-eabi-gcc 16.1.1

Key decisions:
- Used old AT32 port (ChibiOS/AT32F4xx) instead of ChibiOS-Contrib AT32F435_437 to minimize risk
- Corresponds to SPIv1 driver (no slave/data_cb/error_cb fields, only end_cb)

Follow-ups:
- Should consider migrating to ChibiOS-Contrib AT32F435_437 port for proper SPIv2 support
- AT32 flash driver not implemented (EFI_STORAGE_INT_FLASH=FALSE)
- USB is physically absent on m74_9, confirmed via board.mk

## 2026-07-14 - Investigation: "Malformed Packet: packet length" in USB.pcapng

What was done:
- Analyzed USB.pcapng (USBPcap capture of the rusEFI ECU USB link, device address 7)
  using tshark to explain the "malformed packet" warnings.

Findings:
- Protocol hierarchy: 84926 frames, mostly USB mass storage (usbms/scsi) plus 2 CDC
  (usbcom) control frames. 23 SCSI frames + 1 URB frame flagged _ws.malformed.
- The 23 usbms malformed frames are ALL the same case: the device reply to
  SCSI Mode Sense(6) (opcode 0x1a). Wireshark message: "SCSI: length of contained
  item exceeds length of containing item".
- Root cause is a Wireshark dissector strictness issue, NOT bad wire data. The 16-byte
  reply is self-consistent:
      0f 00 00 00  08 0a 00 00 00 00 00 00 00 00 00 00
  Mode Data Length=15 (=total-1), Caching mode page (0x08) with PageLength=0x0a=10.
  SBC-2 mandates the Caching page be 0x12=18 long (20-byte page); rusEFI emits a valid
  but non-standard SHORT caching page. Wireshark decodes the full 20-byte layout, reads
  past the 16-byte buffer, and raises the exception. Windows accepts it -> device works.
- Response is hardcoded in ChibiOS-Contrib USB-MSD SCSI target (os/hal/src/hal_usb_msd.c,
  submodule not checked out locally), driven by
  firmware/hw_layer/mass_storage/mass_storage_device.cpp.
- Frames 27759-27772 "USBPcap did not recognize URB Function code" are a USBPcap capture-
  driver limitation, unrelated to rusEFI traffic.

Decisions:
- Classified as cosmetic; no code change made. Not a functional defect.

Validation:
- tshark -z io,phs, per-frame -V dissection, and raw -x hex confirmed the byte layout
  and that all 23 malformed frames share the Mode Sense(6) cause.

Open follow-ups:
- Optional cleanup if the warning ever matters: set the Caching mode page length to 0x12
  and pad the page to the full 20 bytes, or drop the caching page from the Mode Sense
  reply. Lives upstream in ChibiOS-Contrib hal_usb_msd.c.

## 2026-07-14 - SD ECU<->PC switch soak sandbox + USB CDC link-drop investigation

What was done:
- Created a headless soak sandbox SdEcuPcCycleSandbox in the :ui test subproject
  (java_console/ui/src/test/java/com/rusefi/SdEcuPcCycleSandbox.java), modeled on the
  purple-gateway SdPcToEcuSwitchSandbox. It cycles the SD card ECU/logging <-> PC/MSD 10
  times, 20s dwell per mode, confirming each switch via the sd_present / sd_logging_internal
  / sd_msd output channels, and reports a pass/fail tally. Added Gradle task :ui:runSdCycle.
- Initialized two uninitialized git submodules required by the Java build:
  java_console/peak-can-basic (missing peak.can.basic.* -> :ecu_io compile fail) and
  java_console/luaformatter (missing neoe.formatter.lua -> :ui compile fail).

Result of the run (COM149, purple-gateway fw, USB-powered only / no +12V):
- Cycle 1 fully succeeded BOTH directions. PC/MSD->ECU no longer hits FR_DISK_ERR: firmware
  logged "SD: switched from PC/MSD to ECU/logging" and opened log file re_10.mlg; status bits
  confirmed sd_logging_internal=1.
- ~1.5s after the ECU switch the host CDC serial link dropped:
  "output channels: executeCommand failed: java.io.IOException: write failed: wrote 0 but
  expected 11", COM149 closed. Never recovered, so cycle 2's first command got no response and
  the soak aborted at 1/20. Sandbox behaved correctly - it detected and reported the drop.

Root cause (investigated, code-evidenced):
- CDC console and USB mass storage are interfaces on ONE composite USB device (USBD1). The
  config descriptor is fixed at 3 interfaces - MSD IF0 + CDC-control IF1 + CDC-data IF2, 98
  bytes (usbcfg.cpp DESCRIPTOR_SIZE/NUM_INTERFACES). MSD is always present in the enumerated
  descriptor whenever HAL_USE_USB_MSD is built in.
- The SD mode switch does NOT re-enumerate or reconfigure USB. attachMsdSdCard /
  deattachMsdSdCard (mass_storage_init.cpp) merely hot-swap LUN1's backing block device
  between the real SD card and the null device ND1 on the already-running MSD controller.
- Causal chain: PC/MSD->ECU calls deattachMsdSdCard() which swaps LUN1 (SD card -> ND1) while
  Windows still has that mass-storage volume mounted -> the medium vanishes under the mounted
  volume -> the Windows usbstor stack resets/re-enumerates the whole composite device to
  recover -> firmware gets USB_EVENT_RESET/SUSPEND, whose handler calls sduSuspendHookI(&SDU1)
  (usbcfg.cpp:446), tearing down the CDC channel -> host CDC write returns 0, COM149 drops.
- It is host-side (write wrote 0 = port handle invalidated), not a firmware stall: the switch
  completed cleanly, logging started, and the device kept emitting messages up to the drop.

Remediation directions (not implemented - investigation only):
- Don't swap the MSD LUN to a dead null device under a mounted volume. Instead present a stable
  medium or return SCSI "not ready / medium not present" (unit attention) so Windows performs an
  orderly media-eject rather than treating it as a device fault and resetting the port.
- Or signal proper SCSI medium-removal / unit-attention before switching so the host dismounts
  cleanly.
- Host-side, for a true 10x soak: reconnect LinkManager after each switch (treat the CDC drop
  as expected re-enumeration). The current sandbox intentionally reports it instead.
- The existing USB.pcapng capture can confirm the host-issued bus reset around a mode switch.

Validation:
- ./gradlew :ui:compileTestJava BUILD SUCCESSFUL after submodule init.
- ./gradlew :ui:runSdCycle exercised against real hardware; full log captured.

Open follow-ups:
- Decide remediation approach (firmware SCSI media-eject vs host-side reconnect).
- Consider gating: the soak cannot complete 10 cycles over one connection until the CDC drop is
  addressed.

## 2026-07-14 - SD indicator/output-channel name reuse between SdEcuPcCycleSandbox and .ini

What was done:
- Removed the duplicated magic strings sd_present / sd_logging_internal / sd_msd that existed
  independently in output_channels.txt (bit field names), tunerstudio.template.ini (6 indicator
  expressions) and SdEcuPcCycleSandbox.java (SensorCentral lookups). They now flow from a single
  source of truth via the existing code generation.

Change inventory:
| File                                                        | Change                                                          |
|-------------------------------------------------------------|-----------------------------------------------------------------|
| firmware/integration/rusefi_config_shared.txt               | new OUTPUT_CHANNEL_SD_PRESENT / _SD_LOGGING_INTERNAL / _SD_MSD quoted defines |
| firmware/console/binary/output_channels.txt                 | the 3 SD bit fields renamed to @#OUTPUT_CHANNEL_SD_...#@ references |
| firmware/tunerstudio/tunerstudio.template.ini               | 6 indicator lines reference @#OUTPUT_CHANNEL_SD_...#@           |
| java_tools/.../ReaderStateImpl.java (config_definition_base)| handleBitLine now applies variable substitution to the bit name (comment stays templated, matching plain-field parsing) |
| java_tools/.../ConfigFieldParserTest.java                   | new testBitNameViaVariableReference                             |
| java_console/.../SdEcuPcCycleSandbox.java                   | uses VariableRegistryValues.OUTPUT_CHANNEL_SD_* constants       |
| java_tools/version/.../UiVersion.java                       | CONSOLE_VERSION -> 20260714                                     |

Key decisions and why:
- Constants live in rusefi_config_shared.txt because it is prepended by BOTH pipelines that need
  them: gen_config_common.sh (template .ini + VariableRegistryValues.java) and the LiveData.yaml
  output_channels entry (LiveDataProcessor parsing output_channels.txt).
- Used the existing @#NAME#@ quote-stripping substitution (same as TS_HELLO_COMMAND usages) so the
  quoted define yields a bare identifier in struct field names and { } indicator expressions while
  generating a proper Java String constant.
- handleBitLine substitution was narrowed to the name part only: applying it to the whole line
  expanded @@...@@ comment templates at parse time, which changed engine_state_generated.h
  (clutchDownState comment). The narrowed version keeps every generated artifact byte-identical.
- Did not resurrect the deleted generated TsOutputs.java (removed in #6711); defines + existing
  VariableRegistryValues generation is the sanctioned mechanism.

Validation:
- ./gradlew :config_definition:test :config_definition_base:test green including the new test.
- gen_live_documentation.sh + gen_config_board.sh f407-discovery: all generated outputs
  (output_channels_generated.h, live_data_fragments.ini, data_logs.ini, board .ini indicator
  lines) byte-identical to committed state; only VariableRegistryValues.java gains the 3 new
  String constants. Board-generated .h/.ini signature/date churn reverted (CI regenerates).
- ./gradlew :ui:compileTestJava BUILD SUCCESSFUL with the sandbox on the generated constants.

Open follow-ups:
- Other magic output-channel names shared between java_console and .ini (e.g. sd_error,
  sd_formating, sd_active_wr/rd) could adopt the same pattern when java code starts using them.

---

## 2026-07-14 - N52 preset: bake in TPS/PPS calibration from reference tune

What: Ported TPS + PPS calibration out of the "super N52" TunerStudio tune (CurrentTune.msq)
into the `bmwN52()` engine preset in `firmware/config/engines/bmw.cpp`, so a fresh N52 selection
ships with the real throttle-body/pedal calibration instead of raw defaults.

| Field(s)                                                   | Source (msq, volts) | Stored as |
|------------------------------------------------------------|---------------------|-----------|
| tpsMin / tpsMax                                            | 4.545 / 0.58        | 10-bit ADC via convertVoltageTo10bitADC |
| tps1SecondaryMin / tps1SecondaryMax                       | 0.75 / 4.72         | 10-bit ADC via convertVoltageTo10bitADC |
| throttlePedalUpVoltage / throttlePedalWOTVoltage          | 0.625 / 2.230       | float volts (verbatim) |
| throttlePedalSecondaryUpVoltage / ...WOTVoltage           | 0.947 / 4.197       | float volts (verbatim) |

Key decisions:
- Used shared helpers setTPS1Calibration()/setPPSCalibration() (defaults.h), matching subaru.cpp.
- TPS stored as 10-bit ADC counts (not volts): wrapped msq volts in convertVoltageTo10bitADC
  (= volts*200). PPS stored as float volts, copied through directly. This units split is the main
  porting hazard and is now documented.
- Skipped tps2* fields: msq had them at defaults (0/5, 5/0); N52 runs a single dual-sensor throttle.
- Added #include "defaults.h"; convertVoltageTo10bitADC comes transitively via pch.h.

Docs: new docs/AI/engine_presets.md documents the canned-tune/preset process end to end
(enum -> engine_type_impl.cpp dispatch -> config/engines setup fn), the TPS-vs-PPS units gotcha,
and a step-by-step msq->preset porting recipe.

Validation: static review only - mirrors the established subaru.cpp calibration pattern; all
referenced helpers are declared in the included headers. No generated files touched (presets are
plain code, no gen_config step).

Open follow-ups:
- Confirm on hardware that a defaults-reset N52 reads plausible TPS
---

## 2026-07-14 - N52 preset: bake in TPS/PPS calibration from reference tune

What: Ported TPS + PPS calibration out of the "super N52" TunerStudio tune (CurrentTune.msq)
into the bmwN52() engine preset in firmware/config/engines/bmw.cpp, so a fresh N52 selection
ships with the real throttle-body/pedal calibration instead of raw defaults.

| Field(s)                                           | Source (msq, volts) | Stored as |
|----------------------------------------------------|---------------------|-----------|
| tpsMin / tpsMax                                     | 4.545 / 0.58        | 10-bit ADC via convertVoltageTo10bitADC |
| tps1SecondaryMin / tps1SecondaryMax                | 0.75 / 4.72         | 10-bit ADC via convertVoltageTo10bitADC |
| throttlePedalUpVoltage / throttlePedalWOTVoltage   | 0.625 / 2.230       | float volts (verbatim) |
| throttlePedalSecondaryUpVoltage / ...WOTVoltage    | 0.947 / 4.197       | float volts (verbatim) |

Key decisions:
- Used shared helpers setTPS1Calibration()/setPPSCalibration() (defaults.h), matching subaru.cpp.
- TPS stored as 10-bit ADC counts (not volts): wrapped msq volts in convertVoltageTo10bitADC
  (-> volts*200). PPS stored as float volts, copied through directly. This units split is the
  main porting hazard and is now documented.
- Skipped tps2* fields: msq had them at defaults (0/5, 5/0); N52 runs a single dual-sensor throttle.
- Added #include "defaults.h"; convertVoltageTo10bitADC comes transitively via pch.h.

Docs: new docs/AI/engine_presets.md documents the canned-tune/preset process end to end
(enum -> engine_type_impl.cpp dispatch -> config/engines setup fn), the TPS-vs-PPS units gotcha,
and a step-by-step msq->preset porting recipe.

Validation: static review only - mirrors the established subaru.cpp calibration pattern; all
referenced helpers are declared in the included headers. No generated files touched (presets are
plain code, no gen_config step).

Open follow-ups:
- Confirm on hardware that a defaults-reset N52 reads plausible TPS%/pedal% before user tuning.
---

## 2026-07-15 - SD card logging: overview doc + code comments (f_expand logic)

What: Documented the SD card logging subsystem and annotated its three core source files.

| File | Change |
|-------------------------------------------------------|--------------------------------------------|
| docs/AI/sd_card_logging.md (new)                       | End-to-end overview: SD thread mode state machine (IDLE/ECU/PC/UNMOUNT/FORMAT, all transitions via IDLE), .mlg vs .teeth loggers, FileBufferedWriter path, f_expand pre-allocation, file naming, status channels, console commands |
| firmware/hw_layer/mmc_card.cpp                         | Expanded LOGGER_MAX_FILE_SIZE, f_expand and f_truncate comments; doc pointer in file header |
| firmware/console/binary_mlg_log/binary_mlg_logging.cpp | File-header overview (MLG v2 layout, who owns file lifecycle); comments on writeFileHeader/writeSdBlock/writeSdLogLine/resetFileLogging |
| firmware/console/binary/tooth_logger.cpp               | Comment on freeBuffers/filledBuffers multi-buffering (BigBuffer, interrupt producers -> TS/SD consumers); ToothLoggerWriter() contract incl. 3s idle timeout -> new file |
| CLAUDE.md                                              | Added sd_card_logging.md to Deep Dive AI Guidance list |

Key facts captured (the f_expand logic in particular):
- sdLoggerCreateFile() pre-allocates each log file to 32Mb with f_expand(fd, size, opt=1)
  (allocate-now, contiguous; FF_USE_EXPAND=1 in firmware/ext/FatFS/ffconf.h). All FAT
  updates happen up-front, so writes inside the pre-allocated area never touch FAT
  structures -> sudden power loss loses buffered data but not the filesystem.
- f_expand failure (fragmented card) is deliberately non-fatal: FatFS falls back to
  cluster-by-cluster growth, logging works without the corruption protection.
- sdLoggerCloseFile() f_truncate()s back to actual size; a power-lossed file stays 32Mb
  with trailing garbage.
- Both loggers share sdLoggerCreateFile() (so .teeth files are pre-allocated too) but
  only sdLoggerMlg() enforces the 32Mb rollover cap.

Validation: comment/doc-only changes, no code touched; facts verified against source
(mmc_card.cpp, file_writer.h, ffconf.h FF_USE_EXPAND=1, tooth_logger.cpp, sd_log_trigger.h).

Open follow-ups: none.

## 2026-07-15 - Lua scripting API: categorized hook inventory doc

What: Reviewed every custom Lua method registered around lua_hooks.cpp and documented
them in a new docs/AI/lua_scripting.md, grouped into 11 categories.

| File | Change |
|-------------------------------|--------------------------------------------------------|
| docs/AI/lua_scripting.md (new) | Full inventory of Lua hooks by category: input reads, virtual sensors, virtual switches, closed-loop trims, cut/disable controls, PWM/DAC outputs, CAN, config/calibration access, state queries, luaaa helper classes, framework/test hooks; plus registration-site map, indexing conventions, build-flag gating, and an "adding a new hook" recipe |
| CLAUDE.md | Added lua_scripting.md to the Deep Dive AI Guidance list |

Key decisions / findings:
- Registration is spread over four files: lua_hooks.cpp (bulk + luaaa classes),
  lua_hooks_util.cpp (print/interpolate/find*/mcu_standby), lua.cpp (setTickRate,
  onTick dispatch), lua_can_rx.cpp (onCanRx dispatch, global_can_data workaround).
  lua_hooks_ext.cpp is an empty extension point; boardConfigureLuaHooks() is a weak
  board hook with no in-tree overrides.
- Documented the mixed indexing convention explicitly: 1-based (HUMAN_OFFSET) for
  TS-facing entities (CAN bus, curves, tables, TS buttons, gauges), 0-based for
  sensor indices, PWM channels, aux digital inputs, vin().
- Documented flash-saving exclusions (#if !defined(STM32F4) group) and the
  DISABLE_LUA_* / WITH_LUA_* opt-out macros.
- setTickRate code clamps 1..2000 Hz while its comment says 1..200 - doc records
  the code behavior (comment discrepancy left in source, not a functional issue).

Validation: doc-only change; every listed hook, guard macro and constant
(LUA_PWM_COUNT=8, LUA_GAUGE_COUNT=8, LUA_DIGITAL_INPUT_COUNT=8, LUA_BUTTON_COUNT=10,
CMD_BURNCONFIG="burnconfig") verified against source via grep/read of the four
registration files, lua_pid.h and rusefi_config.txt.

Open follow-ups:
- lua.cpp setTickRate comment ("Limit to 1..200 hz") disagrees with clampF(1, x, 2000).
- lua_hooks.cpp has a commented-out hasCriticalReportFile hook referencing issue #7291.

## 2026-07-17 - loss-of-cdc.pcapng analysis: one-shot composite reset from pre-capture MSD wedge

What: Analyzed loss-of-cdc.pcapng (repo root, USBPcap, 24.6 s, captured 2026-07-17
12:25 - i.e. the day AFTER the #9860 fix series landed) against the recent
mass_storage changes. Goal: confirm/refute whether the CDC drop mechanism from
issue #9860 is still present.

Devices in capture: address 21 = the ECU (VID 0483:5740, composite MSD+CDC),
address 22 = PEAK PCAN-USB adapter (19.9k of the 22k packets - unrelated noise).

Timeline (t = seconds from capture start):
- t=0..10.8: ZERO MSD traffic from the ECU. A healthy medium-less device gets
  ~1 Hz Test Unit Ready polls (visible later in this same capture), so at capture
  start usbstor already had one command in flight that never completed - the MSD
  side was already wedged/stuck before the capture began.
- t=7.79: host opens the COM port (GET/SET LINE CODING burst); CDC request/reply
  traffic (TS-style 7/11-byte commands, 1024-byte replies) runs cleanly for 3 s.
- t=10.847: usbstor ~20 s give-up timer fires -> all-endpoint cancel storm on the
  ECU: 10 URBs with USBD_STATUS_CANCELED (0xc0010000) - MSD bulk-IN 0x81 (the
  stuck data/CSW read, pending since before capture start), CDC data 0x82/0x02,
  CDC interrupt 0x83, plus control. This is the loss-of-CDC moment.
- t=10.883: host immediately retries line coding - those control URBs are
  canceled too (device still resetting).
- t=11.03..11.05: MSD recovers: Test Unit Ready on LUN0 and LUN1 -> Check
  Condition -> Request Sense (Good) -> Mode Sense(6) (the known-cosmetic
  "malformed" short caching page). Both LUNs report medium-not-present.
- t=11.28: CDC port re-opens at USB level (line coding OK) but NO data traffic
  follows - the app-level session was dead, host serial layer sat in its ~10 s
  timeout.
- t=12..24.5: clean steady state: 1 Hz TUR polls per LUN, no stalls, no babble,
  no further cancels or resets.
- t=20.79: app fully reconnects (line coding + control line state), TS-style
  traffic resumes. Total user-visible CDC outage: ~10 s (10.85 -> 20.79).

Reading vs the 2026-07-16 fix series (298162eb075..68e7d77c042, all in
firmware/hw_layer/mass_storage/):
- 298162eb0/8a515546c (MSD diag #9838): sdinfo diagnostics incl. per-opcode
  in-flight timer.
- e1feee380 (isCommandAbandoned #9861): 10 s data-phase timeouts on all SCSI
  transfers + CSW via msdUsb*Timeout helpers -> wedged thread self-recovers,
  re-arms bulk-OUT.
- 12b613c59 (#9864): LUN detach now synchronizes with in-flight command
  (m_lunMutex held around scsiExecCmd+CSW) -> kills the SPI double-waiter
  deadlock from the SD mode switch.
- 04331c28f (#9866) + 68e7d77c0 (uaefi): medium-less data-IN commands answered
  with ZLP instead of STALL -> no EP0 clear-halt round-trip near CDC traffic.
The capture is consistent with the fixes WORKING as designed for the recurring
part: exactly ONE reset (the tail of a wedge that began ~9 s before capture,
matching usbstor's ~20 s timer), then 13.5 s of clean behavior with no repeat
reset - the old signature was a reset every ~20 s.

Remaining gap (why one reset still happens): the firmware 10 s data-phase
timeout releases the MSD *thread*, but leaves the *host's* pending IN URB
hanging - firmware just returns to CBW wait and never completes/STALLs the
IN transfer the host is still waiting on. usbstor therefore still escalates to
a full composite reset once, taking CDC down with it. A full fix would complete
the host's data phase on timeout (e.g. STALL the IN endpoint so the host gets
an immediate error -> clear-halt -> CSW path) instead of leaving the URB
pending. Caveat: cannot verify from the capture which firmware build was
flashed or which opcode wedged (the CBW predates the capture); console sdinfo
counters (data-phase timeouts / no-data ZLPs) on the connected unit would
distinguish "fixed firmware, host-side URB gap" from "stale firmware".

Validation: tshark 3.6.2 field-level analysis (usb.usbd_status, endpoints,
SCSI dissection); code cross-checked at HEAD (mass_storage_device.cpp timeout/
ZLP/mutex mechanisms present).

Open follow-ups:
- On data-phase timeout, also complete the host-visible transfer (STALL data-IN
  or arm+flush) so usbstor never needs its 20 s reset - would remove the single
  remaining CDC drop.
- Confirm via sdinfo on hardware whether the flashed build has the 07-16 fixes
  and whether data-phase timeout counters tick.

## 2026-07-17 - MSD data-phase timeout: close the command host-side (stall + phase-error CSW)

What: Implemented the follow-up from the loss-of-cdc.pcapng analysis (previous
entry). Before this change, a data-phase timeout only freed the MSD *thread*
(e1feee380 #9861); the *host's* pending URB was left hanging and the CSW was
skipped, so usbstor still escalated to one full composite-device reset per
wedge - taking the CDC console down for ~10 s each time.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/hw_layer/mass_storage/mass_storage_device.cpp | ThreadTask: split the abandoned-command check. BOT reset still skips the CSW (host is not expecting one). Data-phase timeout now STALLs the data endpoint in the CBW's direction (usbStallTransmitI/usbStallReceiveI) and then sends a CSW with CSW_STATUS_PHASE_ERROR and honest residue. sendCsw() now returns whether the host read the CSW; sdinfo prints "N data-phase timeouts (M closed by CSW)" |
| firmware/hw_layer/mass_storage/mass_storage_device.h | sendCsw() -> bool; new m_timeoutCswDeliveredCount counter |

Key decisions and why:
- STALL is the BOT-sanctioned "cannot complete this data phase" signal: a host
  still waiting on its data URB completes it with an error immediately (well
  before usbstor's ~20 s give-up), does a clear-halt on this one endpoint, and
  collects the CSW - recovery stays class-level on the MSD interface, the CDC
  endpoints never notice. A host that already canceled its URBs ignores the
  stall and resets anyway - no worse than before.
- Arming the CSW while the endpoint is still stalled is the exact sequence the
  pre-ZLP medium-less path used (04331c28f), already validated on Windows
  hardware (STALL -> clear-halt -> CSW observed on the wire).
- CSW_STATUS_PHASE_ERROR rather than FAILED: after a broken data phase the
  transport has genuinely lost sync; phase error makes the host run Bulk-Only
  Reset Recovery (class request + clear both halts), fully resynchronizing
  data toggles without any port-level reset. The existing onBulkOnlyResetIsr
  path handles that request.
- Safe to stall: all three msdUsb*Timeout helpers clear the endpoint's
  active flag on timeout, so usbStall*I (which refuses while a transfer is
  active) always takes effect by the time ThreadTask runs the recovery.
- The no-data-ZLP timeout path intentionally keeps its plain 'continue': a
  host that will not even take a zero-length packet is gone from the data
  phase entirely; its next action is a new CBW (accepted normally) or a reset.

Validation: uaefi firmware build (see below). No unit-test coverage exists for
this path (EFI_PROD_CODE + HAL_USE_USB_MSD only). Hardware validation plan:
reproduce the wedge (host abandons a command mid-data-phase), then check
1. sdinfo shows "closed by CSW" ticking together with data-phase timeouts,
2. a capture shows STALL -> clear-halt -> CSW(phase error) -> BOT reset
   instead of the all-endpoint cancel storm,
3. the CDC console stays connected across the event.

Open follow-ups:
- Wedges *below* the USB layer (e.g. blkRead stuck on a dying SD card) are
  still uncovered: no timeout wraps lib_scsi's block-device calls, so such a
  wedge never reaches the new recovery path (lib_scsi is in ChibiOS-Contrib).
- The loss-of-cdc.pcapng pre-capture wedge could not be attributed (stale
  firmware vs blkRead wedge); confirm the flashed build via sdinfo counters.

## 2026-08-01 - Decouple VE Analyze from the STFT display scale

What: Restored zero-based STFT presentation without changing the 100-based
correction contract required by TunerStudio VE Analyze. PR #9657 changed the
STFT translation from -1.0 to -100; under TunerStudio's `(raw + translate) *
scale` conversion, a neutral raw multiplier of 1.0 became -9900 percent. The
derived `100 + stftCorrection1` channel then supplied -9800 instead of 100 to
VE Analyze, causing it to remove fuel.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/controllers/algo/engine_state.txt | Restore STFT display metadata to scale 100, translation -1.0, so raw 0.9/1.0/1.1 displays as -10/0/+10 percent |
| firmware/tunerstudio/tunerstudio.template.ini | Feed `egoCorrectionForVeAnalyze` directly from `Gego`, the existing 100-neutral STFT output channel |
| java_tools/configuration_definition/src/test/java/com/rusefi/test/VeAnalyzeCorrectionTest.java | Regression coverage for zero-neutral display and independence of the VE Analyze channel |
| java_tools/version/src/main/java/com/rusefi/UiVersion.java | Bump console version to 20260801 as required for Java changes |

Key decisions and why:
- Reused `Gego` instead of adding another live-data field. `status_loop.cpp`
  already publishes it as `100 * stftCorrection[0]`, so this avoids output
  layout churn and keeps the machine-facing 100-neutral contract explicit.
- Kept the user-facing `stftCorrection` channels zero-neutral and independent
  from AutoTune. Gauge scale or translation changes can no longer alter the
  correction consumed by VE Analyze.
- No persistent calibration field or generated file is part of the change, so
  existing tunes require no migration.
- LTFT behavior is intentionally unchanged in this unit of work. Stored LTFT
  correction still affects delivered fuel without being represented in the VE
  Analyze correction channel; that requires a separate policy change and test.

Validation:
- Regression test first failed on the old code: raw 0.9 displayed as -9910
  instead of -10, and VE Analyze still referenced the visual STFT channel.
- The same test passes after the fix.
- `gradlew.bat :config_definition:test` passes.
- Clean uaefi `make -B -j12 ini` generation passes. The generated INI contains
  `stftCorrection1/2` with `100.0, -1.0`, keeps `Gego` at scale 0.01, and emits
  `egoCorrectionForVeAnalyze = { Gego }`; both VE Analyze and WUE Analyze use
  that alias.

Open follow-ups:
- Define and test the LTFT policy during AutoTune (disable application, require
  applying/resetting learned trims, or introduce an explicit tuning session).
- Decide how a future bank-2-aware VE Analyze correction should select/combine
  STFT banks; this change preserves the existing bank-1 behavior.

## 2026-08-09 - Restore PCAN adapter into the console ports list + surface init errors

What: PCAN-USB adapters were detected (`AvailableHardware.isPCANConnected()`)
but never shown in the ports dropdown: the `ports.add(new PortResult(LinkManager.PCAN,
SerialPortType.CAN))` block had been commented out in Feb 2026 (commits
`6bc0d319e2f`/`ad9856e19a4`, messages "PCANConnected only:hiding"), so the user
saw `Rendering available ports: []` and "No ECU ports to use found" even though
MaintenanceUtil confirmed `Caption : PCAN-USB, ConfigManagerErrorCode : 0`. A
second, hidden-by-design issue: when PCAN init failed, `PCanIoStream.createStream()`
logged the real `TPCANStatus` only through the status consumer and returned null,
so the UI showed the generic "Failed to open port" without the cause.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| java_console/connectivity/src/main/java/com/rusefi/SerialPortScanner.java | Re-enable the PCAN synthetic CAN port (SocketCAN stays hidden as before - it is Linux-only and was deliberately hidden separately) |
| java_console/io/src/main/java/com/rusefi/io/LinkManager.java | PCAN branch now captures the status-consumer message and throws it as IllegalStateException, so PortHolder reports "Exception opening port: Error initializing PCAN: <status>" instead of the generic failure |
| java_console/connectivity/src/test/java/com/rusefi/SerialPortScannerTest.java | FakeProbes gains a scripted `pcanConnected` flag; new tests for PCAN surfacing as a CAN port and for the no-adapter case |

Key decisions and why:
- Restored only the PCAN block, not SocketCAN. `SHOW_SOCKETCAN` is Linux-only
  and the existing `tcpPortWithoutEcuIsReportedUnknownAndNotCached` test asserts
  an exact port count on Linux CI, so un-hiding SocketCAN would break it; the
  owner hid it in a separate commit for its own reasons.
- Kept `PCanIoStream.createStream()` contract (returns null + status consumer)
  and wrapped it at the LinkManager call site instead: the status text flows into
  the thrown exception, so every caller (dropdown connect, ConsoleTools,
  PcanConnectorUI) keeps working unchanged.
- `AtomicReference` needs `java.util.concurrent.atomic` - `import
  java.util.concurrent.*` does not cover subpackages (compile error caught by
  `:ecu_io:compileJava`).

Validation:
- `./gradlew :connectivity:test :ecu_io:test` passes (including new
  `pcanAdapterSurfacesAsSyntheticCanPort` / `noPcanAdapterMeansNoSyntheticPcanPort`
  and the pre-existing scan-policy suite).
- Compile of `:ecu_io` catches the missing-import mistake; fixed with an explicit
  `java.util.concurrent.atomic.AtomicReference` import.

Open follow-ups:
- `PCanHelper.init` still hardcodes `PCAN_USBBUS1` + `PCAN_BAUD_500K`; a bus
  number or baud mismatch (e.g. ECU at 1M) will surface now as the real
  TPCANStatus in the UI - consider making channel/baud configurable.
- If PCAN init succeeds but the ECU never answers, the next diagnostic step is
  the ISO-TP IDs (`CAN_ECU_SERIAL_RX_ID`/`TX_ID`) versus firmware config.

## 2026-08-09 - PowerShell device probes: -Command quoting broken on Windows, switched to -EncodedCommand

What: After the ProcessBuilder conversion of the PowerShell device probes, the user's
Windows console log changed from `powershell -NoProfile -Command "Get-CimInstance ...
-Filter \"Caption like '%PCAN-USB%'\" ..." says Caption : PCAN-USB` (old build, worked)
to `[powershell, -NoProfile, -Command, Get-CimInstance Win32_PnPEntity -Filter "Caption
like '%PCAN-USB%'" ...] says (empty)` (new build, empty). Root cause: Windows PowerShell
parses the `-Command` tail with its own quote handling (it strips/re-interprets quotes,
not the standard C-runtime argv rules), and every Java argv-joining strategy feeds it a
different escaped form - Runtime.exec(String)'s tokenizer produced literal `\"` tokens,
ProcessBuilder's command-line quoting produces `\"` escapes - so the WQL -Filter value
`"Caption like '%PCAN-USB%'"` got mangled and the query matched nothing. The macOS
fake-powershell argv-dump test could not catch this: POSIX execvp passes argv verbatim,
Windows command-line reconstruction is where quoting breaks.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| java_console/ui/src/main/java/com/rusefi/maintenance/MaintenanceUtil.java | New `powershellEncodedCommand(script)` helper: transports the script as Base64 UTF-16LE via `-EncodedCommand` (pure-ASCII payload, no spaces/quotes - immune to any command-line quoting); PCAN query converted; `detectDevice` log line now shows the decoded script via `describeQueryCommand` |
| java_console/ui/src/main/java/com/rusefi/maintenance/DfuFlasher.java | STM32-bootloader and STM32-H7 DFU queries converted to `powershellEncodedCommand` |
| java_console/ui/src/main/java/com/rusefi/maintenance/StLinkFlasher.java | ST-Link query converted; dropped now-unused `Arrays` import |
| java_console/ui/src/test/java/com/rusefi/maintenance/MaintenanceUtilTest.java | `pcanQueryIsSingleIntactPowerShellScriptArgument` replaced by `pcanQueryIsEncodedCommandSurvivingQuoting` (base64 payload shape + decode round-trip) and `describeQueryCommandShowsDecodedScript` |

Key decisions and why:
- `-EncodedCommand` (Base64 of UTF-16LE) instead of more quote-escaping: it is the
  canonical way to pass complex scripts to Windows PowerShell and pwsh alike, and it
  makes the transport deterministic - the script reaches PowerShell byte-exact.
- Left `ExecHelper.executeCommand(List, ...)`/ProcessBuilder in place (correct on
  POSIX and needed for the no-shell path); only the command content changed.
- Left `DfuFlasher.getPnpDevices` (`Get-PnpDevice -PresentOnly`, no quotes) and
  `TunerStudioHelper` (Runtime.exec(String), script without inner quotes) untouched -
  neither has quoted arguments, so both are immune to this class of bug.

Validation:
- `./gradlew :ui:test :connectivity:test :ecu_io:test` passes; the new tests verify
  the payload charset and the decode round-trip back to the intact script.
- Cannot run PowerShell here (macOS host); the user validates on Windows: the log
  line should show `-EncodedCommand <Get-CimInstance ...>` and "says Caption : PCAN-USB".

Open follow-ups:
- If the user's adapter still reports empty with the encoded command, the next step
  is hardware/driver state (PCAN-View holding the channel, unplugged adapter) rather
  than quoting - the query transport is now deterministic.
- `TunerStudioHelper` still uses Runtime.exec(String) with outer quotes; fine today,
  but converting it to the list-based ExecHelper would remove the last fragile caller.

## 2026-08-09 - m74_9 watchdog round 2: CNT alive, ISR->reschedule chain dies; live register dump + pre-fault history

What: Follow-up instrumentation of the m74_9 (AT32F435) `CRITICAL error: Watchdog: no
events for 2s!` lockup at ~14-20 s after boot. Round-1's register snapshot was truncated
by `CRITICAL_BUFFER_SIZE=120` (the message buffer), so the extended state never reached
the console. Round-2 moves the full register picture to the 256-byte console path and
adds a pre-fault history.

| File | Change |
|----------------------------------------------------|----------------------------------------|
| firmware/hw_layer/microsecond_timer/microsecond_timer.cpp | Latched `firmwareError` shrunk to ~103 chars (`isr=%d setHw=%d pending=%d cnt=%u..%u dier=0x%x sr=0x%x cr1=0x%x`) so it survives the 120-byte buffer plus the ` %d@%s` suffix; live `WDT regs: ...` dump (cnt span, dier, sr, cr1, smcr, psc, arr, ccr1, ccmr1, ccer, apb1enr, tim5en) via `efiPrintf` every watchdog period while the error holds; 16-sample ring buffer (`WDT hist: ...`) recording cnt/isr/setHw/freeze/pending/sr/dier/ccr1/cr1 once per healthy 500 ms watchdog period (last ~8 s), dumped once at first latch; state kept private under `#ifdef SCHEDULER_TIMER_DEVICE` (Cypress/Kinetis have no TIM device) |

Key findings from the user's round-2 log (2026-08-09 20:54):
- `cnt=124159183..124559165(+399982)` per 100 ms at 4 MHz -> the TIM5 counter keeps
  running, the clock is NOT gated (rules out frozen CNT and cleared APB1ENR.TIM5EN).
- `isr=37650 setHw=37652` frozen, `pending=1`, `dier=0x2` (CC1IE armed), `sr=0x0` (no
  CC1IF) -> the compare never fires even though the interrupt is armed; the
  ISR -> executeAllPendingActions -> scheduleTimerCallback chain stops at "comparison
  does not trigger".
- Error repeats every ~180 ms while pending=1 holds; the CAN console keeps working, so
  it is not a global freeze.
- Timeline: first error 20:54:54.9 vs boot 20:54:40.05; ~37650 ISR hits at ~1400/s
  matches ~14 s. Preceding events: MFS config read (ok), Lua load (ok, 5.4% heap),
  set date over CAN (RTC 1980 -> 2026).

Validation:
- Diff reviewed for `%d/%u/%x` vs cast consistency; touches only the EFI_PROD_CODE path.
- Cross-compile and on-hardware reproduction pending on the user's Windows machine (no
  arm-none-eabi toolchain on the macOS host).

Open follow-ups:
- Need a full 40-60 s console log with `WDT regs:` (ccr1, ccer, apb1enr, tim5en, smcr)
  and `WDT hist:` lines to decide between "CC1E/OC1M rewritten by a second pwmStart on
  PWMD5" vs "CC1IF set but ISR never dispatched (NVIC/vector)".
- If the counter proves alive but the compare is disabled, check whether any code path
  restarts PWMD5 after boot (would reset CNT/ARR/CCER per pwm_lld_start).

## 2026-08-09 - m74_9 watchdog ROOT CAUSE: compare re-armed in the past, missed CNT==CCR1 equality

What: The user's round-3 register dump (21:23) finally pinned the failure mode. The
latched error repeats with isr=20128 setHw=20130 pending=1 cnt=60158992..60558987
while the live dump shows the counter alive and growing:

- `ccr1=0x30316F9` = 50,521,849 - in the PAST (cnt already 64-76M at dump time): the
  last successful re-arm wrote a compare value that CNT had already passed.
- `ccer=0x0` - NOT the culprit: CC1E is 0 since pwm_lld_start (channel mode
  PWM_OUTPUT_DISABLED) and the design works without it (compare interrupts fire on
  CC1IF + CC1IE only).
- `ccmr1=0x10` = OC1M(1) toggle as set by portInitMicrosecondTimer; `cr1=0x485` =
  PMEN|ARPE|URS|CEN exactly as pwm_lld_start writes (AT32 32-bit mode); both untouched.
- `psc=0x47` (72 divider, 288MHz APB1 -> 4MHz counter), `apb1enr=0x12000018`,
  `tim5en=8`: clock gating ruled out for good.
- `dier=0x2` (CC1IE armed) + `sr=0x0` (CC1IF cleared): the re-arm sequence
  pwm_lld_enable_channel (CCR1 write) -> pwm_lld_enable_channel_notification
  (SR=~(2<<0), DIER|=CC1IE) ran, but with CCR1 already in the past the equality
  CNT==CCR1 can never fire again until the 32-bit counter wraps (~18 min at 4MHz).
  The ISR->reschedule chain therefore dies silently; the 2s watchdog only reports it.

Root cause: setHardwareSchedulerTimer() computes its "too close to now" clamp
(US2NT(2)) against the nowNt passed by the caller (read earlier, under lock). The
hardware write happens later and the free-running CNT keeps advancing, so an event
scheduled only a few microseconds ahead can land in CCR1 already in the past - a
probabilistic race that took 12-20s to hit (isr counts differ per run: 20128, 37650).

Fix (firmware/hw_layer/ports/stm32/microsecond_timer_stm32.cpp,
portSetHardwareSchedulerTimer): re-check the compare value against a FRESH
SCHEDULER_TIMER_DEVICE->CNT read and clamp it at least US2NT(4) = 4us (16 ticks at
4MHz) into the future before writing CCR1. This makes the CNT==CCR1 equality
impossible to miss regardless of how stale the scheduler's setTimeNt is. The
int32-trick on (compare - cnt) is safe because TOO_FAR_INTO_FUTURE_NT is 10s
= 40M ticks << 2^31.

Validation:
- Review-only on macOS (no arm-none-eabi toolchain); the user will rebuild m74_9 on
  Windows and confirm the ECU stays up past the previous 12-20s lockup.
- The change is EFI_PROD_CODE-path only and does not alter the normal path (compare
  values >= 4us ahead are written verbatim).

Open follow-ups:
- If the lockup still reproduces with the clamp in place, the next suspect is an
  event actually landing in the queue with a past time (scheduleTimerCallback's
  efiAssertVoid "setTimer constraint" fires firmwareError but with
  EFI_ENABLE_ASSERTS=0 the guard is a no-op) - then fix there instead.
- WDT hist lines never reached the console in the 21:23 log (printed once at first
  latch, log started after); they are kept for future diagnosis.
