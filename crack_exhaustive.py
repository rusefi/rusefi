#!/usr/bin/env python3
"""
crack_exhaustive.py - Исчерпывающий поиск алгоритма IMMO.

Ключевое наблюдение:
- Frame1 = f(trigger, BCM_key) — детерминирована от триггера (подтверждено rusEFI)
- Frame2 = случайный nonce от BCM
- response = g(Frame1, Frame2, ECU_key) или g(trigger, Frame2, key)

Все 5 сессий + rusEFI trigger:
  trigger → Frame1 (BCM вычисляет)
  Frame2 → response (ECU вычисляет)

Тестируем: AES-ECB/CBC/CTR, DES, 3DES, RC4, SHA, HMAC
с разными ключами и порядками байт.
"""

import struct, os
from pathlib import Path

try:
    from Cryptodome.Cipher import AES, DES, DES3, ARC4
    from Cryptodome.Hash import SHA1, SHA256, MD5, HMAC
    print("Cryptodome доступен")
except ImportError:
    from Crypto.Cipher import AES, DES, DES3, ARC4
    from Crypto.Hash import SHA1, SHA256, MD5, HMAC

# Все сессии: (trigger, Frame1, Frame2, response)
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
QUICK = [  # (challenge, response, session_idx)
    (bytes.fromhex("0eabfe9d351af837"), bytes.fromhex("b15156d14683cd15"), 0),
    (bytes.fromhex("4a90250e855128bf"), bytes.fromhex("14a267bec8d8c3dc"), 1),
    (bytes.fromhex("63b95b1a3fbf41fe"), bytes.fromhex("2fa71d4722835ea2"), 2),
    (bytes.fromhex("596e3885f2460f4a"), bytes.fromhex("2cdf61de56551b86"), 3),
    (bytes.fromhex("a00059b79f90a8ae"), bytes.fromhex("a225de4e470324b6"), 4),
]

def b16(x): return x.ljust(16, b'\x00')
def b8(x):  return x.ljust(8,  b'\x00')

def try_aes_ecb(k16, inputs_expected):
    ok = 0
    for inp, exp in inputs_expected:
        try:
            got = AES.new(k16, AES.MODE_ECB).encrypt(b16(inp))[:8]
            if got == exp: ok += 1
        except: pass
    return ok

def try_aes_cbc(k16, iv16, inputs_expected):
    ok = 0
    for inp, exp in inputs_expected:
        try:
            got = AES.new(k16, AES.MODE_CBC, iv=iv16).encrypt(b16(inp))[:8]
            if got == exp: ok += 1
        except: pass
    return ok

def try_des(k8, inputs_expected):
    ok = 0
    for inp, exp in inputs_expected:
        try:
            got = DES.new(k8, DES.MODE_ECB).encrypt(b8(inp))[:8]
            if got == exp: ok += 1
        except: pass
    return ok

def try_des_cbc(k8, iv8, inputs_expected):
    ok = 0
    for inp, exp in inputs_expected:
        try:
            got = DES.new(k8, DES.MODE_CBC, iv=iv8).encrypt(b8(inp))[:8]
            if got == exp: ok += 1
        except: pass
    return ok

# ---------------------------------------------------------------------------
# Все кандидатные ключи
# ---------------------------------------------------------------------------

# Ключ из flash[0x08200000]
KEY_FLASH = bytes.fromhex("12c2a043dd7b3c50c9265901487a332e")
# LE байты 4 константы
KEY_BE    = bytes.fromhex("43a0c212503c7bdd015926c92e337a48")
# 4 key constants из кода
KEY_CODE  = bytes.fromhex("d2a4482513b7f94d12c2a043524ac7f9")  # LE
# Flash key reversed
KEY_REV   = bytes(reversed(KEY_FLASH))
KEY_BE_REV= bytes(reversed(KEY_BE))

# 8-байтные ключи (для DES)
KEYS_8 = [
    KEY_FLASH[:8], KEY_FLASH[8:], KEY_BE[:8], KEY_BE[8:],
    KEY_REV[:8], KEY_CODE[:8], KEY_CODE[8:],
    bytes.fromhex("43a0c212503c7bdd"),  # first 2 u32 BE
    bytes.fromhex("015926c92e337a48"),  # last 2 u32 BE
    bytes.fromhex("2548a4d24df9123b"),  # code const A LE
    bytes.fromhex("43a0c212f9c74a52"),  # code const B LE
]

KEYS_16 = [KEY_FLASH, KEY_BE, KEY_REV, KEY_BE_REV, KEY_CODE,
           bytes.fromhex("2548a4d24df9123b43a0c212f9c74a52"),  # all 4 consts LE
           bytes.fromhex("25484ad24d12f93b43c2a012f9c74a52"),  # shuffled
           ]

print("="*65)
print("=== ТЕСТ 1: AES-ECB с разными ключами ===")
print("="*65)

for kname, k16 in [
    ("KEY_FLASH", KEY_FLASH), ("KEY_BE", KEY_BE),
    ("KEY_REV", KEY_REV), ("KEY_BE_REV", KEY_BE_REV), ("KEY_CODE", KEY_CODE),
]:
    # response = AES(k, Frame2)
    ok = try_aes_ecb(k16, [(s[2], s[3]) for s in SESSIONS])
    if ok > 0: print(f"  AES(k={kname}, Frame2)→resp: {ok}/5")
    # response = AES(k, Frame1)
    ok = try_aes_ecb(k16, [(s[1], s[3]) for s in SESSIONS])
    if ok > 0: print(f"  AES(k={kname}, Frame1)→resp: {ok}/5")
    # response = AES(k, Frame1||Frame2)
    ok = try_aes_ecb(k16, [(s[1]+s[2], s[3]) for s in SESSIONS])
    if ok > 0: print(f"  AES(k={kname}, F1||F2)→resp: {ok}/5")
    # response = AES(k, trigger||Frame2)
    ok = try_aes_ecb(k16, [(s[0]+s[2], s[3]) for s in SESSIONS])
    if ok > 0: print(f"  AES(k={kname}, trig||F2)→resp: {ok}/5")
    # quick
    ok = try_aes_ecb(k16, [(q[0], q[1]) for q in QUICK])
    if ok > 0: print(f"  AES(k={kname}, quick_ch)→quick_r: {ok}/5")

print("="*65)
print("=== ТЕСТ 2: AES-CBC с разными ключами и IV ===")
print("="*65)

for kname, k16 in [("KEY_FLASH", KEY_FLASH), ("KEY_BE", KEY_BE)]:
    for ivname, iv_fn in [
        ("IV=trig", lambda s: b16(s[0])),
        ("IV=Frame1", lambda s: b16(s[1])),
        ("IV=zeros", lambda s: b'\x00'*16),
        ("IV=KEY[:16]", lambda s: KEY_FLASH),
    ]:
        # AES-CBC(k, IV=iv, plaintext=Frame2) → response
        ok = 0
        for s in SESSIONS:
            try:
                iv = iv_fn(s)
                got = AES.new(k16, AES.MODE_CBC, iv=iv).encrypt(b16(s[2]))[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0: print(f"  AES-CBC(k={kname}, {ivname}, Frame2)→resp: {ok}/5")

        # AES-CBC(k, IV=iv, plaintext=Frame1||Frame2) → response
        ok = 0
        for s in SESSIONS:
            try:
                iv = iv_fn(s)
                got = AES.new(k16, AES.MODE_CBC, iv=iv).encrypt(s[1]+s[2])[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0: print(f"  AES-CBC(k={kname}, {ivname}, F1||F2)→resp: {ok}/5")

print("="*65)
print("=== ТЕСТ 3: DES/3DES ===")
print("="*65)

for kname, k8 in zip(["K1","K2","K3","K4","K5","K6","K7","K8","K9","K10","K11"],
                      KEYS_8):
    try:
        # DES-ECB(k, Frame2)
        ok = try_des(k8, [(s[2], s[3]) for s in SESSIONS])
        if ok > 0: print(f"  DES-ECB({kname}={k8.hex()}, Frame2): {ok}/5")
        # DES-ECB(k, quick_ch)
        ok = try_des(k8, [(q[0], q[1]) for q in QUICK])
        if ok > 0: print(f"  DES-ECB({kname}, quick_ch): {ok}/5")
        # DES-ECB on XOR'd input
        ok = try_des(k8, [(bytes(a^b for a,b in zip(s[1],s[2])), s[3]) for s in SESSIONS])
        if ok > 0: print(f"  DES-ECB({kname}, F1^F2): {ok}/5")
    except: pass

print()
# 3DES with 16-byte keys
for k16 in [KEY_FLASH, KEY_BE, KEY_REV]:
    try:
        k24 = k16 + k16[:8]  # 3DES key (16→24 bytes)
        ok = 0
        for s in SESSIONS:
            try:
                got = DES3.new(k24, DES3.MODE_ECB).encrypt(b8(s[2]))[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0: print(f"  3DES-ECB({k16.hex()[:16]}..., Frame2): {ok}/5")
    except: pass

print("="*65)
print("=== ТЕСТ 4: RC4 ===")
print("="*65)

# RC4 как поточный шифр: response = RC4(key, input)
for kname, k in [("KEY_FLASH", KEY_FLASH), ("KEY_BE", KEY_BE),
                 ("KEY_FLASH[:8]", KEY_FLASH[:8]), ("KEY_BE[:8]", KEY_BE[:8])]:
    for inp_name, inp_fn in [
        ("Frame2", lambda s: s[2]),
        ("Frame1", lambda s: s[1]),
        ("F1||F2", lambda s: s[1]+s[2]),
        ("trig||F2", lambda s: s[0]+s[2]),
    ]:
        ok = 0
        for s in SESSIONS:
            try:
                arc = ARC4.new(k)
                arc.encrypt(inp_fn(s))  # можно также попробовать с IV
                got = ARC4.new(k).encrypt(inp_fn(s))[:8]
                if got == s[3]: ok += 1
            except: pass
        if ok > 0: print(f"  RC4({kname}, {inp_name})→resp: {ok}/5")
    
    # RC4 для quick
    ok = 0
    for q in QUICK:
        try:
            got = ARC4.new(k).encrypt(q[0])[:8]
            if got == q[1]: ok += 1
        except: pass
    if ok > 0: print(f"  RC4({kname}, quick_ch)→quick_r: {ok}/5")

print("="*65)
print("=== ТЕСТ 5: HMAC с разными вариантами ===")
print("="*65)

for kname, k in [("KEY_FLASH", KEY_FLASH), ("KEY_BE", KEY_BE),
                 ("K8_0", KEY_FLASH[:8]), ("K8_1", KEY_BE[:8])]:
    for algo, algo_name in [(SHA1, "SHA1"), (MD5, "MD5"), (SHA256, "SHA256")]:
        for inp_name, inp_fn in [
            ("Frame2", lambda s: s[2]),
            ("F1||F2", lambda s: s[1]+s[2]),
            ("trig||F2", lambda s: s[0]+s[2]),
        ]:
            ok = 0
            for s in SESSIONS:
                try:
                    h = HMAC.new(k, msg=inp_fn(s), digestmod=algo)
                    if h.digest()[:8] == s[3]: ok += 1
                except: pass
            if ok > 0: print(f"  HMAC-{algo_name}({kname}, {inp_name})→resp: {ok}/5")

print("="*65)
print("=== ТЕСТ 6: Паттерн trigger → Frame1 детально ===")
print("="*65)

# Анализ побайтовой зависимости trigger→Frame1
print("\nТrigger и Frame1 по байтам:")
for i, (t, f1, f2, r) in enumerate(SESSIONS):
    print(f"\nСессия {i+1}:")
    print(f"  Trigger: {' '.join(f'{b:02X}' for b in t)}")
    print(f"  Frame1:  {' '.join(f'{b:02X}' for b in f1)}")
    print(f"  XOR:     {' '.join(f'{a^b:02X}' for a,b in zip(t,f1))}")
    print(f"  SUM:     {' '.join(f'{(a+b)&0xFF:02X}' for a,b in zip(t,f1))}")

# Проверяем: может Frame1 = trigger с инкрементом (+N для каждого байта)?
print("\nПроверяем Frame1 = trigger + constant[i] mod 256:")
for i in range(len(SESSIONS)-1):
    t1, f1_1 = SESSIONS[i][0], SESSIONS[i][1]
    t2, f1_2 = SESSIONS[i+1][0], SESSIONS[i+1][1]
    diff1 = bytes((f-t)&0xFF for f,t in zip(f1_1, t1))
    diff2 = bytes((f-t)&0xFF for f,t in zip(f1_2, t2))
    const = (diff1 == diff2)
    if const:
        print(f"  Константный diff между сессиями {i+1} и {i+2}! Diff={diff1.hex()}")

# Для rusEFI: trigger=zeros → Frame1=?
# Если Frame1 = AES(BCM_key, trigger), то Frame1 = AES(BCM_key, 0...0)
# Ищем такой BCM_key в flash
print()
print("=== ТЕСТ 7: Поиск BCM_key в flash (Frame1=AES(BCM_key,trigger)) ===")
FW = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if FW.exists():
    fw = FW.read_bytes()
    t0_padded = SESSIONS[0][0].ljust(16, b'\x00')
    f1_0 = SESSIONS[0][1]
    
    print(f"Быстрый поиск: AES(BCM_key, {SESSIONS[0][0].hex()})[:8] = {f1_0.hex()}")
    
    count = 0
    for off in range(0, len(fw)-16, 4):
        k = fw[off:off+16]
        if k == bytes(16) or k == bytes([0xFF]*16): continue
        try:
            got = AES.new(k, AES.MODE_ECB).encrypt(t0_padded)[:8]
            if got == f1_0:
                # Проверяем все сессии
                ok = sum(1 for s in SESSIONS
                         if AES.new(k, AES.MODE_ECB).encrypt(b16(s[0]))[:8] == s[1])
                print(f"  *** BCM_KEY @ 0x{off:06X}: {k.hex()} [{ok}/5] ***")
                count += 1
        except: pass
        # AES-CBC с zero IV
        try:
            got = AES.new(k, AES.MODE_CBC, iv=b'\x00'*16).encrypt(t0_padded)[:8]
            if got == f1_0:
                ok = sum(1 for s in SESSIONS
                         if AES.new(k, AES.MODE_CBC, iv=b'\x00'*16).encrypt(b16(s[0]))[:8] == s[1])
                print(f"  *** BCM_KEY(CBC) @ 0x{off:06X}: {k.hex()} [{ok}/5] ***")
        except: pass
    
    if count == 0:
        print("  BCM_KEY не найден")

print("\n=== ИТОГ ===")
print("Все тесты завершены. Если 0/5 - алгоритм нестандартный или ключ не в flash.")
print("Следующие гипотезы для проверки:")
print("  1. Ключ в OPTION BYTES (не в основном flash)")
print("  2. Ключ производный: SHA256(flash_content) как ключ")
print("  3. Алгоритм - двухшаговый (AES или SHA chain)")
print("  4. Проверить AT32F435 Secure Library (SLib) - защищённая зона flash")
