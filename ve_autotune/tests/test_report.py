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
