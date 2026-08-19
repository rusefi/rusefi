#!/usr/bin/env python3
"""
mlg2charts - render an interactive HTML chart page straight from a rusEFI
binary MLG log (MLVLG v2). No CSV step, no server: the output HTML is
self-contained (plotly.js embedded) and opens in any browser.

Usage:
    venv/bin/python mlg2charts.py <input.mlg> [output.html]

Setup (once):
    python3 -m venv venv
    venv/bin/pip install plotly

Format reference: firmware/console/binary_mlg_log/mlg_types.h and
binary_mlg_logging.cpp
"""

import struct
import sys

import plotly
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# Chart groups: exact rusEFI output channel names. Missing channels are skipped.
GROUPS = [
    ("RPM", ["rpmvalue"]),
    ("Air / load", ["rawmap", "tpsvalue", "throttlepedalposition", "sdairmassinonecylinder"]),
    ("Temperatures", ["coolant", "rawiat"]),
    ("Lambda / AFR", ["afrgasolinescale", "lambdavalue2", "lambdatimesincegood"]),
    ("Ignition advance", ["baseignitionadvance", "ignitionadvancecyl1", "ignitionadvancecyl2",
                          "ignitionadvancecyl3", "ignitionadvancecyl4"]),
    ("Dwell / coils", ["basedwell", "coildutycycle", "dwelloverchargecounter",
                       "overdwellnotscheduledcounter"]),
    ("Trigger / sync", ["trgsynchronizationcounter", "istriggererror", "trgtriggerstateindex",
                        "triggertoothangleerror", "m_hassynchronizedphase",
                        "vvt1itriggersyncgapratio"]),
    ("Battery", ["rawbattery"]),
    ("Fuel", ["running_basefuel", "totalfuelcorrection", "actuallastinjectionratio"]),
]

# MLG v2 scalar types: 0=U08, 1=S08, 2=U16, 3=S16, 4=U32, 5=S32, 6=S64, 7=F32
TYPE_SIZE = {0: 1, 1: 1, 2: 2, 3: 2, 4: 4, 5: 4, 6: 8, 7: 4}


def cstr(b, off, length):
    end = off
    while end < off + length and b[end] != 0:
        end += 1
    return b[off:end].decode("ascii", "replace")


def parse_mlg(path):
    b = open(path, "rb").read()
    if len(b) < 24 or b[:5] != b"MLVLG":
        raise ValueError(f"not an MLVLG file: {path}")

    data_begin = struct.unpack_from(">I", b, 16)[0]
    record_len = struct.unpack_from(">H", b, 20)[0]
    field_count = struct.unpack_from(">H", b, 22)[0]

    fields = []
    off = 24
    for _ in range(field_count):
        ftype = b[off]
        name = cstr(b, off + 1, 34)
        scale, transform = struct.unpack_from(">ff", b, off + 46)
        fields.append({"name": name, "type": ftype, "size": TYPE_SIZE.get(ftype, 0),
                       "scale": scale, "transform": transform})
        off += 89

    # data records: 4-byte prefix (block type, rolling counter, uint16 10us ts),
    # then values, then 1-byte checksum
    names = [f["name"] for f in fields]
    time = []
    values = {name: [] for name in names}
    pos = data_begin
    prev_ts = None
    t = 0.0
    checksum_bad = 0
    while pos + 4 + record_len + 1 <= len(b):
        block_type = b[pos]
        ts = struct.unpack_from(">H", b, pos + 2)[0]
        if block_type != 0:
            pos += 4 + record_len + 1
            continue
        if prev_ts is not None:
            t += ((ts - prev_ts) & 0xFFFF) * 10e-6
        prev_ts = ts

        payload = pos + 4
        chk = b[payload + record_len]
        if sum(b[payload:payload + record_len]) & 0xFF != chk:
            checksum_bad += 1

        time.append(t)
        foff = payload
        for f in fields:
            size = f["size"]
            if f["type"] == 0:
                raw = b[foff]
            elif f["type"] == 1:
                raw = struct.unpack_from("b", b, foff)[0]
            elif f["type"] == 2:
                raw = struct.unpack_from(">H", b, foff)[0]
            elif f["type"] == 3:
                raw = struct.unpack_from(">h", b, foff)[0]
            elif f["type"] == 4:
                raw = struct.unpack_from(">I", b, foff)[0]
            elif f["type"] == 5:
                raw = struct.unpack_from(">i", b, foff)[0]
            elif f["type"] == 6:
                raw = struct.unpack_from(">q", b, foff)[0]
            elif f["type"] == 7:
                raw = struct.unpack_from(">f", b, foff)[0]
            else:
                raw = 0
            values[f["name"]].append((raw + f["transform"]) * f["scale"])
            foff += size
        pos += 4 + record_len + 1

    print(f"fields: {field_count}, records: {len(time)}, "
          f"checksum mismatches: {checksum_bad}, trailing bytes: {len(b) - pos}")
    return time, values, names


def main():
    if len(sys.argv) < 2:
        print("Usage: venv/bin/python mlg2charts.py <input.mlg> [output.html]")
        sys.exit(1)

    in_path = sys.argv[1]
    out_path = sys.argv[2] if len(sys.argv) > 2 else in_path[:-4] + ".html"

    time, values, names = parse_mlg(in_path)
    name_set = set(names)

    groups = []
    for title, wanted in GROUPS:
        present = [n for n in wanted if n in name_set]
        if present:
            groups.append((title, present))

    if not groups:
        print("no known channels found in the log")
        sys.exit(1)

    fig = make_subplots(rows=len(groups), cols=1, shared_xaxes=True,
                        subplot_titles=[g[0] for g in groups],
                        vertical_spacing=0.04)

    for i, (title, present) in enumerate(groups, start=1):
        for name in present:
            fig.add_trace(
                go.Scattergl(x=time, y=values[name], name=name, mode="lines",
                             line={"width": 1.2}),
                row=i, col=1)

    fig.update_layout(
        title=f"rusEFI log: {in_path.rsplit('/', 1)[-1]}",
        height=280 * len(groups) + 120,
        hovermode="x unified",
        legend={"orientation": "h", "yanchor": "bottom", "y": 1.0},
        margin={"l": 60, "r": 20, "t": 60, "b": 40},
    )
    fig.update_xaxes(title_text="time, s", row=len(groups), col=1)

    plotly.offline.plot(fig, filename=out_path, auto_open=False,
                        include_plotlyjs="inline")
    print(f"written: {out_path} ({len(groups)} groups)")


if __name__ == "__main__":
    main()
