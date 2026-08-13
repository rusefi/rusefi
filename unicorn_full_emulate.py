#!/usr/bin/env python3
"""
unicorn_full_emulate.py  –  полная эмуляция IMMO-ответа на реальные вызовы от BCM

Новые данные с машины (rusEFI):
  Frame1 (константа): 26 17 14 F0 94 E7 29 7F
  Frame2 (случайная): 10 пар

Стратегия:
  1. Эмулируем CAN-приёмник FUN_08206FB8 с синтетическим 0x0714 кадром,
     чтобы понять КАК байты вызова попадают в SRAM.
  2. Затем запускаем вычислительную цепочку и захватываем ответ.
"""

import struct, sys
from pathlib import Path

try:
    from capstone import *
    from capstone.arm import *
    CAPSTONE = True
except ImportError:
    CAPSTONE = False

try:
    from unicorn import *
    from unicorn.arm_const import *
    UNICORN = True
except ImportError:
    UNICORN = False
    print("unicorn not installed"); sys.exit(1)

FIRMWARE = Path("Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin")
data = FIRMWARE.read_bytes()
BASE = 0x08000000

def foff(a): return a - BASE
def u32(o):  return struct.unpack_from('<I', data, o)[0]
def u16(o):  return struct.unpack_from('<H', data, o)[0]

# ---------------------------------------------------------------------------
# Реальные вызовы от BCM (10 сессий с машины)
# ---------------------------------------------------------------------------
FRAME1 = bytes.fromhex("261714F094E7297F")  # константная первая рамка

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

# Полные 16-байтные вызовы
CHALLENGES = [FRAME1 + f2 for f2 in FRAME2_LIST]

# Существующие 5 пар (оригинальный ЭБУ) — для перекрёстной проверки
ORIG_PAIRS = [
    (bytes.fromhex('660be1e2a34b8140b45633a0499a01ec'), bytes.fromhex('9cb7f8ca31431bb6')),
    (bytes.fromhex('cfcfbbf3cdc0f75ce9efe2eb23b62a25'), bytes.fromhex('bff99205ed4ab7a8')),
    (bytes.fromhex('cd4c9070196bbdebb425cb7c4350082c'), bytes.fromhex('96739be6b1299f77')),
    (bytes.fromhex('4a4f2a204fad58fd273622b70c2b559e'), bytes.fromhex('fda32d94ae77c121')),
    (bytes.fromhex('66aaeef37037dee0cdeefb22bde96807'), bytes.fromhex('efaa66f0caa6f0cd')),
]

# ---------------------------------------------------------------------------
# Анализ байтов с точки зрения nibble-check в FUN_08201E2C
# ---------------------------------------------------------------------------
print("=" * 60)
print("=== Анализ новых вызовов ===")
print("=" * 60)
print(f"Frame1 (const): {FRAME1.hex()}")
print(f"Frame1[2]: 0x{FRAME1[2]:02X} → lo={FRAME1[2]&0xF}, hi={FRAME1[2]>>4}")

def valid_nibbles(b): return (b & 0xF) <= 4 and (b >> 4) <= 4

print(f"\nБайты Frame1 с допустимыми nibbles (≤4):")
for i, b in enumerate(FRAME1):
    if valid_nibbles(b):
        print(f"  Frame1[{i}] = 0x{b:02X} → lo={b&0xF}, hi={b>>4}")

print(f"\nДля Frame2 (сессия 1) {FRAME2_LIST[0].hex()}:")
for i, b in enumerate(FRAME2_LIST[0]):
    if valid_nibbles(b):
        print(f"  Frame2[{i}] = 0x{b:02X}")

# Ключевое наблюдение: Frame1[2] = 0x14 → lo=4, hi=1 - ОБА ≤ 4!
b = FRAME1[2]
print(f"\nFrame1[2] = 0x{b:02X}: n1={b&0xF}, n2={b>>4} — ОБА ≤ 4!")
print(f"  Если data[0]=0x14: 4 байта ключа A + 1 байт ключа B")
print(f"  data[1..4] = {FRAME1[3:7].hex()} → r8 = 0x{int.from_bytes(FRAME1[3:7],'big'):08X}")
print(f"  data[5]    = 0x{FRAME1[7]:02X}            → r5 = 0x{FRAME1[7]:08X}")
print(f"  r8 & 0xFFF = 0x{int.from_bytes(FRAME1[3:7],'big') & 0xFFF:03X} (должен быть 0 для flash-адреса)")

# ---------------------------------------------------------------------------
# Попытка 1: CAN FMI-диспетчер с синтетическим 0x0714 кадром
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== Попытка 1: Эмуляция CAN-приёмника (FUN_08206FB8) ===")
print(f"{'='*60}")

FLASH_BASE = 0x08000000
FLASH_SIZE = 0x00400000
SRAM_BASE  = 0x20000000
SRAM_SIZE  = 0x00020000
SENTINEL   = 0x10000000

# CAN1 base (STM32F4): 0x40006400
CAN_BASE   = 0x40006400
PERIPH_BASE= 0x40000000
PERIPH_SIZE= 0x10000000

def make_uc():
    mu = Uc(UC_ARCH_ARM, UC_MODE_THUMB)
    mu.mem_map(FLASH_BASE, FLASH_SIZE)
    mu.mem_write(FLASH_BASE, b'\x00\xBE' * (FLASH_SIZE // 2))
    mu.mem_write(FLASH_BASE, data[:min(len(data), FLASH_SIZE)])
    mu.mem_map(SRAM_BASE, SRAM_SIZE)
    mu.mem_write(SRAM_BASE, b'\x00' * SRAM_SIZE)
    mu.mem_map(SENTINEL, 0x1000)
    mu.mem_write(SENTINEL, b'\xFF\xBE' * 0x800)
    # Периферия (заглушки - нужна для CAN, Flash и т.д.)
    mu.mem_map(PERIPH_BASE, PERIPH_SIZE)
    mu.mem_write(PERIPH_BASE, b'\x00' * PERIPH_SIZE)
    SP = (SRAM_BASE + SRAM_SIZE - 0x100) & ~7
    mu.reg_write(UC_ARM_REG_SP, SP)
    mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)
    return mu

def emulate_can_rx(challenge16):
    """
    Эмулируем приём 0x0714 CAN-кадра через FUN_08206FB8.
    CAN RX FIFO0 mailbox настраиваем с нашими данными.
    """
    mu = make_uc()

    # Настройка CAN1 mailbox 0 (0x40006400 + 0x1B0):
    # RI0R (CAN_RF0R):  CAN ID 0x0714 << 21 = 0xE2800000 (standard frame)
    # Но функция читает RF0R[1:0] = FIFO count, и [1:0] != 0 для "message pending"
    # Нам нужно: CAN_RF0R bits[1:0] != 0 (pending message in FIFO0)

    # CAN_RF0R (at 0x40006400 + 0x0C): bits[1:0] = pending count
    mu.mem_write(CAN_BASE + 0x0C, struct.pack('<I', 0x00000001))  # 1 message pending

    # CAN_RI0R (at 0x40006400 + 0x1B0): standard ID 0x0714 in bits[31:21]
    can_id_reg = 0x0714 << 21
    mu.mem_write(CAN_BASE + 0x1B0, struct.pack('<I', can_id_reg))

    # CAN_RDT0R (at 0x40006400 + 0x1B4): DLC=8, FMI=??
    # FMI (filter match index) в bits[15:8]
    # FUN_08206FB8 читает: ubfx r3, r3, #8, #8 → FMI
    # Нам нужно подобрать FMI чтобы диспетчер нашёл handler для 0x0714
    # Попробуем несколько значений FMI
    fmi = 1  # первый кандидат
    mu.mem_write(CAN_BASE + 0x1B4, struct.pack('<I', (fmi << 8) | 8))  # FMI|DLC

    # CAN_RDL0R (at 0x40006400 + 0x1B8): первые 4 байта данных (LE)
    mu.mem_write(CAN_BASE + 0x1B8, struct.pack('<BBBB', *challenge16[0:4]))

    # CAN_RDH0R (at 0x40006400 + 0x1BC): байты 4-7
    mu.mem_write(CAN_BASE + 0x1BC, struct.pack('<BBBB', *challenge16[4:8]))

    # Настройка таблицы CAN-диспетчера 0x08209834 (должна существовать в flash)
    # Из анализа: DAT_082097D0 = 0x082097D0 (таблица адресов CAN периферии)
    # Пишем адрес CAN1 (0x40006400) в позицию channel 0:
    table_97D0 = foff(0x082097D0)
    if table_97D0 >= 0:
        mu.mem_write(0x082097D0, struct.pack('<I', CAN_BASE))

    # Отслеживаем записи в SRAM диапазон буфера вызова
    sram_writes = {}
    def hook_mem(mu, access, address, size, value, ud):
        v = value & ((1 << (size*8)) - 1)
        if SRAM_BASE <= address < SRAM_BASE + SRAM_SIZE:
            # Записываем в байтах
            for i in range(size):
                sram_writes[address + i] = (v >> (i*8)) & 0xFF

    executed = [0]
    def hook_code(mu, addr, sz, ud):
        executed[0] += 1
        if addr == SENTINEL: mu.emu_stop()
        if executed[0] > 200000: mu.emu_stop()

    def hook_inv(mu, *a): mu.emu_stop(); return False

    mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem)
    mu.hook_add(UC_HOOK_CODE, hook_code)
    mu.hook_add(UC_HOOK_MEM_INVALID, hook_inv)

    try:
        mu.emu_start(0x08206FB8 | 1, SENTINEL, timeout=3_000_000, count=200_000)
    except UcError as e:
        pass

    # Показываем что записалось в ключевых областях
    relevant = {k: v for k, v in sram_writes.items()
                if 0x20000C00 <= k < 0x20000D00}
    return relevant, sram_writes

print(f"\n  Запуск с Frame1+Frame2 сессии 1...")
challenge1 = CHALLENGES[0]
writes, all_writes = emulate_can_rx(challenge1)

if writes:
    print(f"  Записи в буфер вызова (0x20000C00-0x20000CFF):")
    for addr in sorted(writes.keys()):
        print(f"    [0x{addr:08X}] = 0x{writes[addr]:02X}")
else:
    print(f"  Нет записей в буфер (диспетчер не нашёл handler для FMI)")
    print(f"  Всего записей в SRAM: {len(all_writes)}")

# ---------------------------------------------------------------------------
# Попытка 2: Прямой вызов вычислительной функции с разными раскладками SRAM
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== Попытка 2: Прямой вызов FUN_08201E2C с разными раскладками ===")
print(f"{'='*60}")

# Известные параметры из анализа:
# FUN_08201E2C вызывается с:
#   r0 = key_type (должен быть 1)
#   r1 = session_word (должен быть 0xFF00 или 0xFF01)
#   r2 = data_ptr + 4 = 0x20000C18 (начало данных вызова)
#   r3 = count - 4

# Для прохождения nibble-check нужен data[0] с оба nibble ≤ 4.
# Frame1[2] = 0x14 → ОБА ≤ 4!

# Раскладка SRAM (Гипотеза A):
# [0x20000C14] = [0x0A, 0x01, 0xFF, 0x00]  (count=10, key_type=1, sess=0xFF00)
# [0x20000C18..0x20000C1D] = Frame1[2:7] = 14 F0 94 E7 29 7F
# count-4 = 6, n1+n2+1 = 4+1+1 = 6 → LENGTH CHECK PASSES

# Раскладка B: data = Frame1[2:] + часть Frame2
# [0x20000C18..0x20000C25] = Frame1[2:7] + Frame2[0:7] (14 байт)
# count = 14+4 = 18 = 0x12

def run_immo_compute(sram_header, sram_data, label, frame2=None):
    """
    Запуск FUN_08201E2C с заданной раскладкой SRAM.
    Захватываем что вычисляется.
    """
    mu = make_uc()

    # Базовые флаги
    mu.mem_write(0x200002C8, bytes([1]))     # уже инициализировано
    mu.mem_write(0x200003FE, bytes([1]))     # FUN_082037D8 вернёт 1
    mu.mem_write(0x200003FC, bytes([1]))     # table_index = 1
    mu.mem_write(0x200002CA, bytes([1]))     # init flag

    # Регистрируем крипто-обработчик в SRAM-таблице
    table_base = 0x20001018
    entry = table_base + 1 * 0x54
    mu.mem_write(entry + 0x34, struct.pack('<I', 0x08201E2D))  # Thumb addr

    # Пишем заголовок и данные буфера вызова
    mu.mem_write(0x20000C14, sram_header)
    mu.mem_write(0x20000C18, sram_data)

    # Стек с аргументами (sp+0=resp_buf, sp+4=0x3FC, sp+8=&out_len)
    SP = (SRAM_BASE + SRAM_SIZE - 0x200) & ~7
    out_len_ptr = SRAM_BASE + 0x8000
    mu.mem_write(out_len_ptr, b'\x00' * 4)
    SP -= 16
    mu.reg_write(UC_ARM_REG_SP, SP)
    mu.mem_write(SP,     struct.pack('<I', 0x20000C1C))  # resp_buf
    mu.mem_write(SP + 4, struct.pack('<I', 0x3FC))
    mu.mem_write(SP + 8, struct.pack('<I', out_len_ptr))

    count = sram_header[0]
    count_minus4 = count - 4

    # Аргументы для FUN_08201E2C:
    mu.reg_write(UC_ARM_REG_R0, 1)          # key_type = 1
    mu.reg_write(UC_ARM_REG_R1, 0xFF00)     # session_word
    mu.reg_write(UC_ARM_REG_R2, 0x20000C18) # data_ptr
    mu.reg_write(UC_ARM_REG_R3, count_minus4)

    resp_writes = {}
    sram_all = {}
    trace = []

    def hook_mem(mu, access, address, size, value, ud):
        v = value & ((1 << (size*8)) - 1)
        for i in range(size):
            sram_all[address + i] = (v >> (i*8)) & 0xFF
        if 0x20000C00 <= address < 0x20000D00 or 0x200002C0 <= address < 0x20000400:
            resp_writes[address] = (v, size, mu.reg_read(UC_ARM_REG_PC))

    executed = [0]
    def hook_code(mu, addr, sz, ud):
        executed[0] += 1
        if addr == SENTINEL: mu.emu_stop()
        if executed[0] > 500000: mu.emu_stop()
        # Логируем BL/BLX к известным функциям
        if addr in (0x0820630C, 0x08204B54, 0x08206108, 0x08202254, 0x082037D8):
            trace.append(f"  PC=0x{addr:08X} r0=0x{mu.reg_read(UC_ARM_REG_R0):08X} r1=0x{mu.reg_read(UC_ARM_REG_R1):08X}")

    def hook_inv(mu, *a): mu.emu_stop(); return False

    mu.hook_add(UC_HOOK_MEM_WRITE, hook_mem)
    mu.hook_add(UC_HOOK_CODE, hook_code)
    mu.hook_add(UC_HOOK_MEM_INVALID, hook_inv)

    try:
        mu.emu_start(0x08201E2C | 1, SENTINEL, timeout=5_000_000, count=500_000)
    except UcError as e:
        pass

    r0 = mu.reg_read(UC_ARM_REG_R0)
    resp = bytes(mu.mem_read(0x20000C18, 8))

    print(f"\n  [{label}]")
    print(f"    header: {sram_header.hex()}")
    print(f"    data:   {sram_data[:10].hex()}...")
    print(f"    r0={r0}, инструкций={executed[0]}")
    if trace:
        print(f"    Ключевые вызовы:")
        for t in trace[:5]: print(f"    {t}")
    print(f"    SRAM[0x20000C18:0x20000C20] = {resp.hex()}")

    # Показываем что записалось в буфер ответа
    resp_area = {k: v for k,v in resp_writes.items() if 0x20000C10 <= k < 0x20000D00}
    if resp_area:
        print(f"    Записи в буфер ответа:")
        for addr in sorted(resp_area.keys()):
            v, sz, pc = resp_area[addr]
            print(f"      [0x{addr:08X}] = 0x{v & 0xFF:02X}  (от PC=0x{pc:08X})")

    return r0, resp

challenge = CHALLENGES[0]  # Первый вызов

# Гипотеза A: data = Frame1[2:7] (6 байт), count=10
hdr_A = bytes([0x0A, 0x01, 0xFF, 0x00])
dat_A = FRAME1[2:] + b'\x00' * 20  # Frame1[2:7] = 14 F0 94 E7 29 7F
run_immo_compute(hdr_A, dat_A, "A: Frame1[2:7] как данные, count=10")

# Гипотеза B: data = Frame1 || Frame2 (16 байт), count=20
hdr_B = bytes([0x14, 0x01, 0xFF, 0x00])
dat_B = FRAME1 + FRAME2_LIST[0] + b'\x00' * 10
run_immo_compute(hdr_B, dat_B, "B: Frame1+Frame2 raw, count=20")

# Гипотеза C: data = Frame2 (8 байт), count=12
hdr_C = bytes([0x0C, 0x01, 0xFF, 0x00])
dat_C = FRAME2_LIST[0] + b'\x00' * 20
run_immo_compute(hdr_C, dat_C, "C: только Frame2, count=12")

# Гипотеза D: data = Frame1[2:] + Frame2 (14 байт), count=18
hdr_D = bytes([0x12, 0x01, 0xFF, 0x00])
dat_D = FRAME1[2:] + FRAME2_LIST[0] + b'\x00' * 10
run_immo_compute(hdr_D, dat_D, "D: Frame1[2:]+Frame2, count=18")

# Гипотеза E: session=0xFF01 (второй handler)
hdr_E = bytes([0x0A, 0x01, 0xFF, 0x01])
dat_E = FRAME1[2:] + b'\x00' * 20
run_immo_compute(hdr_E, dat_E, "E: Frame1[2:7], session=0xFF01")

# ---------------------------------------------------------------------------
# Попытка 3: Прямой вызов FUN_082027A4 (главный вычислитель)
# ---------------------------------------------------------------------------
print(f"\n{'='*60}")
print("=== Попытка 3: FUN_082027A4 с заполненной SRAM-таблицей ===")
print(f"='*60")

def run_compute_engine(sram_data_at_C14, sram_data_at_C18, label):
    mu = make_uc()

    # Флаги
    mu.mem_write(0x200002C8, bytes([1]))
    mu.mem_write(0x200003FE, bytes([1]))

    # SRAM-таблица: индекс 1, смещение +0x34
    table_base = 0x20001018
    entry = table_base + 1 * 0x54
    mu.mem_write(entry + 0x34, struct.pack('<I', 0x08201E2D))

    # Буфер вызова
    mu.mem_write(0x20000C14, sram_data_at_C14[:4])
    mu.mem_write(0x20000C18, sram_data_at_C18[:20])

    # Литеральные значения в flash:
    # В FUN_082027A4: ldr r2, [pc, #0x94] → 0x08202848 → 0x20000C14
    # и ldr r7, [pc, #0x64] → 0x08202854 → 0x20000C1C
    # Эти значения уже в бинарнике (читаем из flash)

    # Стек
    SP = (SRAM_BASE + SRAM_SIZE - 0x200) & ~7
    out_len_ptr = SRAM_BASE + 0x8000
    mu.mem_write(out_len_ptr, b'\x00' * 4)
    SP -= 0x40
    mu.reg_write(UC_ARM_REG_SP, SP)

    mu.reg_write(UC_ARM_REG_R0, 0)  # session handle
    mu.reg_write(UC_ARM_REG_LR, SENTINEL | 1)

    resp_writes = {}
    executed = [0]
    trace = []

    def hook_m(mu, access, address, size, value, ud):
        v = value & ((1 << (size*8)) - 1)
        if 0x20000C10 <= address < 0x20000D00:
            for i in range(size):
                resp_writes[address + i] = (v >> (i*8)) & 0xFF
    def hook_c(mu, addr, sz, ud):
        executed[0] += 1
        if addr == SENTINEL: mu.emu_stop()
        if executed[0] > 500000: mu.emu_stop()
        if addr in (0x0820630C, 0x08204B54, 0x08202254, 0x082027FA):
            trace.append(f"  0x{addr:08X}: r0=0x{mu.reg_read(UC_ARM_REG_R0):08X} r1=0x{mu.reg_read(UC_ARM_REG_R1):08X}")
    def hook_i(mu, *a): mu.emu_stop(); return False

    mu.hook_add(UC_HOOK_MEM_WRITE, hook_m)
    mu.hook_add(UC_HOOK_CODE, hook_c)
    mu.hook_add(UC_HOOK_MEM_INVALID, hook_i)

    try:
        mu.emu_start(0x082027A4 | 1, SENTINEL, timeout=5_000_000, count=500_000)
    except UcError as e:
        pass

    r0 = mu.reg_read(UC_ARM_REG_R0)
    print(f"\n  [{label}]")
    print(f"    r0={r0}, инструкций={executed[0]}")
    if trace:
        print(f"    Ключевые вызовы:")
        for t in trace[:5]: print(f"    {t}")
    result = bytes([resp_writes.get(0x20000C18+i, 0) for i in range(8)])
    print(f"    Результат @ 0x20000C18: {result.hex()}")
    return r0, result

# Гипотеза A через FUN_082027A4
run_compute_engine(
    bytes([0x0A, 0x01, 0xFF, 0x00]),  # header at 0x20000C14
    FRAME1[2:] + b'\x00' * 14,        # data at 0x20000C18
    "A через FUN_082027A4"
)

print("\n=== Done ===")
print("\nКлючевые новые данные для анализа:")
print(f"  Frame1 (постоянная): {FRAME1.hex()}")
print(f"  Frame1[2] = 0x14: n1=4, n2=1 — оба nibble ≤ 4!")
print(f"  Для получения ответов нужно понять что именно находится")
print(f"  в SRAM[0x20000C18] когда вызывается FUN_08201E2C.")
print(f"\n  Следующий шаг: поставить оригинальный ЭБУ и снять ответы")
print(f"  для тех же вызовов (Frame1=2617..., Frame2=B6E2..., ...)")
