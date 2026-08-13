#!/usr/bin/env python3
"""
find_crypto_fn.py  –  find what crypto function gets registered in the IMMO table

From disassembly we know:
- Table base:  SRAM 0x20001018
- Table stride: 0x54
- Crypto fn ptr: table[index*0x54 + 0x34]  (used at 0x082027CE)
- Registrar fn: at chip 0x08203718 (Thumb addr = 0x08203719)
    signature: fn(r0=fn_ptr, r1=index) -> stores r0 into table[r1*0x54 + 0x34]

Strategy:
  1. Search for all BL/BLX instructions in flash that target 0x08203719 (the registrar).
  2. For each call site, decode preceding instructions to find the value loaded into R0.
  3. That value is the crypto function pointer.
  4. Disassemble that function.

Also:
  5. Find the registrar for +0x38 (quick re-check fn ptr).
  6. Disassemble that function too.
"""

import struct, sys, re
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
    CAPSTONE = True
except ImportError:
    print("capstone not installed"); sys.exit(1)

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if len(sys.argv) > 1:
    FIRMWARE = Path(sys.argv[1])

data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(addr): return addr - BASE
def caddr(off): return off + BASE
def u32(off):   return struct.unpack_from('<I', data, off)[0]
def u16(off):   return struct.unpack_from('<H', data, off)[0]

# ---------------------------------------------------------------------------
# Helper: decode a Thumb-2 BL / BLX instruction at file offset `off`
# Returns target chip address or None.
# ---------------------------------------------------------------------------
def decode_bl_target(off):
    """
    Thumb-2 BL (32-bit): 
      HW1: 1111 0 S imm10
      HW2: 1101 1 J1 1 J2 imm11
    Returns target (Thumb addr without bit0), or None if not a BL.
    """
    if off + 4 > len(data):
        return None
    hw1 = u16(off)
    hw2 = u16(off + 2)
    # Check BL pattern: HW1[15:11]=11110 (0xF000 mask = 0xF000), HW2[15:14]=11 (0xD000 mask)
    if (hw1 & 0xF800) != 0xF000:
        return None
    if (hw2 & 0xD000) != 0xD000:
        return None
    # Decode
    S     = (hw1 >> 10) & 1
    imm10 = hw1 & 0x3FF
    J1    = (hw2 >> 13) & 1
    J2    = (hw2 >> 11) & 1
    imm11 = hw2 & 0x7FF
    I1 = 1 ^ (J1 ^ S)
    I2 = 1 ^ (J2 ^ S)
    # Signed 25-bit offset (26 bits effective)
    offset_bits = (S << 24) | (I1 << 23) | (I2 << 22) | (imm10 << 12) | (imm11 << 1)
    if S:
        offset_bits |= -(1 << 25)
    # PC is instruction address + 4, word-aligned for BL
    pc = (caddr(off) + 4) & ~3   # for Thumb-2, PC aligns to 4? Actually no...
    # For Thumb BL, PC = addr + 4 (no alignment needed for offset calc)
    pc = caddr(off) + 4
    target = (pc + offset_bits) & 0xFFFFFFFF
    return target

# ---------------------------------------------------------------------------
# Search for BL instructions targeting specific addresses
# ---------------------------------------------------------------------------
def find_bl_to(target_addr, search_start=0, search_end=None):
    """Find all BL instructions in flash targeting target_addr (or target_addr|1)."""
    if search_end is None:
        search_end = len(data) - 4
    calls = []
    for off in range(search_start, search_end, 2):
        t = decode_bl_target(off)
        if t is not None and (t == target_addr or t == (target_addr & ~1)):
            calls.append(off)
    return calls

# ---------------------------------------------------------------------------
# Resolve PC-relative LDR in Thumb
# ldr Rd, [pc, #imm8*4]  (16-bit: bits[10:8]=Rd, bits[7:0]=imm8)
# ldr.w Rd, [pc, #imm12] (32-bit T2: 0x4DF...)
# Returns (register, loaded_chip_addr, loaded_value) or None
# ---------------------------------------------------------------------------
def resolve_ldr_pc(off, chip_addr):
    """Try to resolve a PC-relative LDR at given offset. Returns (Rd, target_addr, value)."""
    hw = u16(off)
    # 16-bit LDR Rd, [PC, #imm8*4]  : 01001 Rd imm8
    if (hw >> 11) == 0x9:  # 01001
        Rd = (hw >> 8) & 7
        imm8 = hw & 0xFF
        pc = (chip_addr + 4) & ~3  # word-aligned
        tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data):
            val = u32(foff(tgt))
            return (Rd, tgt, val)
    # 32-bit LDR.W Rd, [PC, #imm12] : 11111000 1101 1111 Rt imm12
    if off + 4 <= len(data):
        hw2 = u16(off + 2)
        if (hw & 0xFF7F) == 0xF85F:  # 1111 1000 0101 1111 (U=1) or 0100 1111 (U=0)
            Rd = (hw2 >> 12) & 0xF
            imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1
            pc = (chip_addr + 4) & ~3  # word-aligned
            if U:
                tgt = pc + imm12
            else:
                tgt = pc - imm12
            if BASE <= tgt < BASE + len(data):
                val = u32(foff(tgt))
                return (Rd, tgt, val)
    return None

# ---------------------------------------------------------------------------
# Disassemble a region with PC-relative resolution
# ---------------------------------------------------------------------------
md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
md.detail = True

def disasm_resolved(chip_start, length, label, highlight_addr=None):
    off = foff(chip_start)
    chunk = data[off:off+length]
    print(f"\n  [{label}] @ 0x{chip_start:08X}, {length} bytes:")
    for insn in md.disasm(chunk, chip_start):
        ann = ""
        # Try to resolve PC-relative LDRs
        ioff = foff(insn.address)
        r = resolve_ldr_pc(ioff, insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> [0x{tgt:08X}] = 0x{val:08X}"
        # Highlight the call to the registrar
        if highlight_addr and insn.address == highlight_addr:
            ann += "  <<<< TARGET CALL"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# ---------------------------------------------------------------------------
# Step 1: Find BL to the +0x34 registrar at 0x08203718
# ---------------------------------------------------------------------------
REGISTRAR_34 = 0x08203718
REGISTRAR_38_approx = 0x08203714  # to be refined
APP_RANGE = (0x08200000, 0x08260000)

print("=" * 70)
print(f"=== 1: Finding BL instructions targeting registrar 0x{REGISTRAR_34:08X} ===")
print("=" * 70)

calls_34 = find_bl_to(REGISTRAR_34,
                       foff(APP_RANGE[0]),
                       foff(APP_RANGE[1]))
print(f"Found {len(calls_34)} call(s):")
for c in calls_34:
    print(f"  from file 0x{c:06X} chip 0x{caddr(c):08X}")

# Also search for BL targets in a range around 0x08203710 (all the small registrars)
print(f"\n=== 1b: BL to any registrar in 0x08203700-0x082037D0 ===")
ALL_REGISTRAR_CALLS = {}
for target in range(0x08203700, 0x082037D1, 2):
    cs = find_bl_to(target, foff(APP_RANGE[0]), foff(APP_RANGE[1]))
    if cs:
        ALL_REGISTRAR_CALLS[target] = cs

for target, cs in sorted(ALL_REGISTRAR_CALLS.items()):
    print(f"  BL -> 0x{target:08X}: {len(cs)} call(s) from "
          f"{[f'0x{caddr(c):08X}' for c in cs[:5]]}")

# ---------------------------------------------------------------------------
# Step 2: For each call site, disassemble context to find R0
# ---------------------------------------------------------------------------
print(f"\n=== 2: Disassemble context of each call site ===")

# Collect all unique call sites across all registrars
all_call_sites = {}
for target, cs in ALL_REGISTRAR_CALLS.items():
    for c in cs:
        site_addr = caddr(c)
        if site_addr not in all_call_sites:
            all_call_sites[site_addr] = target

for site_addr, registrar in sorted(all_call_sites.items()):
    print(f"\n  Call site 0x{site_addr:08X} -> registrar 0x{registrar:08X} (stores at +0x{(registrar-0x08203718+0x34)%0x100:02X}?):")
    # Show 80 bytes before and 8 bytes after the BL
    disasm_resolved(max(APP_RANGE[0], site_addr - 0x50), 0x70,
                    f"context before BL at 0x{site_addr:08X}",
                    highlight_addr=site_addr)

# ---------------------------------------------------------------------------
# Step 3: Find MOVW/MOVT pairs loading flash addresses near call sites
# ---------------------------------------------------------------------------
print(f"\n=== 3: MOVW/MOVT pairs near call sites ===")

def find_movw_movt(chip_start, chip_end):
    """Find all MOVW Rd, #imm16 / MOVT Rd, #imm16 pairs (Thumb-2 32-bit)."""
    results = []
    off_start = foff(chip_start)
    off_end = foff(chip_end)
    for off in range(off_start, min(off_end, len(data)-4), 2):
        hw1 = u16(off)
        hw2 = u16(off+2)
        # MOVW: 11110 i10 0100 0 imm4   1 0 imm3 Rd imm8
        # MOVT: 11110 i10 1100 0 imm4   1 0 imm3 Rd imm8
        if (hw1 & 0xFBF0) == 0xF240 and (hw2 & 0x8000) == 0x8000:  # MOVW
            # Decode: imm16 = imm4:i:imm3:imm8
            i     = (hw1 >> 10) & 1
            imm4  = hw1 & 0xF
            imm3  = (hw2 >> 12) & 0x7
            Rd    = (hw2 >> 8) & 0xF
            imm8  = hw2 & 0xFF
            imm16 = (imm4 << 12) | (i << 11) | (imm3 << 8) | imm8
            results.append(('MOVW', caddr(off), Rd, imm16))
        if (hw1 & 0xFBF0) == 0xF2C0 and (hw2 & 0x8000) == 0x8000:  # MOVT
            i     = (hw1 >> 10) & 1
            imm4  = hw1 & 0xF
            imm3  = (hw2 >> 12) & 0x7
            Rd    = (hw2 >> 8) & 0xF
            imm8  = hw2 & 0xFF
            imm16 = (imm4 << 12) | (i << 11) | (imm3 << 8) | imm8
            results.append(('MOVT', caddr(off), Rd, imm16))
    return results

# Search around each call site for MOVW/MOVT
for site_addr, registrar in sorted(all_call_sites.items()):
    mvs = find_movw_movt(max(APP_RANGE[0], site_addr - 0x80), site_addr + 4)
    if mvs:
        print(f"\n  Near 0x{site_addr:08X}:")
        for op, addr, Rd, imm in mvs:
            print(f"    0x{addr:08X}  {op} R{Rd}, #0x{imm:04X}")
        # Try to pair MOVW + MOVT for R0 and R1
        movw_r = {r: [] for r in range(16)}
        movt_r = {r: [] for r in range(16)}
        for op, addr, Rd, imm in mvs:
            if op == 'MOVW': movw_r[Rd].append((addr, imm))
            if op == 'MOVT': movt_r[Rd].append((addr, imm))
        for r in range(16):
            if movw_r[r] and movt_r[r]:
                lo = movw_r[r][-1][1]
                hi = movt_r[r][-1][1]
                full = (hi << 16) | lo
                print(f"    -> R{r} = 0x{full:08X}", end="")
                if BASE <= full < BASE + len(data):
                    print(f"  (flash code!)", end="")
                elif 0x20000000 <= full < 0x20020000:
                    print(f"  (SRAM)", end="")
                print()

# ---------------------------------------------------------------------------
# Step 4: Scan for ADR/ADD PC pairs loading function addresses
# ---------------------------------------------------------------------------
print(f"\n=== 4: LDR R0, [PC, #X] near call sites (fn pointer loads) ===")

for site_addr, registrar in sorted(all_call_sites.items()):
    start = max(APP_RANGE[0], site_addr - 0x60)
    end = site_addr
    off_s = foff(start)
    off_e = foff(end)
    found = []
    for off in range(off_s, min(off_e, len(data)-4), 2):
        r = resolve_ldr_pc(off, caddr(off))
        if r:
            Rd, tgt, val = r
            if Rd == 0 and BASE <= val < BASE + len(data) and (val & 1):
                # R0 loaded with a Thumb code address (odd address)
                found.append((caddr(off), Rd, tgt, val))
    if found:
        print(f"\n  Near 0x{site_addr:08X} (call to registrar 0x{registrar:08X}):")
        for instr_addr, Rd, tgt, val in found:
            print(f"    0x{instr_addr:08X}  LDR R{Rd}, [PC, ...] -> [0x{tgt:08X}] = 0x{val:08X}  (fn at 0x{val&~1:08X})")

# ---------------------------------------------------------------------------
# Step 5: Full scan for all code in the init range that stores fn ptrs
#         Look for pattern: LDR Rn, [PC, #x] where loaded val is 0x0820xxxx
#         followed by BL to a registrar
# ---------------------------------------------------------------------------
print(f"\n=== 5: All LDR-loaded flash addresses near registrar calls ===")

# Find a broader init region: look for the function that contains all the registrar calls
if all_call_sites:
    min_site = min(all_call_sites.keys())
    max_site = max(all_call_sites.keys())
    print(f"  Call sites span: 0x{min_site:08X} – 0x{max_site:08X}")
    
    # Disassemble the full span with resolution
    span_start = max(APP_RANGE[0], min_site - 0x80)
    span_len   = min(max_site - span_start + 0x100, 0x800)
    disasm_resolved(span_start, span_len, "Full init span")

# ---------------------------------------------------------------------------
# Step 6: If we still don't have the fn ptr, try a broader BL search
#         for BL to 0x082037D8 (the read-SRAM fn used as getter)
# ---------------------------------------------------------------------------
print(f"\n=== 6: Search for any BL to 0x08203700-0x082037E0 from application ===")
for target in range(0x08203700, 0x082037E1, 2):
    cs = find_bl_to(target, foff(0x08200000), foff(0x08240000))
    for c in cs:
        if caddr(c) not in all_call_sites:
            print(f"  BL @ 0x{caddr(c):08X} -> 0x{target:08X}")

# ---------------------------------------------------------------------------
# Step 7: Search all of flash for LDR R0 loading an address in 0x082036xx-0x082038xx
#         These would be loads of the fn ptr for the registrar argument
# ---------------------------------------------------------------------------
print(f"\n=== 7: LDR loading addresses in 0x082036xx-0x082038xx ===")
for off in range(0, len(data)-4, 2):
    # 16-bit LDR Rd, [PC, #imm8*4]
    hw = u16(off)
    if (hw >> 11) == 0x9:  # LDR Rd, [PC, #imm8*4]
        Rd = (hw >> 8) & 7
        imm8 = hw & 0xFF
        pc = (caddr(off) + 4) & ~3
        tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data):
            val = u32(foff(tgt))
            if 0x08203680 <= (val & ~1) <= 0x082038A0:
                print(f"  0x{caddr(off):08X}: LDR R{Rd}, [PC, #0x{imm8*4:02X}] -> 0x{val:08X}")
    # 32-bit LDR.W
    if off + 4 <= len(data):
        hw2 = u16(off+2)
        if (hw & 0xFF7F) == 0xF85F:
            Rd = (hw2 >> 12) & 0xF
            imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1
            pc = (caddr(off) + 4) & ~3
            tgt = (pc + imm12) if U else (pc - imm12)
            if BASE <= tgt < BASE + len(data):
                val = u32(foff(tgt))
                if 0x08203680 <= (val & ~1) <= 0x082038A0:
                    print(f"  0x{caddr(off):08X}: LDR.W R{Rd}, [PC, #0x{imm12:03X}] -> 0x{val:08X}")

print("\n=== Done ===")
