#!/usr/bin/env python3
"""
find_key_algo2.py - Find IMMO key and algorithm in I865LB52 firmware
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
        Rd = (hw >> 8) & 7
        imm8 = hw & 0xFF
        pc = (chip_addr + 4) & ~3
        tgt = pc + imm8 * 4
        if BASE <= tgt < BASE + len(data):
            return (Rd, tgt, u32(foff(tgt)))
    if off + 4 <= len(data):
        hw2 = u16(off+2)
        if (hw & 0xFF7F) == 0xF85F:
            Rd = (hw2 >> 12) & 0xF
            imm12 = hw2 & 0xFFF
            U = (hw >> 7) & 1
            pc = (chip_addr + 4) & ~3
            tgt = (pc + imm12) if U else (pc - imm12)
            if BASE <= tgt < BASE + len(data):
                return (Rd, tgt, u32(foff(tgt)))
    return None

md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
md.detail = True

def disasm(addr, length, label):
    off = foff(addr)
    chunk = data[off:off+length]
    print(f"\n{'='*60}")
    print(f"  {label}  @ 0x{addr:08X}")
    print(f"{'='*60}")
    sram_refs = []
    for insn in md.disasm(chunk, addr):
        ann = ""
        r = resolve_pc_ldr(foff(insn.address), insn.address)
        if r:
            Rd, tgt, val = r
            ann = f"  -> 0x{val:08X}"
            if 0x20000000 <= val < 0x20020000:
                ann += " (SRAM)"; sram_refs.append(val)
            elif BASE <= (val & ~1) < BASE + len(data):
                ann += " (flash)"
        print(f"    0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{ann}")
    return sram_refs

# Disassemble key functions
disasm(0x0821ED2A, 300, "TBH case 0 (0xF188) @ 0x0821ED2A")
disasm(0x0821FB94, 300, "TBH case 1 (0xF189) @ 0x0821FB94")
disasm(0x08203F0C, 150, "FUN_08203F0C @ 0x08203F0C")

# Known pairs
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

print("\n" + "="*60)
print("=== AES key search in flash ===")
print("="*60)

# Try AES with keys from flash
try:
    from Crypto.Cipher import AES
    def aes_enc8(key16, ch8):
        plain = ch8.ljust(16, b'\x00')
        return AES.new(key16, AES.MODE_ECB).encrypt(plain)[:8]
    def aes_enc16(key16, ch16):
        return AES.new(key16, AES.MODE_ECB).encrypt(ch16)[:8]
    def aes_dec8(key16, ch8):
        plain = ch8.ljust(16, b'\x00')
        return AES.new(key16, AES.MODE_ECB).decrypt(plain)[:8]
    HAS_AES = True
except ImportError:
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        from cryptography.hazmat.backends import default_backend
        def aes_enc8(key16, ch8):
            plain = ch8.ljust(16, b'\x00')
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(plain) + e.finalize())[:8]
        def aes_enc16(key16, ch16):
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(ch16) + e.finalize())[:8]
        def aes_dec8(key16, ch8):
            plain = ch8.ljust(16, b'\x00')
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            d = c.decryptor()
            return (d.update(plain) + d.finalize())[:8]
        HAS_AES = True
    except ImportError:
        HAS_AES = False
        print("  No AES library; skipping AES search")

if HAS_AES:
    # Quick 8-byte pair reference for fast check
    ch0_8 = PAIRS8[0][0]; r0_8 = PAIRS8[0][1]
    ch0_16 = PAIRS16[0][0]; r0_16 = PAIRS16[0][1]
    
    CAL_START = 0x040000
    CAL_END = min(0x090000, len(data))
    
    found_keys = []
    print(f"  Scanning flash 0x{BASE+CAL_START:08X}-0x{BASE+CAL_END:08X} for AES keys...")
    for off in range(CAL_START, CAL_END - 16, 4):
        key = data[off:off+16]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key): continue
        try:
            # Test quick pair first (fast)
            if aes_enc8(key, ch0_8) == r0_8:
                # Full check
                ok8 = sum(1 for c,r in PAIRS8 if aes_enc8(key,c)==r)
                if ok8 == 5:
                    print(f"  *** AES ENC key (8-byte pairs) at 0x{BASE+off:08X}: {key.hex()} ***")
                    found_keys.append((off, key, 'enc8'))
            if aes_dec8(key, ch0_8) == r0_8:
                ok8 = sum(1 for c,r in PAIRS8 if aes_dec8(key,c)==r)
                if ok8 == 5:
                    print(f"  *** AES DEC key (8-byte pairs) at 0x{BASE+off:08X}: {key.hex()} ***")
                    found_keys.append((off, key, 'dec8'))
            if aes_enc16(key, ch0_16) == r0_16:
                ok16 = sum(1 for c,r in PAIRS16 if aes_enc16(key,c)==r)
                if ok16 == 5:
                    print(f"  *** AES ENC key (16-byte pairs) at 0x{BASE+off:08X}: {key.hex()} ***")
                    found_keys.append((off, key, 'enc16'))
        except Exception:
            pass
    
    if not found_keys:
        print("  No AES key found in calibration range")
    
    # Also try swapped/reversed key
    print("\n  Trying reversed and XOR'd key variations...")
    # Try a wider scan but sampling
    for off in range(CAL_START, CAL_END - 16, 16):
        key = data[off:off+16]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key): continue
        for variant, kv in [('rev', bytes(reversed(key))), ('xorFF', bytes(b^0xFF for b in key))]:
            try:
                if aes_enc8(kv, ch0_8) == r0_8:
                    ok8 = sum(1 for c,r in PAIRS8 if aes_enc8(kv,c)==r)
                    if ok8 == 5:
                        print(f"  *** AES {variant} key at 0x{BASE+off:08X}: {kv.hex()} ***")
            except Exception:
                pass

# XOR key brute-force with 8-byte keys from flash
print("\n" + "="*60)
print("=== 8-byte fixed XOR key search ===")
print("="*60)
ch0_8 = PAIRS8[0][0]; r0_8 = PAIRS8[0][1]
# Derive candidate key from pair 0: K = ch XOR resp
K0 = bytes(a^b for a,b in zip(ch0_8, r0_8))
print(f"  Derived key from pair 0 (XOR): {K0.hex()}")
# Check if consistent
ok = sum(1 for c,r in PAIRS8 if bytes(a^b for a,b in zip(c,K0))==r)
print(f"  XOR consistency check: {ok}/5 pairs match")
if ok == 5:
    print("  *** SIMPLE XOR KEY FOUND! ***")

# Try: response = f(challenge, key) where f is some simple linear function
# Look for key K such that some_transform(challenge, K) = response
print("\n  Trying: resp = (challenge + key) mod 256 bytewise")
K_add = bytes((b - a) & 0xFF for a, b in zip(ch0_8, r0_8))
ok_add = sum(1 for c,r in PAIRS8 if bytes((a+b)&0xFF for a,b in zip(c,K_add))==r)
print(f"  ADD key consistency: {ok_add}/5")

print("\n  Trying: resp = (key - challenge) mod 256 bytewise")
K_sub = bytes((b + a) & 0xFF for a, b in zip(ch0_8, r0_8))
ok_sub = sum(1 for c,r in PAIRS8 if bytes((b-a)&0xFF for a,b in zip(c,K_sub))==r)
print(f"  SUB key consistency: {ok_sub}/5")

# Test XTEA on quick pairs (32-bit block, 2x32 per 8 bytes)
print("\n" + "="*60)
print("=== XTEA/TEA key search ===")
print("="*60)

def xtea_encrypt(v, key):
    """XTEA encrypt: v = (v0, v1), key = (k0,k1,k2,k3) as 32-bit ints"""
    v0, v1 = v
    delta = 0x9E3779B9
    s = 0
    for _ in range(32):
        v0 = (v0 + (((v1 << 4 ^ v1 >> 5) + v1) ^ (s + key[s & 3]))) & 0xFFFFFFFF
        s = (s + delta) & 0xFFFFFFFF
        v1 = (v1 + (((v0 << 4 ^ v0 >> 5) + v0) ^ (s + key[(s >> 11) & 3]))) & 0xFFFFFFFF
    return (v0, v1)

def try_xtea_key(key_bytes, ch8, resp8):
    if len(key_bytes) < 16: return False
    key = struct.unpack('>IIII', key_bytes[:16])
    v = struct.unpack('>II', ch8)
    r_v = xtea_encrypt(v, key)
    got = struct.pack('>II', r_v[0], r_v[1])
    return got == resp8

# Quick check: does XTEA with any flash 128-bit key work?
XTEA_ch = PAIRS8[0][0]; XTEA_r = PAIRS8[0][1]
print(f"  Scanning for XTEA key (32 rounds)...")
for off in range(CAL_START, CAL_END - 16, 4):
    key = data[off:off+16]
    if all(b == 0 for b in key) or all(b == 0xFF for b in key): continue
    if try_xtea_key(key, XTEA_ch, XTEA_r):
        ok = sum(1 for c,r in PAIRS8 if try_xtea_key(key, c, r))
        if ok == 5:
            print(f"  *** XTEA key at 0x{BASE+off:08X}: {key.hex()} ***")

# Try LE byte order for XTEA  
for off in range(CAL_START, CAL_END - 16, 4):
    key = data[off:off+16]
    if all(b == 0 for b in key) or all(b == 0xFF for b in key): continue
    try:
        key_le = struct.unpack('<IIII', key[:16])
        v_le = struct.unpack('<II', XTEA_ch)
        r_v_le = xtea_encrypt(v_le, key_le)
        got_le = struct.pack('<II', r_v_le[0], r_v_le[1])
        if got_le == XTEA_r:
            ok = 0
            for c,r in PAIRS8:
                vv = struct.unpack('<II', c)
                rv = xtea_encrypt(vv, key_le)
                if struct.pack('<II', rv[0], rv[1]) == r:
                    ok += 1
            if ok == 5:
                print(f"  *** XTEA-LE key at 0x{BASE+off:08X}: {key.hex()} ***")
    except Exception:
        pass

print("\n  XTEA scan complete")

# Statistical analysis: byte-by-byte response dependency
print("\n" + "="*60)
print("=== Byte dependency analysis ===")
print("="*60)

# For each output byte b_out[i], which input bytes b_in[j] have the strongest correlation?
# Use variance of (b_out[i] XOR b_in[j]) across pairs
for i in range(8):
    min_var = 999
    best_j = -1
    best_op = ""
    for j in range(8):
        for op, fn in [
            ("xor", lambda a,b: a^b),
            ("add", lambda a,b: (a+b)&0xFF),
            ("sub", lambda a,b: (a-b)&0xFF),
        ]:
            vals = [fn(PAIRS8[k][0][j], PAIRS8[k][1][i]) for k in range(5)]
            if len(set(vals)) == 1:  # All same → direct relationship!
                print(f"  CONSTANT: resp[{i}] = challenge[{j}] {op} {vals[0]:02x}  (all 5 pairs)")
            var = len(set(vals))
            if var < min_var:
                min_var = var; best_j = j; best_op = f"{op}[{j}]={vals}"

# For 16-byte pairs: check byte dependencies
print("\n  16-byte challenge pairs byte dependency:")
for i in range(8):
    for j in range(16):
        for op, fn in [
            ("xor", lambda a,b: a^b),
            ("add", lambda a,b: (a+b)&0xFF),
        ]:
            vals = [fn(PAIRS16[k][0][j], PAIRS16[k][1][i]) for k in range(5)]
            if len(set(vals)) == 1:
                print(f"  CONSTANT: resp16[{i}] = challenge16[{j}] {op} {vals[0]:02x}")

print("\n=== Done ===")
