#!/usr/bin/env python3
"""
find_trigger_gen.py - Search flash dump for 0x0713 trigger generation code.

Goal: find the function that assembles the 8-byte 0x0713 trigger data,
specifically looking for:
  1. BLX Rn instructions in the readable wrapper (0x08201000-0x08209AFF) -
     these call SLib functions whose address is loaded at runtime
  2. Code that accesses the AT32F435 TRNG hardware (0x50060800 range)
  3. Code that writes CAN ID 0x713 to the TX mailbox (0x40006400 range)
  4. The literal 0x0713 appearing in code as a CAN ID constant
"""

import struct, sys, os

DUMP = 'Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin'
FLASH_BASE = 0x08000000

with open(DUMP, 'rb') as f:
    flash = f.read()

print(f"Flash dump: {len(flash)} bytes  (0x{FLASH_BASE:08X} - 0x{FLASH_BASE+len(flash)-1:08X})")

def offset(addr):
    return addr - FLASH_BASE

def read16(addr):
    o = offset(addr)
    if 0 <= o < len(flash)-1:
        return struct.unpack_from('<H', flash, o)[0]
    return None

def read32(addr):
    o = offset(addr)
    if 0 <= o < len(flash)-3:
        return struct.unpack_from('<I', flash, o)[0]
    return None

def context(addr, before=8, after=16):
    o = offset(addr) & ~1
    b = flash[max(0,o-before):o+after]
    return b.hex(' ')

# -----------------------------------------------------------------------
# 1. Find ALL BLX Rn instructions in the IMMO wrapper (0x08201000-0x082099FF)
# -----------------------------------------------------------------------
print("\n" + "="*70)
print("1. BLX Rn in IMMO wrapper (0x08201000-0x08209AFF)")
print("   These may call SLib functions loaded at runtime")
print("="*70)

WRAPPER_START = 0x08201000
WRAPPER_END   = 0x08209B00

blx_list = []
addr = WRAPPER_START
while addr < WRAPPER_END:
    hw = read16(addr)
    if hw is None:
        break
    lo = hw & 0xFF
    hi = (hw >> 8) & 0xFF
    # BLX Rn: hi==0x47 and lo has pattern 1Rm0_0000
    if hi == 0x47 and (lo & 0x87) == 0x80:
        reg = (lo >> 3) & 0xF
        blx_list.append((addr, reg))
    addr += 2

print(f"Found {len(blx_list)} BLX Rn instructions:")
for addr, reg in blx_list:
    # Look back for the LDR that loaded this register
    ldr_ctx = ""
    for back in range(2, 32, 2):
        prev = read16(addr - back)
        if prev is None:
            break
        lo = prev & 0xFF
        hi = (prev >> 8) & 0xFF
        # LDR Rn, [PC, #imm] in Thumb: 0x48-0x4F (LDR R0..R7 from literal pool)
        if 0x48 <= hi <= 0x4F:
            ldr_reg = hi - 0x48
            imm = (lo & 0xFF) * 4
            ldr_pc = (addr - back + 4) & ~3
            pool_addr = ldr_pc + imm
            val = read32(pool_addr)
            if val and (val & 0xFF000000) == 0x08000000:
                ldr_ctx = f" <- LDR r{ldr_reg},[0x{pool_addr:08X}]=0x{val:08X}"
                if 0x08100000 <= val < 0x08200000:
                    ldr_ctx += "  *** SLIB RANGE ***"
            break
        # Thumb2 LDR.W: F8 5x (LDR Rn, [Rm, Rk])
        # or simpler: check if it's a 32-bit Thumb2 (skip)
    print(f"  0x{addr:08X}: BLX R{reg}{ldr_ctx}  ctx=[{context(addr, 4, 4)}]")

# -----------------------------------------------------------------------
# 2. Find CAN 0x0713 ID constant (shifted: 0x713 << 21 = 0xE2600000)
# -----------------------------------------------------------------------
print("\n" + "="*70)
print("2. CAN ID 0x0713 constant patterns in flash")
print("="*70)

CAN_ID_SHIFTED = 0xE2600000   # 0x713 << 21  (standard frame, no TXRQ)
CAN_ID_TXRQ    = 0xE2600001   # + TXRQ bit

patterns = {
    '13 07 (raw bytes)': b'\x13\x07',
    '0x713 little-endian word': struct.pack('<H', 0x713),
    '0xE2600000 (shifted, no TXRQ)': struct.pack('<I', CAN_ID_SHIFTED),
    '0xE2600001 (shifted + TXRQ)':   struct.pack('<I', CAN_ID_TXRQ),
    'MOVW Rn, #0x713 (any reg)': None,  # handled below
}

for name, pat in patterns.items():
    if pat is None:
        continue
    idx = 0
    found = []
    while True:
        idx = flash.find(pat, idx)
        if idx == -1:
            break
        addr = FLASH_BASE + idx
        found.append(addr)
        idx += 1
    if found:
        print(f"\n  Pattern '{name}':")
        for addr in found[:10]:
            o = offset(addr)
            ctx = flash[max(0,o-4):o+8].hex(' ')
            print(f"    0x{addr:08X}:  {ctx}")
        if len(found) > 10:
            print(f"    ... and {len(found)-10} more")

# MOVW Rn, #imm16 in Thumb2: F2 4x lo hi
# For imm16=0x0713: lo=0x13, hi=0x07
# Encoding: F2 4x (where x=0..F for reg), 0x13 0x07? Actually:
# MOVW Rd, #imm16: 1111 0x10 0100 imm4 | 0 imm3 Rd imm8
# imm16 = 0x0713 = imm4=0, imm3=7, imm8=0x13
# word1: F2 40 | (rd << 8 varies)
# word2: 0x13 | (imm3<<12=0x07<<12=0x7000) | (Rd<<8)
# approximate search for 0x07 0x13 as hi/lo of second word
print("\n  Pattern 'MOVW Rn, #0x713 (Thumb2)':")
idx = 0
found_movw = []
while True:
    idx = flash.find(b'\x07\x13', idx)  # hi=0x07, lo=0x13 in second halfword
    if idx == -1:
        break
    addr = FLASH_BASE + idx
    # Check if preceded by F2 4x (MOVW first halfword)
    if idx >= 2:
        prev = flash[idx-2:idx]
        if prev[0] == 0xF2 and (prev[1] & 0xF0) == 0x40:
            found_movw.append(addr - 2)
    idx += 1
for addr in found_movw[:10]:
    o = offset(addr)
    ctx = flash[o:o+8].hex(' ')
    print(f"    0x{addr:08X}:  {ctx}")
if not found_movw:
    print("    (none found)")

# -----------------------------------------------------------------------
# 3. TRNG register access (AT32F435 TRNG base = 0x50060800)
# -----------------------------------------------------------------------
print("\n" + "="*70)
print("3. TRNG register accesses (AT32F435 TRNG ~ 0x50060800)")
print("="*70)

TRNG_ADDRS = [0x50060800, 0x50060804, 0x50060808, 0x5006080C]
TRNG_BYTES = [struct.pack('<I', a) for a in TRNG_ADDRS]

for trng_addr, pat in zip(TRNG_ADDRS, TRNG_BYTES):
    idx = 0
    found = []
    while True:
        idx = flash.find(pat, idx)
        if idx == -1:
            break
        found.append(FLASH_BASE + idx)
        idx += 1
    if found:
        print(f"  TRNG 0x{trng_addr:08X} referenced at:")
        for a in found[:5]:
            o = offset(a)
            ctx = flash[max(0,o-4):o+8].hex(' ')
            print(f"    0x{a:08X}:  {ctx}")

# Also check for 0x500xxxxx pattern (TRNG base might differ)
print("\n  All 0x5006xxxx literal pool references:")
idx = 0
found50 = []
while True:
    idx = flash.find(b'\x00\x60\x50', idx)  # bytes 2,3,4 of 0x50060xxx LE
    if idx == -1:
        break
    addr = FLASH_BASE + idx - 1
    val = read32(addr)
    if val and (val & 0xFFFF0000) == 0x50060000:
        found50.append((addr, val))
    idx += 1
for addr, val in found50[:10]:
    print(f"    0x{addr:08X} -> 0x{val:08X}")

# -----------------------------------------------------------------------
# 4. CAN TX mailbox registers in literal pools
#    AT32F435 CAN1 base = 0x40006400
#    Mailbox0 IR (transmit identifier) = base + 0x180 = 0x40006580
#    Mailbox0 TDTR (DLC) = 0x40006584
#    Mailbox0 TDLR (data low) = 0x40006588
#    Mailbox0 TDHR (data high) = 0x4000658C
# -----------------------------------------------------------------------
print("\n" + "="*70)
print("4. CAN TX mailbox base addresses in literal pools")
print("="*70)

CAN_ADDRS = {
    0x40006400: "CAN1 base",
    0x40006580: "CAN1 MB0 IR",
    0x40006800: "CAN2 base",
    0x40006980: "CAN2 MB0 IR",
}

for can_addr, name in CAN_ADDRS.items():
    pat = struct.pack('<I', can_addr)
    idx = 0
    found = []
    while True:
        idx = flash.find(pat, idx)
        if idx == -1:
            break
        found.append(FLASH_BASE + idx)
        idx += 1
    if found:
        print(f"  {name} (0x{can_addr:08X}) at {len(found)} locations:")
        for a in found[:5]:
            o = offset(a)
            ctx = flash[max(0,o-4):o+8].hex(' ')
            print(f"    0x{a:08X}: {ctx}")
    else:
        print(f"  {name} (0x{can_addr:08X}): NOT FOUND in literal pools")

# -----------------------------------------------------------------------
# 5. Summary of SLib-range addresses in literal pools (0x08100000-0x081FFFFF)
# -----------------------------------------------------------------------
print("\n" + "="*70)
print("5. SLib-range addresses (0x08100000-0x081FFFFF) in literal pools")
print("   These are candidates for the SLib function pointer table")
print("="*70)

slib_refs = []
for i in range(0, len(flash)-3, 4):
    val = struct.unpack_from('<I', flash, i)[0]
    if 0x08100000 <= val <= 0x081FFFFF:
        addr = FLASH_BASE + i
        slib_refs.append((addr, val))

print(f"Found {len(slib_refs)} SLib-range literals:")
for addr, val in slib_refs[:30]:
    print(f"  0x{addr:08X} -> 0x{val:08X}")
if len(slib_refs) > 30:
    print(f"  ... and {len(slib_refs)-30} more")
