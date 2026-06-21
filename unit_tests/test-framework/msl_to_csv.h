// file msl_to_csv.h
//
// Helper to convert an .msl log file into a human-readable CSV file.
//
// Two .msl flavours are supported:
//  - binary MLG/MLVLG logs produced by the unit_test_logger
//    (see firmware/console/binary_mlg_log/binary_mlg_logging.cpp and mlg_field.h)
//  - text/ASCII TunerStudio MSL logs (tab-separated columns with a couple of
//    quoted metadata header lines, a '#' marker line, a field-name row and a
//    units row)

#pragma once

// Reads `mslPath` (binary MLVLG or text TunerStudio MSL) and writes a CSV to
// `csvPath`. The flavour is auto-detected from the file's magic bytes.
// Returns true on success, false on any I/O or format error.
bool convertMslToCsv(const char* mslPath, const char* csvPath);
