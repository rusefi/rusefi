#!/usr/bin/env python3
"""
parse_all_trc.py  –  извлечь ВСЕ пары вызов/ответ IMMO из TRC файлов
"""

import re, sys
from pathlib import Path

TRC_FILES = [
    "orig_1.trc",
    "orig_2.trc",
    "orig_3.trc",
    "ignon.trc",
    "ignon_and_start.trc",
    "ignoff.trc",
]

def parse_trc(path):
    frames = []
    pat = re.compile(
        r'^\s*\d+\)\s+([\d.]+)\s+\w+\s+([0-9A-Fa-f]+)\s+(\d)\s+((?:[0-9A-Fa-f]{2}\s*)+)'
    )
    for line in Path(path).read_text(errors='ignore').splitlines():
        m = pat.match(line)
        if not m:
            continue
        t = float(m.group(1))
        cid = int(m.group(2), 16)
        dlc = int(m.group(3))
        raw = m.group(4).strip()
        databytes = bytes.fromhex(raw.replace(' ', ''))[:dlc]
        frames.append((t, cid, databytes))
    return frames

# Сбор всех уникальных пар
all_pairs16  = {}  # (f1hex, f2hex) -> (resp, src)
all_pairs8   = {}  # chex -> (resp, src)

print("=" * 65)

for fname in TRC_FILES:
    p = Path(fname)
    if not p.exists():
        print(f"  НЕ НАЙДЕН: {fname}")
        continue
    frames = parse_trc(fname)
    print(f"\n{fname}: {len(frames)} кадров")

    # Ищем все последовательности 0x0714 → 0x0713
    i = 0
    found_full = 0
    found_quick = 0
    while i < len(frames):
        t0, cid0, d0 = frames[i]
        if cid0 != 0x0714 or len(d0) < 8:
            i += 1
            continue

        # Ищем что идёт дальше
        # Вариант 1: следующий кадр тоже 0x0714 (через ≤ 5 мс) — это full challenge
        if i+1 < len(frames):
            t1, cid1, d1 = frames[i+1]
            if cid1 == 0x0714 and len(d1) >= 8 and (t1 - t0) <= 5.0:
                # Полный вызов — ищем 0x0713 ответ в течение 500 мс
                frame1 = bytes(d0[:8])
                frame2 = bytes(d1[:8])
                for j in range(i+2, min(i+100, len(frames))):
                    tj, cidj, dj = frames[j]
                    if (tj - t1) > 500:
                        break
                    if cidj == 0x0713 and len(dj) >= 8:
                        resp = bytes(dj[:8])
                        key = (frame1.hex(), frame2.hex())
                        if key not in all_pairs16:
                            all_pairs16[key] = (resp, fname)
                            found_full += 1
                        i = j + 1
                        break
                else:
                    i += 2
                continue

        # Вариант 2: следующий 0x0713 в течение 500 мс — quick check
        ch8 = bytes(d0[:8])
        for j in range(i+1, min(i+50, len(frames))):
            tj, cidj, dj = frames[j]
            if (tj - t0) > 500:
                break
            if cidj == 0x0714:
                # Между ними ещё один 0x0714 — не quick
                break
            if cidj == 0x0713 and len(dj) >= 8:
                resp = bytes(dj[:8])
                key = ch8.hex()
                if key not in all_pairs8:
                    all_pairs8[key] = (resp, fname)
                    found_quick += 1
                i = j + 1
                break
        else:
            i += 1
        if i <= len(frames) and len(frames) > 0:
            pass  # продолжаем

    print(f"  Найдено: {found_full} полных + {found_quick} quick пар")

# ---------------------------------------------------------------------------
# Все найденные пары
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print(f"ИТОГО: {len(all_pairs16)} полных (16→8) + {len(all_pairs8)} quick (8→8) пар")
print(f"{'='*65}")

print(f"\n--- Полные пары (frame1+frame2 → response) ---")
for (f1h, f2h), (resp, src) in all_pairs16.items():
    print(f"  [{src}]")
    print(f"    F1={f1h}  F1[0]={int(f1h[:2],16):02X}(lo={int(f1h[:2],16)&0xF},hi={int(f1h[:2],16)>>4})")
    print(f"    F2={f2h}")
    print(f"    R ={resp.hex()}")

print(f"\n--- Quick пары (8 байт → 8 байт) ---")
for chex, (resp, src) in all_pairs8.items():
    print(f"  [{src}] {chex} -> {resp.hex()}")

# ---------------------------------------------------------------------------
# Анализ
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("Анализ: Frame1 nibble-check")
print(f"{'='*65}")

def valid_nibbles(b): return (b & 0xF) <= 4 and (b >> 4) <= 4

for (f1h, f2h), (resp, src) in all_pairs16.items():
    f1 = bytes.fromhex(f1h)
    f2 = bytes.fromhex(f2h)
    all_ch = f1 + f2
    valid = [(i, b) for i, b in enumerate(all_ch) if valid_nibbles(b)]
    print(f"  {f1h}|{f2h}:")
    for idx, b in valid:
        loc = "F1" if idx < 8 else "F2"
        print(f"    [{loc}[{idx%8}]] = 0x{b:02X}  lo={b&0xF} hi={b>>4} ✓")

# ---------------------------------------------------------------------------
# XOR анализ
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("XOR(Frame2, Response) для полных пар:")
print(f"{'='*65}")
for (f1h, f2h), (resp, src) in all_pairs16.items():
    f2 = bytes.fromhex(f2h)
    xor = bytes(a^b for a,b in zip(f2, resp))
    print(f"  F2={f2h} R={resp.hex()} XOR={xor.hex()}")

print(f"\nXOR(Challenge, Response) для quick пар:")
for chex, (resp, src) in all_pairs8.items():
    ch = bytes.fromhex(chex)
    xor = bytes(a^b for a,b in zip(ch, resp))
    print(f"  {chex} -> {resp.hex()}  XOR={xor.hex()}")

# ---------------------------------------------------------------------------
# Python код
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("Python код всех пар:")
print(f"{'='*65}")
print("PAIRS16 = [")
for (f1h, f2h), (resp, src) in all_pairs16.items():
    print(f"    # {src}")
    print(f"    (bytes.fromhex('{f1h+f2h}'), bytes.fromhex('{resp.hex()}')),")
print("]")
print("\nPAIRS8 = [")
for chex, (resp, src) in all_pairs8.items():
    print(f"    # {src}")
    print(f"    (bytes.fromhex('{chex}'), bytes.fromhex('{resp.hex()}')),")
print("]")

# ---------------------------------------------------------------------------
# Поиск алгоритма: XTEA/DES/AES с ключами из flash
# ---------------------------------------------------------------------------
print(f"\n{'='*65}")
print("Поиск алгоритма для quick пар...")
print(f"{'='*65}")

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
if FIRMWARE.exists() and all_pairs8:
    import struct
    fw = FIRMWARE.read_bytes()

    def xtea_enc(v0, v1, key):
        delta = 0x9E3779B9; s = 0
        for _ in range(32):
            v0 = (v0 + (((v1<<4^v1>>5)+v1) ^ (s+key[s&3]))) & 0xFFFFFFFF
            s  = (s + delta) & 0xFFFFFFFF
            v1 = (v1 + (((v0<<4^v0>>5)+v0) ^ (s+key[(s>>11)&3]))) & 0xFFFFFFFF
        return v0, v1

    # Первая quick пара для быстрой проверки
    ch0, (r0, _) = next(iter(all_pairs8.items()))
    ch0b = bytes.fromhex(ch0); r0b = bytes.fromhex(r0)
    v0_t = struct.unpack('<I', ch0b[:4])[0]
    v1_t = struct.unpack('<I', ch0b[4:])[0]
    r0_t = struct.unpack('<I', r0b[:4])[0]
    r1_t = struct.unpack('<I', r0b[4:])[0]

    print(f"  Тестовая quick пара: {ch0} -> {r0}")
    print(f"  Поиск XTEA ключа в flash...")

    found = False
    for off in range(0, min(len(fw)-16, 0x3F0000), 4):
        key_be = struct.unpack('>IIII', fw[off:off+16])
        key_le = struct.unpack('<IIII', fw[off:off+16])
        for key, kname in [(key_be, 'BE'), (key_le, 'LE')]:
            if all(k == 0 for k in key) or all(k == 0xFFFFFFFF for k in key):
                continue
            e0, e1 = xtea_enc(v0_t, v1_t, key)
            if e0 == r0_t and e1 == r1_t:
                # Проверяем все пары
                all_ok = True
                for chex2, (r2, _) in all_pairs8.items():
                    ch2 = bytes.fromhex(chex2); r2b = bytes.fromhex(r2)
                    vv0 = struct.unpack('<I', ch2[:4])[0]
                    vv1 = struct.unpack('<I', ch2[4:])[0]
                    ee0, ee1 = xtea_enc(vv0, vv1, key)
                    got = struct.pack('<II', ee0, ee1)
                    if got != r2b:
                        all_ok = False; break
                if all_ok:
                    print(f"  *** XTEA КЛЮЧ НАЙДЕН! Файл 0x{off:06X} chip 0x{0x08000000+off:08X} [{kname}]: {fw[off:off+16].hex()} ***")
                    found = True
                    break
        if found: break

    if not found:
        print(f"  XTEA ключ не найден (проверено {min(len(fw)//4, 0x3F0000//4)} позиций)")

    # Проверим DES если доступен
    try:
        from Crypto.Cipher import DES
        print(f"\n  Поиск DES (8-байт) ключа в flash...")
        for off in range(0, min(len(fw)-8, 0x3F0000), 4):
            key8 = fw[off:off+8]
            if all(b == 0 for b in key8) or all(b == 0xFF for b in key8):
                continue
            try:
                cipher = DES.new(key8, DES.MODE_ECB)
                got = cipher.encrypt(ch0b)
                if got == r0b:
                    all_ok = all(DES.new(key8, DES.MODE_ECB).encrypt(bytes.fromhex(c)) == bytes.fromhex(r)
                                 for c,(r,_) in all_pairs8.items())
                    if all_ok:
                        print(f"  *** DES КЛЮЧ! 0x{off:06X}: {key8.hex()} ***")
                        break
            except Exception:
                pass
        else:
            print(f"  DES ключ не найден")
    except ImportError:
        pass

    # AES для quick пар
    try:
        from Crypto.Cipher import AES
        print(f"\n  Поиск AES-128 ключа для quick пар...")
        ch0p = ch0b.ljust(16, b'\x00')
        for off in range(0, min(len(fw)-16, 0x3F0000), 4):
            key16 = fw[off:off+16]
            if all(b == 0 for b in key16) or all(b == 0xFF for b in key16):
                continue
            try:
                got = AES.new(key16, AES.MODE_ECB).encrypt(ch0p)[:8]
                if got == r0b:
                    all_ok = all(
                        AES.new(key16, AES.MODE_ECB).encrypt(bytes.fromhex(c).ljust(16,b'\x00'))[:8] == bytes.fromhex(r)
                        for c,(r,_) in all_pairs8.items()
                    )
                    if all_ok:
                        print(f"  *** AES КЛЮЧ! 0x{off:06X}: {key16.hex()} ***")
                        break
            except Exception:
                pass
        else:
            print(f"  AES ключ не найден")
    except ImportError:
        pass

print("\n=== Done ===")
