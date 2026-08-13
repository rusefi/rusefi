#!/usr/bin/env python3
"""
find_key_all_flash.py  - поиск ключа во ВСЁМ flash для quick-пар

Пары (из orig_1-3.trc, ignon.trc, ignon_and_start.trc):
Quick (8→8):
  0eabfe9d351af837 -> b15156d14683cd15
  4a90250e855128bf -> 14a267bec8d8c3dc
  63b95b1a3fbf41fe -> 2fa71d4722835ea2
  596e3885f2460f4a -> 2cdf61de56551b86
  a00059b79f90a8ae -> a225de4e470324b6

Full (16→8) — для проверки:
  66..40 | b4..ec -> 9cb7f8ca31431bb6
  cf..5c | e9..25 -> bff99205ed4ab7a8
  cd..eb | b4..2c -> 96739be6b1299f77
  4a..fd | 27..9e -> fda32d94ae77c121
  66..e0 | cd..07 -> efaa66f0caa6f0cd
"""

import struct, sys, itertools
from pathlib import Path

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
fw = FIRMWARE.read_bytes()
BASE = 0x08000000

PAIRS8 = [
    (bytes.fromhex('0eabfe9d351af837'), bytes.fromhex('b15156d14683cd15')),
    (bytes.fromhex('4a90250e855128bf'), bytes.fromhex('14a267bec8d8c3dc')),
    (bytes.fromhex('63b95b1a3fbf41fe'), bytes.fromhex('2fa71d4722835ea2')),
    (bytes.fromhex('596e3885f2460f4a'), bytes.fromhex('2cdf61de56551b86')),
    (bytes.fromhex('a00059b79f90a8ae'), bytes.fromhex('a225de4e470324b6')),
]

PAIRS16 = [
    (bytes.fromhex('660be1e2a34b8140b45633a0499a01ec'), bytes.fromhex('9cb7f8ca31431bb6')),
    (bytes.fromhex('cfcfbbf3cdc0f75ce9efe2eb23b62a25'), bytes.fromhex('bff99205ed4ab7a8')),
    (bytes.fromhex('cd4c9070196bbdebb425cb7c4350082c'), bytes.fromhex('96739be6b1299f77')),
    (bytes.fromhex('4a4f2a204fad58fd273622b70c2b559e'), bytes.fromhex('fda32d94ae77c121')),
    (bytes.fromhex('66aaeef37037dee0cdeefb22bde96807'), bytes.fromhex('efaa66f0caa6f0cd')),
]

def check_all8(fn):
    return sum(1 for c,r in PAIRS8 if fn(c) == r)

def check_all16(fn):
    return sum(1 for c,r in PAIRS16 if fn(c) == r)

# ---------------------------------------------------------------------------
# XTEA
# ---------------------------------------------------------------------------
def xtea_enc_le(challenge8, key16):
    v0, v1 = struct.unpack('<II', challenge8)
    k = struct.unpack('<IIII', key16)
    delta = 0x9E3779B9; s = 0
    for _ in range(32):
        v0 = (v0 + (((v1 << 4 ^ v1 >> 5) + v1) ^ (s + k[s & 3]))) & 0xFFFFFFFF
        s  = (s + delta) & 0xFFFFFFFF
        v1 = (v1 + (((v0 << 4 ^ v0 >> 5) + v0) ^ (s + k[(s >> 11) & 3]))) & 0xFFFFFFFF
    return struct.pack('<II', v0, v1)

def xtea_enc_be(challenge8, key16):
    v0, v1 = struct.unpack('>II', challenge8)
    k = struct.unpack('>IIII', key16)
    delta = 0x9E3779B9; s = 0
    for _ in range(32):
        v0 = (v0 + (((v1 << 4 ^ v1 >> 5) + v1) ^ (s + k[s & 3]))) & 0xFFFFFFFF
        s  = (s + delta) & 0xFFFFFFFF
        v1 = (v1 + (((v0 << 4 ^ v0 >> 5) + v0) ^ (s + k[(s >> 11) & 3]))) & 0xFFFFFFFF
    return struct.pack('>II', v0, v1)

ch0, r0 = PAIRS8[0]

print("=" * 65)
print(f"XTEA (full flash scan, step=4 байта)...")
print(f"Тест: {ch0.hex()} -> {r0.hex()}")

found_xtea = False
for off in range(0, len(fw) - 16, 4):
    key16 = fw[off:off+16]
    if all(b == 0 for b in key16) or all(b == 0xFF for b in key16):
        continue
    for enc_fn, name in [(xtea_enc_le, 'LE'), (xtea_enc_be, 'BE')]:
        try:
            got = enc_fn(ch0, key16)
            if got == r0:
                ok = check_all8(lambda c, k=key16, f=enc_fn: f(c, k))
                if ok >= 2:
                    print(f"*** XTEA-{name} ключ! file=0x{off:06X} chip=0x{BASE+off:08X}: {key16.hex()} [{ok}/5 пар] ***")
                    if ok == 5:
                        found_xtea = True
        except Exception:
            pass

if not found_xtea:
    print(f"  XTEA не найден (просмотрено {len(fw)//4} позиций)")

# ---------------------------------------------------------------------------
# AES-128
# ---------------------------------------------------------------------------
try:
    from Crypto.Cipher import AES
    HAS_AES = True
except ImportError:
    try:
        from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
        from cryptography.hazmat.backends import default_backend
        HAS_AES = 'cryptography'
    except ImportError:
        HAS_AES = False

if HAS_AES:
    print(f"\n{'='*65}")
    print(f"AES-128 (full flash scan)...")

    def aes_enc(key16, plain8):
        padded = plain8.ljust(16, b'\x00')
        if HAS_AES == True:
            return AES.new(key16, AES.MODE_ECB).encrypt(padded)[:8]
        else:
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(padded) + e.finalize())[:8]

    def aes_dec(key16, cipher8):
        padded = cipher8.ljust(16, b'\x00')
        if HAS_AES == True:
            return AES.new(key16, AES.MODE_ECB).decrypt(padded)[:8]
        else:
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            d = c.decryptor()
            return (d.update(padded) + d.finalize())[:8]

    found_aes = False
    for off in range(0, len(fw) - 16, 4):
        key16 = fw[off:off+16]
        if all(b == 0 for b in key16) or all(b == 0xFF for b in key16):
            continue
        try:
            # Быстрая проверка: encrypt(ch0) == r0?
            got_enc = aes_enc(key16, ch0)
            if got_enc == r0:
                ok = check_all8(lambda c, k=key16: aes_enc(k, c))
                print(f"*** AES-ENC ключ! file=0x{off:06X}: {key16.hex()} [{ok}/5] ***")
                if ok == 5: found_aes = True
            # decrypt(ch0) == r0?
            got_dec = aes_dec(key16, ch0)
            if got_dec == r0:
                ok = check_all8(lambda c, k=key16: aes_dec(k, c))
                print(f"*** AES-DEC ключ! file=0x{off:06X}: {key16.hex()} [{ok}/5] ***")
                if ok == 5: found_aes = True
            # AES(key, r0) == ch0?
            inv = aes_enc(key16, r0)
            if inv == ch0:
                ok = check_all8(lambda c, k=key16: aes_enc(k, PAIRS8[[c2 for c2,r2 in PAIRS8].index(c)][1]))
                print(f"*** AES-INV ключ! file=0x{off:06X}: {key16.hex()} [{ok}/5] ***")
        except Exception:
            pass

    if not found_aes:
        print(f"  AES не найден")

    # AES для full 16-byte pairs
    print(f"\nAES для полных 16-байтных пар (ch16 -> r8)...")
    ch0_16, r0_8 = PAIRS16[0]
    def aes16(key16, ch16):
        if HAS_AES == True:
            return AES.new(key16, AES.MODE_ECB).encrypt(ch16)[:8]
        else:
            c = Cipher(algorithms.AES(key16), modes.ECB(), backend=default_backend())
            e = c.encryptor()
            return (e.update(ch16) + e.finalize())[:8]

    found_aes16 = False
    for off in range(0, len(fw) - 16, 4):
        key16 = fw[off:off+16]
        if all(b == 0 for b in key16) or all(b == 0xFF for b in key16):
            continue
        try:
            got = aes16(key16, ch0_16)
            if got == r0_8:
                ok = check_all16(lambda c, k=key16: aes16(k, c))
                print(f"*** AES-16 ключ! file=0x{off:06X}: {key16.hex()} [{ok}/5] ***")
                if ok == 5: found_aes16 = True
        except Exception:
            pass

    if not found_aes16:
        print(f"  AES-16 не найден")

else:
    print("AES-библиотека не доступна")

# ---------------------------------------------------------------------------
# DES
# ---------------------------------------------------------------------------
try:
    from Crypto.Cipher import DES
    print(f"\n{'='*65}")
    print(f"DES-8 (full flash scan, 8-байтный ключ)...")

    found_des = False
    for off in range(0, len(fw) - 8, 4):
        key8 = fw[off:off+8]
        if all(b == 0 for b in key8) or all(b == 0xFF for b in key8):
            continue
        try:
            cipher = DES.new(key8, DES.MODE_ECB)
            got = cipher.encrypt(ch0)
            if got == r0:
                ok = check_all8(lambda c, k=key8: DES.new(k, DES.MODE_ECB).encrypt(c))
                print(f"*** DES ключ! file=0x{off:06X}: {key8.hex()} [{ok}/5] ***")
                if ok == 5: found_des = True
        except Exception:
            pass

    if not found_des:
        print(f"  DES не найден")
except ImportError:
    print("\nDES: pycryptodome не установлен")

# ---------------------------------------------------------------------------
# Blowfish
# ---------------------------------------------------------------------------
try:
    from Crypto.Cipher import Blowfish
    print(f"\n{'='*65}")
    print(f"Blowfish (8-байтные ключи из flash)...")

    found_bf = False
    for off in range(0, len(fw) - 8, 4):
        key = fw[off:off+8]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key):
            continue
        try:
            cipher = Blowfish.new(key, Blowfish.MODE_ECB)
            got = cipher.encrypt(ch0)
            if got == r0:
                ok = check_all8(lambda c, k=key: Blowfish.new(k, Blowfish.MODE_ECB).encrypt(c))
                print(f"*** BLOWFISH ключ! file=0x{off:06X}: {key.hex()} [{ok}/5] ***")
                if ok == 5: found_bf = True
        except Exception:
            pass

    if not found_bf:
        print(f"  Blowfish не найден")
except ImportError:
    print("\nBlowfish: pycryptodome не установлен")

# ---------------------------------------------------------------------------
# Проверка hash-based алгоритмов
# ---------------------------------------------------------------------------
import hashlib, zlib
print(f"\n{'='*65}")
print(f"Hash/HMAC (HMAC-SHA1, HMAC-MD5, HMAC-SHA256)...")
print(f"Ищем ключ для HMAC(key, challenge)[:8] == response")

for klen in [8, 16, 20, 32]:
    print(f"  Ключ длиной {klen} байт...")
    found_hmac = False
    for off in range(0, len(fw) - klen, 4):
        key = fw[off:off+klen]
        if all(b == 0 for b in key) or all(b == 0xFF for b in key):
            continue
        for algo, name in [(hashlib.sha1, 'SHA1'), (hashlib.md5, 'MD5'),
                            (hashlib.sha256, 'SHA256')]:
            try:
                import hmac
                got = hmac.new(key, ch0, algo).digest()[:8]
                if got == r0:
                    ok = sum(1 for c,r in PAIRS8
                             if hmac.new(key, c, algo).digest()[:8] == r)
                    if ok >= 2:
                        print(f"*** HMAC-{name}[{klen}] ключ! file=0x{off:06X}: {key.hex()} [{ok}/5] ***")
                        if ok == 5: found_hmac = True
            except Exception:
                pass

print(f"\n{'='*65}")
print("=== Итог: если не нашли — нужны ещё данные (ответы от BCM) ===")
print("Следующий шаг: Unicorn эмуляция с правильным SRAM layout")
