#!/usr/bin/env python3
"""
crack_algo.py - Систематический поиск алгоритма по всем парам из TRC файлов.

Все сессии:
  trigger → Frame1 (BCM шифрует триггер?) → Frame2 (nonce) → response
"""

import struct, itertools
from pathlib import Path

# Ключ из flash[0x08200000] (16 байт)
KEY = bytes.fromhex("12c2a043dd7b3c50c9265901487a332e")

# Все сессии: (trigger_8, frame1_8, frame2_8, response_8)
SESSIONS = [
    # orig_1.trc
    (bytes.fromhex("5283dba64769c00d"),
     bytes.fromhex("660be1e2a34b8140"),
     bytes.fromhex("b45633a0499a01ec"),
     bytes.fromhex("9cb7f8ca31431bb6")),
    # orig_2.trc
    (bytes.fromhex("a5ea8a93265530dd"),
     bytes.fromhex("cfcfbbf3cdc0f75c"),
     bytes.fromhex("e9efe2eb23b62a25"),
     bytes.fromhex("bff99205ed4ab7a8")),
    # orig_3.trc
    (bytes.fromhex("45d4a5c0f6a1ea72"),
     bytes.fromhex("cd4c9070196bbdeb"),
     bytes.fromhex("b425cb7c4350082c"),
     bytes.fromhex("96739be6b1299f77")),
    # ignon.trc
    (bytes.fromhex("0841994c0b81a14c"),
     bytes.fromhex("4a4f2a204fad58fd"),
     bytes.fromhex("273622b70c2b559e"),
     bytes.fromhex("fda32d94ae77c121")),
    # ignon_and_start.trc
    (bytes.fromhex("d081cc982708d806"),
     bytes.fromhex("66aaeef37037dee0"),
     bytes.fromhex("cdeefb22bde96807"),
     bytes.fromhex("efaa66f0caa6f0cd")),
    # rusEFI в машине (trigger фиксированный, response НЕИЗВЕСТЕН)
    # (bytes.fromhex("0000000000c000"),  # сессия 0
    #  bytes.fromhex("261714f094e7297f"),
    #  bytes.fromhex("b6e2c52595acd6f6"),
    #  None),
]

# Quick пары: (quick_ch_8, quick_resp_8, сессия_index)
QUICK = [
    (bytes.fromhex("0eabfe9d351af837"), bytes.fromhex("b15156d14683cd15"), 0),  # orig_1
    (bytes.fromhex("4a90250e855128bf"), bytes.fromhex("14a267bec8d8c3dc"), 1),  # orig_2
    (bytes.fromhex("63b95b1a3fbf41fe"), bytes.fromhex("2fa71d4722835ea2"), 2),  # orig_3
    (bytes.fromhex("596e3885f2460f4a"), bytes.fromhex("2cdf61de56551b86"), 3),  # ignon
    (bytes.fromhex("a00059b79f90a8ae"), bytes.fromhex("a225de4e470324b6"), 4),  # ignon_and_start
]

try:
    from Cryptodome.Cipher import AES as _AES
except ImportError:
    try:
        from Crypto.Cipher import AES as _AES
    except ImportError:
        _AES = None

if _AES:
    def aes_enc(k, plaintext):
        return _AES.new(k, _AES.MODE_ECB).encrypt(plaintext.ljust(16,b'\x00'))[:8]
    def aes_dec(k, ciphertext):
        return _AES.new(k, _AES.MODE_ECB).decrypt(ciphertext.ljust(16,b'\x00'))[:8]
    HAS_AES = True
else:
    HAS_AES = False
    print("AES недоступен!")

def check_all(fn, pairs):
    return sum(1 for *args, expected in pairs if fn(*args) == expected)

# ---------------------------------------------------------------------------
# 1. Анализ trigger → Frame1 (что BCM делает с триггером?)
# ---------------------------------------------------------------------------
print("="*65)
print("=== 1. Анализ trigger → Frame1 ===")
print("="*65)

# Добавим rusEFI сессию
TRIGGER_FRAME1 = [(s[0], s[1]) for s in SESSIONS] + [
    (bytes.fromhex("000000000000c000"), bytes.fromhex("261714f094e7297f")),  # rusEFI
]

print("\nТриггеры и соответствующие Frame1:")
for t, f1 in TRIGGER_FRAME1:
    print(f"  trigger={t.hex()}  →  Frame1={f1.hex()}")

if HAS_AES:
    print(f"\nЭтот ключ (из flash): {KEY.hex()}")
    
    # Тест: Frame1 = AES(KEY, trigger_padded)?
    ok = 0
    for t, f1 in TRIGGER_FRAME1:
        got = aes_enc(KEY, t.ljust(16,b'\x00'))
        if got == f1:
            ok += 1
            print(f"  MATCH! AES(KEY, {t.hex()}) = {got.hex()}")
    print(f"  AES(KEY, trigger→Frame1): {ok}/{len(TRIGGER_FRAME1)}")
    
    # Тест: Frame1 = AES(KEY, reversed(trigger))?
    ok = sum(1 for t, f1 in TRIGGER_FRAME1
             if aes_enc(KEY, bytes(reversed(t)).ljust(16,b'\x00')) == f1)
    print(f"  AES(KEY, rev_trigger→Frame1): {ok}/{len(TRIGGER_FRAME1)}")
    
    # Тест: AES(KEY, Frame1) = trigger (декодирование)?
    ok = sum(1 for t, f1 in TRIGGER_FRAME1
             if aes_dec(KEY, f1.ljust(16,b'\x00'))[:8] == t)
    print(f"  AES_DEC(KEY, Frame1)→trigger: {ok}/{len(TRIGGER_FRAME1)}")

# ---------------------------------------------------------------------------
# 2. Тест: response = AES(KEY, input) для разных входов
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("=== 2. AES тесты: response от разных комбинаций входов ===")
print(f"{'='*65}")

if HAS_AES:
    # 2a. response = AES(KEY, frame1||frame2)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, f1+f2) == r)
    print(f"\n  AES(KEY, Frame1||Frame2)→response: {ok}/5")
    
    # 2b. response = AES(KEY, frame2||frame1)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, f2+f1) == r)
    print(f"  AES(KEY, Frame2||Frame1)→response: {ok}/5")
    
    # 2c. response = AES(KEY, frame2_padded)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, f2.ljust(16,b'\x00')) == r)
    print(f"  AES(KEY, Frame2)→response: {ok}/5")
    
    # 2d. response = AES(KEY, frame1_padded)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, f1.ljust(16,b'\x00')) == r)
    print(f"  AES(KEY, Frame1)→response: {ok}/5")
    
    # 2e. response = AES(KEY, trigger||frame2)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, t+f2) == r)
    print(f"  AES(KEY, trigger||Frame2)→response: {ok}/5")
    
    # 2f. response = AES(KEY, frame2||trigger)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, f2+t) == r)
    print(f"  AES(KEY, Frame2||trigger)→response: {ok}/5")
    
    # 2g. response = AES(Frame1, frame2)[:8]
    ok = 0
    for t, f1, f2, r in SESSIONS:
        try:
            got = aes_enc(f1.ljust(16,b'\x00'), f2.ljust(16,b'\x00'))
            if got == r: ok += 1
        except: pass
    print(f"  AES(Frame1_as_key, Frame2)→response: {ok}/5")
    
    # 2h. response = AES(Frame2, frame1)[:8]
    ok = 0
    for t, f1, f2, r in SESSIONS:
        try:
            got = aes_enc(f2.ljust(16,b'\x00'), f1.ljust(16,b'\x00'))
            if got == r: ok += 1
        except: pass
    print(f"  AES(Frame2_as_key, Frame1)→response: {ok}/5")
    
    # 2i. response = AES(KEY, Frame1 XOR Frame2)[:8]
    ok = sum(1 for t, f1, f2, r in SESSIONS
             if aes_enc(KEY, bytes(a^b for a,b in zip(f1,f2)).ljust(16,b'\x00')) == r)
    print(f"  AES(KEY, Frame1 XOR Frame2)→response: {ok}/5")
    
    # 2j. response = AES(KEY XOR Frame1, Frame2)[:8]
    ok = 0
    for t, f1, f2, r in SESSIONS:
        k2 = bytes(KEY[i]^f1[i%8] for i in range(16))
        try:
            got = aes_enc(k2, f2.ljust(16,b'\x00'))
            if got == r: ok += 1
        except: pass
    print(f"  AES(KEY^Frame1, Frame2)→response: {ok}/5")
    
    # 2k. response = AES(KEY XOR Frame2, Frame1)[:8]
    ok = 0
    for t, f1, f2, r in SESSIONS:
        k2 = bytes(KEY[i]^f2[i%8] for i in range(16))
        try:
            got = aes_enc(k2, f1.ljust(16,b'\x00'))
            if got == r: ok += 1
        except: pass
    print(f"  AES(KEY^Frame2, Frame1)→response: {ok}/5")

# ---------------------------------------------------------------------------
# 3. QUICK пары: тест с session context
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("=== 3. Quick пары с сессионным контекстом ===")
print(f"{'='*65}")

if HAS_AES:
    # 3a. quick_resp = AES(KEY, Frame1||quick_ch)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        got = aes_enc(KEY, f1+qch)
        if got == qresp: ok += 1
    print(f"\n  AES(KEY, Frame1||quick_ch)→quick_resp: {ok}/5")
    
    # 3b. quick_resp = AES(KEY, trigger||quick_ch)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        got = aes_enc(KEY, t+qch)
        if got == qresp: ok += 1
    print(f"  AES(KEY, trigger||quick_ch)→quick_resp: {ok}/5")
    
    # 3c. quick_resp = AES(KEY, full_resp||quick_ch)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        got = aes_enc(KEY, r+qch)
        if got == qresp: ok += 1
    print(f"  AES(KEY, full_resp||quick_ch)→quick_resp: {ok}/5")
    
    # 3d. quick_resp = AES(KEY, quick_ch||Frame1)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        got = aes_enc(KEY, qch+f1)
        if got == qresp: ok += 1
    print(f"  AES(KEY, quick_ch||Frame1)→quick_resp: {ok}/5")
    
    # 3e. quick_resp = AES(full_resp, quick_ch)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        try:
            got = aes_enc(r.ljust(16,b'\x00'), qch.ljust(16,b'\x00'))
            if got == qresp: ok += 1
        except: pass
    print(f"  AES(full_resp, quick_ch)→quick_resp: {ok}/5")
    
    # 3f. quick_resp = AES(Frame1, quick_ch)[:8]
    ok = 0
    for qch, qresp, sess_idx in QUICK:
        t, f1, f2, r = SESSIONS[sess_idx]
        try:
            got = aes_enc(f1.ljust(16,b'\x00'), qch.ljust(16,b'\x00'))
            if got == qresp: ok += 1
        except: pass
    print(f"  AES(Frame1, quick_ch)→quick_resp: {ok}/5")

# ---------------------------------------------------------------------------
# 4. Анализ побайтовой зависимости: trigger → Frame1
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("=== 4. Побайтовый анализ trigger → Frame1 ===")
print(f"='*65")

print("\nTrigger bytes → Frame1 bytes:")
print(f"{'T0':>6} {'T1':>6} {'T2':>6} {'T3':>6} {'T4':>6} {'T5':>6} {'T6':>6} {'T7':>6}")
print(f"{'F0':>6} {'F1':>6} {'F2':>6} {'F3':>6} {'F4':>6} {'F5':>6} {'F6':>6} {'F7':>6}")
for t, f1 in TRIGGER_FRAME1:
    t_hex = ' '.join(f'{b:02X}' for b in t)
    f_hex = ' '.join(f'{b:02X}' for b in f1)
    print(f"  {t_hex}")
    print(f"  {f_hex}")
    print()

# Проверяем XOR паттерны
print("XOR(trigger, Frame1):")
for t, f1 in TRIGGER_FRAME1:
    xor = bytes(a^b for a,b in zip(t, f1))
    print(f"  {xor.hex()}")

# ---------------------------------------------------------------------------
# 5. Большой перебор: Frame1 = AES(???, trigger)
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("=== 5. Поиск BCM-ключа: Frame1 = AES(BCM_KEY, trigger) ===")
print(f"='*65")

FW = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if FW.exists() and HAS_AES:
    fw = FW.read_bytes()
    BASE = 0x08000000
    
    t0, f1_0 = TRIGGER_FRAME1[0]  # первая пара для быстрой проверки
    t0_padded = t0.ljust(16, b'\x00')
    
    print(f"Ищем BCM_KEY такой что AES(BCM_KEY, trigger0)[:8] == Frame1_0...")
    print(f"trigger0 = {t0.hex()}, Frame1_0 = {f1_0.hex()}")
    
    found = []
    for off in range(0, len(fw)-16, 4):
        k = fw[off:off+16]
        if all(b == 0 for b in k) or all(b == 0xFF for b in k): continue
        try:
            got = aes_enc(k, t0_padded)
            if got == f1_0:
                # Проверяем остальные пары
                ok = sum(1 for t2, f1_2 in TRIGGER_FRAME1
                         if aes_enc(k, t2.ljust(16,b'\x00')) == f1_2)
                if ok >= 2:
                    print(f"  *** BCM KEY @ file 0x{off:06X}: {k.hex()} [{ok}/{len(TRIGGER_FRAME1)}] ***")
                    found.append((off, k, ok))
        except: pass
    
    if not found:
        print("  BCM KEY (AES trigger→Frame1) не найден")
    
    # Ищем ключ для RESPONSE: Frame1||Frame2 → response
    print(f"\nИщем KEY для: AES(KEY, Frame1||Frame2)[:8] = response...")
    t0, f1_0, f2_0, r0 = SESSIONS[0]
    inp0 = f1_0 + f2_0
    
    found2 = []
    for off in range(0, len(fw)-16, 4):
        k = fw[off:off+16]
        if all(b == 0 for b in k) or all(b == 0xFF for b in k): continue
        try:
            got = aes_enc(k, inp0)
            if got == r0:
                ok = sum(1 for t, f1, f2, r in SESSIONS
                         if aes_enc(k, f1+f2) == r)
                if ok >= 2:
                    print(f"  *** KEY(f1||f2→r) @ 0x{off:06X}: {k.hex()} [{ok}/5] ***")
                    found2.append((off, k, ok))
        except: pass
    
    if not found2:
        print("  Ключ для AES(f1||f2→r) не найден")

print(f"\n{'='*65}")
print("=== Итог ===")
print(f"'='*65")
print("""
Все 5 полных пар известны + 5 quick пар.
Если ничего не совпало — алгоритм не AES с одним ключом из flash.
Возможные следующие шаги:
1. Попробовать двухэтапный AES (response = AES(AES(KEY, Frame1), Frame2))
2. Попробовать SHA/HMAC с контекстом сессии  
3. Провести Unicorn-эмуляцию с реальным challenge

Ключ из flash[0x08200000]: 12c2a043dd7b3c50c9265901487a332e
Все 5 сессий имеют уникальный trigger и уникальный Frame1 (BCM шифрует trigger?)
""")
