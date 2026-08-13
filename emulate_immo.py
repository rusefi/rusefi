#!/usr/bin/env python3
"""
emulate_immo.py  – ARM Cortex-M emulation of the I865LB52 IMMO crypto

Strategy:
  1. Disassemble the known IMMO functions to understand the algorithm.
  2. Find the initialization code that populates the SRAM function-pointer table
     at 0x20001018 (stride 0x54, fn-ptr at +0x34).
  3. Emulate the whole IMMO path using unicorn and capture the result.
  4. Validate against known challenge/response pairs.

Usage:
    python3 emulate_immo.py [path_to_bin]
"""

import struct, sys
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
    CAPSTONE = True
except ImportError:
    CAPSTONE = False
    print("Warning: capstone not available; skipping disassembly sections")

try:
    import unicorn
    from unicorn import *
    from unicorn.arm_const import *
    UNICORN = True
except ImportError:
    UNICORN = False
    print("Warning: unicorn not available; skipping emulation sections")

# ---------------------------------------------------------------------------
# Load binary
# ---------------------------------------------------------------------------
FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if len(sys.argv) > 1:
    FIRMWARE = Path(sys.argv[1])

data = FIRMWARE.read_bytes()
BASE = 0x08000000
print(f"Loaded {len(data):,} bytes  base=0x{BASE:08X}\n")

def faddr(chip_addr):
    """Convert chip address to file offset."""
    return chip_addr - BASE

def chip(file_off):
    """Convert file offset to chip address."""
    return file_off + BASE

def u32_at(off):
    return struct.unpack_from('<I', data, off)[0]

def u16_at(off):
    return struct.unpack_from('<H', data, off)[0]

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
# Section A: Disassemble IMMO-related functions
# ---------------------------------------------------------------------------
if CAPSTONE:
    print("=" * 70)
    print("=== A: Capstone disassembly of key functions ===")
    print("=" * 70)

    md = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
    md.detail = True

    def disasm_region(file_off, length, label):
        addr = chip(file_off)
        chunk = data[file_off:file_off + length]
        print(f"\n--- {label} (file 0x{file_off:06X}, chip 0x{addr:08X}, {length} bytes) ---")
        for insn in md.disasm(chunk, addr):
            print(f"  0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}")

    # Dispatcher at 0x08203FFC (file 0x203FFC), ~200 bytes
    disasm_region(0x203FFC, 200, "IMMO dispatcher 0x08203FFC")

    # Computation function at 0x082027A4 (file 0x2027A4), ~300 bytes
    disasm_region(0x2027A4, 300, "IMMO computation 0x082027A4")

    # Candidate crypto handlers 0x08203680-0x082037C0
    disasm_region(0x203680, 0x160, "Candidate crypto handlers 0x08203680")

    # Registration function 0x0820477E (file 0x20477E), ~100 bytes
    disasm_region(0x20477E, 120, "Registration fn 0x0820477E")

    # Challenge reader 0x08201208 (file 0x201208), ~200 bytes
    disasm_region(0x201208, 200, "Challenge reader 0x08201208")

    # State-machine area 0x082016BC (file 0x2016BC), ~200 bytes
    disasm_region(0x2016BC, 200, "State machine 0x082016BC")

# ---------------------------------------------------------------------------
# Section B: Find flash literals pointing to 0x20001018 (table init code)
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== B: Flash literals = 0x20001018 (SRAM table init references) ===")
print("=" * 70)
TARGET_SRAM = 0x20001018
pat = struct.pack('<I', TARGET_SRAM)
hits_1018 = [i for i in range(len(data)-3) if data[i:i+4] == pat]
print(f"  Found {len(hits_1018)} occurrences")
for h in hits_1018:
    # Show 4 bytes before and 4 bytes after
    ctx = data[max(0,h-8):h+12]
    ctx_hex = ' '.join(f'{b:02x}' for b in ctx)
    print(f"  file 0x{h:06X}  chip 0x{chip(h):08X}  ctx: {ctx_hex}")

# ---------------------------------------------------------------------------
# Section C: Find flash code that WRITES TO the SRAM table region
# We're looking for: STR Rn, [Rm, #0x34] where Rm ends up = 0x20001018+i*0x54
# More practically: search for 0x20001018 literal AND nearby 0x0820xxxx addresses
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== C: Search for fn-ptr assignments near 0x20001018 references ===")
print("=" * 70)

# For each hit of 0x20001018, look in a ±256 byte window for 0x0820xxxx literals
FLASH_RANGE = (0x08200000, 0x08300000)
for h in hits_1018:
    window_start = max(0, h - 0x100)
    window_end   = min(len(data)-3, h + 0x100)
    fn_addrs = []
    for i in range(window_start, window_end, 2):
        v = u32_at(i) if i + 4 <= len(data) else 0
        if FLASH_RANGE[0] <= v <= FLASH_RANGE[1]:
            fn_addrs.append((i, v))
    if fn_addrs:
        print(f"\n  Near 0x{chip(h):08X}:")
        for fo, addr in fn_addrs:
            print(f"    file 0x{fo:06X}  -> fn ptr 0x{addr:08X}")

# ---------------------------------------------------------------------------
# Section D: Decode the SRAM table init – look at all code referencing 0x20001018
# For code in the flash application range, disassemble the containing function
# ---------------------------------------------------------------------------
if CAPSTONE:
    print("\n" + "=" * 70)
    print("=== D: Disassemble code that references 0x20001018 ===")
    print("=" * 70)
    md2 = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
    md2.detail = True

    APP_RANGE_LO = 0x08200000
    APP_RANGE_HI = 0x08280000

    shown = set()
    for h in hits_1018:
        chip_addr = chip(h)
        if not (APP_RANGE_LO <= chip_addr <= APP_RANGE_HI):
            continue
        # Disassemble 128 bytes before and 64 bytes after this literal
        region_start = max(0, h - 0x100)
        region_len   = 0x180
        if region_start in shown:
            continue
        shown.add(region_start)
        disasm_region(region_start, region_len, f"fn referencing 0x20001018 @ file 0x{h:06X}")

# ---------------------------------------------------------------------------
# Section E: Full unicorn emulation
# ---------------------------------------------------------------------------
if UNICORN:
    print("\n" + "=" * 70)
    print("=== E: Unicorn emulation of IMMO crypto ===")
    print("=" * 70)

    FLASH_BASE  = 0x08000000
    FLASH_SIZE  = 0x00400000   # 4 MB region (covers ROM alias too)

    SRAM_BASE   = 0x20000000
    SRAM_SIZE   = 0x00020000   # 128 KB

    STACK_TOP   = SRAM_BASE + SRAM_SIZE - 4

    # Addresses from analysis
    CHALLENGE_BUF = 0x20000414  # where challenge bytes are placed
    RESPONSE_BUF  = 0x20000C18  # where response bytes appear

    # The IMMO dispatcher entry for full challenge
    IMMO_DISPATCH = 0x08203FFC  # chip address, Thumb (+1 for BLX)

    # "Magic" return address – we map a tiny trampoline here so we can detect return
    RETURN_SENTINEL = 0x10000000
    SENTINEL_SIZE   = 0x1000

    def emu_run(challenge16, label):
        """Run unicorn with given 16-byte challenge, return response bytes or None."""
        try:
            mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)

            # Map flash (covers both 0x08000000 and 0x08800000 via alias)
            mu.mem_map(FLASH_BASE, FLASH_SIZE)
            # Fill with NOP equivalent (BKPT 0 = 0xBE 0x00 in Thumb)
            mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
            # Write actual firmware
            fw_write = min(len(data), FLASH_SIZE)
            mu.mem_write(FLASH_BASE, data[:fw_write])

            # Map SRAM
            mu.mem_map(SRAM_BASE, SRAM_SIZE)
            mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)

            # Map sentinel page for return detection
            mu.mem_map(RETURN_SENTINEL, SENTINEL_SIZE)
            mu.mem_write(RETURN_SENTINEL, b'\x00\xBE' * (SENTINEL_SIZE // 2))  # BKPTs

            # Stack
            mu.reg_write(UC_ARM_REG_SP, STACK_TOP & ~7)

            # Write challenge into expected buffer
            mu.mem_write(CHALLENGE_BUF, challenge16)

            # Set up IMMO state that the dispatcher checks
            # The dispatcher at 0x08203FFC reads a state flag;
            # we need to figure out what state triggers computation.
            # For now, set LR to sentinel so we catch the return.
            mu.reg_write(UC_ARM_REG_LR, RETURN_SENTINEL | 1)  # Thumb

            # Hooks
            hit_sentinel = [False]
            def hook_code(mu, address, size, user_data):
                if address == RETURN_SENTINEL:
                    hit_sentinel[0] = True
                    mu.emu_stop()

            def hook_invalid(mu, access, address, size, value, user_data):
                # Silently stop on unmapped access
                mu.emu_stop()
                return False

            mu.hook_add(UC_HOOK_CODE, hook_code)
            mu.hook_add(UC_HOOK_MEM_INVALID, hook_invalid)

            # Start emulation from dispatcher in Thumb mode
            mu.emu_start(IMMO_DISPATCH | 1, RETURN_SENTINEL, timeout=5_000_000, count=500_000)

            if hit_sentinel[0]:
                # Read response buffer
                resp = bytes(mu.mem_read(RESPONSE_BUF, 8))
                return resp
            else:
                return None

        except UcError as e:
            return None

    print("\n  Testing full emulation with first challenge pair...")
    ch0, r0 = PAIRS16[0]
    got = emu_run(ch0, "pair1")
    if got:
        print(f"  challenge: {ch0.hex()}")
        print(f"  expected:  {r0.hex()}")
        print(f"  got:       {got.hex()}")
        if got == r0:
            print("  *** MATCH! ***")
        else:
            print("  No match (dispatcher needs context setup)")
    else:
        print("  Emulation did not reach sentinel (needs more setup)")

# ---------------------------------------------------------------------------
# Section F: Pattern analysis on raw pairs
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== F: Deep pattern analysis on challenge/response pairs ===")
print("=" * 70)

def show_xor_patterns(pairs, label):
    print(f"\n  {label}:")
    for i, (ch, resp) in enumerate(pairs):
        # XOR each byte of response with corresponding challenge byte
        n = min(len(ch), len(resp))
        xored = bytes(ch[j] ^ resp[j] for j in range(n))
        print(f"  pair {i+1}: ch={ch.hex()}  resp={resp.hex()}")
        print(f"         xor={xored.hex()}")

show_xor_patterns(PAIRS8,  "8-byte pairs (XOR challenge ^ response)")
show_xor_patterns(PAIRS16, "16-byte pairs (XOR first8 ^ response)")

# Check if quick-pairs could be "second half of 16-byte" pairs
print("\n  Checking if quick challenge = second half of full challenge:")
for qi, (qch, qresp) in enumerate(PAIRS8):
    for fi, (fch, fresp) in enumerate(PAIRS16):
        if fch[8:] == qch:
            print(f"  MATCH: quick pair {qi+1} second-half == full pair {fi+1} ch2")
        if fch[:8] == qch:
            print(f"  MATCH: quick pair {qi+1} == first-half of full pair {fi+1}")

# ---------------------------------------------------------------------------
# Section G: AES-128 candidates
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== G: AES-128 search ===")
print("=" * 70)
try:
    from Crypto.Cipher import AES
    PYCRYPTO = True
except ImportError:
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        from cryptography.hazmat.backends import default_backend
        PYCRYPTO = False
        CRYPTOGRAPHY = True
    except ImportError:
        PYCRYPTO = False
        CRYPTOGRAPHY = False
        print("  Neither pycryptodome nor cryptography installed; skipping AES tests")

KEY_CONSTANTS = [0x2548A4D2, 0x4DF9123B, 0x43A0C212, 0xF9C74A52]
key_bytes_le = b''.join(struct.pack('<I', k) for k in KEY_CONSTANTS)  # 16 bytes LE
key_bytes_be = b''.join(struct.pack('>I', k) for k in KEY_CONSTANTS)  # 16 bytes BE
print(f"  Key (4 constants LE): {key_bytes_le.hex()}")
print(f"  Key (4 constants BE): {key_bytes_be.hex()}")

def test_aes(key16, pairs16, pairs8, label):
    """Test AES-ECB: encrypt(challenge) == response (first 8 bytes)."""
    ok16 = ok8 = 0
    if PYCRYPTO:
        def enc(plaintext):
            # pad to 16 if needed
            p = plaintext.ljust(16, b'\x00')
            return AES.new(key16, AES.MODE_ECB).encrypt(p)[:8]
    elif CRYPTOGRAPHY:
        def enc(plaintext):
            p = plaintext.ljust(16, b'\x00')
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(p) + e.finalize())[:8]
    else:
        return

    for ch, resp in pairs16:
        if enc(ch) == resp: ok16 += 1
    for ch, resp in pairs8:
        if enc(ch) == resp: ok8 += 1
    print(f"  {label}: AES-ECB enc: {ok16}/{len(pairs16)} 16-byte, {ok8}/{len(pairs8)} 8-byte")

    # Also test decrypt
    ok16d = ok8d = 0
    if PYCRYPTO:
        def dec(plaintext):
            p = plaintext.ljust(16, b'\x00')
            return AES.new(key16, AES.MODE_ECB).decrypt(p)[:8]
    elif CRYPTOGRAPHY:
        def dec(plaintext):
            p = plaintext.ljust(16, b'\x00')
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            d = c.decryptor()
            return (d.update(p) + d.finalize())[:8]
    for ch, resp in pairs16:
        if dec(ch) == resp: ok16d += 1
    for ch, resp in pairs8:
        if dec(ch) == resp: ok8d += 1
    print(f"  {label}: AES-ECB dec: {ok16d}/{len(pairs16)} 16-byte, {ok8d}/{len(pairs8)} 8-byte")

if PYCRYPTO or CRYPTOGRAPHY:
    test_aes(key_bytes_le, PAIRS16, PAIRS8, "key-LE")
    test_aes(key_bytes_be, PAIRS16, PAIRS8, "key-BE")

    # Also search for any 16-byte sequence in flash that, as AES key, gives correct response
    print("\n  Brute-forcing 16-byte AES keys from flash at each 0x20001018 reference...")
    
    if PYCRYPTO:
        def try_aes_key(key16, challenge):
            return AES.new(key16, AES.MODE_ECB).encrypt(challenge.ljust(16, b'\x00'))[:8]
    else:
        def try_aes_key(key16, challenge):
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(challenge.ljust(16, b'\x00')) + e.finalize())[:8]
    
    # Test: for each possible 16-byte key in a region near the known code,
    # check if it encrypts all challenges correctly
    # Focus on calibration area 0x04xxxx where key constants were found
    CAL_START = 0x040000
    CAL_END   = min(0x050000, len(data))
    matches = []
    for off in range(CAL_START, CAL_END - 16, 4):
        key = data[off:off+16]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key):
            continue
        # Quick check against first pair only
        try:
            got = try_aes_key(key, PAIRS16[0][0])
            if got == PAIRS16[0][1]:
                # Full check
                if all(try_aes_key(key, ch) == r for ch, r in PAIRS16):
                    matches.append((off, key))
                    print(f"  *** AES KEY FOUND at file 0x{off:06X}: {key.hex()} ***")
        except Exception:
            pass
    
    if not matches:
        print("  No AES key found in calibration area (16-byte pairs)")
    
    # Same for 8-byte pairs (treat challenge as 8 bytes, pad to 16)
    matches8 = []
    for off in range(CAL_START, CAL_END - 16, 4):
        key = data[off:off+16]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key):
            continue
        try:
            got = try_aes_key(key, PAIRS8[0][0])
            if got == PAIRS8[0][1]:
                if all(try_aes_key(key, ch) == r for ch, r in PAIRS8):
                    matches8.append((off, key))
                    print(f"  *** AES KEY (8-byte) FOUND at file 0x{off:06X}: {key.hex()} ***")
        except Exception:
            pass
    
    if not matches8:
        print("  No AES key found for 8-byte pairs in calibration area")

# ---------------------------------------------------------------------------
# Section H: More advanced pattern analysis
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== H: Byte-by-byte response analysis ===")
print("=" * 70)

# For 8-byte pairs, examine each output byte as a function of input bytes
print("\n  8-byte pairs - output[i] XOR input[i] for each pair:")
for pi, (ch, resp) in enumerate(PAIRS8):
    xv = [ch[i]^resp[i] for i in range(8)]
    print(f"  pair {pi+1}: {' '.join(f'{v:02x}' for v in xv)}")

print("\n  8-byte pairs - output[i] XOR input[(i+4)%8] for each pair:")
for pi, (ch, resp) in enumerate(PAIRS8):
    xv = [ch[(i+4)%8]^resp[i] for i in range(8)]
    print(f"  pair {pi+1}: {' '.join(f'{v:02x}' for v in xv)}")

print("\n  8-byte pairs - output[i] XOR input[7-i] for each pair (reversed):")
for pi, (ch, resp) in enumerate(PAIRS8):
    xv = [ch[7-i]^resp[i] for i in range(8)]
    print(f"  pair {pi+1}: {' '.join(f'{v:02x}' for v in xv)}")

# Check if response bytes follow any shift/rotation pattern of challenge
print("\n  8-byte pairs - is resp byte-rotation of challenge?")
for rot in range(8):
    ok = 0
    for ch, resp in PAIRS8:
        rotated = ch[rot:] + ch[:rot]
        if rotated == resp:
            ok += 1
    if ok > 0:
        print(f"  rotation by {rot}: {ok}/{len(PAIRS8)} match")

print("\n  8-byte pairs - is resp = NOT(rotation) of challenge?")
for rot in range(8):
    ok = 0
    for ch, resp in PAIRS8:
        rotated = ch[rot:] + ch[:rot]
        notrot = bytes(~b & 0xFF for b in rotated)
        if notrot == resp:
            ok += 1
    if ok > 0:
        print(f"  NOT(rotation by {rot}): {ok}/{len(PAIRS8)} match")

# KEELOQ-like analysis: look for nonlinear structure
print("\n  Entropy analysis of XOR(challenge, response):")
for pi, (ch, resp) in enumerate(PAIRS8):
    diff = [ch[i]^resp[i] for i in range(8)]
    # Compute popcount of each XOR byte
    pc = [bin(d).count('1') for d in diff]
    print(f"  pair {pi+1}: xor={bytes(diff).hex()}  popcnt={pc}")

print("\n  16-byte pairs - checking if resp = f(first8) only:")
# Check if same function maps 8-byte chunks  
for pi, (ch, resp) in enumerate(PAIRS16):
    f8 = ch[:8]
    s8 = ch[8:]
    diff_f = [f8[i]^resp[i] for i in range(8)]
    diff_s = [s8[i]^resp[i] for i in range(8)]
    print(f"  pair {pi+1}: xor_vs_first8={bytes(diff_f).hex()}  xor_vs_second8={bytes(diff_s).hex()}")

# ---------------------------------------------------------------------------
# Section I: Search for AES S-box or known algorithm constants in flash
# ---------------------------------------------------------------------------
print("\n" + "=" * 70)
print("=== I: Search for crypto algorithm signatures in flash ===")
print("=" * 70)

# AES S-box first 16 bytes: 63 7c 77 7b f2 6b 6f c5 30 01 67 2b fe d7 ab 76
AES_SBOX_START = bytes([0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5,
                         0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76])
# SHA-256 initial H values: 6a 09 e6 67 ...
SHA256_H0 = bytes([0x6a, 0x09, 0xe6, 0x67])
# KEELOQ NLF constant: 0x3A5C742E
KEELOQ_NLF = struct.pack('<I', 0x3A5C742E)
# Hitag2 constant: 0x7907 -> not distinctive
# Crypto-1 LFSR taps: not a standalone constant
# DST-40 constants (not public)

for pattern, label in [
    (AES_SBOX_START, "AES S-box start"),
    (SHA256_H0, "SHA-256 H0"),
    (KEELOQ_NLF, "KEELOQ NLF"),
    (b'\x01\x23\x45\x67', "DES test pattern"),
]:
    hits = [i for i in range(len(data) - len(pattern) + 1)
            if data[i:i+len(pattern)] == pattern]
    if hits:
        print(f"  {label}: {len(hits)} hit(s) at file offsets {[hex(h) for h in hits[:5]]}")
    else:
        print(f"  {label}: not found")

# ---------------------------------------------------------------------------
# Section J: Find the actual crypto function by looking at all code around
# 0x082027A4 and tracing what r6 is loaded with
# ---------------------------------------------------------------------------
if CAPSTONE:
    print("\n" + "=" * 70)
    print("=== J: Trace r6 load chain near 0x082027A4 ===")
    print("=" * 70)
    md3 = Cs(CS_ARCH_ARM, CS_MODE_THUMB + CS_MODE_LITTLE_ENDIAN)
    md3.detail = True

    # Disassemble a large block around the computation function
    START = 0x202600
    LEN   = 0x400
    addr_base = chip(START)
    chunk = data[START:START+LEN]
    print(f"\n  Code 0x{addr_base:08X} – 0x{addr_base+LEN:08X}:")
    for insn in md3.disasm(chunk, addr_base):
        # Highlight interesting instructions
        highlight = ""
        if insn.mnemonic in ('blx', 'bx', 'ldr') and 'r6' in insn.op_str:
            highlight = "  <<<"
        if insn.mnemonic in ('ldr',) and 'pc' in insn.op_str:
            # Resolve PC-relative load
            try:
                # In Thumb: PC = (insn.address + 4) & ~3
                pc = (insn.address + 4) & ~3
                # Parse imm from op_str like "r3, [pc, #0x10]"
                import re
                m = re.search(r'#(0x[0-9a-f]+|-?\d+)', insn.op_str)
                if m:
                    imm = int(m.group(1), 16) if m.group(1).startswith('0x') else int(m.group(1))
                    target = pc + imm
                    if BASE <= target < BASE + len(data):
                        loaded_val = u32_at(target - BASE)
                        highlight = f"  -> 0x{loaded_val:08X}"
            except Exception:
                pass
        print(f"  0x{insn.address:08X}  {insn.mnemonic:<12} {insn.op_str}{highlight}")

print("\n=== Done ===")
