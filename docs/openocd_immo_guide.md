# OpenOCD/SWD: Получение алгоритма IMMO через JTAG

## Что нужно

- AT32F435 (оригинальный чип в блоке m74_9)
- SWD-адаптер (ST-Link v2, CMSIS-DAP, J-Link или любой совместимый)
- OpenOCD
- Оригинальная прошивка уже залита в чип

## Архитектура SLib на AT32F435

```
flash[0x08000000-0x0804F897]  Bootloader + app code     (читаем)
flash[0x0804F898-0x0805FFFF]  SLib zone 1  65KB         (FF в дампе)
flash[0x080BF4F0-0x080FFFFB]  SLib zone 2  258KB        (FF)
flash[0x08100000-0x081FFFFF]  SLib MAIN    1024KB       (FF) ← АЛГОРИТМ
flash[0x08200000-0x0820000F]  SLib key header 16B       (читаем!)
flash[0x08201000-0x08209AF7]  IMMO wrapper  34KB        (читаем)
flash[0x08209AF8-0x083EFFFF]  SLib остальные 1519KB    (FF)
```

IMMO wrapper вызывает SLib через SRAM-указатели:
- `SRAM[0x200002A4]` → SLib IMMO compute function
- `BLX R3` @ `0x08201710` и `0x082017AA` (из wrapper)

---

## Шаг 1: Конфигурация OpenOCD

Создай файл `at32f435_swd.cfg`:
```tcl
source [find interface/stlink.cfg]      # или cmsis-dap.cfg / jlink.cfg
transport select hla_swd                # или swd

set WORKAREASIZE 0x4000

source [find target/at32f403a.cfg]
# Если нет at32f435: используем cortex_m с параметрами вручную:
# source [find target/stm32f4x.cfg]   # временный workaround

reset_config srst_only
```

Запуск:
```bash
openocd -f at32f435_swd.cfg
# В другом терминале:
telnet localhost 4444
```

---

## Шаг 2: Найти SLib entry point

С оригинальной прошивкой, до включения зажигания:

```tcl
# Подключиться и остановить
halt

# Поставить аппаратный breakpoint на BLX R3 @ 0x08201710
# (вызов SLib IMMO compute)
bp 0x08201710 2 hw

# Продолжить выполнение
resume

# *** Включить зажигание в машине ***
# Дождаться 0x0714 вызова от BCM (~1.5 сек)
# Breakpoint сработает:

# Читаем R3 = адрес SLib функции!
reg r3

# Читаем SRAM[0x200002A4] — должен быть тот же адрес
mdb 0x200002A4 4

# Запомни адрес (назовём его SLib_IMMO_ADDR)
# Удалить breakpoint
rbp 0x08201710
```

---

## Шаг 3: Захват пар challenge → response

Ставим breakpoint ПОСЛЕ вызова SLib (там уже есть ответ):

```tcl
# Breakpoint на инструкцию после BLX R3 @ 0x08201710
# (следующая инструкция = 0x08201712 или 0x08201714)
bp 0x08201712 2 hw
resume

# *** Когда breakpoint срабатывает: ***

# Читаем challenge из SRAM[0x20000C14..0x20000C27]
echo "=== Challenge buffer ==="
mdb 0x20000C14 20

# Читаем ответ
echo "=== Response (если уже есть) ==="
mdb 0x20000C18 8
mdb 0x20000C1C 8

# Читаем SRAM[0x200002A4..0x200002AC]
echo "=== SLib fn ptrs ==="
mdb 0x200002A0 16

# Продолжить
resume
```

Повтори 10 раз (выключать/включать зажигание), получишь пары:
```
challenge_frame1[8] || challenge_frame2[8] → response[8]
```

---

## Шаг 4: Прошить rusEFI, сохранив SLib

AT32F435 flash секторы (4KB = 0x1000 байт каждый):
```
Сектор N: 0x08000000 + N * 0x1000  (для N = 0..1023)
```

SLib секторы (НЕ ТРОГАТЬ):
```
Секторы 0x4F-0x5F:  0x0804F000-0x0805FFFF  (SLib 65KB)
Секторы 0x77-0x7F:  0x08077000-0x0807FFFF  (SLib 35KB)
Секторы 0xBF-0xFF:  0x080BF000-0x080FFFFF  (SLib 258KB)
Секторы 0x100-0x1FF: 0x08100000-0x081FFFFF (SLib 1MB ← ГЛАВНАЯ)
Секторы 0x200-0x22D: 0x08200000-0x0822DFFF (SLib + wrapper)
... и другие SLib зоны ...
```

### Стратегия прошивки rusEFI:

rusEFI m74_9 должен уместиться до `0x0804F000` (примерно 316KB = ок для движка).
Если не помещается — использовать диапазон 0x08060000-0x08076FFF тоже.

```tcl
# В OpenOCD:

# 1. Стираем только безопасные секторы (0x00 до 0x4E)
flash erase_sector 0 0 0x4E

# Если нужно больше места, стираем и 0x60-0x76:
# flash erase_sector 0 0x60 0x76

# Если нужно ещё — 0x80-0xBE:
# flash erase_sector 0 0x80 0xBE

# 2. Пишем rusEFI (НЕ трогая SLib секторы)
flash write_image rusEFI.bin 0x08000000

# 3. ВАЖНО: wrapper от 0x08201000-0x08209AFF должен остаться нетронутым
# rusEFI не должен достигать этого адреса!

# 4. Проверяем что SLib на месте
mdb 0x08200000 16
# Должны быть: 12 C2 A0 43 DD 7B 3C 50 C9 26 59 01 48 7A 33 2E

# 5. Сбрасываем
reset run
```

---

## Шаг 5: Реализация computeImmoResponse() в rusEFI

После нахождения SLib entry point (`SLib_IMMO_ADDR`):

```cpp
// В firmware/config/boards/m74_9/m74_9_can.cpp

// SLib IMMO function signature (определить из трассировки регистров)
typedef bool (*SLibImmoFn)(uint32_t flash_start, uint32_t param2, 
                            uint32_t session_mode, void* output_ptr);

// Адрес найден через OpenOCD breakpoint:
static constexpr uint32_t SLIB_IMMO_ADDR = 0x08100xxx;  // заполнить!

static bool computeImmoResponse(const uint8_t challenge[16], uint8_t response[8]) {
    // 1. Заполнить SRAM буфер вызова как ожидает wrapper
    // (layout определить из Unicorn/OpenOCD трассировки)
    uint8_t* sram_buf = (uint8_t*)0x20000C14;
    sram_buf[0] = 0x14;  // count
    sram_buf[1] = 0x01;  // key_type
    sram_buf[2] = 0xFF;  // session_hi
    sram_buf[3] = 0x00;  // session_lo
    memcpy(sram_buf + 4, challenge, 16);
    
    // 2. Установить флаги SRAM
    *(uint8_t*)0x200002C8 = 1;   // initialized
    *(uint8_t*)0x200003FE = 1;   // ready
    
    // 3. Вызвать SLib напрямую (если нашли entry point)
    // ИЛИ вызвать wrapper FUN_082027A4:
    typedef uint32_t (*ImmoCompute)(uint32_t session_handle);
    ImmoCompute compute = (ImmoCompute)(0x082027A4 | 1);  // Thumb
    compute(0);  // triggers async computation
    
    // 4. Ждать async ответа (он придёт через callback и CAN TX)
    // ... нужно адаптировать под rusEFI event loop ...
    
    return false;  // TODO: implement async wait
}
```

---

## Шаг 6: Альтернатива — прямой вызов SLib

Если через breakpoint получили `SLib_IMMO_ADDR` (адрес SLib функции):

```cpp
static bool computeImmoResponse(const uint8_t challenge[16], uint8_t response[8]) {
    // Тип функции определить из трассировки перед BLX R3:
    // r0 = ? r1 = ? r2 = ? при вызове
    
    typedef uint32_t (*SLibFn)(uint32_t r0, uint32_t r1, uint32_t r2, uint32_t r3);
    SLibFn slib_fn = (SLibFn)(SLIB_IMMO_ADDR | 1);
    
    // Параметры из breakpoint трассировки
    uint32_t result = slib_fn(
        *(uint32_t*)0x20000C14,   // r0 из трассировки
        *(uint32_t*)0x200002A4,   // r1
        0,                         // r2
        0                          // r3
    );
    
    if (result == 0) {  // success
        memcpy(response, (void*)0x20000C18, 8);
        return true;
    }
    return false;
}
```

---

## Сессия OpenOCD для полной трассировки

Сохранить как `capture_immo.tcl`:

```tcl
# Capture IMMO challenge/response pairs
proc capture_immo_session {} {
    halt
    
    # BP на BLX R3 (SLib call)
    bp 0x08201710 2 hw
    resume
    
    wait_halt 30000  ;# ждём до 30 сек
    
    set slib_addr [reg r3 -value]
    echo "SLib entry: 0x[format %08X $slib_addr]"
    
    # BP после return из SLib
    rbp 0x08201710
    bp 0x08201712 2 hw
    resume
    wait_halt 5000
    
    # Читаем данные
    echo "=== SRAM at 0x20000C14 ==="
    mem2array challenge 8 0x20000C14 16
    echo "Challenge: [format_hex $challenge]"
    
    mem2array response 8 0x20000C18 8
    echo "Response area: [format_hex $response]"
    
    rbp 0x08201712
}

capture_immo_session
```

---

## Краткий план действий

```
1. Подключить SWD к m74_9 (распиновка на разъёме отладчика)
2. Запустить openocd с конфигом AT32F435
3. Залить ОРИГИНАЛЬНУЮ прошивку (если ещё не залита)
4. Запустить сессию capture_immo.tcl
5. Включить зажигание → дождаться IMMO handshake
6. Получить: SLib_entry_addr + пары challenge/response
7. Реализовать computeImmoResponse() используя SLib_entry_addr
8. Прошить rusEFI (сохранив SLib секторы)
9. Завести машину!
```

---

## Важные замечания

- **Не делать full chip erase!** Это уничтожит SLib → алгоритм потерян навсегда
- **SLib на уровне AT32F435 привязана к конкретному чипу** — копировать нельзя
- **Если OpenOCD не видит чип**: проверь распиновку SWD (CLK, DIO, GND, VCC 3.3V)
- **AT32F435 SWD протокол**: стандартный ARM SWD, поддерживается любым DAP
- **Тактовая частота**: начни с 1MHz (`adapter speed 1000`), увеличь если стабильно
