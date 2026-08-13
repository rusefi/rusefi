#!/usr/bin/env python3
"""
trace_init.py  – disassemble and trace the IMMO table initialization.

Key findings:
  BL -> 0x08203718 (stores fn ptr at +0x34) called from 0x0820204E
  BL -> 0x08203734 (stores fn ptr at +0x38?) called from 0x08202056
  BL -> 0x08203750 (stores fn ptr at +0x3c?) called from 0x08202060
  BL -> 0x08203770 (stores fn ptr at +0x4c?) called from 0x08202068

The function containing these calls should load crypto fn ptrs into R0.
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

HIGHLIGHTS = {0x0820204E, 0x08202056, 0x08202060, 0x08202068}

def disasm_region(chip_start, length, label):
    off = foff(chip_start)
    chunk = data[off:off+length]
    print(f"\n{'='*70}")
    print(f"  {label}  @ 0x{chip_start:08X}, {length} bytes:")
    print(f"{'='*70}")
    pc_loads = {}
    for insn in md.disasm(chunk, chip_start):
        ann = ""
        ioff = foff(insn.address)
        r = resolve_ldr_pc(ioff, insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> [0x{tgt:08X}] = 0x{val:08X}"
            if BASE <= (val & ~1) < BASE + len(data) and (val & 1):
                ann += f"  (THUMB fn @ 0x{val&~1:08X})"
            elif 0x20000000 <= val < 0x20020000:
                ann += "  (SRAM)"
        if insn.address in HIGHLIGHTS:
            ann += "  <<<< REGISTRAR CALL"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")

# ---------------------------------------------------------------------------
# Disassemble the init function 0x08202000 – 0x08202100
# ---------------------------------------------------------------------------
disasm_region(0x08201FE0, 0x180, "Init function containing registrar calls")

# ---------------------------------------------------------------------------
# Disassemble wider range to find what loads R0 before each call
# ---------------------------------------------------------------------------
disasm_region(0x08201F80, 0x200, "Wider context of init function")

# ---------------------------------------------------------------------------
# The function that starts around 0x08201FC0 or before
# Let's find the function prologue (PUSH instruction) before 0x0820204E
# ---------------------------------------------------------------------------
print(f"\n{'='*70}")
print("  Looking for PUSH (function prologue) before 0x0820204E")
print(f"{'='*70}")

# Scan backwards from 0x0820204E looking for PUSH
search_start = 0x08202000
for addr in range(0x0820204E, 0x08201E00, -2):
    off = foff(addr)
    hw = u16(off)
    # PUSH instruction: 1011 0 0 register_list = 0xB400 | regs
    # or PUSH.W
    if (hw & 0xFF00) == 0xB500 or (hw & 0xFF00) == 0xB400:
        print(f"  PUSH found at 0x{addr:08X}:")
        disasm_region(addr, 0x100, "Function starting at PUSH")
        break

# ---------------------------------------------------------------------------
# Disassemble registrar functions to understand what offset they use
# ---------------------------------------------------------------------------
print(f"\n{'='*70}")
print("  Disassemble registrar functions 0x08203718 through 0x08203790")
print(f"{'='*70}")
disasm_region(0x08203714, 0x100, "Registrars")

# ---------------------------------------------------------------------------
# For each registrar's literal pool, read the stored addresses
# ---------------------------------------------------------------------------
print(f"\n{'='*70}")
print("  Literal pools near registrars (should contain 0x20001018)")
print(f"{'='*70}")
for pool_addr in [0x08203730, 0x0820374C, 0x08203768, 0x08203784, 0x082037D4, 0x082037D8]:
    if BASE <= pool_addr < BASE + len(data):
        val = u32(foff(pool_addr))
        print(f"  [0x{pool_addr:08X}] = 0x{val:08X}")

# ---------------------------------------------------------------------------
# Unicorn emulation: emulate the init function and read the SRAM table
# ---------------------------------------------------------------------------
if UNICORN:
    print(f"\n{'='*70}")
    print("  Unicorn: emulate init to populate SRAM table, then run crypto")
    print(f"{'='*70}")

    KNOWN_PAIRS = [
        (bytes.fromhex('660be1e2a34b8140b45633a0499a01ec'), bytes.fromhex('9cb7f8ca31431bb6')),
        (bytes.fromhex('cfcfbbf3cdc0f75ce9efe2eb23b62a25'), bytes.fromhex('bff99205ed4ab7a8')),
    ]
    PAIRS8 = [
        (bytes.fromhex('0eabfe9d351af837'), bytes.fromhex('b15156d14683cd15')),
    ]

    FLASH_BASE  = 0x08000000
    FLASH_SIZE  = 0x00400000
    SRAM_BASE   = 0x20000000
    SRAM_SIZE   = 0x00020000
    STACK_TOP   = SRAM_BASE + SRAM_SIZE - 8
    SENTINEL    = 0x10000000

    def make_uc():
        mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
        # Flash: fill with UNDEF then write firmware
        mu.mem_map(FLASH_BASE, FLASH_SIZE)
        mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))  # BKPT
        mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
        # SRAM
        mu.mem_map(SRAM_BASE, SRAM_SIZE)
        mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
        # Sentinel
        mu.mem_map(SENTINEL, 0x1000)
        mu.mem_write(SENTINEL, b'\x00\xBE' * 0x800)
        # SP
        mu.reg_write(UC_ARM_REG_SP, STACK_TOP & ~7)
        # LR to sentinel
        mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
        return mu

    # -----------------------------------------------------------------------
    # Phase 1: Emulate the init function to populate SRAM table
    # Init function contains the registrar calls starting around 0x08202000
    # We need to find the actual init function entry
    # -----------------------------------------------------------------------
    # From disassembly we saw PUSH {r4,lr} or similar before 0x0820204E
    # Let's try to run from 0x08201FC0 (conservative start)

    # First: just emulate the init function up to and past all registrar calls
    # and capture what gets written to SRAM 0x20001018+

    SRAM_TABLE = 0x20001018

    def emulate_init():
        mu = make_uc()
        
        # Track stores to SRAM table region
        table_writes = {}
        
        def hook_mem_write(mu, access, address, size, value, user_data):
            if SRAM_TABLE <= address < SRAM_TABLE + 7 * 0x54 + 0x50:
                offset = address - SRAM_TABLE
                table_writes[offset] = (value, mu.reg_read(UC_ARM_REG_PC))
        
        def hook_code(mu, address, size, user_data):
            if address == SENTINEL:
                mu.emu_stop()
        
        def hook_invalid(mu, access, address, size, value, user_data):
            mu.emu_stop()
            return False
        
        mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem_write)
        mu.hook_add(UC_HOOK_CODE, hook_code)
        mu.hook_add(UC_HOOK_MEM_INVALID, hook_invalid)
        
        # Find the init function. Look for PUSH near 0x0820204E backwards.
        init_start = None
        for addr in range(0x0820204E, 0x08201E00, -2):
            hw = u16(foff(addr))
            if (hw & 0xFF00) == 0xB500:  # PUSH {rX, lr}
                init_start = addr
                break
        
        if init_start is None:
            print("  Could not find init function entry!")
            return {}
        
        print(f"  Emulating init from 0x{init_start:08X}...")
        try:
            mu.emu_start(init_start | 1, SENTINEL, timeout=10_000_000, count=100_000)
        except UcError as e:
            print(f"  Emulation stopped: {e}")
        
        return table_writes, mu

    print("  Running init emulation...")
    result = emulate_init()
    if result:
        table_writes, mu = result
        print(f"  Writes to SRAM table (0x{SRAM_TABLE:08X}+):")
        for off, (val, pc) in sorted(table_writes.items()):
            idx = off // 0x54
            field = off % 0x54
            print(f"    [0x{SRAM_TABLE+off:08X}] idx={idx} +0x{field:02X} = 0x{val:08X}  (written from PC 0x{pc:08X})",
                  end="")
            if BASE <= (val & ~1) < BASE + len(data) and (val & 1):
                print(f"  <- THUMB fn @ 0x{val&~1:08X}", end="")
            print()
        
        # -----------------------------------------------------------------------
        # Phase 2: Run crypto with the populated table
        # -----------------------------------------------------------------------
        # The IMMO computation function at 0x082027A4 reads:
        #   r2 = SRAM 0x20001018 base
        #   r6 = [r2 + index*0x54 + 0x34]
        # Let's find what's at +0x34 for index 0
        fn_ptr_34 = None
        for off, (val, pc) in table_writes.items():
            idx = off // 0x54
            field = off % 0x54
            if field == 0x34 and idx == 0:
                fn_ptr_34 = val
                break
        
        if fn_ptr_34:
            print(f"\n  Crypto fn ptr at +0x34 index 0: 0x{fn_ptr_34:08X}")
            # Disassemble that function!
            fn_addr = fn_ptr_34 & ~1
            print(f"  Disassembling crypto fn @ 0x{fn_addr:08X}:")
            disasm_region(fn_addr, 0x200, f"Crypto fn 0x{fn_addr:08X}")
        else:
            print("  Could not find +0x34 slot in table writes")
            # Show all +0x34 writes
            for off, (val, pc) in table_writes.items():
                if off % 0x54 == 0x34:
                    print(f"  +0x34 found at idx {off//0x54}: 0x{val:08X}")

# ---------------------------------------------------------------------------
# Direct approach: look at what LDR loads into R0 before 0x0820204E
# ---------------------------------------------------------------------------
print(f"\n{'='*70}")
print("  Direct trace: decode instructions at 0x08202020-0x08202070")
print(f"{'='*70}")
disasm_region(0x08202010, 0x80, "Init registrar call sequence")

print("\n=== Done ===")
