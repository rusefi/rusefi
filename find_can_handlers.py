#!/usr/bin/env python3
"""
find_can_handlers.py - find 0x0713/0x0714 CAN ID handlers and the actual crypto algorithm

Approach:
1. Search for 0x0713/0x0714 as literals in flash (CAN filter/dispatch setup)
2. Find the 0x08201B68 TBH jump table and decode all targets
3. Look at quick/challenge response data flow to find computation
4. Try Unicorn with full state reconstruction
"""

import struct, sys
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
except ImportError:
    print("pip install capstone"); sys.exit(1)

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if len(sys.argv) > 1:
    FIRMWARE = Path(sys.argv[1])

data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(addr): return addr - BASE
def caddr(off): return off + BASE
def u32(off):   return struct.unpack_from('<I', data, off)[0]
def u16(off):   return struct.unpack_from('<H', data, off)[0]

def resolve_ldr_pc(off, chip_addr):
    hw = u16(off)
    if (hw >> 11) == 0x9:
        Rd = (hw >> 8) & 7
        imm8 = hw & 0xFF
        pc = (chip_addr + 4) & ~3
        tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data):
            val = u32(foff(tgt))
            return (Rd, tgt, val)
    if off + 4 <= len(data):
        hw2 = u16(off+2)
        if (hw & 0xFF7F) == 0xF85F:
            Rd = (hw2 >> 12) & 0xF
            imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1
            pc = (chip_addr + 4) & ~3
            tgt = (pc + imm12) if U else (pc - imm12)
            if BASE <= tgt < BASE + len(data):
                val = u32(foff(tgt))
                return (Rd, tgt, val)
    return None

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
md.detail = True

def disasm_with_ann(chip_start, length, label):
    off = foff(chip_start)
    chunk = data[off:off+length]
    print(f"\n{'='*60}")
    print(f"  {label}  @ 0x{chip_start:08X}")
    print(f"{'='*60}")
    for insn in md.disasm(chunk, chip_start):
        ann = ""
        r = resolve_ldr_pc(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> 0x{val:08X}"
            if BASE <= (val & ~1) < BASE + len(data):
                ann += " (flash)"
            elif 0x20000000 <= val < 0x20020000:
                ann += " (SRAM)"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# -----------------------------------------------------------------------
# 1. Search for CAN IDs 0x0713 and 0x0714 in flash
# -----------------------------------------------------------------------
print("="*60)
print("=== 1. CAN ID 0x0713 and 0x0714 in flash ===")
print("="*60)

for can_id, name in [(0x0713, "IMMO_ECU_ID/0x0713"), (0x0714, "IMMO_BCM_ID/0x0714")]:
    # Raw 16-bit value
    pat16 = struct.pack('<H', can_id)
    hits16 = [i for i in range(len(data)-1) if data[i:i+2] == pat16]
    # Shifted for STM32 CAN STDID (<<21)
    shifted = can_id << 21
    pat32 = struct.pack('<I', shifted)
    hits32 = [i for i in range(len(data)-3) if data[i:i+4] == pat32]
    # Also try ID << 5 (some implementations use <<5 for 11-bit ID)
    shifted5 = can_id << 5
    pat32b = struct.pack('<I', shifted5)
    hits32b = [i for i in range(len(data)-3) if data[i:i+4] == pat32b]
    
    print(f"\n  {name} (0x{can_id:04X}):")
    print(f"    As 16-bit LE ({pat16.hex()}): {len(hits16)} hits: {[hex(h) for h in hits16[:8]]}")
    print(f"    As STDID<<21 ({pat32.hex()}): {len(hits32)} hits: {[hex(h) for h in hits32[:4]]}")
    print(f"    As ID<<5 ({pat32b.hex()}): {len(hits32b)} hits: {[hex(h) for h in hits32b[:4]]}")
    
    # Show context for 16-bit hits (they might be in dispatch tables or comparisons)
    if hits16:
        print(f"    Context (16-bit hits):")
        for h in hits16[:10]:
            ctx = data[max(0,h-4):h+6]
            print(f"      file 0x{h:06X} chip 0x{caddr(h):08X}: {' '.join(f'{b:02x}' for b in ctx)}")

# -----------------------------------------------------------------------
# 2. Decode TBH jump table at 0x08201B68
# -----------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 2. TBH jump table at 0x08201B68 ===")
print("="*60)

TBH_ADDR = 0x08201B68
# The TBH instruction at the table base computes: PC + 2*table[Rn]
# Find the TBH instruction just before 0x08201B68

# Disassemble around 0x08201B60 to find TBH instruction
disasm_with_ann(0x08201B40, 0x60, "Code around TBH table 0x08201B68")

# Read the TBH table entries (16-bit halfwords) = offsets
print(f"\n  TBH table entries at file 0x{foff(TBH_ADDR):06X}:")
# The PC for TBH is the instruction address + 4; TBH uses TBH [PC, Rn, LSL #1]
# Target = PC + 2 * table[i]
# We need to find the PC (address of TBH instruction) and the table base

# From context, the TBH is likely 2 bytes before the table
# Try different TBH instruction addresses
for tbh_instr_addr in [TBH_ADDR - 4, TBH_ADDR - 2, TBH_ADDR]:
    pc = tbh_instr_addr + 4  # Thumb PC
    # Read table entries
    print(f"\n  If TBH is at 0x{tbh_instr_addr:08X} (PC=0x{pc:08X}), table at 0x{TBH_ADDR:08X}:")
    toff = foff(TBH_ADDR)
    for i in range(20):
        if toff + i*2 + 2 > len(data):
            break
        entry = u16(toff + i*2)
        target = (pc + 2 * entry) & 0xFFFFFFFF
        # Verify target is in application flash range
        if BASE <= target < BASE + len(data):
            print(f"    case {i:2d}: entry=0x{entry:04X} -> target=0x{target:08X}")
            # Disassemble first instruction at target
            t_off = foff(target)
            for insn in list(md.disasm(data[t_off:t_off+4], target))[:1]:
                print(f"             first insn: {insn.mnemonic} {insn.op_str}")

# -----------------------------------------------------------------------
# 3. Look at state machine function FUN_08201AE0
# -----------------------------------------------------------------------
disasm_with_ann(0x08201AE0, 0x300, "FUN_08201AE0 (state machine)")

# -----------------------------------------------------------------------
# 4. Look at the 16-bit CAN ID reference at file offset 0x201B68 area
# -----------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 4. What's in the raw bytes around 0x08201B68 ===")
print("="*60)
off_b68 = foff(0x08201B68)
raw = data[off_b68:off_b68+40]
print(f"  Raw bytes at 0x08201B68: {' '.join(f'{b:02x}' for b in raw)}")
print(f"  As 16-bit LE entries:")
for i in range(0, len(raw)-1, 2):
    v = struct.unpack_from('<H', raw, i)[0]
    print(f"    [{i//2:2d}] 0x{v:04x}")

# -----------------------------------------------------------------------
# 5. Find the ACTUAL CAN receive dispatcher for IMMO
#    Looking for where 0x0714 is compared/matched
# -----------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 5. Find IMMO CAN receive handler ===")
print("="*60)

# Search for MOVW that loads 0x0714 (or CMP with 0x0714)
# MOVW R3, #0x714 in Thumb-2: F24 13 04 07 (or similar)
# Let's search for "cmp r*, #0x714" or compare operations

# Actually easier: search for the word 0x0714 as an immediate in instructions
# For Thumb-2 CMP: various encodings
# Let's look for the 16-bit value 0x0714 near comparisons

# Search for CMP rn, #0x714 (Thumb-2 32-bit: F1B0 xx14 07...)
# Not straightforward. Instead, search for 0x0714 as a halfword
# that appears in potential comparison or load contexts

# Most likely: the IMMO receive is in a CAN filter list or dispatch table
# Search for 0x0714 as LE 16-bit in filter table
for i in range(0, len(data)-2, 2):
    v = u16(i)
    if v == 0x0714:
        chip_addr = caddr(i)
        # Check if in application range and likely code/table
        if 0x08200000 <= chip_addr < 0x08280000:
            print(f"  0x0714 at code range: file 0x{i:06X} chip 0x{chip_addr:08X}")
            # Show 16 bytes of context
            ctx = data[max(0,i-8):i+12]
            print(f"    ctx: {' '.join(f'{b:02x}' for b in ctx)}")

# -----------------------------------------------------------------------
# 6. Try to find FUN_08201660 (trigger) and trace what it sends
# -----------------------------------------------------------------------
disasm_with_ann(0x08201660, 0x200, "FUN_08201660 (trigger/send function)")

# -----------------------------------------------------------------------
# 7. Check what's in rusefi2.trc - find 0x0714 challenge to rusEFI
# -----------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 7. Parse rusefi2.trc for IMMO messages ===")
print("="*60)
trc_path = Path("rusefi2.trc")
if trc_path.exists():
    content = trc_path.read_text(errors='ignore')
    lines = content.splitlines()
    print(f"  File has {len(lines)} lines")
    immo_lines = [l for l in lines if '0713' in l or '0714' in l]
    print(f"  IMMO lines (0713/0714):")
    for l in immo_lines[:40]:
        print(f"    {l.strip()}")
else:
    print("  rusefi2.trc not found")

# Also check rusefi_ign_and_start.trc
trc_path2 = Path("rusefi_ign_and_start.trc")
if trc_path2.exists():
    content2 = trc_path2.read_text(errors='ignore')
    lines2 = content2.splitlines()
    print(f"\n  rusefi_ign_and_start.trc: {len(lines2)} lines")
    immo_lines2 = [l for l in lines2 if '0713' in l or '0714' in l]
    print(f"  IMMO lines:")
    for l in immo_lines2[:20]:
        print(f"    {l.strip()}")

print("\n=== Done ===")
