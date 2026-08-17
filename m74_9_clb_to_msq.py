#!/usr/bin/env python3
"""Convert Itelma M74 .clb maps (MEproCalibFormat) into rusEFI table text for the m74_9 tune.

Inputs (files exported from the stock M74 calibration):
  - air-charge maps: X = RPM, Z = MAP in mbar (100..1500), values in mg/cyl/cycle
  - ignition maps:   X = RPM, Z = air charge in mg/cyl/cycle (50..525), values in deg BTDC

rusEFI output grid (matches the axes forced/used by the m74_9 tune):
  - RPM bins:  650, 800, 1100, 1400, 1700, 2000, 2300, 2600, 2900, 3200, 3500, 3800, 4100, 4400, 4700, 7000
  - load bins: 20, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100 kPa

VE conversion (same convention as the primary VE table, see docs/report.md 2026-08-16):
  VE% = aircharge_mg / (MAP_kPa * 4.7526), 4.7526 mg/kPa = 400cc cyl charge at 101.325 kPa / 20 C.

Ignition conversion: rusEFI load axis is MAP (kPa); the stock ignition load axis is air charge (mg).
For each rusEFI (rpm, kPa) cell the modeled air charge for that mode is computed from the
corresponding air-charge map, then the stock ignition map is looked up at (rpm, mg).
ЧН (primary) pairs with the IM=0 (long runner) air model; ПМ (second) with IM=1 (short runner).

Usage:
  python3 m74_9_clb_to_msq.py <air0.clb> <air1.clb> <ign_chn.clb> <ign_pm.clb> [msq]
With [msq] given, patches the .msq in place (backup saved next to it).
"""

import re
import sys
import shutil

RPM_BINS = [650, 800, 1100, 1400, 1700, 2000, 2300, 2600, 2900, 3200,
            3500, 3800, 4100, 4400, 4700, 7000]
LOAD_BINS = [20, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75, 80, 85, 90, 95, 100]
MG_PER_KPA = 4.7526


def parse_clb(path):
    """Return (x_axis, z_axis, values[z_row][x_col])."""
    text = open(path, encoding='utf-8').read()
    # header: MEproCalibFormat / NX / NZ / then NX*NZ numbers / then X1=.. / then Z1=..
    m = re.search(r'MEproCalibFormat\s+(\d+)\s+(\d+)\s+(.*?)(X1=.*)', text, re.S)
    nx, nz = int(m.group(1)), int(m.group(2))
    body, axes = m.group(3), m.group(4)
    xm = re.search(r'X1=(.*)', axes, re.S)
    zm = re.search(r'Z1=(.*)', axes, re.S)
    x_axis = [float(v) for v in re.findall(r'X\d+=([\d\.]+)', axes)]
    z_axis = [float(v) for v in re.findall(r'Z\d+=([\d\.]+)', axes)]
    assert len(x_axis) == nx and len(z_axis) == nz, (nx, nz, len(x_axis), len(z_axis))
    nums = [float(t.replace(',', '.')) for t in re.findall(r'-?\d+(?:[.,]\d+)?', body)]
    assert len(nums) == nx * nz, (len(nums), nx, nz)
    values = [nums[i * nx:(i + 1) * nx] for i in range(nz)]
    return x_axis, z_axis, values


def lookup2d(x_axis, z_axis, values, x, z):
    """Bilinear interpolation; clamps to the axis ranges."""
    def idx(a, v):
        if v <= a[0]:
            return 0, 0.0
        if v >= a[-1]:
            return len(a) - 2, 1.0
        for i in range(len(a) - 1):
            if a[i + 1] >= v:
                return i, (v - a[i]) / (a[i + 1] - a[i])
        return len(a) - 2, 1.0
    ix, fx = idx(x_axis, x)
    iz, fz = idx(z_axis, z)
    v00 = values[iz][ix]
    v10 = values[iz][ix + 1]
    v01 = values[iz + 1][ix]
    v11 = values[iz + 1][ix + 1]
    top = v00 + (v10 - v00) * fx
    bot = v01 + (v11 - v01) * fx
    return top + (bot - top) * fz


def air_charge(air, rpm, kpa):
    """Modeled mg/cyl/cycle at (rpm, MAP kPa); clamps rpm and MAP into the model range."""
    x_axis, z_axis, values = air
    mbar = kpa * 10.0
    return lookup2d(x_axis, z_axis, values, rpm, mbar)


def ve_table_from_air(air):
    rows = []
    for kpa in LOAD_BINS:
        row = []
        for rpm in RPM_BINS:
            mg = air_charge(air, rpm, kpa)
            ve = mg / (kpa * MG_PER_KPA) * 100.0
            row.append(round(ve, 1))
        rows.append(row)
    return rows


def ignition_table_from_maps(air, ign):
    rows = []
    for kpa in LOAD_BINS:
        row = []
        for rpm in RPM_BINS:
            mg = air_charge(air, rpm, kpa)
            deg = lookup2d(ign[0], ign[1], ign[2], rpm, mg)
            row.append(round(deg, 1))
        rows.append(row)
    return rows


def fmt_rows(cell_rows):
    """formatValue whitespace: leading newline, 8-space indent + space before the first
    cell of each row, trailing newline."""
    lines = []
    for row in cell_rows:
        lines.append('        ' + ' ' + ' '.join(row))
    return '\n' + '\n'.join(lines) + '\n'


def fmt_table(rows, digits=1):
    """Console-exact text: matches ArrayIniField.formatValue + niceToString output."""
    cell_rows = []
    for row in rows:
        if digits == 1:
            cells = ['%.1f' % v for v in row]
        elif digits == 3:
            cells = ['%.3f' % v for v in row]
        else:
            cells = ['%d' % v for v in row]
        cell_rows.append(cells)
    return fmt_rows(cell_rows)


# Original target-AFR (lambda) table from the stock M74 mixture map conversion
# (docs/report.md 2026-08-16). Values are already on the ECU's 1/147 storage grid
# ({useLambdaOnInterface ? 1/147 : 1/10} scale).
LAMBDA_ORIGINAL = [
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.952 0.952 0.884 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.925 0.912 0.871 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.878 0.864 0.85 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 1.0 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "0.98 0.98 0.98 0.98 0.98 0.98 0.966 0.966 0.966 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "0.959 0.959 0.959 0.959 0.959 0.959 0.939 0.939 0.939 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "0.939 0.939 0.939 0.939 0.939 0.939 0.905 0.905 0.905 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "0.918 0.918 0.918 0.918 0.918 0.918 0.871 0.871 0.871 0.837 0.837 0.837 0.837 0.837 0.837 0.837",
    "0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.83",
    "0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823",
    "0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823",
    "0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.83 0.83 0.816",
    "0.898 0.898 0.898 0.898 0.898 0.898 0.837 0.837 0.837 0.837 0.837 0.837 0.837 0.823 0.816 0.816",
]


def normalize_lambda_table():
    """Restores the original lambda values and renders them exactly the way the console
    does: 1/147 storage grid (Math.round), then Double.toString(Math.round(t*1000)/1000)
    which trims trailing zeros (1.0 stays '1.0', not '1.000'). Any other text re-triggers
    the migration on every load."""
    import math
    cells = []
    for row in LAMBDA_ORIGINAL:
        for v in row.split():
            k = math.floor(float(v) * 147 + 0.5)      # Math.round(v / (1/147))
            t = k / 147.0
            r = math.floor(t * 1000 + 0.5) / 1000.0   # Math.round(t * 1000) / 1000
            cells.append(repr(r))
    assert len(cells) == 256, len(cells)
    return fmt_rows([cells[i * 16:(i + 1) * 16] for i in range(16)])


def main():
    air0_path, air1_path, ign_chn_path, ign_pm_path = sys.argv[1:5]
    air0 = parse_clb(air0_path)
    air1 = parse_clb(air1_path)
    ign_chn = parse_clb(ign_chn_path)
    ign_pm = parse_clb(ign_pm_path)

    ve0 = ve_table_from_air(air0)          # primary VE repro (validation only)
    ve1 = ve_table_from_air(air1)          # second VE
    ign_primary = ignition_table_from_maps(air0, ign_chn)
    ign_second = ignition_table_from_maps(air1, ign_pm)

    print("=== primary VE (IM=0, validation) ===")
    print(fmt_table(ve0))
    print("=== second VE (IM=1) ===")
    print(fmt_table(ve1))
    print("=== primary ignition (ЧН, via IM=0 air) ===")
    print(fmt_table(ign_primary))
    print("=== second ignition (ПМ, via IM=1 air) ===")
    print(fmt_table(ign_second))

    if len(sys.argv) > 5:
        patch_msq(sys.argv[5], ve1, ign_primary, ign_second)


def fmt_bins(bins):
    return fmt_table([[float(b)] for b in bins], 0)


def patch_msq(msq_path, ve1, ign_primary, ign_second):
    src = open(msq_path, encoding='utf-8').read()
    orig = src

    def replace_constant(name, cols, rows, digits, units, value):
        nonlocal src
        pat = re.compile(r'<constant[^>]*name="%s"[^>]*>.*?</constant>' % re.escape(name), re.S)
        assert pat.search(src), name
        src = pat.sub('<constant cols="%d" digits="%s" name="%s" rows="%d" units="%s">%s</constant>'
                      % (cols, digits, name, rows, units, value), src, count=1)

    def set_scalar(name, value):
        nonlocal src
        pat = re.compile(r'(<constant[^>]*name="%s"[^>]*>).*?(</constant>)' % re.escape(name), re.S)
        assert pat.search(src), name
        src = pat.sub(lambda m: m.group(1) + value + m.group(2), src, count=1)

    # Software knock for the m74_9 onboard knock input (connector AA3, see
    # todo-21129.md): 21129 resonant sensors peak at ~7 kHz, single harmonic.
    set_scalar('enableSoftwareKnock', '"enabled"')
    set_scalar('knockFrequency', '7000.0')
    set_scalar('knockDetectionUseDoubleFrequency', '"first harmonic"')

    # Page 0: primary ignition table + its load bins (NA: capped at 100 kPa)
    replace_constant('ignitionTable', 16, 16, '1', 'deg', fmt_table(ign_primary, 1))
    replace_constant('ignitionLoadBins', 1, 16, '0',
                     '{bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}', fmt_bins(LOAD_BINS))

    # lambdaTable: restore the original AFR-conversion values with console-exact text
    # (1/147 storage grid + Double.toString rendering) so the migration comparison
    # settles and the table is not re-burned on every load.
    replace_constant('lambdaTable', 16, 16, '{useLambdaOnInterface ? 2 : 1}',
                     '{useLambdaOnInterface ? &quot;lambda&quot; : &quot;afr&quot;}',
                     normalize_lambda_table())

    # Second tables live on TS page 4 (ini) = msq page number 3 (page - 1, same
    # convention the console's MsqFactory uses: pageIndex ordinal of the
    # pageIdentifier list). Page size 1268 from the m74_9 ini pageSize list.
    ve_load_units = '{bitStringValue(veLoadUnitLabels, veLoadUnitIdx)}'
    ign_load_units = '{bitStringValue(ignLoadUnitLabels, ignLoadUnitIdx)}'
    page3 = ('    <page number="3" size="1268">\n'
             '        <constant cols="16" digits="1" name="secondVeTable" rows="16" units="%">' + fmt_table(ve1, 1) + '</constant>\n'
             '        <constant cols="1" digits="0" name="secondVeLoadBins" rows="16" units="' + ve_load_units + '">' + fmt_bins(LOAD_BINS) + '</constant>\n'
             '        <constant cols="1" digits="0" name="secondVeRpmBins" rows="16" units="RPM">' + fmt_bins(RPM_BINS) + '</constant>\n'
             '        <constant cols="16" digits="1" name="secondIgnitionTable" rows="16" units="deg">' + fmt_table(ign_second, 1) + '</constant>\n'
             '        <constant cols="1" digits="0" name="secondIgnitionLoadBins" rows="16" units="' + ign_load_units + '">' + fmt_bins(LOAD_BINS) + '</constant>\n'
             '        <constant cols="1" digits="0" name="secondIgnitionRpmBins" rows="16" units="RPM">' + fmt_bins(RPM_BINS) + '</constant>\n'
             '    </page>\n')

    # drop an existing secondary-table page block if one is already there, then insert after
    # the main page's closing tag (the drop must not consume the newline that belongs to it).
    existing = re.search(r'    <page number="3" size="1268">.*?</page>\n', src, re.S)
    if existing:
        src = src[:existing.start()] + src[existing.end():]
    assert 'nPages=' in src
    src = src.replace('</page>', '</page>\n' + page3, 1)
    src = src.replace('nPages="1"', 'nPages="2"')

    assert src != orig
    shutil.copy(msq_path, msq_path + '.bak2')
    open(msq_path, 'w', encoding='utf-8').write(src)
    print('Patched %s (backup: %s.bak2)' % (msq_path, msq_path))


if __name__ == '__main__':
    main()
