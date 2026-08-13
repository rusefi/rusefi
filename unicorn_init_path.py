#!/usr/bin/env python3
"""
unicorn_init_path.py  –  эмуляция первичной инициализации FUN_08204B54
и анализ что она записывает в SRAM.

Гипотеза: FUN_08204B54 читает из калибровки ECU и записывает
правильный дескриптор сессии в SRAM[0x20000C14..], а данные
от BCM (Frame1/Frame2) используются иначе.
"""

import struct
from pathlib import Path
from unicorn import *
from unicorn.arm_const import *

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(a): return a - BASE
def u32(o):  return struct.unpack_from('<I', data, o)[0]

FRAME1 = bytes.fromhex("261714F094E7297F")
FRAME2_LIST = [
    bytes.fromhex("B6E2C52595ACD6F6"),
    bytes.fromhex("C0A376B3E9FDB85F"),
    bytes.fromhex("E1F1651D533D54B8"),
    bytes.fromhex("A2699D694B45D96E"),
    bytes.fromhex("976AD10D9A3DE065"),
    bytes.fromhex("619ADFC29B480A80"),
    bytes.fromhex("F38352B522A50DDB"),
    bytes.fromhex("8DDF022D20C69E89"),
    bytes.fromhex("59EF563691AF1D46"),
    bytes.fromhex("EE8CC257E2644898"),
]

FLASH_BASE = 0x08000000; FLASH_SIZE = 0x00400000
SRAM_BASE  = 0x20000000; SRAM_SIZE  = 0x00020000
PERIPH_BASE= 0x40000000; PERIPH_SIZE= 0x10000000
SENTINEL   = 0x10000000

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
# 1. Эмуляция FUN_08204B54 (первичная инициализация)
# ---------------------------------------------------------------------------
print("=" * 60)
print("=== 1. FUN_08204B54 (первичная инициализация, SRAM[0x200002C8]=0) ===")
print("=" * 60)

def run_first_init(frame1, frame2):
    mu = make_uc()
    mu.mem_write(0x200003FE, bytes([1]))  # FUN_082037D8 → 1

    # Кладём 16-байтный вызов в буфер (оба кандидата)
    mu.mem_write(0x20000C14, bytes([0x14, 0x01, 0xFF, 0x00]))
    mu.mem_write(0x20000C18, frame1 + frame2)

    # Регистрируем крипто-обработчик
    mu.mem_write(0x20001018 + 0x54 + 0x34, struct.pack('<I', 0x08201E2D))

    sram_writes = {}

    def hook_m(mu, access, address, size, value, ud):
        v = value & ((1 << (size * 8)) - 1)
        for i in range(size):
            sram_writes[address + i] = (v >> (i * 8)) & 0xFF

    executed = [0]
    trace = []

    def hook_c(mu, addr, sz, ud):
        executed[0] += 1
        if addr == SENTINEL: mu.emu_stop()
        if executed[0] > 200000: mu.emu_stop()
        if addr in (0x0820630C, 0x08204B54, 0x08204C00, 0x08204BAC,
                    0x08205F9C, 0x08204C30, 0x08202254, 0x08201660):
            r0 = mu.reg_read(UC_ARM_REG_R0)
            r1 = mu.reg_read(UC_ARM_REG_R1)
            trace.append(f"  call 0x{addr:08X}: r0=0x{r0:08X} r1=0x{r1:08X}")

    def hook_i(mu, *a): mu.emu_stop(); return False

    mu.hook_add(UC_HOOK_MEM_WRITE, hook_m)
    mu.hook_add(UC_HOOK_CODE, hook_c)
    mu.hook_add(UC_HOOK_MEM_INVALID, hook_i)

    # Вызываем FUN_08201E2C с SRAM[0x200002C8]=0 (первичная инициализация)
    SP = (SRAM_BASE + SRAM_SIZE - 0x200) & ~7
    out_len_ptr = SRAM_BASE + 0x8000
    mu.mem_write(out_len_ptr, b'\x00' * 4)
    SP -= 16
    mu.reg_write(UC_ARM_REG_SP, SP)
    mu.mem_write(SP,     struct.pack('<I', 0x20000C1C))
    mu.mem_write(SP + 4, struct.pack('<I', 0x3FC))
    mu.mem_write(SP + 8, struct.pack('<I', out_len_ptr))

    mu.reg_write(UC_ARM_REG_R0, 1)
    mu.reg_write(UC_ARM_REG_R1, 0xFF00)
    mu.reg_write(UC_ARM_REG_R2, 0x20000C18)
    mu.reg_write(UC_ARM_REG_R3, 10)

    try:
        mu.emu_start(0x08201E2C | 1, SENTINEL, timeout=5_000_000, count=200_000)
    except UcError:
        pass

    r0_ret = mu.reg_read(UC_ARM_REG_R0)
    print(f"\n  Вызов с frame1+frame2, SRAM[0x200002C8]=0:")
    print(f"  r0={r0_ret}, инструкций={executed[0]}")
    if trace:
        print(f"  Цепочка вызовов:")
        for t in trace[:10]: print(f"  {t}")

    # Что записалось в SRAM вокруг буфера вызова
    interesting = {k: v for k, v in sram_writes.items()
                   if (0x20000000 <= k < 0x20000060 or
                       0x200002A0 <= k < 0x20000300 or
                       0x20000C00 <= k < 0x20000D00 or
                       0x20001000 <= k < 0x20001200)}
    if interesting:
        print(f"  SRAM изменения:")
        prev_addr = -1
        for addr in sorted(interesting.keys()):
            if addr - prev_addr > 4 and prev_addr >= 0:
                print()
            print(f"    [0x{addr:08X}] = 0x{interesting[addr]:02X}", end="  ")
            prev_addr = addr
        print()

    return r0_ret, sram_writes

run_first_init(FRAME1, FRAME2_LIST[0])

# ---------------------------------------------------------------------------
# 2. Поиск правильной раскладки через перебор параметров
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 2. Перебор параметров FUN_0820630C ===")
print("Ищем r0/r1 которые проходят проверку нижних 12 бит (4KB-aligned)")
print(f"{'='*60}")

# Нужно: r0 & 0xFFF == 0 и r1 & 0xFFF == 0
# Возможные значения r0 (flash_start, 4KB aligned в диапазоне flash):
candidates_r0 = [a for a in range(0x08000000, 0x08400000, 0x1000)]
candidates_r1 = [s for s in range(0x00001000, 0x00400000, 0x1000)]

print(f"\n  Подбираем какое r0/r1 соответствует данным вызова...")
print(f"  Frame1 = {FRAME1.hex()}")
print(f"  Frame1[2:7] = {FRAME1[2:].hex()}")

# Для Frame1[2:7] = 14 F0 94 E7 29 7F:
# При n1=4: r8 = big_endian(Frame1[3:7]) = 0xF094E729
# При n1=1: r8 = Frame1[3] = 0xF0
# При n1=2: r8 = big_endian(Frame1[3:5]) = 0xF094
# При n1=3: r8 = big_endian(Frame1[3:6]) = 0xF094E7

# Может быть n1=1 (т.е. 1 байт для r8)?
# Тогда data[0]=0x14 → n1=4, n2=1 — это фиксировано.
# Попробуем другой байт как data[0]:

for offset in range(len(FRAME1)):
    b = FRAME1[offset]
    n1 = b & 0xF
    n2 = b >> 4
    if n1 > 4 or n2 > 4:
        continue
    # Вычислим r8 и r5 при этом смещении
    if offset + 1 + n1 > len(FRAME1) + len(FRAME2_LIST[0]):
        continue
    combined = FRAME1 + FRAME2_LIST[0]
    # r8 = big-endian из combined[offset+1..offset+n1]
    if offset + 1 + n1 > len(combined) or offset + 1 + n1 + n2 > len(combined):
        continue
    r8_bytes = combined[offset+1:offset+1+n1]
    r5_bytes = combined[offset+1+n1:offset+1+n1+n2]
    if len(r8_bytes) < n1 or len(r5_bytes) < n2:
        continue
    r8 = int.from_bytes(r8_bytes, 'big') if n1 > 0 else 0
    r5 = int.from_bytes(r5_bytes, 'big') if n2 > 0 else 0

    r8_aligned = (r8 & 0xFFF) == 0
    r5_aligned = (r5 & 0xFFF) == 0
    r8_flash = 0x08000000 <= r8 < 0x08400000
    r5_valid = 0 < r5 < 0x00400000

    marker = ""
    if r8_aligned and r5_aligned:
        marker = "  *** 4KB ALIGNED BOTH ***"
    elif r8_aligned:
        marker = "  * r8 aligned"

    print(f"  Смещение {offset:2d}: byte=0x{b:02X} n1={n1} n2={n2} "
          f"r8=0x{r8:08X}{'✓' if r8_aligned else '✗'}"
          f"{'flash' if r8_flash else ''} "
          f"r5=0x{r5:08X}{'✓' if r5_aligned else '✗'}{marker}")

# ---------------------------------------------------------------------------
# 3. Поиск в calibration area flash байтов с nibble ≤ 4
#    которые указывают на 4KB-aligned flash адреса
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 3. Поиск сессионных дескрипторов в Flash ===")
print("Ищем в flash байты 0xXY где X≤4, Y≤4, указывающие на 4KB-aligned адреса")
print(f"{'='*60}")

def find_session_descriptors(search_start=0x040000, search_end=0x090000):
    results = []
    for off in range(search_start, search_end - 6, 2):
        b = data[off]
        n1 = b & 0xF
        n2 = b >> 4
        if n1 > 4 or n2 > 4 or n1 == 0:
            continue
        # Читаем n1 байт (big-endian) как flash_start
        if off + 1 + n1 + n2 > len(data):
            continue
        flash_start_bytes = data[off+1:off+1+n1]
        flash_start = int.from_bytes(flash_start_bytes, 'big')
        # Читаем n2 байт как flash_size
        flash_size = 0
        if n2 > 0:
            flash_size_bytes = data[off+1+n1:off+1+n1+n2]
            flash_size = int.from_bytes(flash_size_bytes, 'big')

        # Проверяем: flash_start 4KB-aligned, в диапазоне flash
        if ((flash_start & 0xFFF) == 0 and
            0x08000000 <= flash_start < 0x08400000):
            # Дополнительно: flash_size 4KB-aligned (опционально)
            size_ok = (n2 == 0) or ((flash_size & 0xFFF) == 0 and 0 < flash_size <= 0x400000)
            results.append({
                'file_off': off,
                'chip_addr': BASE + off,
                'n1': n1, 'n2': n2,
                'flash_start': flash_start,
                'flash_size': flash_size,
                'size_ok': size_ok,
                'raw': data[off:off+6].hex()
            })
    return results

descs = find_session_descriptors()
print(f"\n  Найдено {len(descs)} кандидатов:")
for d in descs[:20]:
    smark = "✓size" if d['size_ok'] else ""
    print(f"  0x{d['chip_addr']:08X}: byte=0x{data[d['file_off']]:02X} "
          f"n1={d['n1']} n2={d['n2']} "
          f"start=0x{d['flash_start']:08X} "
          f"size=0x{d['flash_size']:08X} {smark}  [{d['raw']}]")

# ---------------------------------------------------------------------------
# 4. Проверка: считает ли FUN_082056D4 правильно для найденных дескрипторов
# ---------------------------------------------------------------------------
if descs:
    print(f"\n{'='*60}")
    print("=== 4. Проверка FUN_082056D4 для найденных flash-регионов ===")
    print(f"{'='*60}")

    for d in descs[:5]:
        fs = d['flash_start']
        fsize = d['flash_size']
        if fsize == 0:
            continue
        foff_start = fs - BASE
        if foff_start < 0 or foff_start + fsize > len(data):
            continue
        region = data[foff_start:foff_start+min(fsize, 0x1000)]
        all_ff = all(b == 0xFF for b in region)
        has_data = not all_ff
        print(f"  flash[0x{fs:08X}..+0x{fsize:X}]: "
              f"{'PROGRAMMED' if has_data else 'ERASED'} "
              f"first_byte=0x{region[0]:02X}")

# ---------------------------------------------------------------------------
# 5. Поиск Frame1 структуры — что кодирует Frame1[0:2] = 26 17
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== 5. Структура Frame1 ===")
print(f"{'='*60}")
print(f"Frame1 = {FRAME1.hex()}")
print(f"  Byte[0] = 0x26 = {FRAME1[0]} = 38 decimal")
print(f"  Byte[1] = 0x17 = {FRAME1[1]} = 23 decimal")
print(f"  Byte[0:2] как uint16 BE: 0x{struct.unpack('>H', FRAME1[0:2])[0]:04X} = {struct.unpack('>H', FRAME1[0:2])[0]}")
print(f"  Byte[0:2] как uint16 LE: 0x{struct.unpack('<H', FRAME1[0:2])[0]:04X}")
print(f"  Byte[2:6] как uint32 BE: 0x{struct.unpack('>I', FRAME1[2:6])[0]:08X}")
print(f"  Byte[2:6] как uint32 LE: 0x{struct.unpack('<I', FRAME1[2:6])[0]:08X}")
print(f"  Byte[2:7] hex: {FRAME1[2:].hex()}")

# Гипотеза: 0x26 = длина данных = 38 байт?
# 0x17 = что-то?

# Поиск 0x2617 в flash (как тип/команда?)
pat = FRAME1[0:2]
hits = [i for i in range(len(data)-1) if data[i:i+2] == pat]
print(f"\n  Паттерн {pat.hex()} в flash: {len(hits)} совпадений:")
for h in hits[:5]:
    ctx = data[max(0,h-4):h+10]
    print(f"    file 0x{h:06X}: {ctx.hex()}")

# Поиск полного Frame1 в flash
hits_full = [i for i in range(len(data)-7) if data[i:i+8] == FRAME1]
print(f"\n  Полный Frame1 в flash: {len(hits_full)} совпадений:")
for h in hits_full[:5]:
    print(f"    file 0x{h:06X} chip 0x{BASE+h:08X}")

print(f"\n{'='*60}")
print("=== Итог ===")
print(f"{'='*60}")
print(f"""
ВАЖНЫЕ ВЫВОДЫ:
1. Frame1 = 26 17 14 F0 94 E7 29 7F (константа для всех сессий rusEFI)
2. Frame1[2] = 0x14: n1=4, n2=1 — ЕДИНСТВЕННЫЙ байт с nibbles ≤ 4
3. При Гипотезе A (data=Frame1[2:7]) FUN_0820630C вызывается с:
   r0=0xF094E729 (НЕ 4KB-aligned → ошибка)
   r1=0x0000007F
4. FUN_0820630C проверяет: r0 & 0xFFF == 0 (нужен 4KB-aligned адрес)

СЛЕДУЮЩИЕ ШАГИ:
A) Поставить оригинальный ЭБУ, снять PCAN с ответами для Frame1=2617...
   (BCM пошлёт те же вызовы, оригинальный ЭБУ даст ответы)

B) Или найти в flash дескриптор сессии где flash_start 4KB-aligned:
   файл output выше показывает кандидатов из поиска по flash.

C) Или запустить FUN_08204B54 (первичная инициализация) и посмотреть
   что она записывает в SRAM[0x20000C14..] до FUN_08201E2C.
""")
