# mlg2csv

Converts rusEFI binary MLG log files (format v2, "MLVLG" magic) to CSV that
MegaLogViewer (MS/HD) or any spreadsheet tool can open. MegaLogViewer does NOT
read the rusEFI binary MLG format directly - convert first.

Format reference:
- firmware/console/binary_mlg_log/mlg_types.h (header + field descriptors)
- firmware/console/binary_mlg_log/binary_mlg_logging.cpp (data records)

Build and run (no dependencies beyond a JDK):

    javac MlgToCsv.java
    java MlgToCsv input.mlg [output.csv]

The output CSV has `time` (seconds, unwrapped from the 16-bit 10us record
timestamp) as the first column and one column per logged field, scaled per
the MLG field descriptors (scale/transform from the file header).
