#!/usr/bin/env python3
"""
trace_fn6108.py - Disassemble FUN_08206108 (actual crypto/crc function)
and related functions to understand the IMMO computation.

FUN_08206108 is called from FUN_08201D84 (+0x40 SRAM table handler) with:
  r0 = SRAM[0x200002D4] (flash start address, set by +0x3C handler)
  r1 = r4 = data pointer (arg to 08201D84)
  r2 = r5 = length     (arg to 08201D84)

Also disassemble:
- FUN_08204B54 (first-time secure element init, called when SRAM[0x200002C8]=0)
- FUN_0820630C (the async engine) more carefully
- FUN_08205950 (called with r0=challenge_value, r1=session_mode)
"""

import struct, sys
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
except ImportError:
    print("pip install capstone"); sys.exit(1)

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(a): return a - BASE
def u32(o): return struct.unpack_from('<I', data, o)[0]
def u16(o): return struct.unpack_from('<H', data, o)[0]

def resolve_pc_ldr(off, chip_addr):
    hw = u16(off)
    if (hw >> 11) == 0x9:
        Rd = (hw >> 8) & 7; imm8 = hw & 0xFF
        pc = (chip_addr + 4) & ~3; tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data): return (Rd, tgt, u32(foff(tgt)))
    if off + 4 <= len(data):
        hw2 = u16(off+2)
        if (hw & 0xFF7F) == 0xF85F:
            Rd = (hw2 >> 12) & 0xF; imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1; pc = (chip_addr + 4) & ~3
            tgt = (pc + imm12) if U else (pc - imm12)
            if BASE <= tgt < BASE + len(data): return (Rd, tgt, u32(foff(tgt)))
    return None

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)

def disasm(addr, length, label):
    off = foff(addr)
    if off < 0 or off + length > len(data):
        print(f"\n  {label} @ 0x{addr:08X}: INVALID OFFSET")
        return
    chunk = data[off:off+length]
    print(f"\n{'='*65}")
    print(f"  {label}  @ 0x{addr:08X}")
    print(f"{'='*65}")
    for insn in md.disasm(chunk, addr):
        ann = ""
        r = resolve_pc_ldr(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> 0x{val:08X}"
            if 0x20000000 <= val < 0x20020000: ann += " (SRAM)"
            elif 0x40000000 <= val < 0x60000000: ann += " (PERIPH)"
            elif BASE <= val < BASE+len(data):  ann += " (flash)"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# FUN_08206108 - THE ACTUAL COMPUTATION FUNCTION
disasm(0x08206108, 0x300, "FUN_08206108 (actual IMMO computation)")

# FUN_08204B54 - first-time init (called when SRAM[0x200002C8] == 0)
disasm(0x08204B54, 0x200, "FUN_08204B54 (first-time secure element init)")

# FUN_08205950 - called by FUN_0820630C with (r0=challenge_val, r1=session_mode)
disasm(0x08205950, 0x200, "FUN_08205950 (session activator / key setup)")

# FUN_08204C30 - called from init function at 0x08202018
disasm(0x08204C30, 0x300, "FUN_08204C30 (CAN channel management)")

# FUN_0820566C - first call in FUN_0820630C
disasm(0x0820566C, 0x100, "FUN_0820566C")

# FUN_08205790 - called with r0=challenge_val
disasm(0x08205790, 0x100, "FUN_08205790")

# FUN_082057F4 - setup 1
disasm(0x082057F4, 0x100, "FUN_082057F4")

# FUN_0820587C - setup 2
disasm(0x0820587C, 0x100, "FUN_0820587C")

# FUN_08205910 - setup 3 (enables interrupts)
disasm(0x08205910, 0x100, "FUN_08205910")

# FUN_08205D24 - trigger flash erase (or something else?)
disasm(0x08205D24, 0x100, "FUN_08205D24")

# FUN_08205AB0 - channel check
disasm(0x08205AB0, 0x100, "FUN_08205AB0")

# FUN_08208408 - dispatch stub
disasm(0x08208408, 0x40, "FUN_08208408 (dispatch stub)")

# What's at 0x20000104 and 0x200001CC (SRAM functions per sub-agent)?
print(f"\n{'='*65}")
print("=== Data section copied to SRAM at startup ===")
print("Data at flash 0x08208410 (copied to SRAM 0x20000100 at startup):")
off = 0x208410
for i in range(0, 0x120, 16):
    row = data[off+i:off+i+16]
    print(f"  flash 0x{BASE+off+i:08X} -> SRAM 0x{0x20000100+i:08X}: {' '.join(f'{b:02x}' for b in row)}")

print("\n=== Done ===")
