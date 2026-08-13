#!/usr/bin/env python3
"""
trace_crypto_init.py - Отследить инициализацию крипто-движка через Unicorn

Цель: понять что делает FUN_08205938 (crypto init) и как ключ из flash[0x08200000]
используется в вычислении ответа.

Ключевые факты:
- flash[0x08200000] = 12 C2 A0 43 DD 7B 3C 50 C9 26 59 01 48 7A 33 2E (16 байт)
- FUN_08204BAC(r0=0) инициализирует крипто с key_const=0x2548A4D2
- FUN_08205938 вызывается из setup fn 0x08205F88
"""

import struct, sys
from pathlib import Path
from unicorn import *
from unicorn.arm_const import *

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(a): return a - BASE
def u32(o): return struct.unpack_from('<I', data, o)[0]

FLASH_BASE = 0x08000000; FLASH_SIZE = 0x00400000
SRAM_BASE  = 0x20000000; SRAM_SIZE  = 0x00020000
PERIPH_BASE= 0x40000000; PERIPH_SIZE= 0x10000000
SENTINEL   = 0x10000000

PAIRS8 = [
    (bytes.fromhex('0eabfe9d351af837'), bytes.fromhex('b15156d14683cd15')),
    (bytes.fromhex('4a90250e855128bf'), bytes.fromhex('14a267bec8d8c3dc')),
    (bytes.fromhex('63b95b1a3fbf41fe'), bytes.fromhex('2fa71d4722835ea2')),
    (bytes.fromhex('596e3885f2460f4a'), bytes.fromhex('2cdf61de56551b86')),
    (bytes.fromhex('a00059b79f90a8ae'), bytes.fromhex('a225de4e470324b6')),
]

def make_uc():
    mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    mu.mem_map(FLASH_BASE, FLASH_SIZE)
    mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
    mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
    mu.mem_map(SRAM_BASE, SRAM_SIZE)
    mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
    mu.mem_map(SENTINEL, 0x1000)
    mu.mem_write(SENTINEL, b'\xFF\xBE' * 0x800)
    mu.mem_map(PERIPH_BASE, PERIPH_SIZE)
    mu.mem_write(PERIPH_BASE, b'\x00' * PERIPH_SIZE)
    SP = (SRAM_BASE + SRAM_SIZE - 0x200) & ~7
    mu.reg_write(UC_ARM_REG_SP, SP)
    mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    return mu

# ---------------------------------------------------------------------------
# Step 1: Trace FUN_08205938 (crypto init) - что оно делает с константами
# ---------------------------------------------------------------------------
print("="*65)
print("Step 1: FUN_08205938 (crypto init) — что делает с данными")
print("="*65)

def run_fn(fn_addr, r0=0, r1=0, r2=0, r3=0, label="", setup_fn=None):
    mu = make_uc()
    if setup_fn: setup_fn(mu)
    mu.reg_write(UC_ARM_REG_R0, r0)
    mu.reg_write(UC_ARM_REG_R1, r1)
    mu.reg_write(UC_ARM_REG_R2, r2)
    mu.reg_write(UC_ARM_REG_R3, r3)

    writes = {}
    executed = [0]
    def hook_m(mu, access, address, size, value, ud):
        v = value & ((1 << (size*8)) - 1)
        for i in range(size):
            b = (v >> (i*8)) & 0xFF
            if SRAM_BASE <= address+i < SRAM_BASE + SRAM_SIZE:
                writes[address+i] = b
    def hook_c(mu, addr, sz, ud):
        executed[0] += 1
        if addr == SENTINEL: mu.emu_stop()
        if executed[0] > 300000: mu.emu_stop()
    def hook_i(mu, *a): mu.emu_stop(); return False

    mu.hook_add(UC_HOOK_MEM_WRITE, hook_m)
    mu.hook_add(UC_HOOK_CODE, hook_c)
    mu.hook_add(UC_HOOK_MEM_INVALID, hook_i)

    try:
        mu.emu_start(fn_addr | 1, SENTINEL, timeout=5_000_000, count=300_000)
    except UcError:
        pass

    r0_ret = mu.reg_read(UC_ARM_REG_R0)
    print(f"\n[{label}] r0_in=0x{r0:08X} r0_ret=0x{r0_ret:08X} instr={executed[0]}")
    relevant = {k: v for k, v in writes.items()
                if 0x20001400 <= k < 0x20002000}
    if relevant:
        addrs = sorted(relevant.keys())
        print(f"  SRAM writes [0x20001400..0x20002000]:")
        prev = -1
        for a in addrs:
            if a - prev > 4 and prev >= 0: print()
            print(f"    [0x{a:08X}]={relevant[a]:02X}", end="  ")
            prev = a
        print()
    return r0_ret, writes, mu

# Запускаем FUN_08205938 напрямую
_, _, _ = run_fn(0x08205938, label="FUN_08205938 (no args)")

# ---------------------------------------------------------------------------
# Step 2: Эмуляция FUN_08204BAC(r0=0) — инициализация ключей
# ---------------------------------------------------------------------------
print("\n" + "="*65)
print("Step 2: FUN_08204BAC(r0=0) — инициализация ключа")
print("="*65)

def init_bac(mu):
    """Настройка перед FUN_08204BAC."""
    mu.mem_write(0x20001C14, bytes([0]))  # не инициализировано
    # Дополнительные флаги
    mu.mem_write(0x20001B16, bytes([0]))  # flash engine state

_, bac_writes, bac_mu = run_fn(0x08204BAC, r0=0, label="FUN_08204BAC(0)", setup_fn=init_bac)

print("\n  ВСЕ записи в SRAM после FUN_08204BAC:")
interesting_ranges = [
    (0x20001ADC, 0x20001AE0, "key_const_A"),
    (0x20000000, 0x20000010, "SRAM[0]"),
    (0x20001B00, 0x20001C50, "IMMO state area"),
]
for start, end, name in interesting_ranges:
    relevant = {k: v for k, v in bac_writes.items() if start <= k < end}
    if relevant:
        print(f"  {name} (0x{start:08X}-0x{end:08X}):")
        for a in sorted(relevant.keys()):
            print(f"    [0x{a:08X}] = 0x{relevant[a]:02X}")

# ---------------------------------------------------------------------------
# Step 3: После инициализации — трассируем что происходит с ключом
# ---------------------------------------------------------------------------
print("\n" + "="*65)
print("Step 3: Полная трассировка с реальными данными challenge")
print("="*65)

# Используем первую quick пару
ch, expected_resp = PAIRS8[0]
print(f"Challenge: {ch.hex()}")
print(f"Expected:  {expected_resp.hex()}")

# Пробуем запустить FUN_08205938 с данными challenge в SRAM
def setup_with_challenge(mu):
    # Сначала инициализируем как BАС делает
    mu.mem_write(0x20001C14, bytes([0]))
    mu.mem_write(0x20001B16, bytes([0]))
    # Кладём challenge в буфер 0x20000C18
    mu.mem_write(0x20000C14, bytes([0x0C, 0x01, 0xFF, 0x00]))
    mu.mem_write(0x20000C18, ch + b'\x00'*4)

run_fn(0x08205938, setup_fn=setup_with_challenge, label="FUN_08205938 with challenge in SRAM")

# ---------------------------------------------------------------------------
# Step 4: Ищем алгоритм методом перебора — пробуем всё что есть
# ---------------------------------------------------------------------------
print("\n" + "="*65)
print("Step 4: Перебор алгоритмов (ключ из 0x08200000)")
print("="*65)

key_raw = data[foff(0x08200000):foff(0x08200000)+16]
print(f"Ключ из flash[0x08200000:16]: {key_raw.hex()}")
print(f"= {[hex(u32(foff(0x08200000)+i)) for i in range(0,16,4)]}")

# TEA алгоритм
def tea_enc(v0, v1, key):
    """TEA encrypt: 64-bit block, 128-bit key"""
    DELTA = 0x9E3779B9; s = 0
    for _ in range(32):
        s = (s + DELTA) & 0xFFFFFFFF
        v0 = (v0 + (((v1 << 4) + key[0]) ^ (v1 + s) ^ ((v1 >> 5) + key[1]))) & 0xFFFFFFFF
        v1 = (v1 + (((v0 << 4) + key[2]) ^ (v0 + s) ^ ((v0 >> 5) + key[3]))) & 0xFFFFFFFF
    return v0, v1

# Разные интерпретации ключа
key_le = struct.unpack('<IIII', key_raw)
key_be = struct.unpack('>IIII', key_raw)

print(f"  TEA ключ LE: {[hex(k) for k in key_le]}")

for kname, k4 in [('LE', key_le), ('BE', key_be)]:
    for endian in ['LE', 'BE']:
        ok = 0
        for ch, resp in PAIRS8:
            if endian == 'LE':
                v0, v1 = struct.unpack('<II', ch)
                e0, e1 = tea_enc(v0, v1, k4)
                got = struct.pack('<II', e0, e1)
            else:
                v0, v1 = struct.unpack('>II', ch)
                e0, e1 = tea_enc(v0, v1, k4)
                got = struct.pack('>II', e0, e1)
            if got == resp: ok += 1
        if ok > 0:
            print(f"  TEA [{kname}] data=[{endian}]: {ok}/5")

# RC4
def rc4(key, data_in):
    S = list(range(256))
    j = 0
    for i in range(256):
        j = (j + S[i] + key[i % len(key)]) % 256
        S[i], S[j] = S[j], S[i]
    out = []; i = j = 0
    for byte in data_in:
        i = (i + 1) % 256
        j = (j + S[i]) % 256
        S[i], S[j] = S[j], S[i]
        out.append(byte ^ S[(S[i] + S[j]) % 256])
    return bytes(out)

for klen in [8, 16]:
    for koff in range(0, 8, 4):
        key = key_raw[koff:koff+klen]
        ok = sum(1 for ch, resp in PAIRS8 if rc4(key, ch) == resp)
        if ok > 0:
            print(f"  RC4 key[{koff}:{koff+klen}]: {ok}/5 → key={key.hex()}")

# Blowfish-like: пробуем custom XOR схему с ключом
# Например: resp = ch XOR (key rotated by some amount)
for rot in range(16):
    k_rotated = (key_raw[rot:] + key_raw[:rot])[:8]
    ok = sum(1 for ch, resp in PAIRS8 if bytes(a^b for a,b in zip(ch, k_rotated)) == resp)
    if ok > 0:
        print(f"  XOR с key[{rot}:{rot+8}]: {ok}/5")

# ---------------------------------------------------------------------------
# Step 5: Дизассемблируем FUN_08205938 и соседей
# ---------------------------------------------------------------------------
print("\n" + "="*65)
print("Step 5: Дизассемблируем FUN_08205938 и FUN_08205C10")
print("="*65)

try:
    from capstone import *; from capstone.arm import *
    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB+CS_MODE_LITTLE_ENDIAN)
    def resolve(off, addr):
        hw = struct.unpack_from('<H', data, off)[0]
        if (hw>>11)==0x9:
            Rd=(hw>>8)&7; imm8=hw&0xFF; pc=(addr+4)&~3; tgt=pc+imm8*4
            if BASE<=tgt<BASE+len(data): return u32(foff(tgt))
        return None
    def dis(addr, length, label):
        off = foff(addr); chunk = data[off:off+length]
        print(f"\n  {label} @ 0x{addr:08X}:")
        for i in md.disasm(chunk, addr):
            ann=''; r=resolve(foff(i.address),i.address)
            if r:
                ann=f'  -> 0x{r:08X}'
                if 0x20000000<=r<0x20020000: ann+=' (SRAM)'
                elif BASE<=r<BASE+len(data): ann+=' (flash)'
            print(f"    0x{i.address:08X}  {i.mnemonic:<12} {i.op_str}{ann}")
    dis(0x08205938, 0x200, "FUN_08205938 (crypto setup/hash init)")
    dis(0x08205C10, 0x80,  "FUN_08205C10 (memcpy completion)")
except ImportError:
    print("capstone not available")

print("\n=== Done ===")
