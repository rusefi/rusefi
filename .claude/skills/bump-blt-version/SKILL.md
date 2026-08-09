---
name: bump-blt-version
description: Bump the OpenBLT bootloader version marker (BLxx) in both required places - set_bl_bin_version.sh and BLT_CURRENT_VERSION in mpu_util.h. Use when the bootloader build has materially changed, or when the user asks to bump/increase the bootloader (BL) version.
---

# Bump OpenBLT bootloader version marker

The bootloader version is a 4-char ASCII tag (`BL07`, `BL08`, ...) stored in the
3rd reserved DWORD of the bootloader vector table (flash `0x08000024`). It is
defined in TWO places that MUST change together, both tagged with the grep
marker `search:openblt_version`. Background: CLAUDE.md section "OpenBLT
Bootloader Version Marker".

## Steps

1. **Read the current version** from `firmware/hw_layer/ports/mpu_util.h`:

   ```
   #define BLT_CURRENT_VERSION 0x37304C42
   ```

   This u32 is the ASCII tag in reverse (little-endian) byte order:
   `0x37304C42` = bytes `42 4C 30 37` = `B` `L` `0` `7` -> current tag `BL07`.

2. **Compute the new tag**: increment the decimal number unless the user asked
   for a specific one (`BL07` -> `BL08`; note `BL09` -> `BL10` changes BOTH
   digit characters). Derive the two encodings:
   - byte sequence: ASCII codes of the 4 chars, e.g. `BL08` -> `\x42\x4c\x30\x38`
   - u32 constant: same bytes reversed, e.g. `BL08` -> `0x38304C42`

   Sanity helper (prints both encodings for a tag):

   ```bash
   TAG=BL08; printf '%s' "$TAG" | xxd -p; \
   printf '0x%s\n' "$(printf '%s' "$TAG" | xxd -p | fold -w2 | tac | tr -d '\n' | tr a-f A-F)"
   ```

3. **Edit place 1** - `firmware/bin/set_bl_bin_version.sh`:
   - the comment line: `# set ascii 'BL07' into third reserved DWORD of vector table`
   - the printf bytes: `printf '\x42\x4c\x30\x37'`

4. **Edit place 2** - `firmware/hw_layer/ports/mpu_util.h`:
   - the comment line: `// ascii 'BL07' in reverse LBS byte order`
   - the define: `#define BLT_CURRENT_VERSION 0x37304C42`

5. **Verify consistency** - the script bytes and the header constant must be
   byte-reversals of each other, and no old-tag stragglers remain:

   ```bash
   grep -rn "search:openblt_version" firmware/ | grep -v ChibiOS
   grep -n "printf" firmware/bin/set_bl_bin_version.sh
   grep -n "BLT_CURRENT_VERSION" firmware/hw_layer/ports/mpu_util.h
   ```

   Check by hand: printf `\xAA\xBB\xCC\xDD` must correspond to constant
   `0xDDCCBBAA`, and both must spell the intended ASCII tag.

6. **Commit both files in the SAME commit**, together with (or referencing)
   the bootloader change that motivated the bump. Precedent: `BL06`
   21e3d069285, `BL07` 438cf802ac8 (PR #9815). A bump with no bootloader
   change is meaningless - it only declares "this bootloader build differs".

## Runtime effect (for the commit message / PR text)

ECUs flashed with the new full `rusefi.bin` report `CURRENT` from the
`show_blt_version` console command; ECUs that only ever took
`rusefi_update.srec` keep the old marker in flash and report `UNEXPECTED`,
flagging that their bootloader is stale.