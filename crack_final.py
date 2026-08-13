#!/usr/bin/env python3
"""
crack_final.py - Последние варианты алгоритма IMMO.

Новые гипотезы:
1. AES(Frame1_as_key, trigger || Frame2) = response (сессионный ключ = Frame1)
2. XTEA/TEA с trigger или Frame1 как ключ
3. CRC-подобные операции над flash содержимым
4. Алгоритм AT32 Secure Library (SLib)
"""

import struct, hashlib
from pathlib import Path

try:
    from Cryptodome.Cipher import AES, DES, ARC4
    from Cryptodome.Hash import SHA1, SHA256, MD5, HMAC, SHA224
    from Cryptodome.Hash import CMAC
except ImportError:
    from Crypto.Cipher import AES, DES, ARC4
    from Crypto.Hash import SHA1, SHA256, MD5, HMAC, SHA224

SESSIONS = [
    (bytes.fromhex("5283dba64769c00d"), bytes.fromhex("660be1e2a34b8140"),
     bytes.fromhex("b45633a0499a01ec"), bytes.fromhex("9cb7f8ca31431bb6")),
    (bytes.fromhex("a5ea8a93265530dd"), bytes.fromhex("cfcfbbf3cdc0f75c"),
     bytes.fromhex("e9efe2eb23b62a25"), bytes.fromhex("bff99205ed4ab7a8")),
    (bytes.fromhex("45d4a5c0f6a1ea72"), bytes.fromhex("cd4c9070196bbdeb"),
     bytes.fromhex("b425cb7c4350082c"), bytes.fromhex("96739be6b1299f77")),
    (bytes.fromhex("0841994c0b81a14c"), bytes.fromhex("4a4f2a204fad58fd"),
     bytes.fromhex("273622b70c2b559e"), bytes.fromhex("fda32d94ae77c121")),
    (bytes.fromhex("d081cc982708d806"), bytes.fromhex("66aaeef37037dee0"),
     bytes.fromhex("cdeefb22bde96807"), bytes.fromhex("efaa66f0caa6f0cd")),
]
QUICK = [
    (bytes.fromhex("0eabfe9d351af837"), bytes.fromhex("b15156d14683cd15"), 0),
    (bytes.fromhex("4a90250e855128bf"), bytes.fromhex("14a267bec8d8c3dc"), 1),
    (bytes.fromhex("63b95b1a3fbf41fe"), bytes.fromhex("2fa71d4722835ea2"), 2),
    (bytes.fromhex("596e3885f2460f4a"), bytes.fromhex("2cdf61de56551b86"), 3),
    (bytes.fromhex("a00059b79f90a8ae"), bytes.fromhex("a225de4e470324b6"), 4),
]

KEY_FLASH = bytes.fromhex("12c2a043dd7b3c50c9265901487a332e")
KEY_BE    = bytes.fromhex("43a0c212503c7bdd015926c92e337a48")

def b16(x): return (x + b'\x00'*16)[:16]
def b8(x):  return (x + b'\x00'*8)[:8]

def xtea_enc(v0, v1, key4):
    delta = 0x9E3779B9; s = 0
    for _ in range(32):
        v0 = (v0 + (((v1<<4^v1>>5)+v1) ^ (s+key4[s&3]))) & 0xFFFFFFFF
        s  = (s + delta) & 0xFFFFFFFF
        v1 = (v1 + (((v0<<4^v0>>5)+v0) ^ (s+key4[(s>>11)&3]))) & 0xFFFFFFFF
    return v0, v1

def tea_enc(v0, v1, key4):
    delta = 0x9E3779B9; s = 0
    for _ in range(32):
        s = (s + delta) & 0xFFFFFFFF
        v0 = (v0 + (((v1<<4) + key4[0]) ^ (v1 + s) ^ ((v1>>5) + key4[1]))) & 0xFFFFFFFF
        v1 = (v1 + (((v0<<4) + key4[2]) ^ (v0 + s) ^ ((v0>>5) + key4[3]))) & 0xFFFFFFFF
    return v0, v1

def test(label, fn, pairs, expected_count=5):
    ok = 0
    for pair in pairs:
        try:
            if fn(*pair[:-1]) == pair[-1]: ok += 1
        except: pass
    if ok > 0:
        print(f"  *** {label}: {ok}/{expected_count} ***")
    return ok

print("="*65)
print("=== 1. AES с сессионным ключом Frame1 ===")
print("="*65)

for s in SESSIONS:
    t, f1, f2, r = s
    k16 = b16(f1)
    for inp, iname in [(t+f2, "trig||F2"), (f2+t, "F2||trig"),
                       (f2, "F2"), (t, "trig"),
                       (bytes(a^b for a,b in zip(t,f2))+b'\x00'*8, "t^f2"),
                       ]:
        try:
            got = AES.new(k16, AES.MODE_ECB).encrypt(b16(inp))[:8]
            if got == r:
                print(f"  *** AES(Frame1_key, {iname}) = response!")
                print(f"      Session: t={t.hex()} f1={f1.hex()} f2={f2.hex()}")
                print(f"      resp={r.hex()}")
        except: pass

# Для quick: AES(Frame1_key, quick_ch) = quick_resp
for q in QUICK:
    qch, qr, si = q
    t, f1, f2, r = SESSIONS[si]
    k16 = b16(f1)
    try:
        got = AES.new(k16, AES.MODE_ECB).encrypt(b16(qch))[:8]
        if got == qr:
            print(f"  *** AES(Frame1_key, quick_ch) = quick_resp! Frame1={f1.hex()}")
    except: pass

print()
print("=== 2. AES с разными ключами Frame1/trigger ===")
# Пробуем Frame1 и trigger как ключи для ВСЕХ сессий
for kid, kfn in [
    ("Frame1",    lambda s: b16(s[1])),
    ("Frame2",    lambda s: b16(s[2])),
    ("trigger",   lambda s: b16(s[0])),
    ("f1^f2",     lambda s: b16(bytes(a^b for a,b in zip(s[1],s[2])))),
    ("f1||f2[:8]",lambda s: s[1]+s[2][:8]),
    ("t||f1[:8]", lambda s: s[0]+s[1][:8]),
]:
    for inp_id, inp_fn in [
        ("F2",         lambda s: b16(s[2])),
        ("trig||F2",   lambda s: b16(s[0]+s[2])),
        ("F1||F2",     lambda s: s[1]+s[2]),
        ("F2||F1",     lambda s: s[2]+s[1]),
        ("trig",       lambda s: b16(s[0])),
        ("t^F2",       lambda s: b16(bytes(a^b for a,b in zip(s[0],s[2])))),
        ("F1^F2",      lambda s: b16(bytes(a^b for a,b in zip(s[1],s[2])))),
    ]:
        ok = 0
        for s in SESSIONS:
            try:
                got = AES.new(kfn(s), AES.MODE_ECB).encrypt(inp_fn(s))[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0:
            print(f"  AES(k={kid}, inp={inp_id}): {ok}/5")

print()
print("=== 3. XTEA/TEA с trigger/Frame1 как ключ (4×u32) ===")

for kid, kfn in [
    ("trig_LE", lambda s: struct.unpack('<II', s[0][:8]) * 2),
    ("f1_LE",   lambda s: struct.unpack('<II', s[1][:8]) * 2),
    ("f2_LE",   lambda s: struct.unpack('<II', s[2][:8]) * 2),
    ("KEY_LE",  lambda s: struct.unpack('<IIII', KEY_FLASH)),
    ("KEY_BE",  lambda s: struct.unpack('>IIII', KEY_FLASH)),
    ("f1_BE",   lambda s: struct.unpack('>II', s[1][:8]) * 2),
]:
    for vfn_name, vfn in [
        ("F2_LE", lambda s: struct.unpack('<II', s[2][:8])),
        ("F2_BE", lambda s: struct.unpack('>II', s[2][:8])),
        ("t_LE",  lambda s: struct.unpack('<II', s[0][:8])),
    ]:
        for algo_name, enc_fn in [("XTEA", xtea_enc), ("TEA", tea_enc)]:
            ok = 0
            for s in SESSIONS:
                try:
                    k4 = kfn(s)
                    v0, v1 = vfn(s)
                    e0, e1 = enc_fn(v0, v1, k4)
                    got = struct.pack('<II', e0, e1)
                    if got == s[3]: ok += 1
                    got_be = struct.pack('>II', e0, e1)
                    if got_be == s[3]: ok += 1
                except: pass
            if ok >= 2: print(f"  {algo_name}(k={kid}, v={vfn_name}): {ok}/10(LE+BE)")

print()
print("=== 4. CMAC (AES-based MAC) ===")
for kid, k16 in [("KEY_FLASH", KEY_FLASH), ("KEY_BE", KEY_BE)]:
    for inp_id, inp_fn in [
        ("F2",       lambda s: s[2]),
        ("F1||F2",   lambda s: s[1]+s[2]),
        ("t||F2",    lambda s: s[0]+s[2]),
    ]:
        ok = 0
        for s in SESSIONS:
            try:
                cobj = CMAC.new(k16, ciphermod=AES)
                cobj.update(inp_fn(s))
                got = cobj.digest()[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0: print(f"  CMAC(k={kid}, {inp_id}): {ok}/5")

print()
print("=== 5. CRC-based: CRC32 flash regions ===")
import zlib

FW = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if FW.exists():
    fw = FW.read_bytes()
    BASE = 0x08000000
    
    # Идея: Frame2 кодирует offset/size в flash, response = CRC32(flash[offset..offset+size])
    # Для quick pair: challenge кодирует offset, response = CRC(flash[...])
    
    print("  Проверяем: quick_response = CRC32(flash[addr_from_challenge..])")
    # Если quick_challenge = flash адрес / offset...
    for q in QUICK:
        qch, qresp, si = q
        # Попробуем разные интерпретации challenge как адреса
        addr_le = struct.unpack('<I', qch[:4])[0]
        addr_be = struct.unpack('>I', qch[:4])[0]
        size_le = struct.unpack('<I', qch[4:])[0]
        size_be = struct.unpack('>I', qch[4:])[0]
        
        for addr, size, desc in [
            (addr_le, size_le&0xFFFF, "addr_LE/size_LE"),
            (addr_be, size_be&0xFFFF, "addr_BE/size_BE"),
            (addr_le&0x3FFFFF, 8, "offset_LE/8"),
        ]:
            if BASE <= addr < BASE+len(fw):
                off = addr - BASE
                if off + size <= len(fw) and size > 0:
                    crc = zlib.crc32(fw[off:off+size]) & 0xFFFFFFFF
                    crc_bytes = struct.pack('<I', crc) * 2
                    if crc_bytes[:8] == qresp:
                        print(f"  *** CRC32 flash[0x{addr:08X}:{size}] = {crc_bytes[:8].hex()} = quick_resp! ***")

print()
print("=== 6. AT32 SLib: проверка если ключ в Option Bytes ===")
# AT32F435 Option Bytes typically at 0x1FFFC000
# Но в нашем дампе это не включено (только main flash)
# Пробуем прочитать option bytes из дампа если есть
if FW.exists():
    fw = FW.read_bytes()
    # Option bytes на STM32/AT32 часто по 0x1FFFC000 или в начале flash
    # В нашем дампе: может быть в первых 0x1000 байт? Или нет их совсем.
    # Ищем паттерн: 0x1FFFF800 или похожие
    print(f"  Размер дампа: {len(fw):,} байт = 0x{len(fw):X}")
    print(f"  Диапазон: 0x08000000 - 0x{0x08000000+len(fw)-1:08X}")
    print(f"  Данные в начале flash (0x08000000..0x08000040):")
    print(f"  {fw[:64].hex()}")

print()
print("=== 7. Анализ SUM(trigger+Frame1) mod 256 ===")
for i, (t, f1, f2, r) in enumerate(SESSIONS):
    s_sum = bytes((a+b)&0xFF for a,b in zip(t, f1))
    s_xor = bytes(a^b for a,b in zip(t, f1))
    # Пробуем: response = AES(KEY, SUM) или AES(SUM||SUM, Frame2)
    for k in [KEY_FLASH, KEY_BE]:
        for inp in [b16(f2), b16(s_sum+f2), b16(s_xor+f2)]:
            k2 = b16(s_sum + s_sum)  # SUM как ключ (16 байт)
            try:
                got = AES.new(k2, AES.MODE_ECB).encrypt(inp)[:8]
                if got == r:
                    print(f"  *** AES(SUM_key, inp) = resp! Session {i+1}")
            except: pass
        try:
            got = AES.new(k, AES.MODE_ECB).encrypt(b16(s_sum+f2))[:8]
            if got == r: print(f"  AES(FLASH_key, sum||F2) = resp! Sess {i+1}")
        except: pass

print()
print("=== 8. Поиск через все 8-байтные слоты flash ===")
print("  (Frame2 → quick_resp: перебираем keys из flash)")
if FW.exists():
    fw = FW.read_bytes()
    q0_ch, q0_resp = QUICK[0][0], QUICK[0][1]
    q1_ch, q1_resp = QUICK[1][0], QUICK[1][1]
    
    print(f"  Q0: {q0_ch.hex()} → {q0_resp.hex()}")
    
    # Пробуем: RC4(flash_key, q0_ch) = q0_resp
    found_rc4 = 0
    for off in range(0, len(fw)-16, 4):
        for klen in [8, 16, 20, 32]:
            k = fw[off:off+klen]
            if k == bytes(klen) or k == bytes([0xFF]*klen): continue
            try:
                got = ARC4.new(k).encrypt(q0_ch)[:8]
                if got == q0_resp:
                    # Full check
                    ok = sum(1 for q in QUICK
                             if ARC4.new(k).encrypt(q[0])[:8] == q[1])
                    if ok >= 2:
                        print(f"  *** RC4(flash[0x{off:06X}:{klen}]) → {ok}/5 ***")
                        print(f"      key = {k.hex()}")
                        found_rc4 += 1
            except: pass
    if not found_rc4:
        print("  RC4 с ключом из flash: не найден")

print()
print("=== Итог ===")
print("Если всё 0/5 → алгоритм в защищённой зоне flash (SLib)")
print("или требует специфического контекста (состояние аппаратного модуля)")
