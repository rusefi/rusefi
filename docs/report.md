# Work Report

## 2026-08-14 - m74_9: IMMO OFF flag landed + confirmed against original ECU

Added the `m74_9ImmoOff` configuration bit and confirmed that the original ECU
silences the IMMO CAN exchange when the immobilizer is disabled in calibration.

What was done:

| Change | File |
| --- | --- |
| Added `bit m74_9ImmoEnabled` and `bit m74_9ImmoOff` to persistent config | `firmware/integration/rusefi_config.txt` |
| IMMO state machine uses both bits; no trigger/response when disabled | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Updated analysis doc with IMMO OFF confirmation and build fix note | `docs/m74_9_immo_analysis.md` |

Key findings:

- **IMMO OFF dump proves 0x08074BF9 is the hardware enable flag.**
  With `0x08074BF9 = 0x00` the original ECU never sends `0x0713` and ignores `0x0714`.
  BCM starts the engine without authentication.

- **rusEFI `m74_9ImmoOff` replicates this behavior.**
  `isImmoEnabled()` returns `m74_9ImmoEnabled && !m74_9ImmoOff`.
  When false, the state machine stays in `Idle`, does not arm the trigger timer,
  and discards incoming `0x0714` frames. `m74_9_isImmobilizerBlocking()` is also gated
  by the same condition, so LimpManager does not cut fuel/ignition.

- **Build error "no member named m74_9ImmoEnabled" was a stale generated header.**
  `engine_configuration_generated_structures_m74_9.h` had not been regenerated after
  the `rusefi_config.txt` edit. `touch firmware/integration/rusefi_config.txt` followed
  by `make clean` regenerates the header and the build passes.

Validation:
- Verified `m74_9ImmoEnabled` and `m74_9ImmoOff` appear at offset 15768 bits 11/12 in
  `engine_configuration_generated_structures_m74_9.h`.
- Verified `m74_9_can.cpp` references both bits in `isImmoEnabled()` and
  `m74_9_isImmobilizerBlocking()`.
- No firmware build run in this session (user builds with `./compile_m74_9.sh`).

Open follow-ups:
- User to build and flash; verify engine starts with `m74_9ImmoOff = yes`.
- Continue reverse-engineering the `0x0713` trigger generator and `computeImmoResponse()`
  algorithm for users who want to keep IMMO enabled (`m74_9ImmoEnabled = yes`).

---

## 2026-08-14 - m74_9: Static binary analysis - 0x0713 crypto significance + SLib call map

Python static analysis of the full 4 MB flash dump (`find_trigger_gen.py`).

What was done:

| Change | File |
| --- | --- |
| New static analysis script | `find_trigger_gen.py` |
| Updated analysis: corrected BLX 0x082078D4, new SLib call map, trigger crypto hypothesis | `docs/m74_9_immo_analysis.md` |

Key findings:

- **0x0713 trigger IS crypto-significant.** BCM returns Frame1[0] with bit 0x40 (RESPONSE
  format) for ALL 6 original-ECU triggers (0x66, 0xCF, 0x4A...) but NOT for rusEFI's
  fixed counter (`0x26`, no 0x40 bit). Probability of 6/6 coincidence ~1.5%.
  BCM validates the trigger content before sending a proper challenge.

- **CORRECTED: BLX R3 at 0x082078D4 does NOT call SLib.**
  The LDR before it loads 0x08209A60 (a wrapper-internal function pointer table).
  That table contains 8 wrapper addresses (0x08205F59...0x082062E9),
  all in 0x082xxxxx (flash attestation functions). The breakpoint is useful
  for catching flash-attestation calls but will NOT reveal a SLib crypto address.

- **36 direct BL/BLX-to-SLib calls found in main firmware (0x080xxxxx).**
  None are in the IMMO wrapper (0x082xxxxx). SLib is called via Thumb2 immediate-offset
  BL (F000+F8xx encoding), not via SRAM function pointer tables.
  All 36 call sites identified; 36 different SLib entry points.

- **Most likely trigger generator: 0x08069028 and 0x080697D0** (both in same 4 KB page).
  These are the only SLib calls near CAN-related code, calling SLib 0x081F102A
  and 0x0819AFEE respectively.

- **IMMO response (flash attestation) does NOT require SLib.** The whole computation
  chain (FUN_08201E2C -> FUN_0820630C -> FUN_08206108 -> FUN_082056D4) lives in the
  readable wrapper. If the algorithm is indeed flash-attestation, rusEFI can implement
  it without calling SLib at all (provided SLib is preserved at 0x08100000-0x081FFFFF
  so the CPU reads real flash content, not 0xFF).

- **SLib call hotspot: 0x08089xxx-0x0808Cxxx (15 calls to 0x08109xxx-0x0810Dxxx).**
  Likely a crypto engine (AES/HMAC) used for something other than IMMO protocol.

Validation:
- Python script confirmed: MOVW Rn, #0x713 = NOT present in flash.
  CAN ID 0x713 is encoded in a message descriptor table or passed via HAL parameter.
- Shifted CAN ID 0x0713 (0xE2600000) = NOT in literal pools.
- Shifted CAN ID 0x0350 (0x6A000000) = FOUND at 3 locations in main firmware,
  confirming that BCM-keepalive sender uses literal-pool CAN IDs while IMMO sender uses tables.
- 0x08209A60 table decoded: 8 function pointers, all in 0x082xxxxx wrapper range.

Open follow-ups:
- Set breakpoints at 0x08069028 and 0x080697D0 on original ECU to catch
  SLib trigger-generation calls. Read R0..R3, LR before BL; read SRAM output after return.
- Alternatively: connect BCM on bench -> natural exchange -> catch trigger gen in
  SLib call sites above.
- Verify flash-attestation hypothesis: parse known challenge Frame1+Frame2 bytes to
  extract flash_start/flash_size parameters; read those flash regions in dump;
  check if result matches the known 8-byte response.
- Add new script `verify_attestation.py` to test the hypothesis against all 5 pairs.

---

## 2026-08-14 - m74_9: SWD live debug session - timing/format of IMMO challenge exchange found

Extended SWD/OpenOCD investigation on the original Itelma I865LB52 ECU on the bench.
Goal: hit breakpoint at `0x082078D4` (BLX R3 in FUN_08207874, the SLib entry call)
to capture the address of the SLib crypto function.

What was done:

| Change | File |
| --- | --- |
| New bench PCAN trace captured without BCM | `stol.trc` |
| Auto-respond script for timed 0x0714 reply | `immo_respond.py` |
| Updated current-status section in analysis doc | `docs/m74_9_immo_analysis.md` |

Key findings:

- **0x0713 trigger is random per power cycle.**  The original ECU generates a fresh
  8-byte trigger (likely from the AT32F435 TRNG) every time it boots and sends it in
  the 0x0713 frame.  The BCM computes Frame1+Frame2 of the 0x0714 challenge from
  this trigger using an unknown algorithm.  rusEFI sends a fixed session-counter
  trigger; the two are not interchangeable.

- **Root cause #1 - timing.**  Analysed all 5 trigger->challenge pairs in
  orig_1/2/3.trc and ignon*.trc.  The BCM sends 0x0714 Frame1 within
  1.2-10.3 ms (avg 7 ms) of receiving 0x0713.  The ECU accepts the challenge
  only within this same window.  Manual PCAN-View clicking takes >100 ms -> always
  misses the window.  Fix: cyclic PCAN transmission (CycleTime=1/2 ms) or the
  `immo_respond.py` python-can auto-responder (responds in < 2 ms after seeing 0x0713).

- **Root cause #2 - wrong Frame1 format.**  User was sending Frame1 =
  `26 17 14 F0 94 E7 29 7F` (byte[0] = 0x26, no 0x40 bit).  This is what the BCM
  sends to *rusEFI*; it is a REQUEST-style frame.  The original ECU expects a
  RESPONSE-style frame (bit 6 of byte[0] set).  In all 5 known original-ECU pairs,
  Frame1[0] is 0x66, 0xCF, or 0x4A - all have the 0x40 bit set.  The original ECU's
  CAN ISR discards Frame1 without this bit, so FUN_082027A4 is never called.
  Correct data to use: Frame1=`66 0B E1 E2 A3 4B 81 40`,
  Frame2=`B4 56 33 A0 49 9A 01 EC` (pair 1 from orig_1.trc).

- **ECU does NOT validate challenge against its own trigger.**  FUN_08201E2C only
  checks key_type==1 and session_word==0xFF00/0xFF01 (from the SRAM buffer populated
  by the CAN ISR).  The 8-byte trigger in 0x0713 is sent outward to the BCM and is
  not stored for backward verification.  Any correctly-formatted 0x0714 (right
  byte[0] format) should be accepted regardless of which trigger triggered it.

- **0x0713 sent autonomously.**  stol.trc (bench, no BCM) shows the ECU sends
  0x0713 exactly once at t=9863.5 ms without needing 0x0350 from BCM.  Sending
  0x0350 is NOT required to get 0x0713.  After no response arrives the ECU stops
  retrying (within the trace window).

- **SWD direct call attempt (summary).**  Multiple attempts to call FUN_082027A4
  directly via OpenOCD (set PC, SP, LR, resume) failed due to: (a) being in Handler
  mode after HardFault, (b) FUN_08202038 not yet complete (SRAM[0x200002CA]=0,
  function pointer at SRAM[0x200010A0] not set), (c) watchdog resets during halt.
  Mitigation: `mww 0xE0042008 0x00001800` freezes IWDG while halted on AT32F435;
  must be written WHILE CHIP IS RUNNING (not after halt); wait >=5 s after reset
  before halting to allow init to complete.

Validation:
- `stol.trc` parsed: exactly 1 x 0x0713, 0 x 0x0714, 0 x 0x0350 confirmed.
- 5-pair timing distribution verified: min 1.2 ms, max 10.3 ms, avg 7.0 ms.
- Frame1[0] bit-pattern confirmed across all 5 original-ECU pairs (0x40 bit always set).
- No firmware build run (algorithm still unknown; stubs in place).

Open follow-ups:
- Change PCAN cyclic data to pair-1 format (`66 0B E1 E2 A3 4B 81 40` /
  `B4 56 33 A0 49 9A 01 EC`) and confirm BP at `0x082027A4` fires.
- If `0x082027A4` fires but `0x082078D4` does not: check SRAM[0x200002C8]
  (0 = SLib init path, 1 = flash-attestation path via FUN_0820630C).
- If no BP fires at all: investigate CAN ISR (set BP at `FUN_08206FB8` or
  watch SRAM[0x20000414] for writes after sending 0x0714).
- Cleanest option: connect BCM on bench (or test in car with JTAG) so the real
  BCM computes the correct Frame1+Frame2 for each trigger automatically.
- Once SLib entry address (R3 at 0x082078D4) is known, try `mdw <addr>` -
  readable -> disassemble; not readable -> capture input/output at BLX site.

---

## 2026-08-13 - m74_9: IMMO dead-lock fix + deep firmware reverse-engineering

Found and fixed the root-cause dead-lock that prevented the engine from starting
with rusEFI.  Also performed extensive static analysis of the original
I865LB52 firmware to understand the crypto algorithm.

What was done:

| Change | File |
| --- | --- |
| Fixed dead-lock: IMMO trigger fires on IGN ON (byte4!=0x00) not crank-switch (byte4==0x44) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Increased trigger delay from 100 ms to 1000 ms (original ECU sends at ~1.5 s) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Removed unused `crankSwitch` variable (would cause compiler warning) | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Rewrote "Current status" and "Next steps" in analysis doc | `docs/m74_9_immo_analysis.md` |
| Added analysis scripts | `emulate_immo.py`, `find_crypto_fn.py`, `trace_init.py`, `crypto_decode.py`, `deep_trace.py`, `find_algo_final.py`, `trace_fn6108.py`, `find_key_algo2.py`, `find_can_handlers.py` |

Key decisions and findings:

- **Dead-lock confirmed**: BCM never sets byte4=0x44 (crank-switch) until IMMO
  is cleared.  rusEFI was waiting for 0x44 to send the trigger.  BCM was waiting
  for the trigger before sending the challenge.  Nobody moved.  The dashboard
  shows correct lamp states because keepalive frames (0x0189 etc.) are correct,
  but the starter relay GND never closes.
- **Fix**: arm IMMO timer on any non-zero byte4 from BCM (= ignition on).  Trigger
  fires after 1000 ms, before user turns key to START.
- **Crypto algorithm**: Not cracked yet.  Proprietary, uses ADD/XOR/shift with a
  key from calibration area.  No standard constants (AES, SHA, XTEA, CRC32)
  found.  Async flash-attestation chain identified:
  `FUN_08201E2C` -> `FUN_0820630C` -> `FUN_08206108` -> `FUN_08205A3C` -> flash ISR.
- **CAN receive chain**: `CAN1_RX0_IRQ` -> `FUN_08207432` -> `FUN_08206FB8` ->
  per-frame handler via FMI index -> SRAM buffer at 0x20000C14.
- **Trigger format**: 8 random bytes; BCM accepts any content and responds.
  rusEFI's rolling-counter trigger is fine.

Validation performed:
- Traced PCAN captures: `rusefi2.trc` shows NO 0x0713/0x0714 frames (confirming
  the old code never sent the trigger).  `ignon.trc` shows trigger at ~1521 ms
  and successful full exchange on original ECU.
- Verified that `ignon_and_start.trc` pair 5 (challenge 66aaeef3... / resp efaa66f0...)
  matches `PAIRS16[4]` in the analysis scripts.
- No firmware build was run (macOS host, needs Linux ARM cross-compiler; user
  must build with `./compile_m74_9.sh` on Linux).

Open follow-ups:
- Deploy trigger fix -> capture new PCAN trace -> verify 0x0713/0x0714 exchange.
- Run Unicorn emulation with actual challenge from new trace to find the response.
- OR: SWD breakpoint at 0x082027FA on original ECU to intercept the algorithm.
- Implement `computeImmoResponse()` once algorithm is known.

---

## 2026-08-13 - m74_9: updated `docs/m74_9_immo_analysis.md` with implementation progress

Consolidated the current state of the m74_9 immobilizer work into `docs/m74_9_immo_analysis.md`. Added a new "Implementation progress in rusEFI" section documenting the landed state machine, logging, `m74_9ImmoEnabled` config bit, and `custom_board_isImmobilizerBlocking` LimpManager hook. Also added "Next steps and blockers" so the next session can pick up from the right place.

What was done:

| Change | File |
| --- | --- |
| Documented the implemented IMMO state machine, logging helpers, public accessors, and fuel/spark cut integration | `docs/m74_9_immo_analysis.md` |
| Documented the `m74_9ImmoEnabled` config bit and its default-off safety behavior | `docs/m74_9_immo_analysis.md` |
| Added next-step checklist: decompile response functions, port algorithm, build/test, decide on default | `docs/m74_9_immo_analysis.md` |

Key decisions:

- Keep `docs/m74_9_immo_analysis.md` as the single source of truth for both the original-firmware reverse-engineering notes and the rusEFI integration status.
- The response algorithm remains the only blocker; all infrastructure around it is in place.

Validation: Reviewed the generated header and TS ini to confirm `m74_9ImmoEnabled` and `ClearReason::Immobilizer` are present. No firmware build was run (still blocked on macOS host tooling; will build on user's Linux environment).

Open follow-ups:

- Run `./compile_m74_9.sh` on a Linux host to verify the current changes compile.
- In Ghidra, find the functions whose addresses are stored at `0x20001A50` and `0x20001A54` and decompile them.
- Implement `computeImmoResponse()` / `computeImmoQuickResponse()` once the algorithm is known.

## 2026-08-13 - m74_9: immobilizer dispatcher decompiled, literal pool mapped

Continued reverse-engineering the m74_9 immobilizer. The Ghidra decompilation of the dispatcher at `0x08203FFC` is now in `docs/m74_9_immo_analysis.md`. It is a two-case state machine:

- `param_1 == 0` -> calls the function pointer stored at RAM `0x20001A50` (likely the full 16-byte challenge response).
- `param_1 == 1` -> calls the function pointer stored at RAM `0x20001A54` (likely the quick 8-byte re-check response).

The literal pool at `0x08204080` was read from the full flash dump and maps to RAM addresses in the `0x20001Axx` region. The key constants `0x2548A4D2`, `0x4DF9123B`, `0x43A0C212`, `0xF9C74A52` are stored in the first flash bank at `0x080BB508` - `0x080BB577` and are copied to RAM `0x20001ADC` during initialization.

What was done:

| Change | File |
| --- | --- |
| Updated analysis notes with the decompiled dispatcher, literal pool mapping, and key-constant source addresses | `docs/m74_9_immo_analysis.md` |
| Extended `analyze_immo.py` to dump the literal pool and key-constant addresses for Ghidra orientation | `analyze_immo.py` |
| Added this report entry | `docs/report.md` |

Key decisions:

- The actual response functions are still not decompiled; they are the targets of the two RAM function pointers. The next work unit is in Ghidra.
- The second flash bank (`0x08200000`) must be mapped in Ghidra. If `0x08203FFC` is not visible, add a memory block at `0x08203F0C` or load the full 4 MB dump at `0x08000000`.

Validation: `analyze_immo.py` was run; it reproduced the 10 captured sessions and printed the literal pool / key-constant addresses. No firmware build was run (the crypto algorithm is still unknown).

Open follow-ups:

- Find the values stored at `0x20001A50` and `0x20001A54` in Ghidra and decompile those functions.
- Verify the decompiled algorithm against the captured challenge/response pairs.
- Port to `firmware/config/boards/m74_9/m74_9_can.cpp` and build/test.

## 2026-08-13 - m74_9: committed immo progress to local repo

Committed the current m74_9 immobilizer work as a single local commit (`04476901f44`). The commit includes the framework, the captured challenge/response test vectors, the `analyze_immo.py` helper, and the `docs/m74_9_immo_analysis.md` hand-off notes for Ghidra.

What was done:
| Change | File |
| --- | --- |
| Committed immobilizer framework + analysis + test vectors | `analyze_immo.py`, `docs/m74_9_immo_analysis.md`, `docs/report.md`, `firmware/config/boards/m74_9/*`, `firmware/controllers/limp_manager.*`, `firmware/hw_layer/board_overrides.h`, `firmware/integration/rusefi_config.txt`, `firmware/tunerstudio/tunerstudio.template.ini` |

Key decisions:
- No generated files or large binaries were committed. `immo_pairs.json`, `immo_code_region.bin`, and the original `.bin`/`.trc` files remain untracked.
- The next work unit is intentionally outside the repo: decompile the immobilizer response function in Ghidra and return with the C code for `computeImmoResponse()` / `computeImmoQuickResponse()`.

## 2026-08-13 - m74_9: immobilizer framework - logging, auth state and LimpManager integration

Analyzed two Largus tune files (IMMOON/IMMOOFF) and a full 4 MB flash dump from an m74_9 ECU with working immobilizer. The only functional difference between the two tunes is byte `0x074BF9` (`0x01` = immo enabled, `0x00` = disabled). The dump confirms this flag is in bank-1 flash calibration area and is copied to RAM `0x2000E25C`; the original firmware gates engine outputs through a function that checks this RAM flag.

Also extracted challenge/response pairs from PCAN `.trc` logs (`ignon.trc`, `ignon_and_start.trc`, `orig_1/2/3.trc`). The protocol matches the existing rusEFI state machine: ECU sends `0x0713` trigger, BCM replies with two `0x0714` challenge frames (16 bytes total), ECU answers with one `0x0713` response. Quick re-check uses a single `0x0714`/`0x0713` exchange. Simple XOR/DES/AES/MD5 models do not explain the captured responses, so the actual algorithm still has to be pulled from the original firmware (or bypassed if the BCM allows it with immo disabled).

What was done:
| Change | File |
| --- | --- |
| Added `isImmoAuthenticated()` to `M74_9BcmCanListener`, plus public `m74_9_immoAuthenticated()` / `m74_9_isImmobilizerBlocking()` accessors | firmware/config/boards/m74_9/m74_9_can.h, firmware/config/boards/m74_9/m74_9_can.cpp |
| Added hex logging for `0x0714` challenge frames (single + 16-byte), `0x0713` responses, and quick re-check, plus "IMMO: authenticated" state log | firmware/config/boards/m74_9/m74_9_can.cpp |
| Added `ClearReason::Immobilizer` and a board hook `custom_board_isImmobilizerBlocking`; when the hook returns true, `LimpManager::updateState()` cuts fuel and spark | firmware/controllers/limp_manager.h, firmware/controllers/limp_manager.cpp, firmware/hw_layer/board_overrides.h |
| Wired the m74_9 hook to `m74_9_isImmobilizerBlocking()` in board overrides | firmware/config/boards/m74_9/board_configuration.cpp |
| Added `m74_9ImmoEnabled` config bit to `rusefi_config.txt` and updated `fuelIgnCutCodeList` in the TS template; regenerated m74_9 config/ini | firmware/integration/rusefi_config.txt, firmware/tunerstudio/tunerstudio.template.ini, firmware/controllers/generated/* |

Key decisions:
- Default `m74_9ImmoEnabled` is `false` (bit-field default) so the ECU runs without immobilizer until the user enables it and a working `computeImmoResponse()` is in place. This avoids bricking the car while the crypto algorithm is still unknown.
- The LimpManager cut is dynamic: it reads the immobilizer state every fast callback, so once authentication succeeds the cut is removed automatically within 5 ms.
- The hook is generic (`custom_board_isImmobilizerBlocking`) so any other board with a similar external immobilizer can reuse it.

Validation: m74_9 config regenerated successfully (`gen_config_board.sh config/boards/m74_9 m74_9`); `m74_9ImmoEnabled` appears in the generated header and TS ini. Full firmware build could not be run locally: macOS `compile_m74_9.sh` fails on `realpath --`, and unit-test build fails on missing `flock`. User will build on his Linux environment.

Open follow-ups:
- Find the actual immobilizer response function in the original firmware (trace the CAN RX `0x0714` handler in Ghidra) and implement `computeImmoResponse()` / `computeImmoQuickResponse()`.
- Capture more challenge/response pairs and try statistical/crypto analysis if the firmware path stays hidden.
- Test on car whether disabling `m74_9ImmoEnabled` lets the BCM close the starter relay without a valid response.

## 2026-08-13 - m74_9: immobilizer analysis helper and captured test vectors

Continued reverse-engineering the immobilizer challenge/response. The original firmware immo module sits in the second flash bank around 0x08203F0C - 0x08204D00 (Ghidra base 0x08000000, ARM Cortex-M LE Thumb). Key functions identified: 0x08203FFC (immo dispatcher / state machine), 0x082047D0 (frame staging helper), 0x082048E8 (0x0714 receive handler), 0x08204B00 - 0x08204C00 (key constants / registration). The actual response computation still needs to be decompiled.

Added `analyze_immo.py` to parse all PCAN `.trc` files in the repo root and extract both full 16-byte challenge sessions and quick single-frame re-checks. The full sessions confirm the protocol: ECU `0x0713` trigger -> BCM two `0x0714` frames (16-byte challenge) -> ECU `0x0713` response. The quick pairs are single `0x0714` challenge -> `0x0713` response. All captured pairs are now saved to `immo_pairs.json` and embedded as test vectors in `m74_9_can.cpp`.

Also verified the IMMOON/IMMOOFF Largus tune difference: the functional flag is byte `0x074BF9` (`0x01` = enabled, `0x00` = disabled). The other four differing bytes (`0x07FFFC` - `0x07FFFF`) are the calibration-area checksum, as expected when one byte changes.

What was done:
| Change | File |
| --- | --- |
| Added `analyze_immo.py` to parse `.trc` logs, extract full/quick challenge-response pairs, save JSON, diff IMMOON/IMMOOFF, and dump the immo code region for Ghidra | `analyze_immo.py` |
| Updated `computeImmoResponse()` / `computeImmoQuickResponse()` stubs with captured test vectors and the correct original-firmware offsets to decompile | `firmware/config/boards/m74_9/m74_9_can.cpp` |

Key decisions:
- The response algorithm is still unknown; simple XOR/DES/AES/MD5 models do not fit the captured pairs. The next step is to decompile the relevant original-firmware functions (especially the dispatcher and any crypto helper called from it) in Ghidra.
- A helper script is the fastest way to keep adding real test vectors as the user captures more logs; once the algorithm is known, the captured pairs become unit-test inputs.

Validation: `analyze_immo.py` runs successfully and produces consistent pairs from all five `.trc` logs. No firmware build attempted locally; the changed code is comments/stubs only.

Open follow-ups:
- Decompile the immo response function in the original firmware and implement `computeImmoResponse()` / `computeImmoQuickResponse()`.
- Capture more sessions with different keys/keys to confirm the algorithm once a candidate is found.
- On-car test whether `m74_9ImmoEnabled = false` is enough for the BCM to allow starting without a valid response.

## 2026-08-12 - m74_9: ETB (TLE9201) revived - the missing piece was the ETC_EN enable chain, not SPI

Throttle now moves on the bench; user confirmed. The blocker from the previous session ("throttle does not move even though TLE9201 diag is clean") traced to the hardware enable: TLE9201 DIS (pin 11) sat at +5V, holding the bridge in tristate ("Outputs disabled", diag EN bit 0x80 = 0). The enable chain on the board is PB13 (ETC_EN) -> Q5A (MUN5311DW1 NPN, inverts) -> DIS (pulled up to +5V via R23), so the MCU-side polarity is ACTIVE-HIGH: PB13 high = Q5A on = DIS low = bridge enabled. PB13 was never driven (board.h leaves it a weak-pullup input).

What was done:
| Change | File |
| --- | --- |
| Drive PB13 (ETC_EN) high as plain GPIO in `m74_9_boardInitHardware()` (gpio_pin_markUsed + PAL_MODE_OUTPUT_PUSHPULL + palSetPad), right before `tle9201_add()` | firmware/config/boards/m74_9/board_configuration.cpp |
| Document in `setupEtb()` why `etbIo[].disablePin` is NOT used: the disable-pin path (DcHardware::start -> OutputPin::initPin(msg, pin)) is fixed OM_DEFAULT (pin low = enable), the inverse of what this board needs; assigning disablePin = B13 would drive DIS high (tristate) whenever the firmware thinks the bridge is enabled | firmware/config/boards/m74_9/board_configuration.cpp |

Key decisions:
- Static high on ETC_EN instead of a config field: there is no `disablePinMode` anywhere (checked `dc_io` in rusefi_config.txt), so inverting the disable pin would need a new config field plus full config regen - overkill for one board bring-up. Runtime disable is PWM=0, on which the TLE9201 coasts (real coast, not drive).
- Boot is safe by construction: PB13 weak pullup (~40k) against Q5A's internal 10k base-emitter divider gives ~0.66V at the base - below Vbe, Q5A stays off, DIS pulled high -> tristate until firmware drives PB13. Measured DIS=0.2V (Q5A Vce(sat)) after the fix.
- The earlier hypothesis (KiCad "DIS -> +3V3 via Q5A/R20") was wrong for the physical board: DIS is pulled to +5V (R23) and Q5A is a real NPN, not open-drain logic. Schematic netlist Y-positions were unreliable; trust datasheet pinout + user's 0-ohm measurements.

Validation (user on bench): DIS = 0.2V vs GND; TLE9201 diag transition 0x5C (Outputs disabled / Open Load, power-on tristate) -> 0xDF (No failure, EN bit set); `etbautocal` passes - throttle sweeps open/close, no "Auto calibrate failed", TPS1 closed/open voltages now differ.

Open follow-ups:
- On-car: confirm direction (if inverted, `stepperDcInvertedPins=true`, issue #4579), re-check TPS divider coefficients (analogInputDividerCoefficient=2.0 read 5.4V closed / 1V open on car), TPS2 warnings (P0223 Tps1Secondary too high, P2135 inconsistent - TPS2 divider may differ from TPS1), P2137 pedal inconsistent.
- The L9779 WDA -> Q5B -> DIS path is a redundant hardware kill (L9779 algorithmic watchdog); firmware feeds the watchdog fine (WDA ok>0 fail=0) and the path is likely depopulated ("not soldered" note near R20).

## 2026-08-11 - m74_9: fix event-scheduler death on firmware error (WDT TIM5 latch during ETB autocal)

User log (2026-08-11_22_47): during ETB TPS autocal the scheduler died - `WDT regs: cnt` still counting at 4 MHz, `dier 0x2 -> 0x0` (CC1IE cleared and never re-enabled), `isr`/`setHw` counters frozen, `pend=0`. Root cause found in code, not in a hardware race:

- `MicrosecondTimerWatchdogController` fires when `setHardwareSchedulerTimer()` has not been called for 2 s.
- `setHardwareSchedulerTimer()` had a `if (hasFirmwareError()) return;` gate (added 2019, #996 debugging). The STM32 ISR (`hwTimerCallback` in microsecond_timer_stm32.cpp) disables the compare notification (CC1IE) *before* invoking the callback, and `portSetHardwareSchedulerTimer()` is the *only* code that re-enables it. So any `firmwareError()` (e.g. the ETB autocal's "Auto calibrate failed" critical error) made the very next timer ISR the last one ever: the whole event scheduler died (soft PWM, watchdog buddy, all scheduled events) until power cycle. This is deterministic, not a race.

What was done:
| Change | File |
| --- | --- |
| Removed the `hasFirmwareError()` early-return from `setHardwareSchedulerTimer()`; documented the CC1IE invariant. Scheduler now always re-arms; engine safety after fatal error is already handled by `LimpManager::fatalError()` (cuts ignition/injection/ETB/trigger), which `firmwareErrorV()` calls - the scheduler gate was redundant for safety and only served to brick the ECU until reboot | firmware/hw_layer/microsecond_timer/microsecond_timer.cpp |
| ETB TPS autocal failure downgraded `firmwareError()` -> `warning()`: failing a bench calibration is a wiring/tune issue the user must fix, not an unrecoverable condition; the ECU keeps running and calibration can be retried without a power cycle | firmware/controllers/actuators/electronic_throttle_impl.h |

Key decisions:
- The 2026-08-09 `compareMinDelta=4us` clamp is orthogonal - that fixes compare-written-in-the-past; this fixes the firmware-error path that never re-arms.
- WDT diagnostic output stays useful: with the gate gone, the watchdog only fires on genuine scheduler breakage (clock gate, missed compare, stuck ISR).

Validation: not compiled here (user builds m74_9 himself). Static cross-check: `firmwareErrorV()` -> `getLimpManager()->fatalError()` (error_handling.cpp:716); `EtbController::setOutput()` gates on `allowElectronicThrottle()` (electronic_throttle.cpp:552).

Open follow-ups:
- On the car, the throttle still does not move even though TLE9201 diag is clean (0x5F "No failure" with motor attached). Candidates: DIS (TLE9201 pin 11) pulled to +3V3 via Q5A/R20 per KiCad (bridge held disabled - verify R20 populated and ETC_EN wired to the MCU), and PWM/DIR pins (PB14/PB15 are not in the HW-PWM table, so ETB runs on soft PWM; KiCad rev says PC0/PC1, which collide with PPS1/PPS2).
- Confirm `stepperDcInvertedPins` (#4579) once the motor actually turns.

## 2026-08-11 - m74_9: enable TLE9201 ETB H-bridge diagnostics on SPI2 (AT32F435)

Goal (user): bring the electronic throttle alive - the TLE9201 (U3) H-bridge was not wired to any SPI and had no driver in firmware. Per KiCad dump `m749_kicad.txt` the only L9779-to-TLE9201 link is the WDA_INT watchdog line (L9779 pin 38 -> Q5B -> DIS), so diagnostics must be driven directly from the AT32 via SPI2, not through the L9779.

What was done:
| Change | File |
| --- | --- |
| `DDEFS += -DBOARD_TLE9201_COUNT=1` (driver stub otherwise returns -1) and `-DSTM32_SPI_USE_SPI2=TRUE` | firmware/config/boards/m74_9/board.mk |
| SPI2 enabled: `is_enabled_spi_2`, pins PD1/PD3/PD4 (ETC_SCK/SO/SI, AF6 per `spi2_af` in at32_spi.cpp), `tle9201_cfg` on `&SPID2` with CS PD0 (GPIO, idle high), `tle9201_add(0, ...)` + PD0 pin claim in `board_init_ext_gpios()` | firmware/config/boards/m74_9/board_configuration.cpp |
| Wrap `STM32_SPI_USE_SPI2` in `#ifndef` in the AT32 mcuconf (was a bare `#define ... FALSE`, so the board.mk `-D` redefinition would warn -> error under `-Werror`, and worse, the mcuconf value would override the command line one, leaving `SPID2` undeclared). Same pattern as stm32h7/cfg/mcuconf.h and the existing USART1/USB guards in this very file. | firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h |

Key decisions:
- SPI2 pin/AF mapping verified against `at32_spi.cpp` `spi2_af` table: PD1 SCK AF6, PD3 MISO AF6, PD4 MOSI AF6 - exactly what the board needs (TLE9201 SO=3/SI=8/CSN=9/SCK=10 per user's pin list).
- CS handled the same way as the (working) L9779: GPIO push-pull output, idle high; ChibiOS `spiSelectI`/`spiUnselectI` in `SPI_SELECT_MODE_PAD` clear/set the pad (active low CSN) - no LLD work needed.
- `TLE9201_CONFIG_CR1/CR2` macros resolve on AT32: `device_mpu_util.h` comes from `hw_layer/ports/stm32/stm32f4` (CPU_HWLAYER for ARCH_AT32F4), where 8-bit mode is `0` (SPIv1 DFF=0).
- `etbFunctions[0] = DC_Throttle1` is already the default via `setDefaultEtbParameters()`; TPS1/PPS1 inputs and ETB PWM/DIR (PB14/PB15) were already configured, so no ETB-params change was needed.

Validation: not compiled here (user builds m74_9 himself; no arm-none-eabi-gcc on this host). Static cross-check done against at32_spi.cpp, SPIv1 LLD (SPID2 declared under STM32_SPI_USE_SPI2, RCC+DMA streams in spi_lld_start), spi.cpp getSckPin/initSpiModules and tle9201.cpp thread lifecycle.

Open follow-ups:
- After flash: expect console line `tle9201 Detected! (rev=...)` and periodic `tle9201 Diag` lines; `reset_9201` console command clears diag latch. If the thread stays silent, check that the saved MFS config has `is_enabled_spi_2=yes` and the three SPI2 pins set - stale saved settings override board defaults.
- If SPI2 still does not come up, next candidate is the DMA stream allocation: AT32 mcuconf uses `STM32_DMA_STREAM_ID_ANY` for SPI2, same as SPI1 (L9779) which works.
- Consider exposing ETB pin options (prepend.txt) and confirming the DIS disable-pin strategy with the watchdog line before finalizing the ETB control path.

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

## 2026-08-09 - m74_9 watchdog fix VALIDATED on hardware

What: The user rebuilt m74_9 on Windows with commit 1236c24c150 (compare re-arm
clamped >= 4us into the future against a fresh CNT read) and ran the ECU for
several minutes - no more `CRITICAL error: Watchdog: no events for 2s!`. The
previous firmware reliably latched after 12-20 s of uptime, so the race (stale
nowNt vs advancing CNT putting CCR1 in the past, equality CNT==CCR1 missed until
the 32-bit wrap) is confirmed fixed. No further WDT regs/hist output was produced,
which is the expected healthy behavior - the diagnostic prints only on latch.

Validation:
- User-reported on hardware (Windows host, PCAN-USB console): continuous run for
  several minutes, zero watchdog errors. This is the first on-target confirmation
  of the fix; earlier rounds were review-only on macOS.

Open follow-ups:
- Long-run soak (hours) and a powered-cycle test are desirable to make sure no
  rarer variant of the race remains; a fresh-flash boot was already covered.
- The WDT regs/hist diagnostics (commit 1371adf526f) are harmless when quiet and
  stay in place as a first-line tool for any future timer/lockup report.

## 2026-08-09 - TS-over-CAN link stability: ISO-TP flow control + desync recovery

Symptom (user log): `Got only 18 bytes while expecting 50 for command 0x43` and
`TunerStudio errors: outofrange=86/470` - multi-frame TS-over-CAN requests were
being truncated on m74_9 (CAN-only ECU, PCAN-USB host, 500k). 50 bytes = 1 FIRST
+ 7 CONSECUTIVE = exactly 8 CAN frames; only 18 bytes (6+7+5) assembled, so
frames were dropped and the ECU's ISO-TP state never recovered until the next
FIRST frame.

Root cause (three coupled problems):
1. The host (IsoTpConnector.sendStrategy) bursts FIRST + all CONSECUTIVE frames
   with no pause - `receiveData()` was an empty no-op - while the ECU's receive
   FIFO (CanTsListener.rxFifo, `CAN_FIFO_FRAME_SIZE=8`) holds only 8 frames and
   the TS thread may be busy sending the previous response, so a burst can
   overflow and `decodeFrame` silently drops.
2. A single dropped frame permanently desynchronized the ECU's ISO-TP state:
   CanStreamerState::receiveFrame / streamReceiveTimeout / IsoTpRx::readTimeout
   returned/aborted on index mismatch without calling reset(), so consecutive
   frames of the next message kept failing until a new FIRST arrived.
3. The Java decoder (IsoTpCanDecoder) treated the ECU's flow-control frame as a
   data packet (empty chunk) - noise, not a functional break.

What was done:
| Change | File |
| --- | --- |
| `CAN_FIFO_FRAME_SIZE` 8 -> 32 (headroom for host bursts; ~640 B static RAM on top of 384 KB, fine) | firmware/controllers/can/isotp/isotp.h |
| reset() on consecutive-index mismatch in receiveFrame | firmware/controllers/can/isotp/isotp.cpp |
| reset() when streamReceiveTimeout gets numReceived < 1 (lost/ignored frame) | firmware/controllers/can/isotp/isotp.cpp |
| reset() on desync/timeout in IsoTpRx::readTimeout (defensive; TS path uses CanStreamerState) | firmware/controllers/can/isotp/isotp.cpp |
| reset() now also clears the byte-level rxFifoBuf (drop stale leftovers) | firmware/controllers/can/isotp/isotp.cpp |
| PCanIoStream overrides receiveData(): wait up to 200 ms for the ECU FC frame (monitor + flag, reader thread signals on FC); FC frames are intercepted in readOnePacket and not fed to the decoder | java_console/io/src/main/java/com/rusefi/io/can/PCanIoStream.java |
| `receiveData()` documented as the FC-wait hook between FIRST and CONSECUTIVE; default stays no-op (SocketCAN/ELM327 unchanged) | java_console/io/src/main/java/com/rusefi/io/can/isotp/IsoTpConnector.java |
| public `ISO_TP_FRAME_FLOW_CONTROL=3` constant | java_console/io/src/main/java/com/rusefi/io/can/isotp/IsoTpConstants.java |
| New test: receiveData() is called between FIRST and CONSECUTIVE | java_console/io/src/test/java/com/rusefi/io/can/IsoTpConnectorTest.java |

Design notes:
- The FC wait is a monitor/flag pair, not a per-send CountDownLatch, because
  CountDownLatch cannot be reset. Writer thread waits in receiveData(); PCAN
  reader thread sets the flag + notifyAll on a frame whose type nibble is 3.
  Timeout falls back to the historical burst behavior (no regression if FC is
  lost). Both threads only touch the monitor under synchronization, so there is
  no lost-wakeup race.
- FC timeout 200 ms was chosen because the ECU sends FC immediately on FIRST
  (sendFlowControl, blockSize=0, stmin=0); the wait normally costs ~1 ms of
  latency per multi-frame write and paces the burst against the 32-frame FIFO.
- SocketCANIoStream keeps the default no-op receiveData() (Linux kernel path,
  same historical behavior) - flagged as a possible follow-up.

Validation:
- `./gradlew :ecu_io:test` BUILD SUCCESSFUL (JDK 11 toolchain via
  -Porg.gradle.java.installations.paths=...). New test passed; existing
  IsoTpConnectorTest / IsoTpCanDecoderTest untouched behavior.
- `cd unit_tests && ./test.sh testCanSerial`: 6/6 PASSED (TestCanStreamerState
  round-trips are index-consistent, so the new reset() paths are not hit there;
  macOS build needed a flock shim - `flock` does not ship on macOS - and the JDK
  11 toolchain env var for the gradle codegen steps).

Open follow-ups:
- User rebuilds m74_9 on Windows (firmware/config/boards/m74_9/compile_m74_9.sh)
  and re-runs the TS session over PCAN-USB; expect the `outofrange` counter and
  `not enough bytes in stream` errors to drop to zero.
- If truncation persists, the next suspect is host-side: PCanIoStream.decodePacket
  is still fed a 127-byte buffer for its assembly math even though the size-aware
  overload receives the real DLC - verify byte 0 of every 0x720 frame against a
  PCAN-View capture.
- Consider the same FC-wait for SocketCANIoStream.

## 2026-08-09 - TS-over-CAN ISO-TP fix VALIDATED on hardware (m74_9)

What: The user rebuilt m74_9 on Windows with commit b75fc82311e (32-frame receive
FIFO + host FC wait + ISO-TP desync reset) and ran the TS session over PCAN-USB:
no more `Got only 18 bytes while expecting 50`, no `not enough bytes in stream`
errors, the connection stays up - "кажется что работает". This confirms the
root-cause chain: burst overflow of the 8-frame FIFO plus a permanently stuck
ISO-TP state after a lost frame.

Validation:
- User-reported on hardware (Windows host, PCAN-USB console, 500k): continuous
  TS session without truncation errors.

Open follow-ups:
- Long-run soak to make sure the `outofrange` counter stays flat.
- Optional: same FC-wait treatment for SocketCANIoStream (Linux path still uses
  the no-op default).

## 2026-08-10 - m74_9 L9779WD-SPI: link fixed, VDA 2.0 watchdog feeding added

What: On the m74_9 bench the L9779WD-SPI power driver did not drive injectors
(OUT1..4) or coils (IGN1..4). Root cause turned out to be twofold: (a) the SPI
chip-select pad was never configured as an output, so the chip saw CS permanently
asserted and rejected every frame with SPI_ERR (DO = 0x8000), and (b) even with
a valid link the chip keeps OUT1..4/IGN1..4 forced off because the VDA 2.0
query-answer watchdog (WDA) is not fed - after any reset the error counter EC
starts at 6 and EC > 4 sets WDA_INT and disables the LSA drivers.

Working SPI configuration (proven on hardware, must not be changed): LSB-first
16-bit frames, CPHA=1, BR = div 16, SSM|SSI, CS = PE12, chip-select pad
configured push-pull output idle high. Everything else fails on the bench:
MSB-first and div 64 give 100% parity errors, CPHA=0 makes the chip answer
SPI_ERR (0x8000).

What was done:
| Change | File |
| --- | --- |
| SPI read pipeline: the DO reply to a read request arrives in one of the frames that follow it (datasheet 6.16.2); requests are queued (rd_pending) and matched to replies in order, rx_subaddr carries the answered sub-address | firmware/hw_layer/drivers/gpio/l9779.cpp |
| VDA 2.0 level 3 watchdog feeding (datasheet 6.15): every ~112 ms read the question from REQULO (0x10\|0x0e) plus REQUHI (0x10\|0x0f) status flags, write the expected 32-bit answer from Table 51 via WD_ANSW (0x0e); REQUHI flags (RESP_TO_EARLY / NO_RESP) adapt the response delay in 60..150 ms | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Driver thread: only pushes CONTR_REG writes when outputs changed (o_dirty), wakes 1 ms before the watchdog response is due | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Chip-select pad configured push-pull output idle high in init() before the first frame | firmware/hw_layer/drivers/gpio/l9779.cpp |
| tlead/tcsn timing: 2 us CS-low-to-SCK and CS-high-between-frames delays via the DWT cycle counter (datasheet requires >= 525 ns / >= 640 ns) | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Fixed output mapping: regs[2] bit 4 now mirrors OUT13 (Fuel pump, BG1) instead of OUT14 (Starter) | firmware/hw_layer/drivers/gpio/l9779.cpp |
| Injector pins follow the physical wiring: OUT4 -> Injector 1, OUT3 -> Injector 2, OUT2 -> Injector 3, OUT1 -> Injector 4 (matches m74_9.yaml and the board OUTPUTS[] meta list) | firmware/config/boards/m74_9/board_configuration.cpp |

Design notes:
- EC starts at 6 after SW_RST, so the watchdog must be answered correctly
  (value AND timing) at least twice before EC drops below 4 and the LSA drivers
  turn on; after that it must keep being answered every cycle or the outputs
  drop again.
- The answer value comes from a hardcoded 16-entry table (Table 51) derived from
  the RESP_SOLL7..0 formulas in 6.15.2; there is no cheap way to disable the
  watchdog in configuration, so the driver feeds it.
- cs: the ChibiOS SPI HAL used here (SPI_SELECT_MODE_PAD) toggles the CS pad
  level but never sets its mode; without the explicit palSetPadMode() the pad
  floats and the chip never frames a transfer.

Validation:
- Bench (user-reported): with the stock config restored, `pins` shows
  parity_err=0 frame_err=0 addr_err=0, ident=0x0001 - the chip decodes the
  frames. Watchdog feeding is new in this change and needs a bench flash.

Open follow-ups:
- User flashes, then checks `pins`: expect `WDA: req=<4-bit> ec=0 wda_int=0
  ok=<growing> fail=0 delay=~105ms`. If `fail` grows, adjust the response
  timing (REQUHI flags already steer wd_delay_ms).
- Then verify outputs on the bench: injector pulse on AF4 (OUT4, Injector 1)
  and fuel pump relay on BG1 (OUT13) when commanded from the console/TS.
- Then on the car: coils via IGN1..3 (AL1/AM1, wasted spark), injector bank
  sequencing per cylinder.

## 2026-08-11 - m74_9 L9779WD-SPI: real power-stage diagnostics + corrected SPI config

What: finish the L9779 driver bring-up. The 2026-08-10 entry below claimed
LSB-first + CPHA=1 + div 16 was the working SPI config - that was wrong: LSB-first
is rejected by the chip (it answers 0x0001 to everything, ident stays 0x0000),
CPHA=0 answers SPI_ERR (0x8000) on every frame, and div 16 (9 MHz) exceeds the
8 MHz limit (Table 53). The config proven on hardware (car, engine running) is
MSB-first, mode 3 (CPOL=1, CPHA=1), BR = div 32 (4.5 MHz), plus the parity bit
forced on every transmitted frame in spi_rw_array() - frames with an even number
of set bits (e.g. every WD_ANSW byte) are silently rejected by the chip.

What was done:
| Change | Commit |
| --- | --- |
| SPI: MSB-first, mode 3 (CPOL=1, CPHA=1), div 32; parity forced in spi_rw_array() | 735f14c9ecb, 13a8b17c014 |
| Real power-stage diagnosis: DIA_REG1..8 cached in the driver thread every 100 ms, getOutputDiag() maps each output pin to its 2-bit field (SCG/OL/SCB/OK) and reports PIN_SHORT_TO_GND/PIN_OPEN/PIN_SHORT_TO_BAT/PIN_OK; pins without a power stage (OUT8..12, OUT19, MRD) report PIN_OK, cache returns PIN_UNKNOWN until the first refresh | 862efa4b8c3 |
| OUT_DIS check moved to the right register: DIA_REG10 bit 1 (was looking at IDENT bit 0); START must clear it before CONTR writes are accepted | 862efa4b8c3 |
| 'pins' now dumps DIA10 + per-channel diagnosis (OUT1-4, IGN1-4, OUT21-24, OUT25-28) and the last 32 SPI frames | 862efa4b8c3 |

Design notes:
- Reading a DIA register clears its fault bits on the chip, so the cache refresh
  (100 ms) is slower than the ~112 ms monitoring cycle - a persistent fault is
  re-armed by the chip between reads and always visible; a fault shorter than one
  cycle can be missed (inherent to the chip).
- getOutputDiag()/SensorChecker never touch SPI: they read the cache filled by
  the driver thread (uint16_t/bool, atomic on Cortex-M).
- The bench failure where injectors would not switch was NOT firmware: the bench
  rig lacked power ground (BG2/BG3) - without it the low-side drivers report
  OL and never conduct. On the car, with BG2/BG3 + BH1/BH2 + BF2 powered, the
  power stage works and L9779 drives injectors/coils.

Validation (user-reported):
- Car, engine running: ident=0x0209, spi errors 0, WDA ok=460 fail=0 (delay
  self-tuned to ~106 ms), OUT_DIS=0, OUT1-4:OK, IGN1-4:OK, fuelbench pulses the
  injector, power stage controls the loads.
- Bench lamp test across the low side never lights (low-side driver sinks to
  ground) - measure AF4/AJ4 against +12 V instead: ~9 V with the load driven.

Open follow-ups:
- User rebuilds and checks 'pins': the `ext L9779.*: diagnostic:` lines and the
  SensorChecker injector/coil OBD faults should now reflect the real chip state
  (SCG/OL/SCB on the bench without loads, OK with loads connected).
- Known race (pre-existing): 'pins' (console thread) issues SPI diag reads while
  the driver thread feeds WDA; both share rd_pending/rx_subaddr. Benign in
  practice (reads retry 3x), but a mutex around spi_rw()/spi_rw_array() would
  make it deterministic.

## 2026-08-11 - TS-over-CAN link stability on a busy bus (m74_9, AT32F435)

Symptom (user): on the car the java_console/PCAN link works 5-10 s then dies with
`Got only 4 bytes while expecting 9 for command 0x4F` + `TunerStudio errors:
underrun=1 ... outofrange=54`; on the bench (no other nodes) it is stable. Root
cause is foreign bus traffic reaching the ISO-TP receiver: `CanListener::acceptFrame`
compares only the raw identifier (`CAN_ID()` returns EID for extended frames), so an
extended frame whose low 11 bits equal 0x710 also lands in `CanTsListener.rxFifo`,
and any non-ISO-TP frame arriving between FIRST and CONSECUTIVE used to abort the
partial receive (`numReceived < 1` -> reset + break), truncating TS packets.

What was done:
| Change | File |
| --- | --- |
| bxCAN HW filter on CAN1 (single-CAN boards only: `STM32_CAN_USE_CAN2==FALSE`): FIFO0 accepts only standard 0x710 (mask includes the IDE bit, so extended collisions are rejected in HW), FIFO1 accepts everything else; RX thread drains FIFO0 first so TS frames are never starved by bus noise | firmware/hw_layer/drivers/can/can_hw.cpp |
| `CanTsListener::decodeFrame` rejects extended frames (`frame.IDE`) before enqueueing | firmware/console/binary/serial_can.cpp |
| `CanStreamerState::receiveFrame` distinguishes garbage from loss: FC/bad-type/IDE frames return 0 with ISO-TP state kept; a stale CONSECUTIVE returns -1 after `reset()` | firmware/controllers/can/isotp/isotp.cpp |
| `streamReceiveTimeout` continues on ignored frames (0) and only breaks on real desync (-1) | firmware/controllers/can/isotp/isotp.cpp |
| `sendDataTimeout` FC wait skips up to 8 foreign frames before giving up (was: first non-FC frame aborted every multi-frame TX on a busy bus) | firmware/controllers/can/isotp/isotp.cpp |
| `CAN_FIFO_BUF_SIZE` 76 -> 128, `CAN_FIFO_FRAME_SIZE` 32 -> 64 (static RAM, ~1 KB more) | firmware/controllers/can/isotp/isotp.h |
| Diagnostics: `rxFifoOverflow/ignoredFrames/desyncResets/rxFifoBufOverflow` counters + new console command `isotpinfo` | isotp.h, serial_can.h, serial_can.cpp |

Design notes:
- HW filter is applied in `initCan()` before `canStart()` (`canSTM32SetFilters` asserts
  `CAND1.state == CAN_STOP`); `can_lld_start`/`canStop` do not touch filters, so
  `setCanBaud`/`setCanListenMode` restarts keep it. With `STM32_CAN_USE_CAN2==TRUE`
  the shared filter bank split is board-specific, so the filter is left at the
  driver default there.
- FIFO0 priority in `can_lld_receive` (`CAN_ANY_MAILBOX` drains RX0 first) is what
  makes the TS-first split safe: foreign frames go to FIFO1 and can never push TS
  frames out of the 3-slot HW FIFO.
- FC frames were the most plausible truncation trigger (`receiveFrame` returned 0,
  old `streamReceiveTimeout` turned that into reset+break, dropping the pending
  multi-frame packet); the new 0/-1 contract fixes exactly that path.

Validation:
- Not compiled here (user builds m74_9 himself); unit-test build on this Mac fails
  earlier in the Java toolchain (no JDK 11, `flock` missing) - unrelated to these
  edits. Change set touches only CAN RX path + static buffers, no L9779 logic.

Open follow-ups:
- On the car run `isotpinfo` while connected: counters tell whether drops are
  foreign frames (`ignoredFrames` growing) or FIFO overflow (`rxFifoOverflow`/`rxFifoBufOverflow`).
- If a third-party node genuinely transmits standard 0x710, only changing the
  console bus ID or moving TS to a dedicated CAN avoids it - HW filter cannot help.
- Long-term: revisit `IsoTpRx::readTimeout` and the `IsoTpRxTx` path with the same
  garbage-vs-loss contract.

## 2026-08-11 - java_console ISO-TP decoder: FIRST-frame length sign-extension bug

Symptom (car, PCAN): java_console dies at connect with
`IllegalArgumentException: 2 > -126` in `IsoTpCanDecoder.decodePacket` line 93;
PCAN reader thread exits, link is gone.

Root cause: `IsoTpCanDecoder` computed the FIRST-frame total length as
`((pci & 0xf) << 8) | data[1]` where `data[1]` is a signed Java `byte`. Any
multi-frame response whose length low byte has bit 7 set (e.g. 0x180 = `0x11
0x80`, or a foreign bus node transmitting `0x10 0x80` on the same ID) turned
into a negative `waitingForNumBytes`; `Arrays.copyOfRange(data, 2, 2 + n)`
then threw. Secondary defect: `PCanIoStream.readOnePacket` let any decode
RuntimeException kill the reader thread instead of skipping the frame.

What was done:
| Change | File |
| --- | --- |
| Mask the length byte: `data[isoHeaderByteIndex + 1] & 0xff` | java_console/io/.../can/isotp/IsoTpCanDecoder.java |
| `readOnePacket` wraps `decodePacket` in try/catch, logs the offending frame hex and continues | java_console/io/.../can/PCanIoStream.java |
| `UiVersion.CONSOLE_VERSION` bumped to 20260811 (project rule for Java changes) | java_tools/version/.../UiVersion.java |

Validation: javac-level diagnostics clean; java_console not rebuilt here (user
builds). Fix is the Java counterpart of the firmware-side busy-bus work above:
firmware now survives foreign frames, and the console no longer dies on them.

Open follow-ups:
- On the car, reconnect and check `isotpinfo` counters (`ignoredFrames` etc.)
  plus console-side frame-skip messages to confirm the link holds.
- If a third-party node genuinely owns the 0x710/0x711 IDs, consider moving TS
  to a dedicated CAN or changing the console bus ID.

## 2026-08-11 - m74_9 ETB (TLE9201 via SPI2): hard-hang root cause found, init moved to custom_board_InitHardware

Symptom: after adding TLE9201 ETB support (SPI2 + PD0 CS + tle9201_add) the
board died at power-on - no response, no JTAG reaction, only flashing right
after power-up ("poking" openocd) worked. Rolling back the change restored
normal boot, so the change itself was the culprit.

Root cause (proved by code reading): `tle9201_add()` calls `chip.init()`
synchronously, and `Tle9201::init()` calls `chThdCreateStatic()`
(firmware/hw_layer/drivers/gpio/tle9201.cpp). The first version added
tle9201_add() from `board_init_ext_gpios()`, which is called by `boardInit()`.
ChibiOS `halInit()` calls `boardInit()` at the end (os/hal/src/hal.c:149),
and firmware main.cpp calls `halInit()` BEFORE `chSysInit()`. So
`chThdCreateStatic` ran on a non-initialized scheduler -> guaranteed hang /
HardFault at power-on. L9779 was never affected because `l9779_add()` only
registers a gpiochip; its thread is created later by `gpiochips_init()` from
`initSmartGpio()` (after the kernel is up).

Fix: split init by phase, same pattern as hellen154hyundai_f7:
- `boardInit()` keeps only `board_init_ext_gpios()` (L9779 registration,
  safe pre-OS);
- new `m74_9_boardInitHardware()` (PD0 CS markUsed + push-pull idle high +
  `tle9201_add(0, &tle9201_cfg)`) registered as `custom_board_InitHardware`,
  which `initHardware()` invokes after `chSysInit()` and before
  `initSpiModules()` (firmware/hw_layer/hardware.cpp:381 vs 402).

Change set (firmware/config/boards/m74_9/):
| Change | File |
| --- | --- |
| -DBOARD_TLE9201_COUNT=1, -DSTM32_SPI_USE_SPI2=TRUE | board.mk |
| SPI2 pins D1(SCK AF6)/D3(MISO AF6)/D4(MOSI AF6), is_enabled_spi_2, tle9201_cfg on &SPID2, CS PD0, boardInit/boardInitHardware split | board_configuration.cpp |
| STM32_SPI_USE_SPI2 guard (#ifndef) so board.mk define wins | hw_layer/ports/at32/at32f4/cfg/mcuconf.h |

Notes: TLE9201_CONFIG_CR1/CR2 reuse SPIv1 defines; AT32 reuses
stm32f4/device_mpu_util.h (CPU_HWLAYER = ports/stm32/stm32f4 in rusefi.mk for
ARCH_AT32F4), so SPI_CR1_8BIT_MODE=0 etc. compile as on F4. spi2_af table in
at32_spi.cpp confirms PD1/PD3/PD4 AF6 for SCK/MISO/MOSI.

Validation: not built here (user builds firmware). User to run
`cd firmware/config/boards/m74_9 && ./compile_m74_9.sh`; expected log:
`tle9201_add()=0`, then after ~1-2 s `tle9201 Detected!` and periodic
`tle9201 Diag` lines; console command `reset_9201` available.

Open follow-ups:
- Confirm board boots with the fix and TLE9201 answers on SPI2.
- Then verify throttle movement on bench (PWM=B14, DIR=B15, TPS1 PC2/PC3,
  PPS1 PC0/PC1 already configured).
- If hang persists: isolate stepwise - A) board.mk+mcuconf only, B)
  +BOARD_TLE9201_COUNT=1, C) +pins.

## 2026-08-12 - m74_9: BCM CAN protocol implemented

Implemented BCM (Body Control Module) CAN emulation for m74_9 based on PCAN-View dumps
(ignoff.trc / ignon.trc / ignon_and_start.trc, 12.08.2026).
Starter relay is 100% on BCM; rusEFI does not use starterControlPin on this vehicle.

Protocol decoded from dumps:
- 0x0350 [100ms] BCM->all: byte4 = 0x04 (ign on) / 0x44 (crank switch) / 0x84 (starter active) / 0xC4 (running)
- 0x0303 [100ms] BCM->all: byte5 bit1 = starter relay active
- 0x01F6 [10ms] ECU->all: byte2 = 0x02|0x40(crank)|0x80(run); RPM*16 BE in 0x0189/0x018A bytes 0-1
- Burst group [100ms]: 11 ECU identification frames (0x05DA .. 0x041D)

Change set:
| Change | File |
| --- | --- |
| New header: `initM74_9Can()`, `m74_9_bcmStarterActive()` | firmware/config/boards/m74_9/m74_9_can.h |
| `M74_9BcmCanListener`: RX 0x0350+0x0303, TX 10/20/100ms ECU frames, static instance | firmware/config/boards/m74_9/m74_9_can.cpp |
| Added m74_9_can.cpp to BOARDCPPSRC | firmware/config/boards/m74_9/board.mk |
| `#include "m74_9_can.h"` + `initM74_9Can()` in `setup_custom_board_overrides()` | firmware/config/boards/m74_9/board_configuration.cpp |

Key decisions:
- Single CanListener subclass: `acceptFrame()` accepts two RX IDs; `request()` (called every 5ms by
  CanWrite) drives TX via own counter (% 2 = 10ms, % 4 = 20ms, % 20 = 100ms burst).
- RPM encoded as RPM*16 big-endian (verified: 0x3203/16=800.2rpm from dump).
- No heap: one static M74_9BcmCanListener at file scope.
- All wrapped in #if EFI_CAN_SUPPORT.

Validation: m74_9 build disabled (meta-info.disabled_env); compile not run.
Code reviewed for API consistency against can_dash.cpp, can_sensor.h.

Open follow-ups:
- Validate frame timings on car with PCAN-View.
- Hook `m74_9_bcmStarterActive()` into any board-specific crank detection if needed.
- Unknown byte fields (0x0189 bytes 2-3, 0x01F6 byte3) may need tuning if BCM rejects them.

## 2026-08-12 - m74_9: BCM starter relay fix (CF error state / GND not asserted)

Car dump `rusefi_ign_and_start.trc` showed BCM physically NOT closing starter relay GND
despite 0x0350 byte4=0x84 in CAN. Root causes found by diffing against original firmware dump:

1. **0x0189/0x0186/0x018A bytes 0-1 = 0x0000** - rusEFI encoded actual RPM (0 when stopped).
   Original ECU always broadcasts 0x3200 (800 RPM * 16) as "ECU calibration loaded" flag
   even when engine is off. BCM checks this field; seeing 0x0000 = ECU not ready -> CF
   error state (byte0=CF in 0x0350) -> relay GND not asserted despite CAN showing 0x84.
   Fixed: `encodeRpmOrBaseline()` returns 0x3200 when RPM < 1.

2. **0x05E2 absent** - 1 Hz keepalive (2 bytes 0x00 0x00) present in original, missing
   from rusEFI. Added at 1000 ms period (m_counter % 200 == 0).

3. **Timing**: in the rusEFI dump ECU messages appear only at t=2094ms (BCM key event
   was at t=1856ms). Root cause: user turned key to START before ECU CAN was ready.
   Procedure: turn to RUN, wait ~3s for ECU CAN messages to appear, then turn to START.

Change set:
| Change | File |
| --- | --- |
| `encodeRpm` -> `encodeRpmOrBaseline`: returns 0x3200 when RPM < 1 | m74_9_can.cpp |
| `send0x05E2()`: 2-byte keepalive `00 00` at 1000 ms | m74_9_can.cpp |
| ECU_KEEPALIVE_ID = 0x05E2 constant | m74_9_can.cpp |
| m_counter % 200 == 0: call send0x05E2() | m74_9_can.cpp |

Validation: compile not run (board disabled in CI). Logic verified against dump.

## 2026-08-14 - m74_9: IMMO ON/OFF binary diff + decompilation progress

- Compared `LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOON.bin` and
  `LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOOFF.bin`.
  Only meaningful difference is byte at flash offset `0x08074BF9` (`0x01` = IMMO ON,
  `0x00` = OFF). The 4-byte block at `0x0807FFFC..0x0807FFFF` changes as a consequence
  (likely bank checksum/signature). The same `0x08074BF9` byte is `0x01` in the original
  fullflash dump.
- Updated `docs/m74_9_immo_analysis.md` with:
  - binary diff section,
  - full decompilation of `FUN_082013d0` (IMMO state machine),
  - `FUN_08201716` 1000 ms timer -> state 5,
  - callbacks A/B/C and `0x0713` transmit helpers,
  - `FUN_08202e20` / `FUN_08202cbc` / `FUN_08202254` / `FUN_0820511c`,
  - `07 14` search results (false positives: `ubfx` instructions),
  - `FUN_0800f5e4` helper and note on `FUN_08098860` / `FUN_08867b24` being identical
    main-loop copies.

Validation: binary diff verified with `cmp -l`; fullflash cross-check with Python.
No firmware build run (m74_9 disabled in CI).

Open follow-ups:
- Determine exact checksum algorithm for `0x0807FFFC..0x0807FFFF`.
- Trace `0x200003FC` to identify which slot of `0x20001018` handles IMMO response.
- Capture live IMMO exchange with corrected trigger timing.

## 2026-08-14 - m74_9: IMMO OFF dump confirms trigger gating + live trace analysis

- New CAN dump with `IMMOOFF` calibration shows the original ECU **does not send**
  any `0x0713` frame when the immobilizer is disabled. This confirms the
  `0x08074BF9` calibration byte is the hardware enable flag.
- Added `parse_immo_traces.py` to extract all IMMO exchanges from PCAN-View `.trc`
  files with timings.
- Located stored checksums in `Read_FULLFLASH_…bin`:
  - Boot KS `94B8B6D7` at `0x08022DFFC`
  - Calibration KS `DD5630D0` at `0x0807FFFC`
  - Program KS `61F8857E` at `0x080FFFFC`
- Verified that simple combinations of these checksums do not reproduce the
  observed `0x0713` trigger bytes; the trigger likely includes a session counter
  in addition to flash-derived data.
- Updated `firmware/config/boards/m74_9/m74_9_can.cpp`:
  - IMMO state machine arms only when `engineConfiguration->m74_9ImmoEnabled` is true.
  - `0x0714` challenge frames are ignored when IMMO is disabled.
  - `m74_9_isImmobilizerBlocking()` now uses the config bit and auth state.
- Updated `docs/m74_9_immo_analysis.md` with the live-trace section and captured
  tables; added `parse_immo_traces.py` helper.

Validation:
- Trace parsing verified against 5 original-ECU captures.
- Code compiles not run (m74_9 disabled in CI; no local Linux build environment).

Open follow-ups:
- Identify the exact flash-derived data + counter used to build the `0x0713` trigger.
- Reconstruct the full `0x20001018` function table to find the response-computation
  function (slots 0/2-6 are not registered in the scanned code region).
- Test `m74_9ImmoEnabled = false` on the car to confirm BCM allows start without
  IMMO exchange.

## 2026-08-14 - m74_9: IMMO type-check, function table, and calibration-flag access decoded

Capstone disassembly of the IMMO wrapper (`0x08200000..0x08240000`) plus the
pointer/descriptor tables in the first flash bank.

What was done:

| Change | File |
| --- | --- |
| Disassembled `FUN_08204ad8`, `FUN_08204b54`, `FUN_08204c30`, `FUN_082035dc`, `FUN_082021e5` | Capstone session |
| Mapped all `0x20001018` table registrars (`0x082036c4..0x08203788`) and the registration function `FUN_08202038` | Capstone session |
| Decoded the calibration pointer table at `0x0803E590` and descriptor table at `0x0804A900` | Python full-flash scan |
| Added a new analysis section to the hand-off doc | `docs/m74_9_immo_analysis.md` |

Key findings:

- **IMMO enable decision is already in RAM before the wrapper runs.**
  `FUN_08204c30` returns a type (0..3) derived from markers at `0x08200000` and
  `0x20000000`. `FUN_082035dc` then sets `SRAM[0x200003FC]` to `0` (disabled) for
  types 0/2 and `1` for types 1/3, and clears `SRAM[0x200003FE]` to `0`.
  The actual read of calibration byte `0x08074BF9` happens earlier, outside the
  readable wrapper (likely in shared library `0x080xxxxx` or SLib init).

- **`0x200003FE` getter found at `FUN_082037d8`.** Many IMMO functions start with
  `bl FUN_082037d8` and fail with `0x33` if the flag is not `1`. The only stores
  to `0x200003FE` found in the wrapper are clears (`0x08202212`, `0x08202d56`);
  the setter that initializes it from the calibration descriptor table is still
  missing.

- **Function-pointer table `0x20001018` layout decoded.** Registrars at
  `0x082036c4` (+0x04), `0x082036e0` (+0x10), `0x08203700` (+0x14),
  `0x08203718` (+0x34), `0x08203734` (+0x3c), `0x08203750` (+0x40/+0x44),
  `0x08203770` (+0x4c). `FUN_08202038` registers index `1` with function
  pointers `0x08202019`, `0x08201e2d`, `0x08201df1`, `0x08201d85`, `0x082017d9`,
  `0x08201ae1`, `0x082017f9`.

- **Full-challenge flow confirmed.** `FUN_08202cbc` parses the first `0x0714`
  frame, selects a slot index `0..6` from the challenge type, calls
  `table[idx].+0x04`, and if successful sets `0x200003FC = idx` and arms timer
  `0x20000410` for 5000 ms. `FUN_08201e2c` is the full-response entry point;
  it routes `session_word == 0xFF00` to flash-attestation
  (`FUN_0820630c` -> `FUN_08206108`) and `0xFF01` to the SLib path via
  `FUN_08204bac`.

- **Calibration descriptor table at `0x0804A900` contains entries for
  `0x08074BF8` and `0x08074BF9`.** The pointer table at `0x0803E590`
  references it, but no direct literal references to either address exist in the
  wrapper, confirming table-driven access.

Validation:
- Capstone disassembly reproduced known `0x0713` send paths (`FUN_08201208`,
  `FUN_08201260`, `FUN_082012e8`, `FUN_08204ab8`).
- `parse_immo_traces.py` still shows the same 5 full + 5 quick pairs; no new
  algorithmic match found.
- No firmware build run (m74_9 disabled in CI; macOS host).

Open follow-ups:
- Find the shared-library/SLib setter that initializes `SRAM[0x200003FE]` from
  descriptor table item `0x08074BF9`.
- Locate the `0x0713` trigger generator (likely SLib call at `0x08069028` or
  `0x080697D0`, or a not-yet-decompiled function-table slot).
- Reconstruct the full `0x20001018` table for all 7 indices to identify the
  quick-response function.

## 2026-08-14 - m74_9: added `m74_9ImmoOff` config flag for physical immo bypass

User found a physical way to bypass the immobilizer on the car and asked for a
rusEFI config flag to disable the IMMO logic.

What was done:

| Change | File |
| --- | --- |
| Added `bit m74_9ImmoOff` next to `m74_9ImmoEnabled` in persistent config | `firmware/integration/rusefi_config.txt` |
| Use both flags to decide whether the IMMO state machine arms and whether LimpManager cuts fuel/spark | `firmware/config/boards/m74_9/m74_9_can.cpp` |
| Documented the new flag in the analysis notes | `docs/m74_9_immo_analysis.md` |

Key decisions:

- Kept `m74_9ImmoEnabled` unchanged; added `m74_9ImmoOff` as an explicit override.
- IMMO is active only when `m74_9ImmoEnabled == true && m74_9ImmoOff == false`.
- `m74_9ImmoOff = true` forces the car to start without any `0x0713`/`0x0714`
  exchange, which matches a physical bypass installation.
- Config regeneration was interrupted by the user; they will run the build
  themselves on their Linux environment, which will regenerate
  `engine_configuration_generated_structures_m74_9.h`, `rusefi_m74_9.ini`,
  `VariableRegistryValues.java`, etc.

Validation:
- Code change reviewed; no compiler run locally.

Open follow-ups:
- Run `./compile_m74_9.sh` (or `gen_config_board.sh config/boards/m74_9 m74_9`) to regenerate generated files and verify the build.
- Test on car with `m74_9ImmoOff = true` and the physical bypass installed.

## 2026-08-15 - m74_9: MAP always 0 - AT32 fast ADC (TIM6 -> ADC2) does not run, switched board to slow ADC

User reports MAP on AC3 (AC3 -> RS358A -> PA1, EFI_ADC_1) always reads 0 kPa
while the rest of the board works.

Investigation (console logs from user's bench):

- `adc_report` showed `fast 0 samples` with `F ch[0] @ PA1 ADC2 12bit=0` while
  `slow 805833 samples` and all slow channels (PA0, PA2, PA3, PC3, PC5...)
  sampled fine. So the slow ADC (ADCD1/ADC1) works on AT32 with the reused
  STM32 ADCv2 LLD, but the fast ADC (ADCD2/ADC2, triggered by GPTD6/TIM6 at
  10 kHz) never completes a single conversion.
- MAP is the only fast channel on this board (TPS/PPS/CLT are slow), which is
  why only MAP showed the symptom. Both SensorType::Map branches (MapFast
  averager and MapSlow subscription) read the fast buffer because
  `enableFastAdcChannel("Fast MAP")` removes PA1 from the slow conversion
  group, so MAP was 0 while everything else worked.
- The AT32 port (ChibiOS AT32F4xx in the rusEFI fork) drives ADC via the
  unmodified STM32 ADCv2 LLD; the Artery ADC1 works that way but the fast
  path (TIM6 GPT interrupt -> ADC2 DMA) evidently does not. Not debugged
  further remotely (would need JTAG/bench): unknown whether TIM6 never fires
  or ADC2 conversion never completes. On the old firmware the "ECU: Fast ADC
  errors" gauge distinguishes the two (grows = ADC2 path, stays 0 = TIM6).

What was done:

| Change | File |
| --- | --- |
| Disable the fast ADC for this board (`EFI_USE_FAST_ADC=FALSE`) so MAP is sampled by the working slow ADC | `firmware/config/boards/m74_9/efifeatures.h` |
| Added `!EFI_USE_FAST_ADC` stubs for `enableFastAdcChannel`/`getFastAdc` so `calcFastAdcIndexes()` and `onFastAdcComplete()` still link on v2-port boards without a fast ADC | `firmware/hw_layer/ports/stm32/stm32_adc_v2.cpp` |

Key decisions:

- Did not guard `hardware.cpp` call sites with `EFI_USE_FAST_ADC` because H7
  also builds with `EFI_USE_FAST_ADC=FALSE` but still needs the v4 fast path;
  stubs in the v2 port keep the change local to F4/AT32 boards.
- Workaround, not root-cause fix: MapFast stays invalid, SensorType::Map falls
  back to MapSlow (slow-ADC rate). Map averaging (`isMapAveragingEnabled`)
  will log "No MAP values to average" per window while spinning - disable it
  in TS for now.

Validation:

- Full m74_9 build in the rusefi_build container (`make clean && bash
  bin/compile.sh -b config/boards/m74_9/meta-info.env`) passed; `rusefi.bin`
  produced. Map file confirms the `fastAdc` object is no longer linked (only
  the LLD's ADCD2 definition remains).

Open follow-ups:

- Flash and check `adc_report`: slow channel `S ch[1] @ PA1` now shows the
  real PA1 voltage. ~1.5-2.5 V = firmware fix confirmed; 0 V = the RS358A
  chain/sensor supply is also broken (measure PA1 with a multimeter).
- Proper root-cause fix of the AT32 fast ADC: re-enable `EFI_USE_FAST_ADC`,
  watch "ECU: Fast ADC errors" to split TIM6-GPT vs ADC2-DMA, then fix the
  ChibiOS AT32F4xx port (TIMv1 GPT or ADCv2 LLD ADC2 path).

Update (same day, bench flash): user reflashed and `adc_report` now shows a
real slow-ADC sample for PA1 (`S ch[1] @ PA1 12bit=25 0.020V`), so the slow
path works and MAP is no longer stuck at the dead fast buffer. Note: 0.020 V
at rest is far below the ~1.8 V expected from an MPX4250 at atmospheric
pressure while TPS on the same +5V rail reads 3.16 V - sensor/op-amp chain
still to be validated with vacuum and a multimeter.

## 2026-08-15 - m74_9: AT32 fast ADC root cause fixed (DMAMUX TBL_SEL never enabled)

After the slow-ADC workaround (previous entry) the AT32 fast ADC path (TIM6 GPT
-> ADC2 -> DMA) was brought up for real.

Investigation:

- Added board-local console command `fastadcdiag` (m74_9/board_configuration.cpp)
  dumping TIM6/GPTD6/ADC2/DMA-channel/DMAMUX state. First run: TIM6 running
  (CR1=CEN, CNT moving), ADC2 converting (SR OCCS/EOC activity, err grew with
  lastErr=2 = ADC_ERR_OVERFLOW), DMA channel configured (CNDTR=4, CPAR=ADC2->DR,
  mux CTRL=36) but `DMA1 MUXSEL=0x00000000` - the flexible request table was
  never selected, so the REQSEL=36 programmed into MUXCCTRL was ignored and the
  DMA channel served its fixed default request (not ADC2). ADC1 worked only
  because DMA1_CH1's default request is ADC1.
- Cross-checked the fork's Artery compat headers against the official
  AT32F435/437 Reference Manual (V2.07): DMA_MUXSEL is at offset 0x100, TBL_SEL
  bit 0 = flexible mapping; the fork's `DMA_TypeDef` placed MUXSEL at 0x78
  (ch[7] array stride 0x10 vs hardware 0x14) and `at32_registry.h` did not
  define `STM32_DMA_HAS_DMAMUXSEL`, so `dmaInit()` never wrote it at all.
- Even with both fixed, the write still did not land: `hal_lld_init` calls
  `dmaInit()` right after `rccResetAHB1()` with the DMA clocks OFF - writes to
  the unclocked AT32 DMA are silently dropped.

Fixes (ChibiOS fork AT32 port):

| Change | File |
| --- | --- |
| Enable `rccEnableDMA1/2(false)` before `dmaInit()` | `firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/hal_lld.c` |
| `STM32_DMA_HAS_DMAMUXSEL TRUE` | `firmware/ChibiOS/os/hal/ports/AT32/AT32F4xx/at32_registry.h` |
| Pad `DMA_TypeDef` so MUXSEL/MUXC/MUXG are at 0x100/0x104/0x120 | `firmware/ChibiOS/os/common/ext/Artery/AT32F4xx/at32f435xx.h` |
| `STM32_ADC_ADCPRE = DIV6` -> ADCDIV = HCLK/4 = 72 MHz (was 96 MHz, RM max 80 MHz) | `firmware/hw_layer/ports/at32/at32f4/cfg/mcuconf.h` |
| Keep fast ADC enabled for the board (comment updated) | `firmware/config/boards/m74_9/efifeatures.h` |

Validation (bench, m74_9):

- Built with `make clean` + `bin/compile.sh -b` (user's flow), 0 errors.
- `fastadcdiag`: `MUXSEL=0x00000001`, `fast cnt=31770` growing, `lastErr=0`
  (the small residual `fast err` counter ticks during conversion-overlap
  windows, same as on working F4 boards).
- `adc_report`: `fast 45692 samples`, `F ch[0] @ PA1 ADC2 12bit=8 0.006V`
  (no MAP sensor connected on the bench - expected; TPS on the same +5V rail
  reads 3.17 V).
- MAP is back on the fast ADC; `MapFast`/map averaging functional again.
  `fastadcdiag` kept as a board-local bring-up command.

Open follow-ups:

- Connect a MAP sensor (or apply pressure/vacuum) and confirm MAP kPa tracks
  the voltage (MT_MPX4250: ~1.86 V = 100 kPa).
- Push the ChibiOS fork fixes to rusefi/ChibiOS and bump the submodule pointer
  in rusefi main (human pushes; commits are local).
