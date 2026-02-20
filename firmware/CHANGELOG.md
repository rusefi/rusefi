<!---

How to use this file:
- When making a change to the firmware that users should know about, add it to the
	appropriate section (breaking/added/fixed) in the next release.
- If it's something interesting or breaking, make a post about it on the forum! We want users
	to find out about changes before they have to get help because their car won't start.

Release template (copy/paste this for new release):

## Month 202x Release - "Release Name"

### Breaking Changes
 -

### Added
 -

### Fixed
 -

### Removed
 -

 -->

# Changelog

## Unreleased

### Fixed
 - ETB: calibration, wrong scale 3% calibration error #8863
 - luaHeapAlloc crashed when not enough RAM #8874
 - uaefi121: pin 43 conflict with green LED #8884
 - super-uaefi: high side 43 conflict with green LED
 - TS: annoying numeric values after proper named values #8898
 - true/false? is that French? #8530

### Added
 - FatFS update to R0.16
 - WBO: update from sd #8870
 - Switch to 7z for ini file compression on MSD #8895
 - stm32: flash: abort erase/write in case of undervoltage #9137
 - freedom units progress #9029

### Breaking Changes
 - CAN bitrare 666k #8784

## October 2025 "Day 1333"

### Added
 - OilTempGauge
 - LUA_USER_HEAP is no longer used
 - official binaries use gcc14
 - Jeep EVA 36-2-2 trigger pattern
 - migration to ChibiOS 21.11
 - long term fuel trim LTFT using SD card #6223
 - Add Lua hook for Launch Control request with 'setLaunchTrigger' method #7161
 - option to use Fahrenheit units #4788
 - lua editor highlighting #8413
 - lua: count dropped CAN messages #8225
 - larger LUA_canFrameCount on F7
 - native OpenBLT updater should help Mac/Linux #8495
 - firmware binaries to have detailed file names, older files go into .rusefi/older-fw folder #7830
 - predictive MAP AE Blend Duration is now a curve #8579

### Fixed
 - 3000gt 6g72 triggers cam and crank 3-0 more accuracy
 - unneeded ETB PID reset causes AE and causes AFR dip #8392
 - Torque reduction while using "Clutch Up Switch" trigger is inverted. #8447
 - TPS AE Fuel During Cranking #8500
 - migration fails in case of large diff #8524

## June 2025 "Day 1210"

### Added
 - SD card mode selection: do not wait for USB if vbatt voltage #7424
 - SD card mass storage read access performance improvements
 - migrate injector lag is now a table #7522
 - Allow fractional tachometer pulse ratio for fine tachometer calibration #7547
 - separate boost control open loop axis from closed loop axis #7580
 - verbose CAN PerCylinderKnock
 - modern Chrysler VVT cam decoder https://wiki.rusefi.com/All-Supported-Triggers#vvt
 - SD card formatting indicator
 - additional status on WBO dialog
 - default Lua tick rate changed from 10Hz to 200Hz
 - GMLAN slow single wire baud rate

### Fixed
 - map window averaging is broken since April 20th #7867
 - map window averaging does not use crank angle #7869

## February 2025 "Day 1075"

### Added
 - Flex Fuel sensor settings should be under "Fuel sensor" #7097
 - GM SENT fuel pressure sensor #6997
 - ETB calibration automatically enables ETB if needed #7197
 - Fuel Level Sensor should have a RAW option #6979
 - SD card bit data points #7227
 - stm32 build-in watchdog for F7/H7 #1339
 - improvements to SD card management #7327
 - FatFS: update to latest R0.15a version #7315
 - Honda J30 trigger wheel

### Fixed
 - Critical error when using big values on Boost Control Open Loop with Y axis as MAP #7093
 - "Baro Correction" axis label
 - unused "Throttle Up switch" is removed
 - Lua: fix heap used accumulator calculation: Do not increase if allocation failed #6924
 - Check Engine output hidden since the whole feature pretty much does not exist

### Breaking Changes
 - Lua: onCanRx parameter value to be use Lua-indexing #7110
 - After start enrichment Engine Cycles is now represented with U16 data type

## November 2024 "Day 998"

### Added
 - Support wasted spark on odd cylinder count 4-stroke engines. Improves startup and allows running without a cam sensor!
 - CLT/IAT boost duty/target compensation #6424
 - DFCO MAP condition hysteresis #6915
 - Add I-term bounds for Alternator Control #6988
 - knock fuel trim + disable suppression then deceleration #7017
 - TS runtime rate improvement

### Fixed
 - Removed questionable MAP sampling trigger index option
 - boost control load axis range and rename #6840
 - alphaNUseIat calculations are wrong #6907
 - TPS/PPS Error Detection Threshold visible twice in same dialog #6922
 - V-Twin MAP phase sense does not need to always wait for sync #6985
 - Organize sensor categories #6856
 - USB serial is sometimes lost #6595
 - fix detect cylinder for knock then wasted_spark #7062
 - Change BMW E46 CAN interval to 10 ms #6974

### Breaking Changes
- Now secondary injectors use `Secondary injector flow compensation mode` and `Secondary injector reference pressure` instead of `Injector flow compensation mode` and `Injector reference pressure` Tuner Studio settings #6972

## August 2024 "Day 898"

### Added
 - new rusEFI console tab: Knock analyzer #6467
 - Suzuki K6A (NON VVTI Trigger) #6490
 - explicit error if 32 bit java is used #6497
 - configurable Open Loop Boost vertical axis #4778
 - reduce JRE download complexity #6569
 - simpler basic firmware updater #5577
 - prohibit mcu_standby on ECU start-up #6589
 - Allow to choose lower RPM cutoff for AC Compressor #6597
 - New TPS/TPS enrichment mode: percent adder #3167
 - Launch control has a variable ignition cut BEFORE the main Hard cut #6566
 - Experimental mapExpAverageAlpha filtering #6579
 - minimum priming delay of 0.1 second
 - Launch to accept lua-based clutch and brake pedals #6658
 - Console command `set_sensor_mock` now accepts a sensor by name, instead of index. Example: `set_sensor_mock CLT 85.5`
 - STM32Cube v2.17
 - Better validation of thermistor configuration #6724
 - Full layout vs tuning layout #5154
 - Usability: turn on boost controller without ECU power cycle #6761
 - Knock frequency inputs to be bore diameter and checkbox #6767
 - Nissan HR crank trigger wheel #6773

### Fixed
 - knock logic not activated until any configuration change via TS #6462
 - VE Table have wrong units on Y-Axis #6479
 - General Purpose PWM tables have wrong names on X-axis and Y-axis #6491
 - Ignition scheduling errors after 40+ hours of uptime #6451
 - luaSoftSparkSkip/luaHardSparkSkip probably bug #6528
 - improved 4G63/4G9x cam wheel definition #6544
 - Launch RPM + Launch Control window is not correct #6533
 - MAX3185X_REFRESH_TIME is now 0.1 sec.
 - ADC array out of bounds when using analog muxes #6376
 - DFCO without a MAP sensor #6771

### Breaking Changes
 - "60/2 VW" trigger wheel renamed to "60-2 Wrong Polarity" #988
 - exhaust VVTs now use it's own Edge Select setting
 - removing weird 1+16 trigger wheel #6418
 - critical error in case of unneeded second channel #6419
 - Open Loop Boost vertical axis needs to be configured #4778
 - Cranking Cycle Multiplier is now a Map with Coolant Temp as the additional axis. #6584
 - Now we use "Alternator Voltage Target Table" instead of "Target(Volts)" setting on "Alternator Settings" area to calculate alternator voltage target. #6523
 - Now console generates current_configuration.binary_image in new format instead of current_configuration.rusefi_binary #6863

## April 2024 "Day 785"

### Added
 - Boost Open Loop gear-based modifier takes negative values #6189
 - "12 tooth crank" renamed to "12crank/24cam" #6192
 - second CAN WBO channel detailed logging #6306
 - Make sure TUNERSTUDIO IS DISCONNECTED FROM ECU #6325
 - 1_6_5_10_2_7_3_8_4_9 firing order BMW S85, also Audi 5.2 R8 and huracan GDI
 - boost duty gauge is now clamped to match actuator #6339
 - "MAP estimate table" is only available in speed density mode #6344
 - 1-8-6-2-7-3-4-5 Ferrari-Maserati F136 firing order

### Fixed
 - ETB traction control drop is affected by not-highlighted cell #5952
 - Aux Speed sensors stopped working on calibration change #6157
 - EGO renamed to O2 sensor to reduce confusion #6159
 - Disabling slow connectivity mode for now #6170
 - hellen128 better ETB pin choice between rev A/B and C
 - Base map values for Open Loop Boost Duty should be 0% #6188
 - HPFP control on cam with variable timing #6245
 - Negative timing is not displayed correctly fix #5250
 - update-ts-cacerts work-around is no longer needed #6327
 - uaEFI board knock configuration #6338
 - Fix a scenario where noisy trigger can cause overdwell #6349
 - knock retard calibration lookup #6370

### Breaking Changes
 - wrong trigger naming for missing teeth #6258
 - flipping timing_offset_cylinder direction #6095
 - clutch and flex inputs inverted options renamed #2808

## March 2024 "Day 740"

### Breaking Changes
 - Trigger Edge settings renamed from "true"/"false" to "Falling"/"Rising"

### Added
 - Allow brief operation over 100% injector duty cycle and add configurable duty cycle limits #4798
 - Staged injection support #5247
 - Ford Voodoo 1-5-4-8-3-7-2-6 firing order
 - 6/0 CKP trigger pattern for 1995 Lamborghini Diablo #5876
 - VE/Ign/Boost blend tables can select a Y axis override for even more flexibility
 - Disable radiator fans after certain vehicle speed #5875
 - LIS2DW12TR on-board Accelerometer support #4870
 - Wheel slip ratio using aux speed inputs #5897
 - 4 cylinder daihatsu cam sensor decoder #5925
 - allow inverted flex input #6131

### Fixed
 - Honda CBR 600 cam decoder #5909
 - boosted applications to log VE above 127
 - removing confusing numeric options from enumerable drop downs #5942
 - Subaru 6/7 improvement for cranking
 - Improve performance with Lua CAN reception of a high volume of frames #5841
 - odd fire timing_offset_cylinder not accounted by injection #6095
 - odd fire first spark out of order coilIndex logic in prepareCylinderIgnitionSchedule #6094

### Removed
 - Narrow to Wideband approximation

## December 2023 "Day 677"

### Breaking Changes
 - "acIdleRpmBump" renamed to "acIdleRpmTarget", and changed the way of RPM rise needed for proper A/C operation from added to absolute target #5628
 - After Cranking Enrichment changed from fixed value to table, it can help with some engines that want lots of fuel on cold, but don't run too well with big enrichment on hot #5632

### Added
 - DAC with Lua #5601
 - TunerStudio cacerts fix script #5536
 - Idle Timing Control - Soft Entry mechanism, in case of aggressive PID tuning this can help when engine enters close loop idle
 - Idle Timing Control - I factor configurable now #5635
 - Make TPS/PPS split threshold configurable #5620
 - Mitsubishi 6G72 CAM Trigger Pattern #5593
 - 3/0 trigger #5788
 - CANbus front to rear axle speed slip rate gauge on Nissan 350z and modern Hyundai/Kia #5842

### Fixed
 - Changing idle stepper settings causes kernel panic
 - Data log channel "Fuel: Injection timing SOI" indicates correct values
 - Flex sensor input
 - odd fire engines do not appreciate wasted mode on spinning #5715

## October 2023 "Day 591"

### Breaking Changes
 - Simplified VVT single tooth cam modes. "Single tooth second half" and "single tooth first half" have been replaced by simply "single tooth". This may require re-setting your trigger and VVT timing offsets if you used "single tooth first half" prior to this change.
 - "Single Tooth" renamed to "Half Moon", more proper "Single Tooth" crank trigger as a new trigger #5488

### Added
 - Latest jSerialComm 2.10.3
 - Working disconnect button in Console #4862
 - Preset for Mpxh6300 MAP sensor #5481
 - Renault F 60-2-2 trigger wheel #5513
 - AEM WBO EGO preset
 - Mitsubishi 4G69 Cam pattern #5541
 - Preset for Mazdaspeed Miata (NB) IAT sensor
 - option to Suppress start button on start up #5578

### Fixed
 - simulator: sensor mocking UI #5473
 - Custom Trigger Tooth with 1+0 causes endless loop #5471
 - nucleo-h7 bundle st-link flushing #5486
 - 2JZ cam trigger configuration is much easier to adjust #5557
 - lua reads stale output values #5579
 - High Pressure Fuel Pump scheduling goes into negative range #5596

## July 2023 Release "Day 518"

### Added
 - Dashpot for return-to-idle from coasting #5303
 - injector duty cycle limited at 50% for GDI engines #5416
 - STM32Cube v2.14 #5418
 - turning coils and injectors off in case of extremely unexpected errors #5429
 - Configurable ignition timing limits, adding safety particularly for engines running wasted spark with a large cam FOME#211

### Fixed
 - fuel_pump_control is accessible as getOutput("isFuelPumpOn") in Lua #5239
 - start button logic does not crank started for first X seconds since ECU boot #5324
 - launch control: Speed threshold should not depend on launch mode, also zero speed limit handling #5419
 - hard fault due to misaligned big buffer on F7 firmware #5354
 - Pointing at https://adoptium.net/download/ as JRE source

### Breaking Changes
 - Always operate in "two wire" mode for batch fuel, fixing batch firing order #5353
 - rusEFI console requires java 11

## June 2023 Release "Day 466"

### Breaking Changes
 - LCD & joystick support removed
 - CJ125 support removed
 - Single value to be used in event of a failed MAP removed, fallback table is the only option now

### Added
 - Detected gear and wastegate position to CAN format
 - Pause closed loop fuel after cuts
 - More trigger UI tweaks
 - Lua memory leak detection and prevention
 - Support gauge fuel pressure sensors
 - Better idle VE table transition
 - Detected gear available as gppwm channel
 - Y axis override for Idle VE table. Helps with idle quality on ITB setups.
 - Blend tables for boost open and closed loop control
 - Boost Cut Pressure Hysteresis #4529
 - Enable knock sensing on F407 Discovery build, hardcoded to PA3 #5254

## April 2023 Release - "Day 419"

### Added
 - Rearrange & simplify trigger configuration UI #5077
 - Selectable CanBus channel for WBO inputs #5028
 - STM32Cube v2.13 #5135
 - Native (modern, fancy, digital) SENT TPS input #5079
 - Allow k-line as a runtime option on default build
 - microRusEFI: Indicator for injector hardware issues #5189

### Breaking Changes
 - rusefi_default_bundle.zip was replaced by rusefi_bundle_f407-discovery.zip #5078

### Fixed
 - Antilag switch pin mode was ignored #5036
 - 50/83.3/125kbit CAN #5072
 - console creates new log file every 100 ms #5112
 - CAN bus AFR value inverted #5160
 - Subaru EZ30 tdcPosition is now 240
 - ETB control improvements hellen boards #5196
 - Require either analog AFR or CAN AFR input not both #5226
 - Ignition Knock response logging progress
 - sensor checker is not checking sensors #5232

## February 2023 Release - "Day 346"

### Breaking Changes
 - Change ignition IAT correction table to use IAT/load #4948

### Added
 - Electronic throttle supply voltage compensation, giving more consistent behavior as battery voltage changes #4838
 - VR trigger input oscilloscope for boards with "discrete VR" hardware (AlphaX ECUs, some Hellen) [#4885](https://github.com/rusefi/rusefi/issues/4885)
 - Jammed ETB detection #4873
 - RPM correction/multiplier for Accel Enrich #4760
 - Tooth logger writes to SD card #4897
 - Antilag #2403
 - Lua 'setIgnDisabled' function for all kinds of cranking safety systems
 - STM32Cube v2.12
 - detect and ignore doubled trigger edges #4656
 - Additional fuel and timing correction gauges in Tunerstudio #4955
 - Default bundle no longer initializes UART TTL connectivity by default #4998

### Fixed
 - multi-line tooltips in TS #4927
 - Negative temperature values for IAT Corr table #4941
 - DC wastegate #4965
 - [LSE] RTC (SD log dates) #4556

## December 2022 Release - "Day 289"

### Breaking Changes
 - SD logging rate is now set with hz instead of ms period #4553

### Added
 - VVT minimum RPM setting #4545
 - Fuel/spark cut codes to TS indicators #4592
 - Flexible ignition and VE adder/trim tables #4586 #4640
 - Enforce board configuration overrides more strictly #4614
 - rusEFI console Startup Frame should scan for available hardware #4633
 - Don't fire the engine without the ignition on (avoids USB keeping engine alive after ignition off) #4474
 - Lua: function to access VIN setting #3967
 - Lua: designated Lua gauges with logging #4672
 - GCC11 is the suggested compiler now, GCC9 no longer supported #4680
 - Add fuel adder to launch #4777
 - 3-cylinder Benelli trigger
 - ETB intermittent sensors protection #4832
 - ETB output average and rate of change gauges #4833
 - Alternator Duty Cycle Adder for AC Fan #4973

### Fixed
 - Fuel Priming reset fix #4627
 - Slower than expected RPM information was slowing engine start #4629
 - Fix 36-2-1 trigger (Mitsubishi 4B11, etc) #4635
 - Improve injection phase table defaults #4808

### Removed
 - Idle timing deadzone #4729
 - "Use only rising edge" trigger option - triggers now select automatically based on trigger type #4621

## September 2022 Release - "Day 203"

### Added
 - Time delay before DFCO #4292
 - Idle coasting position table is now RPM-based, instead of CLT-based #4487
 - Manual electronic throttle synchronization #3680
 - Delay before enabling AC compressor #4502
 - Require full sync for odd cylinder count #4533
 - Hysteresis on some fuel cuts to avoid engine damage #4541
 - Engine Sniffer into TS data logs #4535

### Fixed
 - Inverted vvt control #4464
 - Lua canRxAdd bus parameter handling #4467
 - STM32Bootloader driver install from console requests elevation #4313
 - VQ trigger gaps improvement #4469
 - Injection Offset for skipped wheel triggers #4536
 - Randomly hanging for a complete second #4557

### Removed
 - Alternator on/off mode #4548

## August 2022 Release - "Day 170"

### Added
 - raw voltage gauges for Lua script aux analog inputs #4346
 - microRusEFI and Proteus F4 have CAN OpenBLT bootloader #4199 #4230
 - counter-noise measure for custom skipped trigger wheels #4375
 - Write hard faults to backup ram, print on next boot #4324
 - Many more options for Lua CAN rx filters/callbacks #4387
 - Password protection against tune access #4243
 - Additional CAN messages #4401
 - Option to invert VVT control (exhaust cams, etc) #4424
 - Raw Battery gauge
 - More accurate/stable VVT angle calculation #4433

### Fixed
 - Lua CAN reception fixed for 11-bit IDs where the frame would be received, but a corrupt ID was passed to the handler function. #4321
 - Many drop downs menues are now sorted #4339
 - rusEFI TS plugin launcher fixed
 - Console autoupdate error dialogs #4352
 - custom skipped wheel could be located on camshaft #4377
 - it's impossible to receive AcceleratorPedal sensor via CAN/Lua #4369
 - MRE stepper disables LS2 control #4381
 - VVT target tables allow negative values #4424

### Removed
 - ICU trigger input logic since it is unused in any current ECU #639
 - Idle PID "offset" field, as this role is filled more effectively by the various open loop parameters.

## July 2022 Release - "Day 130"

### Added
 - verbose trigger sync should reuse engineSnifferRpmThreshold #4259
 - Subaru EZ30 variation of 36/2/2/2 trigger
 - Linux version of rusEFI simulator SocketCAN integration (#4312)

### Fixed
 - Improved logic used to disambiguate trigger sync using cam/VVT information. Engine now runs in wasted spark until cam sync is achieved, at which point it switches to fully sequential. #4099
 - visiting "live data" tab causes fatal error on ECU #4271
 - SD card logs are broken by double fuel: wall correction #4291
 - RPM-related tune fields can now hold larger values for higher rev limits, etc #4298

## June 2022 Release - "Day 98"

### Added
 - FasterEngineSpinUp should be enabled by default #4094
 - ETB low limit is now configurable #4119
 - Advanced Trigger option: require engine phase synchronization in sequential mode #4151
 - Maintainability: frequency sensors need a simple event counter in the logs #4173
 - TS bench test commands are now configurable #4192
 - microRusEFI can now use more pins for Cam position sensor #3032
 - hard requirement for console version match #4187

### Fixed
 - Make 36-2 trigger sync parameters more generous #4132
 - Synchronized Phase handling fixes #4099

### Removed
 - Various Honda 24/4/1 trigger modes (that didn't work very well), replaced with "12 tooth crank" mode.

## April 2022 Release - "Day 58"

All notable user-facing or behavior-altering changes will be documented in this file.

### Fixed
 - An attempt to make 'Trigger' dialog a bit less confusing #4021
 - Fixed knock threshold table, improved knock sensing status gauges

### Added
 - Mitsubishi 36-2-1-1 trigger wheel
 - Mitsubishi 3A92 VVT trigger
 - Toyota 4-1 VVT trigger
 - use extra RAM for lua if your microRusEFI unit has stm32f42x microcontroller
 - VVT activation delay #3443
 - Allow setting injection phase by start or center of injection #4068
 - New 'View' menu in TunerStudio showing a lot of live data points #3614

## March 2022 Release - "Day 22"

### Added
 - Use board-specific output and digital input pin names in error messages #3886
 - Support LPS22 in addition to LPS25 baro sensors #3900
 - Proteus F7 flashing work-around

### Fixed
 - SD card logging with SDIO hardware #3873
 - rusEFI console stability improvement #3912
 - console updater does not work if folder name contains spaces #3927
 - SD card logs PPS as TPS #3943

## Removed
 - Miata NB1 trigger was merged with Miata NB2 trigger
 - FSIO

## January 2022 Release - "Green Juice"

### Added
 - Per-cylinder fuel trim tables
 - Per-cylinder ignition timing trim tables
 - SDRAM reference binary for stm32f429-discovery

### Fixed
 - rusefi_Console: Too long re-connection time #3305
 - Bosch Quick Start on B6 Passat is confused about phase #3812

## December 2021 Release - "Cookie Exchange"

### Added
 - Improved vehicle speed sensor configuration: now uses real physical constants about tires, gear ratio, sensor, etc.
 - Improved priming logic. Now includes a table of priming fuel mass vs. engine temperature, in addition to a delay before priming to allow fuel pressure to build. #3674
 - ISO-TP connector in firmware & ISO-TP to TCP/IP bridge in rusEFI console #3667
 - Lua: mcu_stop method for stm32 F4 and F7

### Fixed
 - Faster engine sync + startup on engines with crank-speed primary trigger

### Added
 - Lua 'Format' button

## November 2021 Release - "Loosen Up Lighten Up Day"

### Added
 - Higher Data Rate with TS 3.1.06.14 Beta or newer
 - turbo speed sensor #2935
 - A TON of Lua progress
 - Overall performance
 - Knock retard #3396

### Fixed
 - A/C control bugfix
 - Better Windows build-in DFU #3338
 - Proteus now waits for VBatt to enable main relay output

## Removed
 - Remove engine load acceleration enrichment #3357
 - FSIO removed from user interface #2928

## October 2021 Release - "Pierogi Day"

### Added
 - Ford PIP trigger decoder https://wiki.rusefi.com/Images/triggers/trigger_65.png
 - Dwell battery voltage correction table
 - rusEFI firmware feature should be honest about it's Windows-only limitation
 - Bosch MM5.10 accelerometer integration
 - CLT/IAT sensor calibration presets

### Fixed
 - Fix of 36/2/2/2 attributes to help rotary #3242
 - SD card doesn't work on H7
 - rusEFI console to validate current bundle against bundle already in ECU #3266
 - GPPWM usability: notes field #3270
 - overdwell protection #3220

## September 2021 Release - "Iguana Awareness Day"

### Added
 - RUSEFI MSD now contains bundle-specific URL #2848
 - Nissan 4 cylinder QR trigger wheel #3118
 - 1-6-2-4-3-5 firing order for all our 911 fans!
 - Add minimum MAP threshold for closed-loop boost control
 - rusEFI console Lua tab loads scripts from ECU on start

### Fixed
 - Composite Logger uses same engineSnifferRpmThreshold setting as engine sniffer #3161

## Removed
 -  TS file read feature #3004

## July 2021 Release "Ice Cream Day"

### Breaking Changes
 - vvtOffset field migrated to four vvtOffsets fields. Anyone using VVT would need to manually adjust their configuration.

### Added
 - Injector nonlinearity (small pulse) correction - so far just polynomial, but table modes coming soon.
 - 1-4-3-6-2-5 firing order for VAG v6
 - vq30de trigger wheel decoder #2974
 - new hardware testing console commands #2978

### Fixed
 - rusEFI console start-up speed #2964
 - wrong handling of symmetrical crank wheels NB2 VQ35 Renix #2980

## June 2021 Release "National Logistics Day"

### Fixed
 - Improved cranking defaults
 - Simplified/clarified idle and cranking TunerStudio pages
 - Minor change of Miata NA trigger shape #2855
 - Improved trigger sync for 60/2 36/1 36/22 triggers as part of VW warnings on cranking part #1 #2861

### Added
 - Idle improvements! Open loop corrections are now applied in closed loop mode.
 - Electronic throttle rev limiter: close the throttle before hitting hard limit, OEM style.
 - "inhibit start until oil pressure" prevents starting the engine with no/low oil pressure #2799
 - Dual cooling fans, with new "enable fan with AC" option
 - Lua scripting: implement custom ECU behavior with a Lua script. https://wiki.rusefi.com/Lua-Scripting

# 2021 May "Piercing Day"
### Fixed
 - LCD screen works again #2576
 - Support STM32F7 1MB devices
### Added
 - Per-bank closed loop fuel correction (each cylinder corrected by the sensor that sees its exhaust)

## April 2021 Release - "Malbec"
### Added
 - mount SD card and drive containing TunerStudio ini at the same time
 - VAG MQB tachometer CAN integration
### Fixed
 - fix error/warning buffer overflow #2456
 - engine sniffer "show only interesting" fix #2528
 - SD card logs bugfix #2556
 - Too many small logs are zero size #2553
 - Launcher splash screen is freezy #2560
 - NB2 trigger decoder #2533

## April 2021 Release "Be Kind to Lawyers Day"
### Added
 - mount SD card and drive containing TunerStudio ini at the same time
### Fixed
 - engine sniffer "show only interesting" fix #2528

## Potato Chip Day 2021 Release

### Added
 - VAG MQB tachometer CAN integration

### Fixed
 - fix error/warning buffer overflow #2456

## Life in the Cracks 2021 Release

### Added
 - USB Mass Storage: The connected SD card will be mounted over USB if rusEFI connected to a PC via USB.
 - GM 60/2/2/2 trigger https://wiki.rusefi.com/All-Supported-Triggers#gm
 - TriTach trigger https://wiki.rusefi.com/All-Supported-Triggers#tritach
 - Skoda Favorit trigger https://wiki.rusefi.com/All-Supported-Triggers#skoda-favorit
 - Add fallback logic handling failed MAP sensor.  In case of failed MAP, ses either a fixed MAP value, or a table that estimates MAP based on TPS and RPM.
 - STM32H7 is mostly working

### Fixed
 - Improved fuel/ignition bench testing precision - now accurate to ~1 microsecond #2440
 - Fix internal MCU temperature sensing on STM32F7-based ECUs #2392

### Breaking Changes
 - VVT target is now taken from VVT not FSIO table #2243

### 2021 Printing Ink Day

### Breaking Changes
 - Closed loop idle timing behavior changed to no longer fall off control as entering/leaving the idle region. It now sharply engages/disengages upon entering/leaving the idle area.
 - Idle phase logic uses the same idle detection thresholds as the main idle controller instead of its own thresholds.

### Added
 - Main Relay bench test button

### Fixed

### 2021 Orthodox Christmas Day

### Breaking Changes

### Added
 - Second wideband O2 processing as channels "Lambda 2" and "Air/Fuel Ratio 2".  Currently only supports CAN sensors, but analog will be supported too soon.
 - Basic fueling-only flex fuel implementation. Automatic adjustment of stoichiometric ratio based on ethanol content, compatible with the common GM/Continental 50-150hz flex fuel sensor 🎉 🎉 🎉

### Fixed
 - Simplify idle control dialog in TunerStudio
 - microRusEFI outputs are in trouble on configuration change
 - CLT gauge no longer erroneously shows "deg F" by default.

### December 2020 Bake Cookies Day

### Breaking Changes
 - GM x24 trigger now uses 0 offset (offset is now built in, previously needed 50 degrees offset)

### Added
 - DAIHATSU 3/4 cylinder trigger https://rusefi.com/images/triggers/trigger_6.png
 - Ford ST170 VVT cam trigger https://rusefi.com/images/triggers/trigger_42.png
 - Changed scheduling timer driver to improve scheduling accuracy #1983

### Fixed
 - microRusEFI GP3 & GP4 fixed

## December 2020 National Mutt Day

### Fixed
 -  minor bugfix - engine sniffer missing pin names #1995

## November 2020 Release - "Black Friday"

### Breaking Changes
 -   "AFR target" table is now "Lambda target" table.  It should convert itself correctly, but a backup of that table is recommended when updating firmware/ini.
 -   "Base cranking fuel" is now in milligrams, using the full injector model at all times now.  60mg per liter per cylinder is a good starting point

### Added
 -   High & low pressure fuel sensor channels
 -   Electronic throttle-style wastegate control found on some VW turbos (and VGTs)
 -   Improved TLE8888 driver on microRusEFI
 -   Improved setting ECU presets/defaults from TunerStudio

### Fixed
 -   Improved TunerStudio protocol reliability - should see fewer CRC errors now #1943

## October 2020 Release - "Sausage Pizza Day"

### Added
 -   `iat` as FSIO variable to read intake air temperature
 -   [ALPHA] Software based knock detection on Proteus v0.4+.  Possible on microRusEFI and Frankenso too with hardware modification.
 -   [ALPHA] Early framing work for automatic transmission control!

### Fixed
 -   Fixed fuel pump logic
 -   Improve trigger input/decode and PWM performance
 -   Fixes to SD card logging


# Old Changelog

| Release date  | Revision  | Details |
| ------------  | --------- | ------- |
| 07/26/2020    | r24635    | improvement #1637: DC motor idle air valve for late 90s German vehicles |
| 07/21/2020    |           | bugfix #1592 injectors could stay open on transition from cranking to running under certain conditions |
| 06/17/2020    | r23656    | bugfix #1491 major performance/scheduling improvement |
| 05/21/2020    | r22961    | rusEFI console start-up time improvements |
| 04/18/2020    | r22231    | Renix 44-2-2 trigger support added |
| 04/02/2020    |           | Start button feature |
| 03/28/2020    |           | Critical error text is now displayed in TunerStudio |
| 03/26/2020    |           | Multi-spark feature |
| 09/05/2019    | r19849    | Electronic Throttle Body including idle control seems to work |
| 07/28/2019    | r19612    | improvement #809: software jump to DFU |
| 04/25/2019    | r17317    | bugfix #775: electrical noise reboot during settings change causes with full tune loss |
| 04/07/2019    | r17098    | improvement #714: TLE8888 Enable Outputs SPI integration |
| 02/27/2019    | r16886    | bugfix #698: concurrency defect with single timer executor initialization |
| 02/23/2019    | r16857    | improvement #688: better support for 144 and 176 pin packages |
| 02/05/2019    | r16713    | improvement #631: ChibiOS 18 |
| 01/11/2019    | r16346    | bugfix #663: SPI fixes for CJ125 for stm32 errata STM32f405/7/15/17 |
| 12/09/2018    | r16057    | electronic throttle body control is now BETA version |
| 08/19/2018    | r15811    | bugfix #604: no interpolation outside of the table |
| 01/29/2018    | r15514    | improvement #215: CJ125 wideband controller |
| 01/23/2018    | r15442    | improvement #463: ChibiOS 17 / 4 |
| 01/07/2018    | r14952    | usability & minor bugfix #532 idle defaults are broken |
| 12/17/2017    | r14892    | bugfix #513: console: erase on older chips does not work |
| 11/26/2017    | r14789    | bugfix #500: set_rpn_expression command is broken |
| 11/23/2017    | r14779    | bugfix #497: engine does not start with SD card |
| 11/19/2017    | r14766    | improvement #496: console flasher to support older boards |
| 11/19/2017    | r14760    | improvement #495: incompatible change - larger settings area |
| 11/06/2017    | r14733    | improvement #487: single-point injection mode |
| 10/26/2017    | r14700    | improvement #385: narrow band to wide band conversion table |
| 08/31/2017    |           | improvement #442: ADC_VCC should be configurable |
| 07/24/2017    | r14513    | bugfix #307: TS bench test blinks three times |
| 07/13/2017    | r14476    | bugfix #461: SAXParserException on console start-up due to damaged settings.xml file |
| 07/09/2017    | r14473    | improvement: IAC solenoid frequency changeable on the fly |
| 06/19/2017    | r14393    | bugfix: pinMode, milMode |
| 05/27/2017    | r14221    | improvement: ochGetCommand to support offset and count |
| 05/05/2017    | r13974    | bugfix #404: 36/1 FATAL error: angle range trgSync |
| 05/03/2017    | r13967    | improvement: ChibiOS 3.2 |
| 04/06/2017    | r13759    | major improvement #72: ChibiOS 3.1 |
| 03/26/2017    | r13330    | super annoying bug #336 fixed |
| 03/20/2017    | r13233    | improvements #375 & #376: hard FPU mode & migrating to fresh version of arm gcc |
| 03/19/2017    | r13223    | bugfix #374: persistent configuration starts to touch firmware - settings would need to be reloaded |
| 03/09/2017    | r13146    | bugfix #370: duty cycle vs error code 6050 |
| 03/06/2017    | r13123    | bugfix #363: 2/1 skipped wheel is a corner-case |
| 03/05/2017    | r13108    | bugfix #363: trigger front only processing |
| 02/22/2017    | r12980    | bugfix: false error message in case of single coil or simultaneous injection |
| 02/22/2017    | r12973    | unused property 'custom Use Rise Edge' removed |
| 02/22/2017    | r12972    | protocol signature changed to 'v0.02' (this would happen from time to time to ensure version match between console and TS project |
| 02/20/2017    | r12939    | https://github.com/rusefi/rusefi is now the official primary repository |
| 02/18/2017    | r11565    | improvement: level1 default brown out https://sourceforge.net/p/rusefi/tickets/354/ |
| 02/18/2017    | r11554    | bugfix: rusEfi console program/erase buttons fixed with ST-LINK 2.1 |
| 12/09/2016    | r10991    | bugfixes, bugfixes, bugfixes & release 1.0
| 02/15/2016	| r9600     | automatic warm-up fuel correction, performance improvements
| 08/31/2015	| r8725     | acceleration fuel correction, console improvements
| 07/06/2015	| r8554     | more CAN, better console, bugfixes, performance
| 04/26/2015	| r7938	    | vehicle speed sensor, stepper idle valve, lots of improvements
| 11/13/2014	| r5327	    | ChibiOS/RT 2.6.6, better cold start logic, bugfixes, bugfixes, bugfixes...
| 06/12/2014	| r3477	    | Precise event scheduling, protocol selection
| 03/26/2014	| r2413	    | C++, refactoring & improvements
| 02/23/2013	| r1777	    | trigger is now configurable via TunerStudio
| 01/30/2014	| r1309	    | true trigger angles, VBatt signal
| 01/12/2014	| r1007	    | refactoring, refactoring & refactoring
| 12/19/2013	| r605	    | ignition control & a little bit of CAN bus
| 11/08/2013	| r100	    | Tuner Studio fuel map tuning, fuel pump control
| 10/14/2013	| r39	    | USB serial bug, missing IAR files, self-contained Makefile
| 10/13/2013	| r33	    | IAR project file
| 10/04/2013	| r26	    | Patched ChibiOS/RT 2.6.1 sources are now included
| 09/23/2013	| r20	    | Tuner Studio integration, configuration persistence
| 08/30/2013	| r14	    | initial documentation & refactoring. tunerstudio integration
| 08/03/2013	| r13	    | wideband O2 input, better idling algorithm, serial-over-USB
| 07/05/2013	| r10	    | Second CKP, sequential injection.
| 06/19/2013	| r9	    | Initial version - batch injection & ignition with advance table lookup.
