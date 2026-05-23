// file msl_to_csv.h
//
// Helper to convert a binary MLG (.msl) log file produced by the
// unit_test_logger into a human-readable CSV file.
//
// See firmware/console/binary_mlg_log/binary_mlg_logging.cpp and mlg_field.h
// for the format that is consumed here.

#pragma once

// Reads `mslPath` (MLVLG binary) and writes a CSV to `csvPath`.
// Returns true on success, false on any I/O or format error.
bool convertMslToCsv(const char* mslPath, const char* csvPath);
