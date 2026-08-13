#!/usr/bin/env python3
"""
analyze_immo.py - IMMO challenge/response analysis for m74_9 / Itelma I865LB52

Searches the firmware binary for the IMMO algorithm and tests hypotheses against
known challenge/response pairs.

Usage:
    python3 analyze_immo.py [path_to_bin]
"""
import struct, sys, hashlib, zlib, itertools
from pathlib import Path

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if len(sys.argv) > 1:
    FIRMWARE = Path(sys.argv[1])

data = FIRMWARE.read_bytes()
BASE = 0x08000000
print(f"Loaded {len(data):,} bytes from {FIRMWARE.name}")
print(f"Flash base: 0x{BASE:08X}, range 0x{BASE:08X}–0x{BASE+len(data):08X}\n")

# ---------------------------------------------------------------------------
# Known challenge/response pairs
# ---------------------------------------------------------------------------
# Full 16-byte challenge → 8-byte response
PAIRS16 = [
    (bytes.fromhex('660be1e2a34b8140b45633a0499a01ec'), bytes.fromhex('9cb7f8ca31431bb6')),
    (bytes.fromhex('cfcfbbf3cdc0f75ce9efe2eb23b62a25'), bytes.fromhex('bff99205ed4ab7a8')),
    (bytes.fromhex('cd4c9070196bbdebb425cb7c4350082c'), bytes.fromhex('96739be6b1299f77')),
    (bytes.fromhex('4a4f2a204fad58fd273622b70c2b559e'), bytes.fromhex('fda32d94ae77c121')),
    (bytes.fromhex('66aaeef37037dee0cdeefb22bde96807'), bytes.fromhex('efaa66f0caa6f0cd')),
]
# Quick 8-byte challenge → 8-byte response
PAIRS8 = [
    (bytes.fromhex('0eabfe9d351af837'), bytes.fromhex('b15156d14683cd15')),
    (bytes.fromhex('4a90250e855128bf'), bytes.fromhex('14a267bec8d8c3dc')),
    (bytes.fromhex('63b95b1a3fbf41fe'), bytes.fromhex('2fa71d4722835ea2')),
    (bytes.fromhex('596e3885f2460f4a'), bytes.fromhex('2cdf61de56551b86')),
    (bytes.fromhex('a00059b79f90a8ae'), bytes.fromhex('a225de4e470324b6')),
]

def check(fn, pairs, label):
    """Test fn(challenge)->response against all pairs; return True if all match."""
    ok = 0
    for ch, resp in pairs:
        try:
            got = fn(ch)
            if got == resp:
                ok += 1
        except Exception:
            pass
    if ok == len(pairs):
        print(f"  *** MATCH ({label}): all {ok}/{len(pairs)} pairs correct! ***")
        return True
    elif ok > 0:
        print(f"  partial ({label}): {ok}/{len(pairs)} pairs match")
    return False

# ---------------------------------------------------------------------------
# Section 1 – Key constants from prior analysis
# ---------------------------------------------------------------------------
KEY_CONSTANTS = [0x2548A4D2, 0x4DF9123B, 0x43A0C212, 0xF9C74A52]
print("=== Section 1: Key constant search ===")
for kc in KEY_CONSTANTS:
    pattern = struct.pack('<I', kc)
    hits = [i for i in range(len(data)-3) if data[i:i+4] == pattern]
    print(f"  0x{kc:08X}: {len(hits)} hits at file offsets "
          f"{[hex(h) for h in hits[:6]]}")
print()

# ---------------------------------------------------------------------------
# Section 2 – Simple algorithm tests (no key)
# ---------------------------------------------------------------------------
print("=== Section 2: Keyless algorithm tests ===")

def xor_halves(ch):
    h = len(ch)//2
    return bytes(a^b for a,b in zip(ch[:h], ch[h:]))

def reverse(ch):        return bytes(reversed(ch))
def not_bytes(ch):      return bytes(~b & 0xFF for b in ch)
def xor_self_shift1(ch): return bytes(ch[i]^ch[(i+1)%len(ch)] for i in range(8))

for fn, label in [
    (lambda c: xor_halves(c),              "XOR(half1, half2)"),
    (lambda c: bytes(reversed(xor_halves(c))), "REV(XOR halves)"),
    (lambda c: not_bytes(c[:8]),           "NOT(first8)"),
    (lambda c: reverse(c[:8]),             "REVERSE(first8)"),
    (lambda c: xor_self_shift1(c[:8]),     "XOR shift-1 on first8"),
]:
    check(fn, PAIRS16, label + " [16→8]")
    check(fn, PAIRS8,  label + " [8→8]")

# ---------------------------------------------------------------------------
# Section 3 – XOR with fixed 8-byte key extracted from pairs
# ---------------------------------------------------------------------------
print("\n=== Section 3: Fixed-key XOR derivation ===")
# For 8-byte pairs: if R = C XOR K, then K = C XOR R
keys8 = [bytes(a^b for a,b in zip(c,r)) for c,r in PAIRS8]
print("  Derived keys from 8-byte pairs:")
for i, k in enumerate(keys8):
    print(f"    pair {i+1}: {k.hex()}")
# Check if any key is consistent across ALL pairs
unique_keys = set(keys8)
if len(unique_keys) == 1:
    k = keys8[0]
    print(f"  CONSTANT KEY: {k.hex()}")
    check(lambda c: bytes(a^b for a,b in zip(c, k)), PAIRS8, "XOR const key [8→8]")
else:
    # Try XOR of consecutive keys
    print("  No constant key. Checking XOR of key pairs:")
    for i in range(len(keys8)-1):
        xk = bytes(a^b for a,b in zip(keys8[i], keys8[i+1]))
        print(f"    key{i+1} XOR key{i+2} = {xk.hex()}")

# For 16-byte pairs: try R = f(C) where f is XOR with each half
print()
print("  Derived keys: response XOR first-half-of-challenge:")
for i, (c,r) in enumerate(PAIRS16):
    k = bytes(a^b for a,b in zip(c[:8], r))
    print(f"    pair {i+1}: {k.hex()}")

# ---------------------------------------------------------------------------
# Section 4 – Known crypto functions
# ---------------------------------------------------------------------------
print("\n=== Section 4: Known crypto functions ===")

for fn, label in [
    (lambda c: hashlib.md5(c).digest()[:8],    "MD5[:8]"),
    (lambda c: hashlib.sha1(c).digest()[:8],   "SHA1[:8]"),
    (lambda c: hashlib.sha256(c).digest()[:8], "SHA256[:8]"),
    (lambda c: struct.pack('<I', zlib.crc32(c) & 0xFFFFFFFF) * 2, "CRC32×2"),
]:
    check(fn, PAIRS16, label + " [16→8]")
    check(fn, PAIRS8,  label + " [8→8]")

# ---------------------------------------------------------------------------
# Section 5 – Known key constants XOR test
# ---------------------------------------------------------------------------
print("\n=== Section 5: Key constant XOR tests ===")
import itertools

key_bytes = b''
for kc in KEY_CONSTANTS:
    key_bytes += struct.pack('<I', kc)   # 16 bytes total

print(f"  Key constants bytes: {key_bytes.hex()}")

# Try: response = challenge_first8 XOR key_first8
for off in range(0, len(key_bytes)-7):
    kslice = key_bytes[off:off+8]
    fn = lambda c, k=kslice: bytes(a^b for a,b in zip(c[:8], k))
    if check(fn, PAIRS8, f"XOR key_const[{off}:{off+8}]"):
        print(f"    KEY SLICE: {kslice.hex()}")
    if check(fn, PAIRS16, f"XOR key_const[{off}:{off+8}] [16→8]"):
        print(f"    KEY SLICE: {kslice.hex()}")

# ---------------------------------------------------------------------------
# Section 6 – Search for IMMO function by surrounding code
# ---------------------------------------------------------------------------
print("\n=== Section 6: Code region search ===")

# The challenge reader function is at chip 0x082047D4 (file offset 0x2047D4)
# The IMMO response function should be nearby
CHALLENGE_READER_OFF = 0x2047D4
print(f"  Challenge reader at file offset 0x{CHALLENGE_READER_OFF:06X}")

# Dump 256 bytes before and 512 bytes after
for start, length, label in [
    (CHALLENGE_READER_OFF - 0x100, 0x100, "Before challenge reader"),
    (CHALLENGE_READER_OFF,          0x80,  "Challenge reader itself"),
    (CHALLENGE_READER_OFF + 0x80,   0x200, "After challenge reader"),
]:
    if start < 0 or start + length > len(data):
        continue
    chunk = data[start:start+length]
    print(f"\n  {label} (file 0x{start:06X}, chip 0x{BASE+start:08X}):")
    for i in range(0, len(chunk), 16):
        hex_part = ' '.join(f'{b:02X}' for b in chunk[i:i+16])
        print(f"    0x{BASE+start+i:08X}: {hex_part}")

# ---------------------------------------------------------------------------
# Section 7 – Registration function neighbours
# ---------------------------------------------------------------------------
print("\n=== Section 7: Registration function (0x0820477E) neighbors ===")
REG_OFF = 0x20477E
chunk = data[REG_OFF - 0x40 : REG_OFF + 0x100]
start = REG_OFF - 0x40
for i in range(0, len(chunk), 16):
    hex_part = ' '.join(f'{b:02X}' for b in chunk[i:i+16])
    print(f"  0x{BASE+start+i:08X}: {hex_part}")

# ---------------------------------------------------------------------------
# Section 8 – Search for "write to 0x20001A50" in code
# ---------------------------------------------------------------------------
print("\n=== Section 8: Search for stores to 0x20001A50 ===")
# In ARM Thumb-2, storing to a SRAM address involves LDR Rn, [PC, #literal]
# where literal = 0x20001A50 stored in flash as 50 1A 00 20
target_bytes = bytes([0x50, 0x1A, 0x00, 0x20])
hits = [i for i in range(len(data)-3) if data[i:i+4] == target_bytes]
print(f"  0x20001A50 literal (50 1A 00 20): {len(hits)} hits")
for h in hits:
    # Show context
    ctx_start = max(0, h - 4)
    ctx = data[ctx_start:h+8]
    ctx_hex = ' '.join(f'{b:02X}' for b in ctx)
    print(f"    file 0x{h:06X} chip 0x{BASE+h:08X}: ...{ctx_hex}...")

print("\n=== Done ===")
