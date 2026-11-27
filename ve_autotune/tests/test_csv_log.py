import pytest

from ve_autotune.pipeline.csv_log import MissingChannel, load_csv_rows

CSV_TEXT = "Time,RPM,MAP,AFR\n0.0,1200,35.0,14.6\n0.1,1210,35.2,14.5\n"


def test_load_csv_rows_maps_channels_by_name(tmp_path):
    p = tmp_path / "log.csv"
    p.write_text(CSV_TEXT)
    rows = load_csv_rows(str(p), {"rpm": "RPM", "map": "MAP"})
    assert rows[0] == {"rpm": 1200.0, "map": 35.0}
    assert rows[1] == {"rpm": 1210.0, "map": 35.2}


def test_load_csv_rows_missing_channel_raises(tmp_path):
    p = tmp_path / "log.csv"
    p.write_text(CSV_TEXT)
    with pytest.raises(MissingChannel):
        load_csv_rows(str(p), {"clt": "CoolantTemp"})


def test_load_csv_rows_matches_header_with_unit_suffix(tmp_path):
    # Real convert_log_to_csv output names columns "Label (units)" (e.g.
    # "RPM (RPM)", "MAP (kPa)") per MslToCsv.java's heading() helper.
    # channel_map values are the bare label; the unit suffix must be
    # stripped from the CSV header before matching.
    text = "Time,RPM (RPM),MAP (kPa)\n0.0,1200,35.0\n"
    p = tmp_path / "log.csv"
    p.write_text(text)
    rows = load_csv_rows(str(p), {"rpm": "RPM", "map": "MAP"})
    assert rows[0] == {"rpm": 1200.0, "map": 35.0}


def test_load_csv_rows_header_match_is_case_and_whitespace_insensitive(tmp_path):
    text = " rpm ,MAP\n1200,35.0\n"
    p = tmp_path / "log.csv"
    p.write_text(text)
    rows = load_csv_rows(str(p), {"rpm": "RPM"})
    assert rows[0] == {"rpm": 1200.0}


def test_load_csv_rows_malformed_cell_names_channel_and_row(tmp_path):
    text = "Time,RPM\n0.0,not_a_number\n"
    p = tmp_path / "log.csv"
    p.write_text(text)
    with pytest.raises(ValueError) as excinfo:
        load_csv_rows(str(p), {"rpm": "RPM"})
    message = str(excinfo.value)
    assert "rpm" in message
    assert "row 1" in message


def test_load_csv_rows_duplicate_normalized_headers_raise(tmp_path):
    # "RPM" and "RPM (RPM)" both normalize to "rpm" -- ambiguous, must raise
    # rather than silently keeping whichever header csv.DictReader kept last.
    text = "RPM,RPM (RPM)\n1200,1200\n"
    p = tmp_path / "log.csv"
    p.write_text(text)
    with pytest.raises(ValueError):
        load_csv_rows(str(p), {"rpm": "RPM"})
