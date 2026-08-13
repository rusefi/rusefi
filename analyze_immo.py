#!/usr/bin/env python3
"""
Immo analysis helper for m74_9 (Largus / Renault CAN immobilizer).

Parses PCAN-View .trc files to extract 0x0713 (ECU) and 0x0714 (BCM) frames,
forms challenge/response pairs, and prints them. Also diffs the IMMOON/IMMOOFF
tune binaries and dumps the immobilizer-related code region from the original
firmware full flash dump for Ghidra analysis.

Run from the repo root:
    python3 analyze_immo.py
"""

import json
import re
import struct
from pathlib import Path
from typing import List, Tuple, Dict, Any

ROOT = Path(__file__).parent


def parse_trc(path: Path) -> List[Tuple[float, str, bytes]]:
    """Return (time_ms, id_hex, data_bytes) for 0x0713/0x0714 frames."""
    msgs = []
    with open(path, "r", encoding="utf-8", errors="ignore") as f:
        for line in f:
            line = line.strip()
            if not line or line.startswith(";"):
                continue
            m = re.search(
                r"(\d+)\)\s+([\d.]+)\s+(Rx|Tx)\s+([0-9A-F]+)\s+(\d+)\s+(.*)",
                line,
            )
            if not m:
                continue
            _num, t, _dir, id_hex, _dlc, data = m.groups()
            if id_hex in ("0713", "0714"):
                try:
                    b = bytes.fromhex(data)
                except ValueError:
                    continue
                msgs.append((float(t), id_hex, b))
    return msgs


def extract_sessions(msgs: List[Tuple[float, str, bytes]]) -> List[Dict[str, Any]]:
    """Group messages into full 16-byte challenge sessions and quick checks."""
    sessions = []
    i = 0
    while i < len(msgs):
        t, idh, data = msgs[i]
        if idh == "0713":
            # Full session: ECU trigger, two 0x0714 challenge frames, ECU response
            if (
                i + 3 < len(msgs)
                and msgs[i + 1][1] == "0714"
                and msgs[i + 2][1] == "0714"
                and msgs[i + 3][1] == "0713"
            ):
                sessions.append(
                    {
                        "type": "full",
                        "trigger_time": t,
                        "trigger": data.hex(),
                        "challenge1": msgs[i + 1][2].hex(),
                        "challenge2": msgs[i + 2][2].hex(),
                        "response_time": msgs[i + 3][0],
                        "response": msgs[i + 3][2].hex(),
                    }
                )
                i += 4
                continue

        if idh == "0714":
            # Quick re-check: BCM sends a single 0x0714 challenge, ECU replies
            if i + 1 < len(msgs) and msgs[i + 1][1] == "0713":
                sessions.append(
                    {
                        "type": "quick",
                        "challenge_time": t,
                        "challenge": data.hex(),
                        "response_time": msgs[i + 1][0],
                        "response": msgs[i + 1][2].hex(),
                    }
                )
                i += 2
                continue
        i += 1
    return sessions


def diff_binaries(path1: Path, path2: Path) -> List[Tuple[int, int, int]]:
    """Return list of (offset, byte_a, byte_b) differences."""
    a = path1.read_bytes()
    b = path2.read_bytes()
    diffs = []
    for offset, (ba, bb) in enumerate(zip(a, b)):
        if ba != bb:
            diffs.append((offset, ba, bb))
    return diffs


def dump_region(path: Path, out: Path, start: int, end: int) -> None:
    data = path.read_bytes()
    out.write_bytes(data[start:end])


def dump_ghidra_hints(path: Path) -> None:
    """Print flash-dump addresses that help orient Ghidra analysis."""
    if not path.exists():
        return
    data = path.read_bytes()

    def addr_off(addr: int) -> int:
        return addr - 0x08000000

    # Literal pool consumed by the immo dispatcher at 0x08203FFC.
    pool = addr_off(0x08204080)
    print("\nLiteral pool at 0x08204080 (values are RAM addresses):")
    labels = [
        "counter/struct base",
        "state byte (DAT_08204084)",
        "state byte (DAT_08204088)",
        "func-ptr slot 0 -> 0x20001A50",
        "arg for callback 0",
        "flags/counter",
        "func-ptr slot 1 -> 0x20001A54",
        "arg for callback 1",
    ]
    for i, label in zip(range(0, 32, 4), labels):
        val = struct.unpack_from("<I", data, pool + i)[0]
        print(f"  0x{0x08204080 + i:08x}: 0x{val:08x}  -> {label}")

    # Key constants embedded in the first flash bank.
    constants = [0x2548A4D2, 0x4DF9123B, 0x43A0C212, 0xF9C74A52]
    print("\nKey constants in flash (source of values copied to RAM 0x20001ADC):")
    for c in constants:
        needle = struct.pack("<I", c)
        idx = data.find(needle)
        while idx >= 0:
            print(f"  0x{c:08x} at 0x{idx + 0x08000000:08x}")
            idx = data.find(needle, idx + 1)

    # RAM references inside the immo code region.
    ram_addrs = [
        0x20001A50,
        0x20001A54,
        0x20001A44,
        0x20001A45,
        0x20001A46,
        0x20001A48,
        0x20001A4C,
        0x2000162C,
    ]
    print("\nReferences to dispatcher RAM addresses inside 0x08203F0C-0x08204D00:")
    for a in ram_addrs:
        needle = struct.pack("<I", a)
        idx = data.find(needle)
        refs = []
        while idx >= 0:
            addr = idx + 0x08000000
            if 0x08203F0C <= addr <= 0x08204D00:
                refs.append(f"0x{addr:08x}")
            idx = data.find(needle, idx + 1)
        print(f"  0x{a:08x}: {', '.join(refs) if refs else '(none in region)'}")


def main() -> None:
    all_sessions: List[Dict[str, Any]] = []
    for trc in sorted(ROOT.glob("*.trc")):
        msgs = parse_trc(trc)
        sessions = extract_sessions(msgs)
        if not sessions:
            continue
        print(f"\n=== {trc.name} ===")
        for s in sessions:
            if s["type"] == "full":
                print(
                    f"  full: trigger {s['trigger']} -> "
                    f"challenge {s['challenge1']}{s['challenge2']} -> "
                    f"response {s['response']}"
                )
            else:
                print(
                    f"  quick: challenge {s['challenge']} -> "
                    f"response {s['response']}"
                )
        for s in sessions:
            s["source"] = trc.name
        all_sessions.extend(sessions)

    out_json = ROOT / "immo_pairs.json"
    with open(out_json, "w") as f:
        json.dump(all_sessions, f, indent=2)
    print(f"\nWrote {len(all_sessions)} session(s) to {out_json}")

    # Compare the two known tune binaries
    on = ROOT / "LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOON.bin"
    off = ROOT / "LARGUS_TUN_V14-8_ANTIJRK_OFF_UOZNEW_OTSKOK-6_AFR09_IMMOOFF.bin"
    if on.exists() and off.exists():
        diffs = diff_binaries(on, off)
        print(f"\nIMMOON vs IMMOFF differences: {len(diffs)} byte(s)")
        # Show first 20 diffs
        for offset, ba, bb in diffs[:20]:
            print(f"  0x{offset:06x}: 0x{ba:02x} -> 0x{bb:02x}")
        if len(diffs) == 1:
            offset, ba, bb = diffs[0]
            print(f"  Single immo enable flag at 0x{offset:06x}: 0x{ba:02x} (ON) vs 0x{bb:02x} (OFF)")

    # Dump immobilizer-related code region from the full flash dump
    full = ROOT / "Read_FULLFLASH_I865LB52_w2404b1____(240626_103727).bin"
    if full.exists():
        # 0x08200000 is the second bank in the dump; absolute address 0x08203F0C
        # maps to file offset 0x203F0C. We dump from the immo handler to the
        # end of the immo module (~0x8204D00).
        start = 0x203F0C
        end = 0x204D00
        out_bin = ROOT / "immo_code_region.bin"
        dump_region(full, out_bin, start, end)
        print(f"\nDumped immo code region 0x{start:08x}-0x{end:08x} to {out_bin}")
        print("Load this into Ghidra as an additional memory block at base")
        print("0x08203F0C (or load the full 4 MB flash dump at 0x08000000).")
        print("Language: ARM Cortex-M, LE Thumb.")
        print("Key symbols to inspect: 0x08203FFC (immo dispatcher), 0x082047D0,")
        print("0x082048E8, 0x08204B00-0x08204C00 (key constants/registration).")

    dump_ghidra_hints(full)


if __name__ == "__main__":
    main()
