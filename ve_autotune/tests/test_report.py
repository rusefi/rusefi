from types import SimpleNamespace

from ve_autotune.pipeline.correction import CellResult
from ve_autotune.pipeline.report import render_report


def test_render_report_includes_summary_and_corrected_cell():
    results = [
        CellResult(row=0, col=0, old_ve=80.0, new_ve=92.0, delta_pct=15.0,
                   sample_count=25, measured_afr=15.0, target_afr=13.0, status="corrected"),
        CellResult(row=0, col=1, old_ve=70.0, new_ve=70.0, delta_pct=0.0,
                   sample_count=3, measured_afr=0.0, target_afr=13.0, status="not_enough_data"),
    ]
    text = render_report(results)
    assert "1/2 cells corrected" in text
    assert "1 not enough data" in text
    assert "92.0" in text and "15.0" in text


def test_not_enough_data_row_shows_placeholder_instead_of_zero_afr():
    results = [
        CellResult(row=0, col=1, old_ve=70.0, new_ve=70.0, delta_pct=0.0,
                   sample_count=3, measured_afr=0.0, target_afr=13.0, status="not_enough_data"),
    ]
    text = render_report(results)
    row_line = [line for line in text.splitlines() if line.startswith("| 0 | 1 |")][0]
    assert "| - | 13.00 | not_enough_data |" in row_line
    assert "0.00" not in row_line


def test_render_report_includes_per_log_filter_counts_when_given():
    results = [
        CellResult(row=0, col=0, old_ve=80.0, new_ve=80.0, delta_pct=0.0,
                   sample_count=0, measured_afr=0.0, target_afr=13.0, status="not_enough_data"),
    ]
    filter_counts = [
        SimpleNamespace(
            log_path="ride_a.csv",
            total_rows=25,
            rejected_transient=1,
            rejected_cold=0,
            rejected_cranking=2,
            rejected_dfco=3,
            rejected_railed=0,
            kept=19,
        )
    ]
    text = render_report(results, filter_counts)
    assert "Rows filtered per log" in text
    assert "| ride_a.csv | 25 | 1 | 0 | 2 | 3 | 0 | 19 |" in text


def test_render_report_omits_filter_section_when_not_given():
    results = [
        CellResult(row=0, col=0, old_ve=80.0, new_ve=80.0, delta_pct=0.0,
                   sample_count=0, measured_afr=0.0, target_afr=13.0, status="not_enough_data"),
    ]
    assert "Rows filtered per log" not in render_report(results)
