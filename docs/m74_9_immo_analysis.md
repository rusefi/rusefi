# m74_9 immobilizer analysis notes

This document captures everything known about the m74_9 / Itelma I865LB52
CAN immobilizer, so subsequent sessions can continue without re-doing
reconnaissance.

---

## Current status  (updated 2026-08-13)

### Root-cause bug FIXED

The rusEFI IMMO trigger condition had a **dead-lock** bug:

- Old code armed the trigger on `byte4 == 0x44` (crank-switch).
- BCM **never** asserts byte4=0x44 until the IMMO handshake succeeds.
- Therefore rusEFI never sent the trigger, BCM never sent the challenge,
  engine never started even though all keepalive frames were correct.

Fix applied in `m74_9_can.cpp`:
- Trigger fires when `byte4 != 0x00` (first BCM frame after IGN ON).
- Delay increased to 1000 ms (original ECU sends at ~1.5 s; 1 s is safe).

This means rusEFI will now initiate the IMMO handshake.  BCM will respond
with the 0x0714 challenge.  ECU still cannot respond correctly because
`computeImmoResponse()` is a stub — engine will not start until that is
implemented, but the protocol exchange will at least be visible in PCAN.

### Crypto algorithm — status

The response algorithm is **proprietary**.  Deep static analysis reveals:

- Application IMMO handler registered at SRAM[0x20001018 + 1*0x54 + 0x34] =
  `FUN_08201E2C`.  Called from `FUN_08202854` (the IMMO compute engine at
  0x082027A4).
- `FUN_08201E2C` parses the BCM's 0x0714 challenge data, extracts
  flash-start and flash-size fields, then calls `FUN_0820630C` (async flash
  attestation engine).
- The async engine calls `FUN_08206108` → `FUN_08205A3C` → sets up
  FLASH hardware registers, calls `FUN_082056D4` which checks if a flash
  range is erased (all 0xFF) or programmed.
- All 10 response bytes (5 × 16-byte + 5 × 8-byte pairs) were verified NOT
  to appear literally in the flash binary, ruling out a simple flash-read
  response.
- No standard crypto constants (AES S-box, SHA-256, XTEA, KEELOQ, CRC32
  polynomial) found.  No hardware AES/HASH engine access.
- The algorithm uses ADD / XOR / shift arithmetic with a key embedded in
  the calibration area.  Exact key location not yet pinned.

### CAN receive chain (confirmed)

- CAN1_RX0 IRQ → `FUN_08207432` → `FUN_08206FB8(channel=0)`.
- Dispatcher reads FMI from CAN_RDT0R bits [15:8], indexes
  table at `0x08209834` to find per-message handler.
- 0x0714 challenge reception → stores bytes in SRAM buffer around
  `0x20000C14` (4-byte header + 16-byte challenge).
- FLASH ISR → `0x082063FC` → calls callback at `SRAM[0x20001B88]` →
  eventually calls `FUN_0820378C` → queues 0x0713 response over CAN.

Open tasks:

- Identify the EXACT computation from challenge bytes → 8-byte response.
  Best approach: Unicorn emulation with correct SRAM layout, or SWD
  breakpoint on the real ECU at `0x082027FA` (blx r6).
- Implement `computeImmoResponse()` and `computeImmoQuickResponse()`.
- Build and flash; verify BCM byte4 reaches 0x84 (starter relay).
- Enable `m74_9ImmoEnabled` in tune once proven.

---

## Protocol observed on CAN (confirmed by timing analysis of PCAN-View traces)

### Message IDs

| ID | Direction | Description |
|----|-----------|-------------|
| `0x0713` | ECU → BCM | ECU initiates session (trigger) and sends cryptographic response |
| `0x0714` | BCM → ECU | BCM sends challenge (two back-to-back 8-byte frames = 16 bytes total) and periodic quick re-check (single frame) |

### Full authentication cycle

1. ECU sends `0x0713` trigger (8 bytes, session counter in bytes 0-1).
2. BCM responds with two consecutive `0x0714` frames within 0.2 ms of each other
   → together they form a 16-byte challenge.
3. ECU computes response and sends one `0x0713` frame (~15 ms after challenge).
4. Periodically BCM sends a single `0x0714` re-check; ECU replies with `0x0713`
   within ~5 ms.

### BCM state machine (0x0350, 100 ms period)

| byte0 | byte4 | Meaning |
|-------|-------|---------|
| C3 | `0x04` | IGN on, no crank request |
| C4 | `0x44` | Crank switch pressed (key to START) |
| **CF** | `0x84` | **BCM timeout / IMMO error** – starter relay asserted in CAN but **NOT physically closed** |
| C5 | `0x84` | Starter relay active (relay GND physically asserted) |
| C5 | `0xC4` | Engine running confirmed |

**Critical:** the `CF` state means BCM saw the crank request but did not receive
a valid `0x0713` response in time. In this state BCM writes `0x84` to the CAN
status byte but does **not** close the physical relay. The engine does not crank.

To avoid `CF`:
- ECU must be sending its CAN keepalive frames (`0x01F6`, `0x0189`, etc.)
  **before** the user turns the key to START.
- bytes 0-1 of `0x0189` / bytes 2-3 of `0x0186` / bytes 0-1 of `0x018A` must
  be `0x3200` (baseline idle-target RPM, 800 × 16), **not** `0x0000`. BCM uses
  this field as an "ECU calibration loaded" flag; `0x0000` triggers `CF`.

### Other BCM frames checked

| ID | Role |
|----|------|
| `0x0303` byte5 bit1 (`0xE2`) | Starter relay physically active |
| `0x0211` byte4 (`0x01`) | BCM signals "start permitted" |
| `0x04AC` byte4 (`0x20`) | ECU-on-bus presence flag (transitions when ECU CAN appears) |

---

## Captured challenge/response pairs

### Full 16-byte challenge → 8-byte response

| Source | Trigger (0x0713) | Challenge frame1 + frame2 (0x0714) | Response (0x0713) |
|--------|------------------|-------------------------------------|-------------------|
| orig_1.trc | `5283dba64769c00d` | `660be1e2a34b8140` + `b45633a0499a01ec` | `9cb7f8ca31431bb6` |
| orig_2.trc | `a5ea8a93265530dd` | `cfcfbbf3cdc0f75c` + `e9efe2eb23b62a25` | `bff99205ed4ab7a8` |
| orig_3.trc | `45d4a5c0f6a1ea72` | `cd4c9070196bbdeb` + `b425cb7c4350082c` | `96739be6b1299f77` |
| ignon.trc | `0841994c0b81a14c` | `4a4f2a204fad58fd` + `273622b70c2b559e` | `fda32d94ae77c121` |
| ignon_and_start.trc | `d081cc982708d806` | `66aaeef37037dee0` + `cdeefb22bde96807` | `efaa66f0caa6f0cd` |

None of the simple algorithms (fixed XOR, NOT, byte-reverse, MD5, SHA-1,
AES-ECB, DES) fit these pairs. The algorithm is a custom ROM function accessed
via the SRAM callback table.

### Quick 8-byte challenge → 8-byte response (periodic re-check)

| Source | Challenge (0x0714) | Response (0x0713) |
|--------|-------------------|-------------------|
| orig_1.trc | `0eabfe9d351af837` | `b15156d14683cd15` |
| orig_2.trc | `4a90250e855128bf` | `14a267bec8d8c3dc` |
| orig_3.trc | `63b95b1a3fbf41fe` | `2fa71d4722835ea2` |
| ignon.trc | `596e3885f2460f4a` | `2cdf61de56551b86` |
| ignon_and_start.trc | `a00059b79f90a8ae` | `a225de4e470324b6` |

---

## Firmware binary

File: `Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin`  
Size: 0x3F0000 bytes (3.94 MB).  
Ghidra base address: `0x08000000`.

### Memory layout

The file contains at least two distinct regions:

| File offset | Size | Chip address | Contents |
|------------|------|-------------|----------|
| 0x000000 | 0x200000 | 0x08000000 | Lower flash: calibration tables, CAN descriptor tables, utility functions |
| 0x200000 | 0x1F0000 | 0x08800000 (and mirrors) | Upper ROM: engine control OS, IMMO module |

### ROM aliasing (confirmed in Ghidra)

The ROM region (file 0x200000+) is accessible from multiple base addresses
simultaneously. All three blocks contain identical data:

| Block base address | File offset |
|--------------------|-------------|
| `0x08800000` | `0x200000` |
| `0x09000000` | `0x200000` |
| `0x09200000` | `0x200000` |

When Ghidra shows a call to e.g. `0x09235CA0`, the actual function in the file is
at: `0x200000 + (0x09235CA0 - 0x09200000) = 0x235CA0`.

**In Ghidra add the upper ROM as a second memory block:**
Window → Memory Map → "+" (Add Block):
- Name: `ROM`
- Start address: `08800000`
- Length: `1F0000`
- File offset: `200000`
- Mode: File Bytes (read + execute)

Set Processor Options to Thumb (TMode=1) for this block.

---

## Application-side IMMO state machine (0x082016BC)

Located in the application code layer (0x082xxxxx range, file offset 0x200000+).
Called by the CAN receive dispatcher when a message matching the IMMO CAN ID
range is received.

### Dispatch chain

```
CAN1 RX IRQ (0x08207433)
  → CAN dispatch (0x08206FB8)
    → UndefinedFunction_08204a68 (table at 0x08208910, 0x20-byte entries)
      → state machine at 0x082016BC
```

`UndefinedFunction_08204a68` is registered as the CAN RX callback via
`FUN_08206f48()`, called from `FUN_08204a9c()`.

### State machine structure (0x082016BC)

Uses a **TBB (Table Branch Byte)** switch on the lower nibble of a state byte
stored at `SRAM[0x200002A8 + 0x10]`.

| State nibble | Case | Target address | Description |
|:---:|:---:|---|---|
| 1 | 0 | `0x082016DA` (`FUN_082016da`) | **Initial state**: start 1000 ms timer, set state → 2 |
| 2 | 1 | `0x082017B6` | Exit (idle/waiting) |
| 3 | 2 | `0x082016F6` (`UndefinedFunction_082016f6`) | Clear state to 0, call callback with CAN ID |
| 4 | 3 | `0x08201716` (`FUN_08201716`) | Receive 0x0714: reset comm, start 1000 ms timer, state → 5 |
| 5 | 4 | `0x082017B6` | Exit |
| 6 | 5 | `0x0820173A` (`FUN_0820173a`) | Timer tick: fire ROM timer callbacks |

### Key decompiled functions

**`FUN_082016da`** (case 0 – initialise):
```c
void FUN_082016da(void) {
    FUN_082010d4(*DAT_082017c8, 1000);              // start 1000 ms timer
    *(byte*)(DAT_082017c4 + 0x10) =
        (*(byte*)(DAT_082017c4 + 0x10) & 0xF0) | 2; // state → 2
}
```

**`UndefinedFunction_082016f6`** (case 2 – reset and notify):
```c
void UndefinedFunction_082016f6(void) {
    *(byte*)(DAT_082017c4 + 0x10) &= 0xF0;         // state → 0 (idle)
    if ((code*)*puRam082017cc != NULL)
        (*(code*)*puRam082017cc)(*(ushort*)(DAT_082017c4 + 0x14) & 0xFFF); // callback(CAN_ID)
}
```

**`FUN_08201716`** (case 3 – receive 0x0714 challenge):
```c
void FUN_08201716(void) {
    FUN_08201104(*DAT_082017d0);    // reset comm channel
    FUN_082010d4(*puRam082017d4, 1000); // 1000 ms watchdog timer
    *(byte*)(SRAM + 0x10) = (... & 0xF0) | 5; // state → 5
}
```

**`FUN_0820173a`** (case 5 – timer tick):
Calls `FUN_08010F0A` → `FUN_08006468` + `FUN_080106ec` → ROM callback dispatcher
`FUN_08010ee8(4)` / `FUN_08010ee8(5)`.

### SRAM structure at 0x200002A8

| Offset | Role |
|--------|------|
| `+0x10` | State nibble (lower 4 bits = switch case selector) |
| `+0x14` | CAN ID of most-recently-matched message (masked to 0xFFF) |

### Timer infrastructure

`FUN_082010d4(index, timeout_ms)` writes to a 12-byte timer entry at
`DAT_08201100 + index * 0xC`:
- `+0`: flags (`& 1 | 2` = enable timer)
- `+2`: counter (reset to 0)
- `+4`: timeout value

---

## ROM-layer IMMO infrastructure (0x08800000 alias)

### ROM startup chain

```
0x089D02C2  (aliased: 0x08800000 at file 0x200000)
    → UndefinedFunction_089d02c2   – memory init (.data copy, .bss zero)
    → FUN_089d416c                 – ROM main entry point
        → FUN_089d42ec             – init 1
        → FUN_089d4720             – init 2
        → FUN_089d4080             – init 3
        → FUN_089d40d8             – init 4
        → FUN_089d40e8(1)          – fire software IRQ 1 (starts event loop)
        → FUN_089d4140 (loop)      – idle heartbeat counter
```

`FUN_089d4140` is just an idle counter; real work happens in ROM IRQ handlers.

### ROM callback system

**Callback registration:**
```c
// FUN_08010ed6 — stores a function pointer into the SRAM callback table
void FUN_08010ed6(param1, param2, param3, int slot) {
    *(uint*)(0x2001A200 + slot * 4) = param2;  // param2 = function pointer
}
```
`0x2001A200` (SRAM) is the runtime function-pointer table.

**Callback index array:**
`DAT_08010f00` = flash address `0x0804F280` – a read-only byte array where
`[slot]` gives the entry index K into the function-pointer table.

**Callback dispatcher:**
```c
// FUN_08010ee8 — dispatches to a registered callback by slot
void FUN_08010ee8(int param_1) {
    byte K = *(byte*)(0x0804F280 + param_1);
    if (K != 0) {
        code *fn = *(code**)(0x2001A200 + (K-1) * 4);
        if (fn) fn();
    }
}
```

When case 5 of the application state machine fires, it calls
`FUN_08010ee8(4)` and `FUN_08010ee8(5)`. Those dispatch to whatever function was
registered in slots 4 and 5 of the SRAM table.

**Finding the IMMO crypto function:**
The crypto function is registered by one of the ROM init functions
(`FUN_089d42ec`, `FUN_089d4720`, `FUN_089d4080`, `FUN_089d40d8`) by calling
`FUN_08010ed6`. In Ghidra:

1. Right-click `FUN_08010ed6` → References → Find All References.
2. For each CALL reference, look at what is passed as `param_2` (the function pointer).
3. Check the `param_4` argument (the slot index). Slots 4 and 5 are the ones
   used by the IMMO timer path.

**Also look at `0x0804F280`** to see the index array values for slots 4 and 5.
Those indices tell you which entries in the 0x2001A200 table to follow.

### CAN peripheral access pattern

ROM functions access CAN/timer hardware registers directly at 0x40000000+:

| Address | Register |
|---------|---------|
| `0x4000000C` | TIM2_DIER (interrupt enable) |
| `0x40000010` | TIM2_SR (status / clear flags) |
| `0x40000024` | TIM2_CNT |
| `0x40000034` | TIM2_CCR1 |
| `0x40010410` | TIM5_SR or another peripheral |

`FUN_08005eec` is a TIM2 capture-compare IRQ handler that measures timing signals
(crank/cam) – not IMMO-related.

---

## Previous-session dispatcher notes (0x08203FFC)

A prior analysis session identified a different state machine at `0x08203FFC`
with this literal pool:

| Flash literal | RAM address | Role |
|---|---|---|
| `DAT_08204080` | `0x2000162C` | Counter base |
| `DAT_0820408c` | `0x20001A50` | **Function pointer: full 16-byte challenge** |
| `DAT_08204098` | `0x20001A54` | **Function pointer: quick 8-byte re-check** |

These addresses (`0x20001A50`, `0x20001A54`) may be the application-layer
function pointers for the IMMO response, populated by a different initialisation
path than the ROM's 0x2001A200 table. **Both sets of pointers should be
investigated.**

Key constants reportedly copied from flash to RAM `0x20001ADC` during init:

```
0x2548A4D2   (flash: 0x080BB570)
0x43A0C212   (flash: 0x080BB574)
0x4DF9123B   (flash: 0x080BB508)
0xF9C74A52   (flash: 0x080BB50C)
```

These may be key material for the IMMO crypto. They also appear in the immo
code region around `0x08204B00`–`0x08204BFC`.

---

## Tune-level IMMO enable/disable

Comparing `LARGUS_TUN_*_IMMOON.bin` vs `_IMMOOFF.bin`:

| Offset | IMMOON | IMMOOFF | Meaning |
|--------|--------|---------|---------|
| `0x074BF9` | `0x01` | `0x00` | Immobilizer enabled flag |
| `0x07FFFC`–`0x07FFFF` | `A2 B2 4C C6` | `DE 1D BE F0` | Calibration checksum |

Single functional difference is the flag at `0x074BF9`; the checksum must be
recalculated after changing it.

---

## Implementation progress in rusEFI

All code lives in `firmware/config/boards/m74_9/m74_9_can.cpp`.

### A. ECU keepalive frames (BCM "ECU alive" signal)

All frames below must be transmitted **before** the user turns the key to START:

| ID | Period | Key bytes | Notes |
|----|--------|-----------|-------|
| `0x01F6` | 10 ms | byte2: `0x02\|0x40(crank)\|0x80(run)` | ECU state flags |
| `0x0189` | 10 ms | bytes 0-1: `RPM × 16` BE, min `0x3200` | **Must not be 0x0000** — BCM uses as "ECU calibrated" flag |
| `0x0186` | 10 ms | bytes 2-3: same RPM encoding | Same constraint |
| `0x018A` | 10 ms | bytes 0-1: same RPM encoding | Same constraint |
| `0x0217` | 10 ms | byte2: `0x70` when active | ECU status |
| `0x02A9` | 10 ms | `0xF4` | Heartbeat |
| `0x02C6` | 20 ms | `00 00 00 40 00 00` | Static |
| `0x05E2` | 1000 ms | `00 00` | Keepalive (absent → BCM fault after ~30 s) |
| Burst group | 100 ms | 11 frames: 0x05DA…0x041D | ECU identification |

### B. IMMO state machine (M74_9BcmCanListener)

States: `Idle` → `WaitingToTrigger` (100 ms after crank switch) → `TriggerSent`
→ `Challenge1Rx` → `ChallengeComplete` → `ResponseSent`.

- Sends `0x0713` trigger with rolling 16-bit session counter.
- Collects two consecutive `0x0714` frames into a 16-byte buffer.
- Calls `computeImmoResponse(challenge16, response8)` → currently returns
  `false` (stub).
- Handles single-frame quick re-check via `computeImmoQuickResponse()`.

### C. Configuration bit

`bit m74_9ImmoEnabled` in `engine_configuration_s` (default **off** until
algorithm is known).

### D. LimpManager fuel/spark cut

`ClearReason::Immobilizer` added; hook `custom_board_isImmobilizerBlocking`
wired to `m74_9_isImmobilizerBlocking()` in `board_configuration.cpp`.

### E. Build status

The board is disabled in CI (`meta-info.disabled_env`). Build on Linux:
```bash
cd firmware/config/boards/m74_9 && ./compile_m74_9.sh
```

---

## Ghidra step-by-step to find the crypto function

1. Load full dump at base `0x08000000` (Language: `ARM:LE:32:v7`, Thumb).
2. Add second memory block from file offset `0x200000`, base `0x08800000`,
   length `0x1F0000`, read+execute, TMode=1 (Thumb).
3. Analyse image.
4. Navigate to `FUN_08010ed6` (0x08010ED6).
5. Right-click → References → Find All References.
6. For each CALL, check `param_4` (slot index) and `param_2` (function pointer).
   Target slots are **4 and 5** (used by the IMMO timer path).
7. Navigate to the registered function(s) and decompile.
8. Alternatively: search writes to SRAM `0x20001A50` and `0x20001A54`
   (application-layer pointers from earlier session).
9. Verify against the captured pairs above.

---

## Files in the repo

| File | Role |
|------|------|
| `docs/m74_9_immo_analysis.md` | This document |
| `firmware/config/boards/m74_9/m74_9_can.cpp` | CAN keepalive + IMMO state machine + stubs |
| `firmware/config/boards/m74_9/m74_9_can.h` | Public API |
| `firmware/config/boards/m74_9/board.mk` | Includes m74_9_can.cpp |
| `firmware/config/boards/m74_9/board_configuration.cpp` | Registers hooks |
| `firmware/integration/rusefi_config.txt` | m74_9ImmoEnabled config bit |
| `firmware/controllers/limp_manager.cpp` | ClearReason::Immobilizer |
| `firmware/hw_layer/board_overrides.h` | custom_board_isImmobilizerBlocking |
| `Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin` | Original ECU full flash dump |
| `ignoff.trc`, `ignon.trc`, `ignon_and_start.trc` | Original firmware PCAN captures |
| `orig_1.trc`, `orig_2.trc`, `orig_3.trc` | Original firmware start captures |
| `rusefi_ign_and_start.trc`, `rusefi2.trc` | rusEFI PCAN captures |

---

## IMMO dispatcher decompilation (0x08203FFC)

Full Thumb-2 disassembly confirms the call mechanism:

```asm
; FUN_08203FFC — called with (param_1, param_2)
push {r3, lr}
cbz  r0, case_0        ; if param_1==0 → full 16-byte challenge
cmp  r0, #1
beq  case_1            ; if param_1==1 → quick 8-byte challenge
pop  {r3, pc}          ; else return

; case_0, sub-case param_2==0 (challenge received, compute response):
ldr  r3, [pc, #0x6c]   ; literal → 0x20001A50 (full-challenge cb pointer)
ldr  r3, [r3]          ; R3 = *(0x20001A50) = actual crypto function address
cmp  r3, #0
beq  exit
ldr  r2, [pc, #0x64]   ; literal → 0x20001A46
ldrb r1, [r2]          ; R1 = *(0x20001A46)  (arg1)
ldr  r2, [pc, #0x5c]   ; literal → 0x20001A45
ldrb r0, [r2]          ; R0 = *(0x20001A45)  (arg0)
blx  r3                ; ← CALLS THE CRYPTO FUNCTION

; case_1, sub-case param_2==0 (quick re-check):
ldr  r3, [pc, #0x3c]   ; literal → 0x20001A54 (quick cb pointer)
ldr  r3, [r3]          ; R3 = *(0x20001A54) = quick crypto function
...
blx  r3                ; ← CALLS QUICK CRYPTO
```

**Key insight**: the crypto function address is fetched via double-indirect pointer:
`*(*(0x20001A50))`. The SRAM word at 0x20001A50 is written at runtime during
IMO module initialisation (by FUN_0820477e called with param_1=0). No static
callers of FUN_0820477e were found — it is called through the same indirect
mechanism. Static analysis cannot resolve the crypto function address without
a runtime observation (hardware debugger or ARM emulator).

**Crypto function arguments**: R0=`*(0x20001A46)`, R1=`*(0x20001A45)` (single
bytes). The actual challenge bytes (16 bytes for full, 8 for quick) are read
internally from the CAN RX buffer at offset +0x18C from the CAN table base at
0x20001644.

## Key code addresses (confirmed by Capstone disassembly)

| Chip address | Description |
|---|---|
| `0x08203FFC` | **IMMO dispatcher**: receives (type, status); dispatches to crypto via 0x20001A50/0x20001A54 |
| `0x08203F84` | CAN TX queue (10-slot) — used to queue 0x0713 responses |
| `0x08203F0C` | CAN TX queue (50-slot) |
| `0x0820402C` | `blx r3` — the exact instruction that calls the full-challenge crypto |
| `0x08204068` | `blx r3` — the exact instruction that calls the quick-challenge crypto |
| `0x0820477e` | Registration dispatcher: `param_1=0` → writes to 0x20001A50/0x20001A54 |
| `0x08204790` | Registration (param_1=0 branch): `*0x20001A50 = param_3`, `*0x20001A54 = param_2` |
| `0x08204458` | IMMO init: registers FUN_08203FFC via 0x08207754, sets up CAN RX via 0x08207874 |
| `0x0820432C` | IMMO setup: allocates 0x18C stack frame, copies challenge data via ldrb [r0,#0x18c] |
| `0x082047DC` | Challenge reader: reads from CAN table + 0x18C; literal pool at 0x082047D4 |
| `0x08204B04` | Key constant writer: stores 0x4DF9123B or 0xF9C74A52 to SRAM |
| `0x082040A0` | Version checker: compares two SRAM values (uses key constants) |
| `0x08207754` | Callback registration helper #1 (registers IMMO dispatcher) |
| `0x08207874` | Callback registration helper #2 (registers CAN RX config) |

## Key constants in flash (confirmed)

At file offsets 0xBB508–0xBB574 (chip 0x080BB508–0x080BB574, calibration area):

```
file 0xBB508 chip 0x080BB508: 3B 12 F9 4D = 0x4DF9123B
file 0xBB50C chip 0x080BB50C: 52 4A C7 F9 = 0xF9C74A52
file 0xBB570 chip 0x080BB570: D2 A4 48 25 = 0x2548A4D2
file 0xBB574 chip 0x080BB574: 12 C2 A0 43 = 0x43A0C212
```

Same constants also at 0x08204AF0–0x08204BFF (in IMMO code area, literal pools).

## How to find the crypto function

### Option A — Hardware debugger (JTAG/SWD)

Set a breakpoint at `0x0820402C` (the `blx r3` that calls the full-challenge crypto).
When the breakpoint hits, read R3 — that is the crypto function address.

### Option B — ARM emulator (unicorn-engine)

```python
import unicorn, struct
# Load flash at 0x08000000
# Set up SRAM at 0x20000000
# Map real CAN challenge bytes at *(0x20001644) + 0x18C
# Single-step from 0x08203FFC with param_1=0, param_2=0
# Capture R3 at the blx instruction at 0x0820402C
```

### Option C — Ghidra memory map fix + analysis

Delete the current wrong ROM block (base 0x00880000, file 0x30D40).
Add correct ROM_alias: base 0x08800000, file offset 0x000000, length 0x3F0000.
Then re-analyse; ROM functions (0x089Dxxxx) that initialise the callbacks
will become visible and Ghidra can trace what is written to 0x20001A50.

## Next steps and blockers

### Priority 1 — Build and test the trigger fix

```bash
cd firmware/config/boards/m74_9 && ./compile_m74_9.sh
```

Flash to the ECU.  Connect PCAN.  Turn ignition ON.  After ~1 s, you should
see a 0x0713 trigger frame from the ECU.  BCM should respond within ~5 ms
with two 0x0714 challenge frames.  ECU will log:
  `IMMO: challenge received but response algorithm not implemented`
and retry every 500 ms.  Engine will not start yet, but the exchange should
be visible and BCM should no longer report CF error.

### Priority 2 — Capture challenge and run Unicorn emulation

With the trigger fix deployed, capture a new PCAN trace.
Take the 16-byte 0x0714 challenge that BCM sends to rusEFI.
Run Unicorn emulator with the original firmware binary:

1. Set up SRAM at 0x20000C14 with 4-byte header + 16 challenge bytes.
2. Header format: [count=20, key_type=1, 0xFF, 0x00]  (session word = 0xFF00).
3. Set SRAM[0x200002C8]=1 (init done), SRAM[0x200003FE]=1.
4. Call `FUN_08201E2C | 1` (Thumb) with:
   - R0=1 (key_type), R1=0xFF00 (session_word),
   - R2=0x20000C18 (data_ptr+4), R3=12 (count-4).
5. Hook all writes to 0x20000C18-0x20000C27 (response buffer).
6. Compare captured output with expected BCM response.

Analysis scripts in repo root: `emulate_immo.py`, `find_crypto_fn.py`,
`trace_init.py`, `crypto_decode.py`, `deep_trace.py`, `find_algo_final.py`,
`trace_fn6108.py`, `find_key_algo2.py`, `find_can_handlers.py`.

### Priority 3 — Hardware SWD debug (fastest path)

Attach JTAG/SWD to the original ECU running with key in ignition.
Set a breakpoint at `0x082027FA` (the `blx r6` call in the compute engine).
When the BCM sends the 0x0714 challenge:
- r6 = address of the actual crypto function.
- Read what r6 points to, set breakpoint there.
- Read all register values and SRAM state.
- Step through to capture the 8-byte response computation.

### Priority 4 — Implement and test

Fill `computeImmoResponse()` and `computeImmoQuickResponse()` in `m74_9_can.cpp`.
Enable `m74_9ImmoEnabled` in tune; confirm BCM byte4 reaches `0x84` (relay GND),
engine cranks and starts.

### Priority 5 — Default behaviour

Once proven, set `engineConfiguration->m74_9ImmoEnabled = true`
in `m74_9_boardDefaultConfiguration()`.
