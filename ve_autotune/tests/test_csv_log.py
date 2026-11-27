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
