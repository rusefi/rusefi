#!/usr/bin/env python3
"""
find_algo_final.py  - Final algorithm search

From TBH at 0x08201B68 (PC=0x08201B6C):
- The table DATA starts at 0x08201B6C (NOT 0x08201B68)
- Entry [0] = halfword at 0x08201B6C = bytes BD 00 = 0x00BD
  → target = 0x08201B6C + 2*0x00BD = 0x08201CE6  (case 0xF188)
- Entry [1] = halfword at 0x08201B6E = bytes C8 00 = 0x00C8
  → target = 0x08201B6C + 2*0x00C8 = 0x08201CFC  (case 0xF189)

And checking what produces valid nibbles for the crypto function:
- The first "data byte" (at r7+0) must have both nibbles ≤ 4
- For pair 1: challenge[7]=0x40 (nibbles 4,0) ✓
- Look for which offset in the challenge data satisfies this for all pairs

Plan:
1. Disassemble TBH cases 0 and 1 (0x08201CE6 and 0x08201CFC) - UDS DID 0xF188/0xF189 handlers
2. Find where the key material is in flash
3. Try computing CRC/checksums of flash regions matching challenge pattern
4. Emulate full IMMO chain with correct SRAM layout
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
def caddr(o): return o + BASE
def u32(o): return struct.unpack_from('<I', data, o)[0]
def u16(o): return struct.unpack_from('<H', data, o)[0]

def resolve_pc_ldr(off, chip_addr):
    hw = u16(off)
    if (hw >> 11) == 0x9:
        Rd = (hw >> 8) & 7; imm8 = hw & 0xFF
        pc = (chip_addr + 4) & ~3
        tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data):
            return (Rd, tgt, u32(foff(tgt)))
    if off + 4 <= len(data):
        hw2 = u16(off+2)
        if (hw & 0xFF7F) == 0xF85F:
            Rd = (hw2 >> 12) & 0xF; imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1; pc = (chip_addr + 4) & ~3
            tgt = (pc + imm12) if U else (pc - imm12)
            if BASE <= tgt < BASE + len(data):
                return (Rd, tgt, u32(foff(tgt)))
    return None

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)

def disasm(addr, length, label):
    off = foff(addr)
    chunk = data[off:off+length]
    print(f"\n{'='*60}")
    print(f"  {label}  @ 0x{addr:08X}")
    print(f"{'='*60}")
    for insn in md.disasm(chunk, addr):
        ann = ""
        r = resolve_pc_ldr(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> 0x{val:08X}"
            if 0x20000000 <= val < 0x20020000: ann += " (SRAM)"
            elif BASE <= val < BASE+len(data):  ann += " (flash)"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# Correct TBH targets (table at 0x08201B6C, PC=0x08201B6C)
# Entry [0] = 0x00BD → 0x08201B6C + 2*0xBD = 0x08201CE6
# Entry [1] = 0x00C8 → 0x08201B6C + 2*0xC8 = 0x08201CFC
disasm(0x08201CE6, 0x200, "TBH case 0 (DID 0xF188) @ 0x08201CE6")
disasm(0x08201CFC, 0x200, "TBH case 1 (DID 0xF189) @ 0x08201CFC")

# Also look at what FUN_08203AEC does (called before the TBH dispatch, at 0x08201B52)
disasm(0x08203AEC, 0x200, "FUN_08203AEC (UDS request parser, called before TBH)")

# And FUN_08203BEC (called in the 0xFD09 handler path, at 0x08201BC4)
disasm(0x08203BEC, 0x200, "FUN_08203BEC (called in FD09 handler)")

# ---------------------------------------------------------------------------
# Byte offset analysis: which challenge byte has valid nibbles?
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== Valid nibble byte positions in challenges ===")
print("="*60)

PAIRS16 = [
    (bytes.fromhex('660be1e2a34b8140b45633a0499a01ec'), bytes.fromhex('9cb7f8ca31431bb6')),
    (bytes.fromhex('cfcfbbf3cdc0f75ce9efe2eb23b62a25'), bytes.fromhex('bff99205ed4ab7a8')),
    (bytes.fromhex('cd4c9070196bbdebb425cb7c4350082c'), bytes.fromhex('96739be6b1299f77')),
    (bytes.fromhex('4a4f2a204fad58fd273622b70c2b559e'), bytes.fromhex('fda32d94ae77c121')),
    (bytes.fromhex('66aaeef37037dee0cdeefb22bde96807'), bytes.fromhex('efaa66f0caa6f0cd')),
]
PAIRS8 = [
    (bytes.fromhex('0eabfe9d351af837'), bytes.fromhex('b15156d14683cd15')),
    (bytes.fromhex('4a90250e855128bf'), bytes.fromhex('14a267bec8d8c3dc')),
    (bytes.fromhex('63b95b1a3fbf41fe'), bytes.fromhex('2fa71d4722835ea2')),
    (bytes.fromhex('596e3885f2460f4a'), bytes.fromhex('2cdf61de56551b86')),
    (bytes.fromhex('a00059b79f90a8ae'), bytes.fromhex('a225de4e470324b6')),
]

def valid_nibbles(b):
    return (b & 0xF) <= 4 and (b >> 4) <= 4

print("\n  Full 16-byte pairs — bytes with valid nibbles:")
for pi, (ch, resp) in enumerate(PAIRS16):
    valid = [(i, ch[i]) for i in range(16) if valid_nibbles(ch[i])]
    print(f"  pair {pi+1} ({ch.hex()[:8]}...): {[(f'{i}=0x{v:02x}' if True else '') for i,v in valid]}")

print("\n  Quick 8-byte pairs — bytes with valid nibbles:")
for pi, (ch, resp) in enumerate(PAIRS8):
    valid = [(i, ch[i]) for i in range(8) if valid_nibbles(ch[i])]
    print(f"  pair {pi+1} ({ch.hex()}): {[(f'{i}=0x{v:02x}') for i,v in valid]}")

# ---------------------------------------------------------------------------
# Look at what data is at 0x08049EE9 area (CAN dispatch table)
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== CAN dispatch table around 0x08049EE9 ===")
print("="*60)
off = 0x049E80
print("  Bytes at 0x08049E80-0x08049FFF:")
for i in range(0, 0x180, 16):
    row = data[off+i:off+i+16]
    hex_str = ' '.join(f'{b:02x}' for b in row)
    print(f"  0x{BASE+off+i:08X}: {hex_str}")

# ---------------------------------------------------------------------------
# Look for CAN receive handler for 0x0714
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== Look at 0x08049F5D area (0x0714 table entry) ===")
print("="*60)
off_714 = 0x049F5D - 8
chunk = data[off_714:off_714+0x30]
hex_str = ' '.join(f'{b:02x}' for b in chunk)
print(f"  Raw: {hex_str}")
# Try interpreting as 12-byte entries
print("\n  As 12-byte entries:")
for i in range(0, len(chunk)-11, 4):
    ptr = u32(off_714 + i)
    id_ = u16(off_714 + i + 4)
    dlc = data[off_714 + i + 6]
    flags = data[off_714 + i + 7]
    extra = u32(off_714 + i + 8)
    print(f"  +{i:2d}: ptr=0x{ptr:08X} id=0x{id_:04x} dlc={dlc} flags=0x{flags:02x} extra=0x{extra:08x}")

# Or try as 16-byte entries
print("\n  As 16-byte entries:")
for i in range(0, len(chunk)-15, 8):
    v0 = u32(off_714 + i)
    v1 = u32(off_714 + i + 4)
    v2 = u32(off_714 + i + 8)
    v3 = u32(off_714 + i + 12) if i+15 < len(chunk) else 0
    print(f"  +{i:2d}: 0x{v0:08X} 0x{v1:08X} 0x{v2:08X} 0x{v3:08X}")

# ---------------------------------------------------------------------------
# Unicorn: try full IMMO computation chain with different SRAM layouts
# ---------------------------------------------------------------------------
try:
    from unicorn import *
    from unicorn.arm_const import *
    print(f"\n{'='*60}")
    print("=== Unicorn: try crypto computation with different challenge layouts ===")
    print("="*60)

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x00400000
    SRAM_BASE  = 0x20000000
    SRAM_SIZE  = 0x00020000
    SENTINEL   = 0x10000000

    # The crypto function at 0x08201E2C needs:
    # - SRAM[0x200002C8] = 1 (trigger real auth, not first-time init)
    # - SRAM[0x200003FE] = 1 (FUN_082037D8 returns 1)
    # - data_ptr = 0x20000C14 (4-byte header)
    # - data_ptr+4 = 0x20000C18 = "challenge" bytes
    # - header: [count, key_type=1, 0xFF, 0x00 or 0x01]
    # - challenge first byte must have nibbles ≤ 4

    # Try: what layout puts a byte with valid nibbles at 0x20000C18?
    # For pair 1, challenge[7]=0x40 has valid nibbles.
    # Maybe the SRAM layout is: header | reversed(frame1) | reversed(frame2)?
    # Or: header | frame2 | frame1?

    def emulate_immo(challenge_bytes, header_byte2=0xFF, header_byte3=0x00, label=""):
        """Try to emulate the IMMO crypto function with given challenge bytes at 0x20000C18."""
        mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        mu.mem_map(FLASH_BASE, FLASH_SIZE)
        mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
        mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
        mu.mem_map(SRAM_BASE, SRAM_SIZE)
        mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
        mu.mem_map(SENTINEL, 0x1000)
        mu.mem_write(SENTINEL, b'\xFF\xBE' * 0x800)

        # Set up SRAM flags
        mu.mem_write(0x200002C8, bytes([1]))      # already initialized → call FUN_0820630C
        mu.mem_write(0x200003FE, bytes([1]))      # FUN_082037D8 returns 1
        mu.mem_write(0x200002CA, bytes([1]))      # already initialized in init fn
        
        # Set up challenge buffer at 0x20000C14
        count = len(challenge_bytes) + 4
        header = bytes([count, 0x01, header_byte2, header_byte3])
        mu.mem_write(0x20000C14, header + challenge_bytes[:min(len(challenge_bytes), 20)])

        # Set up SRAM table (0x20001018) for index 1
        # Register crypto fn 0x08201E2D at +0x34 for index 1
        table_base = 0x20001018
        index = 1
        entry_base = table_base + index * 0x54
        mu.mem_write(entry_base + 0x34, struct.pack('<I', 0x08201E2D))

        # Set up return address as sentinel
        SP = (SRAM_BASE + SRAM_SIZE - 0x100) & ~7
        # Stack: sp+0 = response buf, sp+4 = 0x3FC, sp+8 = &output_len
        OUT_LEN = SRAM_BASE + 0x8000
        RESP_BUF = 0x20000C1C
        SP -= 16
        mu.reg_write(UC_ARM_REG_SP, SP)
        mu.mem_write(SP,     struct.pack('<I', RESP_BUF))
        mu.mem_write(SP + 4, struct.pack('<I', 0x3FC))
        mu.mem_write(SP + 8, struct.pack('<I', OUT_LEN))

        # Set up caller context that FUN_0820630C expects
        # SRAM table at 0x20001018 index, byte at [base] = index
        mu.mem_write(0x200003FC, bytes([1]))      # index byte

        # Callback storage
        mu.mem_write(0x200002E4, b'\x00' * 8)
        mu.mem_write(0x200002E8, b'\x00' * 8)

        mu.reg_write(UC_ARM_REG_R0, 1)           # key_type
        mu.reg_write(UC_ARM_REG_R1, 0xFF00)      # session_mode
        mu.reg_write(UC_ARM_REG_R2, 0x20000C18)  # data_ptr+4
        mu.reg_write(UC_ARM_REG_R3, len(challenge_bytes) - 4)  # count-4
        mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)

        resp_writes = {}
        callback_called = [False]
        def hook_mem(mu, access, address, size, value, ud):
            v = value & ((1 << (size*8)) - 1)
            if 0x20000C00 <= address < 0x20000D00:
                resp_writes[address] = (v, size)
        def hook_code(mu, addr, sz, ud):
            if addr == SENTINEL: mu.emu_stop()
            # Detect callback invocations
            if addr == 0x08201A48: callback_called[0] = True
        def hook_inv(mu, *a): mu.emu_stop(); return False

        mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem)
        mu.hook_add(UC_HOOK_CODE, hook_code)
        mu.hook_add(UC_HOOK_MEM_INVALID, hook_inv)

        try:
            mu.emu_start(0x08201E2C | 1, SENTINEL, timeout=5_000_000, count=300_000)
        except UcError as e:
            pass

        r0 = mu.reg_read(UC_ARM_REG_R0)
        result = bytes(mu.mem_read(0x20000C00, 0x40))
        return r0, result, resp_writes, callback_called[0]

    # Test pair 1 with various layouts
    ch = PAIRS16[0][0]  # 660be1e2a34b8140b45633a0499a01ec
    expected = PAIRS16[0][1]  # 9cb7f8ca31431bb6

    print(f"\n  Pair 1 challenge: {ch.hex()}")
    print(f"  Expected response: {expected.hex()}")
    
    for layout_name, challenge_buf in [
        ("raw 16 bytes",            ch),
        ("reversed 16 bytes",       bytes(reversed(ch))),
        ("frame2||frame1",          ch[8:] + ch[:8]),
        ("rev(frame1)||frame2",     bytes(reversed(ch[:8])) + ch[8:]),
        ("frame1||rev(frame2)",     ch[:8] + bytes(reversed(ch[8:]))),
        ("rev(frame2)||rev(frame1)", bytes(reversed(ch[8:])) + bytes(reversed(ch[:8]))),
    ]:
        r0, result, writes, cb = emulate_immo(challenge_buf, label=layout_name)
        resp = result[0x18:0x20]  # bytes at 0x20000C18-0x20000C1F
        match = "*** MATCH! ***" if resp == expected else ""
        print(f"\n  Layout: {layout_name}")
        print(f"    r0={r0}  callback={cb}")
        print(f"    SRAM[0x20000C18:0x20000C28] = {result[0x18:0x28].hex()}")
        if match:
            print(f"    {match}")

except ImportError:
    print("unicorn not available")

print("\n=== Done ===")
