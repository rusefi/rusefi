#!/usr/bin/env python3
"""
immo_slib_trap.py
-----------------
Автоматически:
  1. Запускает OpenOCD (или подключается к уже запущенному)
  2. Ставит HW-breakpoints на SLib-вызовы (генерация 0x0713 триггера)
  3. Делает reset+run
  4. При каждом останове читает регистры и дампит SRAM
  5. Пишет подробный лог в ~/immo_slib_trap.log

Использование:
  python3 immo_slib_trap.py              # запустить OpenOCD + подключиться
  python3 immo_slib_trap.py --attach     # подключиться к уже работающему OpenOCD

Запуск OpenOCD пользователем:
  ~/openocd -f ~/tool-openocd-at32/scripts/interface/stlink-dap.cfg \
             -f ~/tool-openocd-at32/scripts/target/at32f435xM.cfg
"""

import subprocess, socket, time, sys, re, threading
from datetime import datetime
from pathlib import Path

# ─── Конфигурация ────────────────────────────────────────────────────────────

HOME        = str(Path.home())
OPENOCD_BIN = f'{HOME}/openocd'
CFG_IFACE   = f'{HOME}/tool-openocd-at32/scripts/interface/stlink-dap.cfg'
CFG_TARGET  = f'{HOME}/tool-openocd-at32/scripts/target/at32f435xM.cfg'
LOGFILE     = f'{HOME}/immo_slib_trap.log'
TELNET_PORT = 4444

# Breakpoints: SLib-вызовы из основного firmware (найдены анализом дампа)
# Предполагаемые генераторы криптографического триггера 0x0713
BREAKPOINTS = [
    (0x08069028, 'CALL  -> SLib 0x081F102A  [триггер кандидат 1]'),
    (0x0806902C, 'RET   <- SLib 0x081F102A'),
    (0x080697D0, 'CALL  -> SLib 0x0819AFEE  [триггер кандидат 2]'),
    (0x080697D4, 'RET   <- SLib 0x0819AFEE'),
]

REGS = ['pc', 'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'lr']

# ─── Логгер ──────────────────────────────────────────────────────────────────

_log = open(LOGFILE, 'w', buffering=1)

def log(msg, color=None):
    ts  = datetime.now().strftime('%H:%M:%S.%f')[:-3]
    line = f'[{ts}] {msg}'
    colors = {'red': '\033[91m', 'green': '\033[92m',
              'yellow': '\033[93m', 'cyan': '\033[96m', 'reset': '\033[0m'}
    if color and sys.stdout.isatty():
        print(f"{colors.get(color,'')}{line}{colors['reset']}")
    else:
        print(line)
    _log.write(line + '\n')

# ─── OpenOCD Telnet клиент ────────────────────────────────────────────────────

class OCD:
    def __init__(self):
        self.s = None
        self._buf = b''

    # ── подключение ──────────────────────────────────────────────────────────
    def connect(self, max_wait=25):
        log(f'Подключаюсь к OpenOCD :{TELNET_PORT}...')
        deadline = time.time() + max_wait
        attempt  = 0
        while time.time() < deadline:
            attempt += 1
            try:
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.settimeout(1.0)
                s.connect(('127.0.0.1', TELNET_PORT))
                self.s = s
                time.sleep(0.4)
                self._raw_read(1.5)   # eat banner
                log('Подключено!', 'green')
                return True
            except (ConnectionRefusedError, OSError):
                if attempt % 5 == 0:
                    log(f'  Жду OpenOCD... ({int(deadline-time.time())} с осталось)')
                time.sleep(0.4)
        log('ОШИБКА: не удалось подключиться', 'red')
        return False

    # ── внутренние методы ────────────────────────────────────────────────────
    def _raw_read(self, timeout=0.5):
        """Читает всё доступное за timeout секунд."""
        buf = b''
        self.s.settimeout(timeout)
        try:
            while True:
                d = self.s.recv(4096)
                if not d:
                    break
                buf += d
        except socket.timeout:
            pass
        return buf

    def _read_until_prompt(self, timeout=12.0):
        """Читает до появления '> ' (конец ответа на команду)."""
        buf = b''
        self.s.settimeout(0.3)
        deadline = time.time() + timeout
        while time.time() < deadline:
            try:
                d = self.s.recv(4096)
                if d:
                    buf += d
                    if buf.rstrip(b' ').endswith(b'>'):
                        break
            except socket.timeout:
                pass
        return buf.decode('utf-8', errors='replace')

    # ── публичный API ────────────────────────────────────────────────────────
    def cmd(self, command, timeout=12, silent=False):
        """Отправляет команду, ждёт ответа, возвращает текст."""
        if not silent:
            log(f'  CMD: {command}')
        self.s.sendall((command + '\r\n').encode())
        raw = self._read_until_prompt(timeout)
        # Убрать эхо команды и trailing prompt
        lines = raw.replace('\r', '').split('\n')
        out = '\n'.join(l for l in lines
                        if l.strip() and l.strip() != '>' and command not in l)
        out = out.strip()
        if out and not silent:
            log(f'       -> {out}')
        return out

    def wait_for_halt(self, timeout=90):
        """
        Ждёт асинхронного сообщения 'target halted due to breakpoint'.
        Печатает всё что приходит от OpenOCD.
        Возвращает PC (int) или None по таймауту.
        """
        buf = b''
        self.s.settimeout(0.3)
        deadline = time.time() + timeout
        while time.time() < deadline:
            try:
                d = self.s.recv(4096)
                if d:
                    buf += d
                    text = buf.decode('utf-8', errors='replace')
                    # Печатаем строки по мере поступления
                    while '\n' in text:
                        line, text = text.split('\n', 1)
                        stripped = line.strip()
                        if stripped and stripped not in ('>', ''):
                            log(f'  OCD: {stripped}')
                    buf = text.encode('utf-8')
                    # Ищем останов по breakpoint
                    full = buf.decode('utf-8', errors='replace')
                    if 'halted due to breakpoint' in full:
                        m = re.search(r'pc:\s*(0x[0-9a-fA-F]+)', full, re.I)
                        if m:
                            pc = int(m.group(1), 16)
                            buf = b''
                            return pc
            except socket.timeout:
                remaining = int(deadline - time.time())
                if remaining % 10 == 0 and remaining > 0:
                    log(f'  ... ожидание ({remaining} с)', 'yellow')
        return None

# ─── Логика сессии ────────────────────────────────────────────────────────────

def setup_and_run(ocd):
    """Инициализация, установка BP, reset."""
    log('\n─── Инициализация ────────────────────────────────', 'cyan')
    ocd.cmd('halt')
    ocd.cmd('rbp all')

    # Заморозить watchdog AT32F435 во время halt
    ocd.cmd('mww 0xE0042008 0x00001800')

    # Установить breakpoints
    log('\n─── Breakpoints ──────────────────────────────────', 'cyan')
    for addr, desc in BREAKPOINTS:
        r = ocd.cmd(f'bp 0x{addr:08X} 2 hw', silent=True)
        log(f'  BP 0x{addr:08X} [{desc}]  -> {r.strip()}')

    # Проверить что BP приняты
    log('\n─── Список активных BP ───────────────────────────', 'cyan')
    ocd.cmd('bp')

    log('\n─── Reset + Run ──────────────────────────────────', 'cyan')
    ocd.cmd('reset run')
    log('ECU запущен. Жду breakpoint...\n', 'green')


def on_halt(ocd, pc, hit_num):
    """Обработка останова по breakpoint."""
    # Определить какой BP
    desc = 'неизвестный адрес'
    is_call   = False
    ret_addr  = None
    for addr, d in BREAKPOINTS:
        if pc == addr:
            desc      = d
            is_call   = 'CALL' in d
            ret_addr  = addr + 4   # BL занимает 4 байта (Thumb2)
            break

    log(f'\n{"="*60}', 'yellow')
    log(f'  BP #{hit_num} сработал!  PC = 0x{pc:08X}', 'yellow')
    log(f'  {desc}', 'yellow')
    log(f'{"="*60}', 'yellow')

    # ── Читаем регистры ──────────────────────────────────────────────────
    log('\n  Регистры:')
    reg_vals = {}
    for reg in REGS:
        out = ocd.cmd(f'reg {reg}', silent=True)
        m = re.search(r'0x([0-9a-fA-F]+)', out)
        if m:
            val = int(m.group(1), 16)
            reg_vals[reg] = val
            log(f'    {reg:3s} = 0x{val:08X}')
        else:
            log(f'    {reg:3s} = {out.strip()}')

    # ── SRAM-буферы ──────────────────────────────────────────────────────
    log('\n  SRAM-указатели в R0..R3:')
    buf_found = False
    for reg in ['r0', 'r1', 'r2', 'r3', 'r4']:
        v = reg_vals.get(reg, 0)
        if 0x20000000 <= v < 0x20020000:
            log(f'    {reg.upper()} = 0x{v:08X} -> SRAM, дамп 16 байт:', 'cyan')
            out = ocd.cmd(f'mdb 0x{v:08X} 16', silent=True)
            log(f'      {out.strip()}')
            buf_found = True

    if not buf_found:
        log('    (ни один регистр не указывает на SRAM)')

    # При RETURN — широкий дамп SRAM (ищем 8-байтный триггер)
    if 'RET' in desc:
        log('\n  Дамп SRAM 0x20000000 (256 байт) — ищем триггер:')
        out = ocd.cmd('mdb 0x20000000 0x100', silent=True)
        log(f'  {out}')

    # При CALL — ставим BP на возврат автоматически
    if is_call and ret_addr:
        log(f'\n  Установка BP на возврат @ 0x{ret_addr:08X}...')
        ocd.cmd(f'bp 0x{ret_addr:08X} 2 hw', silent=True)

    log('\n  Продолжаю...', 'green')
    ocd.cmd('resume')


# ─── Запуск OpenOCD ───────────────────────────────────────────────────────────

def start_openocd():
    log(f'Запускаю OpenOCD...')
    log(f'  {OPENOCD_BIN}')
    log(f'  -f {CFG_IFACE}')
    log(f'  -f {CFG_TARGET}')

    proc = subprocess.Popen(
        [OPENOCD_BIN, '-f', CFG_IFACE, '-f', CFG_TARGET],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=HOME,
    )

    # Ждём "Listening on port 4444"
    log('Жду готовности OpenOCD (строка "Listening on port 4444")...')
    ready = False
    t0 = time.time()
    while time.time() - t0 < 20:
        line_b = proc.stdout.readline()
        if not line_b:
            break
        line = line_b.decode('utf-8', errors='replace').rstrip()
        log(f'  ocd| {line}')
        if 'Listening on port' in line and '4444' in line:
            ready = True
            break
        if proc.poll() is not None:
            log('ОШИБКА: OpenOCD завершился!', 'red')
            sys.exit(1)

    if not ready:
        log('Не увидел "Listening on port 4444", продолжаю...')

    # Фоновое чтение stdout OpenOCD (чтобы не блокировался)
    def _drain():
        for raw in proc.stdout:
            log(f'  ocd| {raw.decode("utf-8",errors="replace").rstrip()}',
                color=None)   # только в файл

    threading.Thread(target=_drain, daemon=True).start()
    return proc


# ─── main ─────────────────────────────────────────────────────────────────────

def main():
    attach_only = '--attach' in sys.argv

    log('=' * 60)
    log('  IMMO SLib Breakpoint Trap')
    log(f'  Лог: {LOGFILE}')
    log('=' * 60)

    proc = None
    if not attach_only:
        proc = start_openocd()
        time.sleep(0.5)

    ocd = OCD()
    if not ocd.connect(max_wait=25):
        if proc:
            proc.terminate()
        sys.exit(1)

    setup_and_run(ocd)

    hit = 0
    while True:
        pc = ocd.wait_for_halt(timeout=120)
        if pc is None:
            log('\nТаймаут — breakpoint не сработал за 120 с.', 'red')
            log('Проверьте что ECU включён и SWD подключён.')
            break
        hit += 1
        on_halt(ocd, pc, hit)

    log(f'\nЗавершено. Всего остановов: {hit}')
    log(f'Лог записан: {LOGFILE}')
    if proc:
        proc.terminate()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        log('\nПрерывание (Ctrl+C).')
        sys.exit(0)
