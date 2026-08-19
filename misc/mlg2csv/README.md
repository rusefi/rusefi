# mlg2csv / mlg2charts

Two tools for rusEFI binary MLG logs (format v2, "MLVLG" magic).
MegaLogViewer does NOT read this format directly - convert first.

Format reference:
- firmware/console/binary_mlg_log/mlg_types.h (header + field descriptors)
- firmware/console/binary_mlg_log/binary_mlg_logging.cpp (data records)

## mlg2charts.py - interactive HTML charts straight from the MLG

Parses the binary log directly (no CSV step) and renders a self-contained
HTML page with zoomable per-group charts (RPM, air/load, temperatures,
lambda/AFR, ignition advance, dwell/coils, trigger/sync, battery, fuel).
The output HTML embeds plotly.js - open it in any browser, no server.

Setup (once):

    python3 -m venv venv
    venv/bin/pip install plotly

Run:

    venv/bin/python mlg2charts.py input.mlg [output.html]

## MlgToCsv.java - CSV export for MegaLogViewer / spreadsheets

Build and run (no dependencies beyond a JDK):

    javac MlgToCsv.java
    java MlgToCsv input.mlg [output.csv]

The output CSV has `time` (seconds, unwrapped from the 16-bit 10us record
timestamp) as the first column and one column per logged field, scaled per
the MLG field descriptors (scale/transform from the file header).
