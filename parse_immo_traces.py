#!/usr/bin/env python3
"""Parse PCAN-View .trc files and extract IMMO 0x0713 / 0x0714 exchanges."""

import re
import sys
from pathlib import Path

LINE_RE = re.compile(
    r"^\s*\d+\)\s+([\d.]+)\s+\w+\s+([0-9A-Fa-f]+)\s+(\d)\s+((?:[0-9A-Fa-f]{2}\s*)+)"
)

IMMO_IDS = {0x0713, 0x0714}


def parse_trc(path: Path):
    msgs = []
    with open(path, "r", encoding="ascii", errors="ignore") as f:
        for line in f:
            m = LINE_RE.match(line)
            if not m:
                continue
            t = float(m.group(1))
            can_id = int(m.group(2), 16)
            dlc = int(m.group(3))
            data = bytes.fromhex(m.group(4))
            if can_id in IMMO_IDS:
                msgs.append((t, can_id, data))
    return msgs


def analyze(msgs):
    sessions = []
    quicks = []
    i = 0
    while i < len(msgs):
        t, cid, data = msgs[i]
        if cid == 0x0713 and len(data) == 8:
            # possible trigger (no preceding 0x0714 within 1s) or response/quick response
            # a trigger is a 0x0713 that is not immediately preceded by 0x0714
            trigger = data.hex()
            # look ahead for two 0x0714 frames -> full challenge
            if i + 2 < len(msgs) and msgs[i + 1][1] == 0x0714 and msgs[i + 2][1] == 0x0714:
                f1 = msgs[i + 1][2].hex()
                f2 = msgs[i + 2][2].hex()
                dt1 = msgs[i + 1][0] - t
                dt2 = msgs[i + 2][0] - msgs[i + 1][0]
                # response should follow
                resp = None
                dt_resp = None
                if i + 3 < len(msgs) and msgs[i + 3][1] == 0x0713:
                    resp = msgs[i + 3][2].hex()
                    dt_resp = msgs[i + 3][0] - msgs[i + 2][0]
                sessions.append({
                    "trigger": trigger,
                    "frame1": f1,
                    "frame2": f2,
                    "response": resp,
                    "dt_trigger_to_f1": dt1,
                    "dt_f1_to_f2": dt2,
                    "dt_f2_to_response": dt_resp,
                    "trigger_time": t,
                })
                i += 4 if resp else 3
                continue
            else:
                # could be quick response: preceded by 0x0714 within ~10 ms
                if i > 0 and msgs[i - 1][1] == 0x0714:
                    quicks.append({
                        "challenge": msgs[i - 1][2].hex(),
                        "response": data.hex(),
                        "dt": t - msgs[i - 1][0],
                    })
        i += 1
    return sessions, quicks


def main():
    files = sorted(Path(".").glob("*.trc"))
    for f in files:
        msgs = parse_trc(f)
        if not msgs:
            continue
        sessions, quicks = analyze(msgs)
        print(f"\n=== {f.name} ===")
        print(f"Total IMMO messages: {len(msgs)}")
        if sessions:
            print(f"Full sessions: {len(sessions)}")
            for s in sessions:
                print(
                    f"  trigger={s['trigger']}  "
                    f"f1={s['frame1']} f2={s['frame2']}  "
                    f"resp={s['response']}"
                )
                print(
                    f"    timings: T->F1={s['dt_trigger_to_f1']:.2f} ms  "
                    f"F1->F2={s['dt_f1_to_f2']:.2f} ms  "
                    f"F2->R={s['dt_f2_to_response']:.2f} ms"
                )
        if quicks:
            print(f"Quick checks: {len(quicks)}")
            for q in quicks:
                print(f"  challenge={q['challenge']} -> response={q['response']}  dt={q['dt']:.2f} ms")


if __name__ == "__main__":
    main()
