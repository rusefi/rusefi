#!/usr/bin/env python3
"""
immo_slib_tcl.py - OpenOCD TCL RPC - IMMO wrapper breakpoints
--------------------------------------------------------------
Версия 2: BP поставлены в IMMO wrapper (0x082xxxxx) - они точно
вызываются при запуске ECU. Для запуска IMMO нужен 0x0350 от BCM.

ВАЖНО: запустить параллельно immo_respond.py (или PCAN) для
симуляции BCM с кадром 0x0350 каждые 100 мс:
  ID=0x0350 DLC=8 Data=C3 00 00 00 04 14 94 05

OpenOCD серверы:
  :4444 - telnet (prompt-based)
  :6666 - TCL RPC (cmd+0x1a -> result+0x1a)  <- этот скрипт

Использование:
  python3 immo_slib_tcl.py             # запустить OpenOCD + подключиться
  python3 immo_slib_tcl.py --attach    # только подключиться
"""

import subprocess, socket, time, sys, re, threading
from pathlib import Path
from datetime import datetime

# ═══ Конфигурация ════════════════════════════════════════════════════════════

HOME        = str(Path.home())
OPENOCD_BIN = f'{HOME}/openocd'
CFG_IFACE   = f'{HOME}/tool-openocd-at32/scripts/interface/stlink-dap.cfg'
CFG_TARGET  = f'{HOME}/tool-openocd-at32/scripts/target/at32f435xM.cfg'
LOGFILE     = f'{HOME}/immo_slib_tcl.log'

TCL_PORT    = 6666

# ─────────────────────────────────────────────────────────────────────────────
# Breakpoints — IMMO wrapper (0x082xxxxx), подтверждено анализом
#
# Почему НЕ 0x08069028 / 0x080697D0:
#   Они не вызываются в нормальном режиме без BCM (скорее всего OBD/диагностика).
#   90 секунд ожидания — ни одного срабатывания (см. лог).
#
# Новые BP — функции которые точно вызываются:
#   0x082027A4  FUN_082027A4  — вычислитель IMMO-ответа (главная точка)
#   0x08201E2C  FUN_08201E2C  — IMMO crypto handler (зарегистрирован в SRAM-таблице)
#   0x08202038  FUN_08202038  — IMMO init (выполняется при первом запуске)
#   0x0820743A  CAN TX wrapper — функция в wrapper'е, использующая CAN1 base
#               (вероятно посылает 0x0713 / другие IMMO-кадры)
#
# Порядок: сначала 0x08202038 (init), потом 0x0820743A (TX),
#          потом 0x082027A4 (ответ на 0x0714), потом 0x08201E2C (крипто).
# ─────────────────────────────────────────────────────────────────────────────
BREAKPOINTS = [
    (0x08202038, 'IMMO init (FUN_08202038) — первый запуск, init SRAM-таблицы'),
    (0x0820743A, 'CAN TX wrapper — вероятно посылает 0x0713 триггер'),
    (0x082027A4, 'IMMO вычислитель ответа (FUN_082027A4) — при получении 0x0714'),
    (0x08201E2C, 'IMMO crypto handler (FUN_08201E2C) — сам расчёт'),
]

REGS = ['pc', 'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'lr']

# ═══ Логгер ══════════════════════════════════════════════════════════════════

_lf = open(LOGFILE, 'w', buffering=1)

def log(msg: str, c: str = '') -> None:
    ts   = datetime.now().strftime('%H:%M:%S.%f')[:-3]
    line = f'[{ts}] {msg}'
    clr  = {'g': '\033[92m', 'y': '\033[93m', 'r': '\033[91m',
             'c': '\033[96m', 'b': '\033[94m'}
    if c and sys.stdout.isatty():
        print(f"{clr.get(c,'')}{line}\033[0m")
    else:
        print(line)
    _lf.write(line + '\n')

# ═══ OpenOCD TCL RPC ═════════════════════════════════════════════════════════

class OpenOCDTCL:
    """
    OpenOCD TCL RPC (порт 6666).
    Протокол: bytes(cmd) + 0x1a  →  bytes(result) + 0x1a
    """
    TERM = b'\x1a'

    def __init__(self, host='localhost', port=TCL_PORT):
        self._addr = (host, port)
        self._sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    def connect(self, max_wait=25.0) -> bool:
        log(f'TCL RPC: подключаюсь к {self._addr}...')
        deadline = time.time() + max_wait
        n = 0
        while time.time() < deadline:
            n += 1
            try:
                self._sock.connect(self._addr)
                log('TCL RPC: подключено!', 'g')
                return True
            except (ConnectionRefusedError, OSError):
                if n % 6 == 0:
                    log(f'  жду OpenOCD... ({int(deadline-time.time())} с)')
                time.sleep(0.4)
        log('TCL RPC: не удалось подключиться', 'r')
        return False

    def run(self, cmd: str, timeout=10.0) -> str:
        log(f'  >> {cmd}')
        self._sock.settimeout(timeout)
        self._sock.sendall(cmd.encode() + self.TERM)
        buf = b''
        deadline = time.time() + timeout
        while time.time() < deadline:
            try:
                chunk = self._sock.recv(4096)
                if not chunk:
                    break
                buf += chunk
                if buf.endswith(self.TERM):
                    break
            except socket.timeout:
                break
        result = buf.rstrip(self.TERM).decode('utf-8', errors='replace').strip()
        if result:
            log(f'     {result}')
        return result

    def halt(self):    return self.run('halt')
    def resume(self):  return self.run('resume')

    def reset_run(self):
        # reset run может давать "already halted" — игнорируем
        return self.run('reset run', timeout=5)

    def wait_halt(self, timeout_sec=90.0) -> bool:
        ms = int(timeout_sec * 1000)
        log(f'  Жду останова до {int(timeout_sec)} с...', 'y')
        result = self.run(f'wait_halt {ms}', timeout=timeout_sec + 5)
        return 'timed out' not in result.lower()

    def reg(self, name: str):
        out = self.run(f'reg {name}', timeout=5)
        m = re.search(r'0x([0-9a-fA-F]+)', out)
        return int(m.group(1), 16) if m else None

    def mdb(self, addr: int, count: int) -> str:
        return self.run(f'mdb 0x{addr:08X} {count}', timeout=10)

    def mww(self, addr: int, val: int) -> str:
        return self.run(f'mww 0x{addr:08X} 0x{val:08X}', timeout=5)

    def bp_set(self, addr: int) -> str:
        return self.run(f'bp 0x{addr:08X} 2 hw', timeout=5)

    def bp_clear(self, addr: int) -> str:
        return self.run(f'rbp 0x{addr:08X}', timeout=5)

    def bp_clear_all(self) -> str:
        return self.run('rbp all', timeout=5)

    def pc(self) -> int:
        return self.reg('pc') or 0

    def close(self):
        try: self._sock.close()
        except: pass

# ═══ Основная логика ═════════════════════════════════════════════════════════

def read_regs(ocd: OpenOCDTCL) -> dict:
    regs = {}
    for r in REGS:
        v = ocd.reg(r)
        regs[r] = v
        log(f'  {r:3s} = {"0x%08X" % v if v is not None else "?"}')
    return regs


def dump_sram_args(ocd: OpenOCDTCL, regs: dict) -> None:
    """Дампит SRAM-адреса из R0..R4 (вероятные буферы аргументов)."""
    found = False
    for r in ['r0', 'r1', 'r2', 'r3', 'r4']:
        v = regs.get(r) or 0
        if 0x20000000 <= v < 0x20020000:
            log(f'\n  {r.upper()} = 0x{v:08X} -> SRAM, читаю 16 байт:', 'c')
            log(f'    {ocd.mdb(v, 16)}')
            found = True
    if not found:
        log('  (R0..R4 не указывают на SRAM)')


def dump_key_sram(ocd: OpenOCDTCL) -> None:
    """Дамп ключевых SRAM-адресов для диагностики IMMO."""
    pairs = [
        (0x200002C8, 1,  'init_flag (0=первый раз, 1=инициализирован)'),
        (0x200002CA, 1,  'init_complete (1=FUN_08202038 завершил)'),
        (0x200003FE, 1,  'ready_flag (должен быть 1)'),
        (0x200003FC, 1,  'table_index'),
        (0x20000402, 2,  'session_count'),
        (0x20000414, 8,  'challenge_buffer [key_type, session, data...]'),
        (0x20001ADC, 4,  'SLib_key_constant (0 или 0x2548A4D2)'),
        (0x200010A0, 4,  'fn_ptr[1]+0x34 (должен быть 0x08201E2D)'),
    ]
    log('\n  Ключевые SRAM-адреса:')
    for addr, n, desc in pairs:
        cmd = 'mdb' if n <= 4 else 'mdb'
        out = ocd.mdb(addr, n)
        log(f'  0x{addr:08X} ({desc}): {out}')


def on_halt(ocd: OpenOCDTCL, pc: int, hit: int,
            dynamic_ret_bps: set) -> None:
    # Определить BP
    bp_desc  = None
    is_known = False
    for addr, desc in BREAKPOINTS:
        if pc == addr:
            bp_desc  = desc
            is_known = True
            break

    if pc in dynamic_ret_bps:
        bp_desc = f'AUTO-RETURN BP (установлен после предыдущего CALL)'
        dynamic_ret_bps.discard(pc)
        ocd.bp_clear(pc)
        is_known = True

    if not is_known:
        log(f'  PC=0x{pc:08X} — не наш BP (reset-halt?), продолжаю...', 'y')
        ocd.resume()
        return

    log(f'\n{"="*64}', 'y')
    log(f'  СТОП #{hit}  PC = 0x{pc:08X}', 'y')
    log(f'  {bp_desc}', 'y')
    log(f'{"="*64}', 'y')

    log('\n  Регистры:')
    regs = read_regs(ocd)

    dump_sram_args(ocd, regs)
    dump_key_sram(ocd)

    # Для ключевых точек — ставим return-BP автоматически
    if pc in {0x082027A4, 0x08201E2C, 0x0820743A}:
        # Узнать размер инструкции по PC и прочитать следующий адрес
        # (выходим через LR — можно поставить BP на LR)
        lr = regs.get('lr', 0) or 0
        if lr and 0x08000000 <= lr < 0x08300000:
            log(f'\n  Авто-BP на возврат по LR = 0x{lr:08X}')
            # lr Thumb = нечётный, выравниваем
            ret_addr = lr & ~1
            ocd.bp_set(ret_addr)
            dynamic_ret_bps.add(ret_addr)

    log('\n  Продолжаю...', 'g')
    ocd.resume()


def start_openocd() -> subprocess.Popen:
    log(f'Запуск OpenOCD: {OPENOCD_BIN}')
    proc = subprocess.Popen(
        [OPENOCD_BIN, '-f', CFG_IFACE, '-f', CFG_TARGET],
        stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
        cwd=HOME,
    )
    log('Жду "Listening on port 6666"...')
    t0 = time.time()
    while time.time() - t0 < 20:
        raw = proc.stdout.readline()
        if not raw:
            break
        line = raw.decode('utf-8', errors='replace').rstrip()
        log(f'  ocd| {line}')
        if 'Listening on port' in line and '6666' in line:
            log('OpenOCD готов!', 'g')
            break
        if proc.poll() is not None:
            log('OpenOCD завершился!', 'r')
            sys.exit(1)

    def _drain():
        for raw in proc.stdout:
            log(f'  ocd| {raw.decode("utf-8",errors="replace").rstrip()}')
    threading.Thread(target=_drain, daemon=True).start()
    return proc


def main():
    attach_only = '--attach' in sys.argv

    log('=' * 64)
    log('  IMMO Wrapper Breakpoint Trap  (OpenOCD TCL RPC v2)')
    log(f'  Лог: {LOGFILE}')
    log('=' * 64)

    log("""
ТРЕБОВАНИЯ ДЛЯ ЗАПУСКА:
  1. ECU подключён (питание + CAN + SWD)
  2. PCAN посылает 0x0350 каждые 100 мс:
       ID=0x0350  DLC=8  Data: C3 00 00 00 04 14 94 05
     Без этого кадра ECU не запустит IMMO-последовательность!
  3. Или запустить параллельно:  python3 immo_respond.py
""", 'y')

    proc = None
    if not attach_only:
        proc = start_openocd()
        time.sleep(0.3)

    ocd = OpenOCDTCL()
    if not ocd.connect(max_wait=25):
        if proc: proc.terminate()
        sys.exit(1)

    # ── Инициализация ────────────────────────────────────────────────────────
    log('\n─── Инициализация ────────────────────────────────────', 'c')
    ocd.halt()
    ocd.bp_clear_all()
    ocd.mww(0xE0042008, 0x00001800)   # freeze AT32 watchdog
    ocd.mww(0xE000ED94, 0x00000000)   # disable MPU (на случай проблем)

    # Проверяем состояние чипа
    pc_init = ocd.pc()
    log(f'PC после halt: 0x{pc_init:08X}')

    # ── Breakpoints ──────────────────────────────────────────────────────────
    log('\n─── Breakpoints ──────────────────────────────────────', 'c')
    for addr, desc in BREAKPOINTS:
        r = ocd.bp_set(addr)
        log(f'  0x{addr:08X}  {desc}')

    # Верификация
    log('\n─── Активные BP ──────────────────────────────────────', 'c')
    ocd.run('bp')

    # ── Сброс и запуск ───────────────────────────────────────────────────────
    log('\n─── Reset + Run ──────────────────────────────────────', 'c')
    ocd.reset_run()
    time.sleep(0.1)
    log('ECU запущен!', 'g')
    log('Ожидаю BP... (нужен 0x0350 от BCM/PCAN для запуска IMMO)', 'y')

    # ── Главный цикл ─────────────────────────────────────────────────────────
    hit             = 0
    dynamic_ret_bps = set()

    while True:
        halted = ocd.wait_halt(timeout_sec=90)

        if not halted:
            log('\n90 с истекло без останова.', 'r')
            log('Проверь: ECU включён? PCAN шлёт 0x0350?')
            log('Пробую продолжить ожидание...')
            # Не завершаемся — продолжаем ждать
            continue

        pc = ocd.pc()
        if pc == 0:
            log('Не удалось прочитать PC, продолжаю...', 'r')
            ocd.resume()
            continue

        hit += 1
        on_halt(ocd, pc, hit, dynamic_ret_bps)

        if hit >= 50:
            log('\n50 остановов — завершаю.', 'y')
            break

    log(f'\nВсего остановов: {hit}')
    log(f'Лог: {LOGFILE}')
    ocd.close()
    if proc: proc.terminate()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        log('\nПрерывание (Ctrl+C).')
        sys.exit(0)
