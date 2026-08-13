#!/usr/bin/env python3
"""
crypto_decode.py  – disassemble and emulate the IMMO crypto fn at 0x08201E2C

Known:
  - Init registers 0x08201E2D (Thumb) as fn ptr at SRAM[0x20001018 + 1*0x54 + 0x34]
  - Called from 0x082027FA (blx r6) when computing IMMO response to full 16-byte challenge
  - Calling convention (from 0x082027A4 context):
      r0 = key_type (SRAM byte & 0x7f, probably = 1 for standard key)
      r1 = session_counter_word  (orr r1 | r0 << 8 etc.)
      r2 = challenge data ptr + 4
      r3 = byte_count - 4
      sp+0  = 0x20000C1C  (response output buffer)
      sp+4  = 0x3FC
      sp+8  = &sp[0x16]   (ptr to output length counter)
    On success: returns 0, response written starting at **(sp+0)
    On failure: returns non-zero

Also disassemble the registrar call targets for +0x3c, +0x40, +0x4c:
  - 0x08201DF0
  - 0x08201D84
  - 0x082017D8
"""

import struct, sys
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
except ImportError:
    print("pip install capstone"); sys.exit(1)

try:
    from unicorn import *
    from unicorn.arm_const import *
    UNICORN = True
except ImportError:
    UNICORN = False

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

def disasm(chip_start, length, label):
    off = foff(chip_start)
    chunk = data[off:off+length]
    print(f"\n{'='*70}")
    print(f"  {label}  @ 0x{chip_start:08X}")
    print(f"{'='*70}")
    for insn in md.disasm(chunk, chip_start):
        ann = ""
        r = resolve_ldr_pc(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> [0x{tgt:08X}] = 0x{val:08X}"
            if BASE <= (val & ~1) < BASE + len(data) and (val & 1):
                ann += f"  (fn @ 0x{val&~1:08X})"
            elif 0x20000000 <= val < 0x20020000:
                ann += "  (SRAM)"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# Disassemble crypto fn and its neighbors
disasm(0x08201E2C, 0x300, "CRYPTO FN: 0x08201E2C (full challenge response)")
disasm(0x08201DF0, 0x60,  "fn @ 0x08201DF0 (registered at +0x3c)")
disasm(0x08201D84, 0x80,  "fn @ 0x08201D84 (registered at +0x40+0x44)")
disasm(0x082017D8, 0x60,  "fn @ 0x082017D8 (registered at +0x4c)")

# Also look at what fn is registered at +0x04 and +0x10:
disasm(0x08202018, 0x80,  "fn @ 0x08202018 (registered at +0x04)")
disasm(0x08201AE0, 0x80,  "fn @ 0x08201AE0 (registered at +0x10)")
disasm(0x082017F8, 0x80,  "fn @ 0x082017F8 (registered at +0x14)")

# ---------------------------------------------------------------------------
# Known challenge/response pairs
# ---------------------------------------------------------------------------
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

# ---------------------------------------------------------------------------
# Unicorn emulation of the crypto function directly
# ---------------------------------------------------------------------------
if UNICORN:
    print(f"\n{'='*70}")
    print("  Unicorn: direct emulation of crypto fn 0x08201E2C")
    print(f"{'='*70}")

    FLASH_BASE = 0x08000000
    FLASH_SIZE = 0x00400000
    SRAM_BASE  = 0x20000000
    SRAM_SIZE  = 0x00020000
    STACK_BOT  = SRAM_BASE + 0x10000
    STACK_TOP  = SRAM_BOT = STACK_BOT + 0x1000  # 4KB stack for this fn
    SENTINEL   = 0x10000000
    RESP_BUF   = 0x20000C1C   # response output buffer (sp+0 arg)

    def emulate_crypto(challenge, is_full=True):
        """
        Emulate the crypto function at 0x08201E2C.
        Returns the bytes written to RESP_BUF, or None on failure.
        """
        mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        # Flash
        mu.mem_map(FLASH_BASE, FLASH_SIZE)
        mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
        mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
        # SRAM
        mu.mem_map(SRAM_BASE, SRAM_SIZE)
        mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
        # Sentinel
        mu.mem_map(SENTINEL, 0x1000)
        mu.mem_write(SENTINEL, b'\xFF\xBE' * 0x800)

        # --- Populate SRAM that the crypto fn may read ---
        # IMMO state at 0x20000402 (status/session counter)
        mu.mem_write(0x20000402, struct.pack('<H', 5))   # status = 5 (valid state)
        # Challenge data at 0x20000414
        if is_full:
            mu.mem_write(0x20000414, challenge)          # 16 bytes
        else:
            mu.mem_write(0x20000414, challenge + b'\x00'*8)  # 8 bytes + pad
        # Key material in calibration area (just in case fn reads it)
        # Populate 0x200003FC and nearby SRAM with reasonable values
        mu.mem_write(0x200003FC, bytes([1, 1, 0, 0, 0, 0, 0, 0]))  # key_type=1

        # --- Set up stack ---
        SP = (SRAM_BASE + SRAM_SIZE - 0x100) & ~7
        mu.reg_write(UC_ARM_REG_SP, SP)

        # Push stack args (from 0x082027A4 context):
        # sp+0  = RESP_BUF (response output ptr)
        # sp+4  = 0x3FC
        # sp+8  = ptr to output length (some SRAM addr, use SP-4)
        OUT_LEN_PTR = SRAM_BASE + 0x8000
        mu.mem_write(SRAM_BASE + 0x8000, b'\x00' * 4)
        # Write stack args below current SP
        SP -= 16
        mu.reg_write(UC_ARM_REG_SP, SP)
        struct.pack_into('<III', bytearray(16), 0)
        mu.mem_write(SP,     struct.pack('<I', RESP_BUF))
        mu.mem_write(SP + 4, struct.pack('<I', 0x3FC))
        mu.mem_write(SP + 8, struct.pack('<I', OUT_LEN_PTR))

        # Registers for the call:
        # r0 = key_type = 1
        # r1 = session counter word (try 0x0001)
        # r2 = challenge ptr + 4
        # r3 = byte_count - 4  (for 16-byte challenge: count=16 -> r3=12)
        CHALLENGE_PTR = 0x20000414
        if is_full:
            mu.reg_write(UC_ARM_REG_R0, 1)              # key_type
            mu.reg_write(UC_ARM_REG_R1, 0x0001)         # session counter
            mu.reg_write(UC_ARM_REG_R2, CHALLENGE_PTR + 4)  # data + 4
            mu.reg_write(UC_ARM_REG_R3, 12)             # count - 4
        else:
            mu.reg_write(UC_ARM_REG_R0, 1)
            mu.reg_write(UC_ARM_REG_R1, 0x0001)
            mu.reg_write(UC_ARM_REG_R2, CHALLENGE_PTR + 4)
            mu.reg_write(UC_ARM_REG_R3, 4)              # 8 bytes - 4

        mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)

        resp_buf_writes = {}

        def hook_mem_write(mu, access, address, size, value, user_data):
            if RESP_BUF <= address < RESP_BUF + 0x20:
                resp_buf_writes[address - RESP_BUF] = (value & ((1 << (size*8))-1), size)

        def hook_code(mu, address, size, user_data):
            if address == SENTINEL:
                mu.emu_stop()

        def hook_invalid(mu, access, address, size, value, user_data):
            mu.emu_stop()
            return False

        mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem_write)
        mu.hook_add(UC_HOOK_CODE, hook_code)
        mu.hook_add(UC_HOOK_MEM_INVALID, hook_invalid)

        FN_ADDR = 0x08201E2C
        try:
            mu.emu_start(FN_ADDR | 1, SENTINEL, timeout=5_000_000, count=500_000)
        except UcError as e:
            return None, f"UcError: {e}"

        # Read response buffer
        try:
            result = bytes(mu.mem_read(RESP_BUF, 16))
            ret_r0 = mu.reg_read(UC_ARM_REG_R0)
            out_len = struct.unpack_from('<I', bytes(mu.mem_read(OUT_LEN_PTR, 4)))[0]
            return result, f"r0={ret_r0} out_len={out_len}"
        except Exception as e:
            return None, str(e)

    print("\n  Testing with known challenge/response pairs:")
    for i, (ch, expected) in enumerate(PAIRS16):
        result, info = emulate_crypto(ch, is_full=True)
        if result:
            got = result[:8]
            match = "MATCH!" if got == expected else "no match"
            print(f"  pair {i+1}: [{info}]")
            print(f"    expected:  {expected.hex()}")
            print(f"    got[0:8]:  {got.hex()}  {match}")
            print(f"    full[0:16]: {result.hex()}")
        else:
            print(f"  pair {i+1}: emulation failed ({info})")

    # Try without the +4 offset on the challenge pointer
    print("\n  Retry: r2 = challenge ptr (no +4 offset):")
    for i, (ch, expected) in enumerate(PAIRS16[:2]):
        mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        mu.mem_map(FLASH_BASE, FLASH_SIZE)
        mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
        mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
        mu.mem_map(SRAM_BASE, SRAM_SIZE)
        mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
        mu.mem_map(SENTINEL, 0x1000)
        mu.mem_write(SENTINEL, b'\xFF\xBE' * 0x800)
        mu.mem_write(0x20000402, struct.pack('<H', 5))
        mu.mem_write(0x20000414, ch)
        mu.mem_write(0x200003FC, bytes([1, 1, 0, 0, 0, 0, 0, 0]))

        SP = (SRAM_BASE + SRAM_SIZE - 0x100) & ~7
        OUT_LEN_PTR = SRAM_BASE + 0x8000
        mu.mem_write(SRAM_BASE + 0x8000, b'\x00' * 4)
        SP -= 16
        mu.reg_write(UC_ARM_REG_SP, SP)
        mu.mem_write(SP,     struct.pack('<I', RESP_BUF))
        mu.mem_write(SP + 4, struct.pack('<I', 0x3FC))
        mu.mem_write(SP + 8, struct.pack('<I', OUT_LEN_PTR))
        mu.reg_write(UC_ARM_REG_R0, 1)
        mu.reg_write(UC_ARM_REG_R1, 0x0001)
        mu.reg_write(UC_ARM_REG_R2, 0x20000414)   # NO +4
        mu.reg_write(UC_ARM_REG_R3, 16)
        mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        resp = {}
        def hook_w(mu, access, address, size, value, user_data):
            if RESP_BUF <= address < RESP_BUF + 0x20:
                resp[address - RESP_BUF] = (value & ((1<<(size*8))-1), size)
        def hook_c(mu, addr, sz, ud):
            if addr == SENTINEL: mu.emu_stop()
        def hook_inv(mu, *a): mu.emu_stop(); return False
        mu.hook_add(UC_HOOK_MEM_WRITE, hook_w)
        mu.hook_add(UC_HOOK_CODE, hook_c)
        mu.hook_add(UC_HOOK_MEM_INVALID, hook_inv)
        try:
            mu.emu_start(0x08201E2C | 1, SENTINEL, timeout=5_000_000, count=500_000)
            res = bytes(mu.mem_read(RESP_BUF, 16))
            r0 = mu.reg_read(UC_ARM_REG_R0)
            print(f"  pair {i+1} (r2=ptr): r0={r0}  got={res[:8].hex()}  expected={expected.hex()}")
        except UcError as e:
            print(f"  pair {i+1}: {e}")

# ---------------------------------------------------------------------------
# Look at what 0x08201E2C reads: scan for known challenge bytes in SRAM
# ---------------------------------------------------------------------------
print(f"\n{'='*70}")
print("  What SRAM addresses does the crypto fn read?")
print("  (Looking at all LDR that could be reading challenge/response data)")
print(f"{'='*70}")

# Scan 0x08201E2C for any references to known SRAM ranges
# that could be key material or challenge data
crypto_off = foff(0x08201E2C)
chunk = data[crypto_off:crypto_off+0x300]
for insn in md.disasm(chunk, 0x08201E2C):
    r = resolve_ldr_pc(foff(insn.address), insn.address)
    if r:
        Rd, tgt, val = r
        # Is it loading an address in the calibration or key area?
        if 0x20000000 <= val < 0x20020000:
            print(f"  0x{insn.address:08X} -> SRAM 0x{val:08X} (r{Rd})")
        elif 0x08040000 <= (val & ~1) < 0x08060000:
            print(f"  0x{insn.address:08X} -> CAL  0x{val:08X} (r{Rd})")

print("\n=== Done ===")
