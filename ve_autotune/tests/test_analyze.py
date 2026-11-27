import pytest

from ve_autotune.pipeline.analyze import ClosedLoopActive, analyze, write_output
from ve_autotune.pipeline.correction import CellResult
from ve_autotune.pipeline.msq import load_tune


def test_analyze_pools_samples_across_two_logs(tmp_path):
    snap, results, divergences, filter_counts = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_a.csv",  # same log twice == doubled sample count
    ])
    corrected = [r for r in results if r.status == "corrected"]
    assert corrected and all(r.sample_count % 2 == 0 for r in corrected)
    assert len(filter_counts) == 2
    assert all(fc.total_rows == filter_counts[0].total_rows for fc in filter_counts)


def test_analyze_flags_snapshot_divergence_between_logs():
    _, _, divergences, _ = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_b.csv",
    ])
    assert len(divergences) == 1
    divergence = divergences[0]
    assert divergence.log_path == "ve_autotune/tests/fixtures/ride_a.csv"
    assert divergence.baseline_path == "ve_autotune/tests/fixtures/ride_b.csv"
    field_names = {name for name, _, _ in divergence.fields}
    assert "lambdaLoadBins" in field_names


def test_analyze_flags_divergence_in_unmodeled_field():
    # ride_d's .msq differs from ride_a's only in `injectorFuelFlow`, a
    # constant TuneSnapshot doesn't model at all -- this is the critical-
    # finding regression check: the divergence check must compare *every*
    # .msq constant, not just the 6 TuneSnapshot-modeled fields.
    _, _, divergences, _ = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_d.csv",
    ])
    assert len(divergences) == 1
    field_names = {name for name, _, _ in divergences[0].fields}
    assert field_names == {"injectorFuelFlow"}


def test_analyze_raises_when_closed_loop_was_active(tmp_path):
    # ride_c fixture's .msq has fuelClosedLoopCorrectionEnabled = "enabled"
    with pytest.raises(ClosedLoopActive):
        analyze(["ve_autotune/tests/fixtures/ride_c.csv"])


def test_write_output_initializes_ve_table_from_baseline_for_missing_cells(tmp_path):
    baseline_path = "ve_autotune/tests/fixtures/ride_a.msq"
    baseline = load_tune(baseline_path)
    # Simulate a gap in cell coverage: only report a correction for cell (0,0).
    results = [
        CellResult(row=0, col=0, old_ve=baseline.ve_table[0][0], new_ve=99.9, delta_pct=0.0,
                   sample_count=25, measured_afr=14.0, target_afr=14.7, status="corrected"),
    ]
    out_dir = str(tmp_path / "out")
    msq_path, report_path = write_output(baseline, baseline_path, results, out_dir)

    reloaded = load_tune(msq_path)
    assert reloaded.ve_table[0][0] == pytest.approx(99.9)
    # A cell with no CellResult at all must keep the baseline's value, not
    # silently become 0.0.
    assert reloaded.ve_table[1][1] == pytest.approx(baseline.ve_table[1][1])
    assert reloaded.ve_table[1][1] != 0.0

    # baseline.msq is written alongside the corrected output, for the
    # burn-time manual-VE-edit check (finding #5).
    baseline_msq_path = str(tmp_path / "out" / "baseline.msq")
    with open(baseline_msq_path, encoding="latin-1") as f:
        baseline_text = f.read()
    with open(baseline_path, encoding="latin-1") as f:
        source_text = f.read()
    assert baseline_text == source_text
    assert report_path == str(tmp_path / "out" / "report.md")
