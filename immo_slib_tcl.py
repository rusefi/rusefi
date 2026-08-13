#!/usr/bin/env python3
"""
immo_slib_tcl.py - OpenOCD TCL RPC client for IMMO SLib breakpoint trap
------------------------------------------------------------------------
OpenOCD opens two servers:
  :4444 - telnet  (text prompts)
  :6666 - TCL RPC (frame: cmd+0x1a -> result+0x1a)   <- this script

TCL RPC protocol is much cleaner: no prompt parsing, just function calls.

Usage:
  python3 immo_slib_tcl.py             # start OpenOCD automatically
  python3 immo_slib_tcl.py --attach    # connect to already-running OpenOCD
"""

import subprocess, socket, time, sys, re, threading
from pathlib import Path
from datetime import datetime

# ============================================================ Configuration ==

HOME        = str(Path.home())
OPENOCD_BIN = f'{HOME}/openocd'
CFG_IFACE   = f'{HOME}/tool-openocd-at32/scripts/interface/stlink-dap.cfg'
CFG_TARGET  = f'{HOME}/tool-openocd-at32/scripts/target/at32f435xM.cfg'
LOGFILE     = f'{HOME}/immo_slib_tcl.log'

TCL_PORT    = 6666   # OpenOCD TCL RPC server

# SLib call-site breakpoints found by Python binary analysis
BREAKPOINTS = [
    (0x08069028, 'CALL -> SLib 0x081F102A  [trigger generator candidate 1]'),
    (0x080697D0, 'CALL -> SLib 0x0819AFEE  [trigger generator candidate 2]'),
]

REGS = ['pc', 'r0', 'r1', 'r2', 'r3', 'r4', 'r5', 'lr']

# ================================================================== Logger ==

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


# ======================================================= OpenOCD TCL RPC ==

class OpenOCDTCL:
    """
    Thin wrapper over OpenOCD TCL RPC server (port 6666).

    Protocol:
        send:    bytes(command) + 0x1a
        receive: bytes(result)  + 0x1a

    No prompts, no line buffering - just clean request/response framing.
    'wait_halt N' blocks server-side for up to N milliseconds, then returns.
    """

    TERM = b'\x1a'     # ASCII SUB  -  OpenOCD TCL RPC frame delimiter

    def __init__(self, host: str = 'localhost', port: int = TCL_PORT):
        self._addr  = (host, port)
        self._sock  = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    # --------------------------------------------------------- connection --

    def connect(self, max_wait: float = 25.0) -> bool:
        log(f'TCL RPC: connecting to {self._addr}...')
        deadline = time.time() + max_wait
        attempt  = 0
        while time.time() < deadline:
            attempt += 1
            try:
                self._sock.connect(self._addr)
                log('TCL RPC: connected!', 'g')
                return True
            except (ConnectionRefusedError, OSError):
                if attempt % 6 == 0:
                    log(f'  waiting for OpenOCD... ({int(deadline-time.time())} s left)')
                time.sleep(0.4)
        log('TCL RPC: connection failed', 'r')
        return False

    # ---------------------------------------------------------- transport --

    def run(self, cmd: str, timeout: float = 10.0) -> str:
        """Send one TCL command, return the result string."""
        log(f'  tcl> {cmd}')
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
            log(f'       {result}')
        return result

    # ------------------------------------------ high-level OpenOCD API  --

    def halt(self) -> str:
        return self.run('halt')

    def resume(self) -> str:
        return self.run('resume')

    def reset_run(self) -> str:
        return self.run('reset run')

    def wait_halt(self, timeout_sec: float = 90.0) -> bool:
        """
        Block until target halts (breakpoint, reset, etc).
        Uses OpenOCD built-in 'wait_halt <ms>' which is server-side blocking -
        no polling needed on our side.
        Returns True if target halted, False on timeout.
        """
        ms = int(timeout_sec * 1000)
        log(f'  Waiting for halt (up to {int(timeout_sec)} s)...', 'y')
        # Give the socket extra time beyond the OpenOCD timeout
        result = self.run(f'wait_halt {ms}', timeout=timeout_sec + 5)
        timed_out = 'timed out' in result.lower()
        return not timed_out

    def reg(self, name: str):
        """Read register, return int value or None."""
        out = self.run(f'reg {name}', timeout=5)
        m = re.search(r'0x([0-9a-fA-F]+)', out)
        return int(m.group(1), 16) if m else None

    def mdb(self, addr: int, count: int) -> str:
        """Memory dump: count bytes at addr."""
        return self.run(f'mdb 0x{addr:08X} {count}', timeout=10)

    def mww(self, addr: int, value: int) -> str:
        """Memory write: 32-bit word."""
        return self.run(f'mww 0x{addr:08X} 0x{value:08X}', timeout=5)

    def bp_set(self, addr: int, size: int = 2) -> str:
        """Set hardware breakpoint."""
        return self.run(f'bp 0x{addr:08X} {size} hw', timeout=5)

    def bp_clear_all(self) -> str:
        return self.run('rbp all', timeout=5)

    def bp_clear(self, addr: int) -> str:
        return self.run(f'rbp 0x{addr:08X}', timeout=5)

    def pc(self) -> int:
        return self.reg('pc') or 0

    def close(self) -> None:
        try:
            self._sock.close()
        except Exception:
            pass


# ========================================================== Main logic ====

def start_openocd() -> subprocess.Popen:
    log(f'Starting OpenOCD: {OPENOCD_BIN}')
    proc = subprocess.Popen(
        [OPENOCD_BIN, '-f', CFG_IFACE, '-f', CFG_TARGET],
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        cwd=HOME,
    )
    # Wait for "Listening on port 6666" line
    log('Waiting for OpenOCD ready...')
    t0 = time.time()
    while time.time() - t0 < 20:
        raw = proc.stdout.readline()
        if not raw:
            break
        line = raw.decode('utf-8', errors='replace').rstrip()
        log(f'  ocd| {line}')
        if 'Listening on port' in line and '6666' in line:
            log('OpenOCD is ready!', 'g')
            break
        if proc.poll() is not None:
            log('OpenOCD exited unexpectedly!', 'r')
            sys.exit(1)

    # Drain stdout in background so OpenOCD doesn't block
    def _drain():
        for raw in proc.stdout:
            log(f'  ocd| {raw.decode("utf-8", errors="replace").rstrip()}')
    threading.Thread(target=_drain, daemon=True).start()
    return proc


def read_halt_state(ocd: OpenOCDTCL) -> dict:
    """Read all registers, return dict reg->value."""
    regs = {}
    for r in REGS:
        v = ocd.reg(r)
        regs[r] = v
        log(f'  {r:3s} = {"0x%08X" % v if v is not None else "?"}')
    return regs


def dump_sram_pointers(ocd: OpenOCDTCL, regs: dict) -> None:
    """Dump any SRAM addresses found in R0..R4."""
    log('\n  SRAM pointers in R0..R4:')
    found = False
    for r in ['r0', 'r1', 'r2', 'r3', 'r4']:
        v = regs.get(r) or 0
        if 0x20000000 <= v < 0x20020000:
            log(f'  {r.upper()} = 0x{v:08X}  -> SRAM, read 16 bytes:', 'c')
            out = ocd.mdb(v, 16)
            log(f'    {out}')
            found = True
    if not found:
        log('  (no SRAM pointers found in R0..R4)')


def on_halt(ocd: OpenOCDTCL, pc: int, hit_num: int,
            active_ret_bps: set) -> None:
    """Handle a halt event."""

    # Identify which breakpoint fired
    bp_desc   = None
    is_call   = False
    ret_addr  = None

    for addr, desc in BREAKPOINTS:
        if pc == addr:
            bp_desc  = desc
            is_call  = True
            ret_addr = addr + 4    # Thumb2 BL is 4 bytes
            break

    if pc in active_ret_bps:
        bp_desc = f'RETURN from SLib (after call at 0x{pc-4:08X})'
        is_call = False
        active_ret_bps.discard(pc)
        ocd.bp_clear(pc)

    if bp_desc is None:
        log(f'  Halt at 0x{pc:08X} (not our BP), resuming...', 'y')
        ocd.resume()
        return

    log(f'\n{"="*60}', 'y')
    log(f'  HIT #{hit_num} at 0x{pc:08X}', 'y')
    log(f'  {bp_desc}', 'y')
    log(f'{"="*60}', 'y')

    # Read registers
    log('\n  Registers:')
    regs = read_halt_state(ocd)

    # Dump SRAM pointers
    dump_sram_pointers(ocd, regs)

    # On RETURN: wide SRAM scan to find the trigger bytes
    if not is_call:
        log('\n  SRAM scan 0x20000000 (256 bytes) - looking for 8-byte trigger:')
        out = ocd.mdb(0x20000000, 256)
        log(f'  {out}')

    # On CALL: auto-set return breakpoint
    if is_call and ret_addr:
        log(f'\n  Auto-setting return BP at 0x{ret_addr:08X}...')
        ocd.bp_set(ret_addr)
        active_ret_bps.add(ret_addr)

    log('\n  Resuming...', 'g')
    ocd.resume()


def main() -> None:
    attach_only = '--attach' in sys.argv

    log('=' * 60)
    log('  IMMO SLib Breakpoint Trap  (OpenOCD TCL RPC)')
    log(f'  Log: {LOGFILE}')
    log('=' * 60)

    # 1. Start OpenOCD (or reuse existing)
    proc = None
    if not attach_only:
        proc = start_openocd()
        time.sleep(0.3)

    # 2. Connect via TCL RPC
    ocd = OpenOCDTCL()
    if not ocd.connect(max_wait=25):
        if proc:
            proc.terminate()
        sys.exit(1)

    # 3. Initialize chip
    log('\n--- Init ---', 'c')
    ocd.halt()
    ocd.bp_clear_all()

    # Freeze AT32F435 watchdog during debug halt
    ocd.mww(0xE0042008, 0x00001800)

    # 4. Set breakpoints
    log('\n--- Breakpoints ---', 'c')
    for addr, desc in BREAKPOINTS:
        r = ocd.bp_set(addr)
        log(f'  0x{addr:08X}  {desc}  [{r}]')

    # Verify
    log('\n--- Active BPs ---', 'c')
    ocd.run('bp')

    # 5. Reset and run
    log('\n--- Reset + Run ---', 'c')
    ocd.reset_run()
    log('ECU running. Waiting for breakpoint...', 'g')

    # 6. Main wait loop
    hit           = 0
    active_ret_bps = set()   # return BPs set dynamically

    while True:
        halted = ocd.wait_halt(timeout_sec=90)

        if not halted:
            log('\nTimeout - no halt in 90 s', 'r')
            log('Check: is ECU powered? Is SWD connected?')
            break

        pc = ocd.pc()
        if pc == 0:
            log('Could not read PC, resuming...', 'r')
            ocd.resume()
            continue

        hit += 1
        on_halt(ocd, pc, hit, active_ret_bps)

    log(f'\nTotal halts: {hit}')
    log(f'Log saved to: {LOGFILE}')
    ocd.close()
    if proc:
        proc.terminate()


if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        log('\nInterrupted by user (Ctrl+C).')
        sys.exit(0)
