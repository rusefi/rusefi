#!/usr/bin/env python3
"""
deep_trace.py – disassemble FUN_0820630C (async crypto engine) and callback 0x08201A48
Also analyse what FUN_08204B54 and FUN_08205F9C do (secure element setup).
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
        hw2 = u16(off + 2)
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

def disasm(chip_start, length, label, show_calls=True):
    off = foff(chip_start)
    chunk = data[off:off+length]
    print(f"\n{'='*70}")
    print(f"  {label}  @ 0x{chip_start:08X}")
    print(f"{'='*70}")
    bl_targets = set()
    for insn in md.disasm(chunk, chip_start):
        ann = ""
        r = resolve_ldr_pc(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> [0x{tgt:08X}] = 0x{val:08X}"
            if BASE <= (val & ~1) < BASE + len(data) and (val & 1):
                ann += f"  (fn @ 0x{val&~1:08X})"
            elif 0x20000000 <= val < 0x20020000:
                ann += f"  (SRAM)"
            elif 0x40000000 <= val < 0x60000000:
                ann += f"  (PERIPH)"
        # detect BL / BLX targets
        if insn.mnemonic in ('bl', 'blx') and show_calls:
            try:
                # Try to decode BL target from bytes
                ioff = foff(insn.address)
                hw1 = u16(ioff)
                hw2 = u16(ioff+2)
                if (hw1 & 0xF800) == 0xF000 and (hw2 & 0xD000) == 0xD000:
                    S     = (hw1 >> 10) & 1
                    imm10 = hw1 & 0x3FF
                    J1    = (hw2 >> 13) & 1
                    J2    = (hw2 >> 11) & 1
                    imm11 = hw2 & 0x7FF
                    I1 = 1 ^ (J1 ^ S)
                    I2 = 1 ^ (J2 ^ S)
                    offset_bits = (S << 24)|(I1 << 23)|(I2 << 22)|(imm10 << 12)|(imm11 << 1)
                    if S: offset_bits |= -(1 << 25)
                    tgt = (insn.address + 4 + offset_bits) & 0xFFFFFFFF
                    ann += f"  [-> 0x{tgt:08X}]"
                    bl_targets.add(tgt)
            except: pass
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")
    if bl_targets and show_calls:
        print(f"\n  BL/BLX targets from {label}:")
        for t in sorted(bl_targets):
            print(f"    0x{t:08X}")
    return bl_targets

# Key functions to disassemble:

# 1. FUN_0820630C – the async crypto engine call
disasm(0x0820630C, 0x200, "FUN_0820630C (async crypto engine)")

# 2. Callback 0x08201A48 – called when crypto completes
disasm(0x08201A48, 0x200, "Callback 0x08201A48 (crypto completion handler)")

# 3. FUN_08204B54 – first-time secure element init (called when state=0)
disasm(0x08204B54, 0x200, "FUN_08204B54 (secure element first-time init)")

# 4. FUN_08205F9C – callback registration
disasm(0x08205F9C, 0x100, "FUN_08205F9C (callback registration)")

# 5. FUN_08204BAC – called after crypto init
disasm(0x08204BAC, 0x100, "FUN_08204BAC")

# 6. FUN_08204C00 – called with fn ptr arg (possibly sets up state machine)
disasm(0x08204C00, 0x100, "FUN_08204C00")

# 7. FUN_08204C30 – called from init function at 0x08202018 area
disasm(0x08204C30, 0x100, "FUN_08204C30")

# 8. What does 0x08201FCC do? (function ptr registered in init path)
disasm(0x08201FCC, 0x80,  "FUN_08201FCC (fn ptr from init path)")

# 9. FUN_08202254 – called from "error" path after blx r6
disasm(0x08202254, 0x100, "FUN_08202254 (async state advance?)")

# 10. Let's look at what reads are done around the hardware registers
# Search for STM32 CAN/SPI/I2C/HASH/AES peripheral base addresses
print(f"\n{'='*70}")
print("  Peripheral access patterns (looking for 0x40XXXXXX references)")
print(f"{'='*70}")

# STM32F7 peripheral addresses
PERIPHS = {
    0x40012C00: "SPI1",
    0x40003800: "SPI2/I2S2",
    0x40003C00: "SPI3/I2S3",
    0x40013400: "SPI4",
    0x40015000: "SPI5",
    0x40015400: "SPI6",
    0x40005400: "I2C1",
    0x40005800: "I2C2",
    0x40005C00: "I2C3",
    0x50060400: "HASH",
    0x50060000: "CRYP (AES/DES/TDES)",
    0x50061400: "RNG",
    0x40006800: "CAN1",
    0x40006C00: "CAN2",
    0x40007400: "CAN3",
    0x40011000: "USART6",
    0x40004400: "USART2",
    0x40004800: "USART3",
}

# Find all 0x40xxxxxx or 0x50xxxxxx literals in the crypto-related functions
KEY_FUNS = [0x0820630C, 0x08204B54, 0x08204BAC, 0x08204C00, 0x08204C30]
for fn in KEY_FUNS:
    off = foff(fn)
    found = []
    for i in range(off, min(off + 0x400, len(data) - 3), 4):
        v = u32(i)
        if (0x40000000 <= v < 0x60000000) or (0x50000000 <= v < 0x60000000):
            found.append((caddr(i), v))
    if found:
        print(f"\n  In/near 0x{fn:08X}:")
        for addr, val in found:
            periph = PERIPHS.get(val & 0xFFFFF000, PERIPHS.get(val, "?"))
            print(f"    [0x{addr:08X}] = 0x{val:08X}  ({periph})")

# Also check what SPI/I2C communication looks like
# Look for known SPI control register patterns
print(f"\n{'='*70}")
print("  Search for SPI/I2C peripheral base (0x4001 2C00 etc.) in entire flash")
print(f"{'='*70}")
for name, base in PERIPHS.items():
    pat = struct.pack('<I', base)
    hits = [i for i in range(0, len(data)-3, 4) if data[i:i+4] == pat]
    if hits:
        print(f"  {PERIPHS.get(base,hex(base))}: {len(hits)} hits at {[hex(h) for h in hits[:4]]}")

print("\n=== Done ===")
