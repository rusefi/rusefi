import pytest

from ve_autotune.pipeline.analyze import ClosedLoopActive, analyze


def test_analyze_pools_samples_across_two_logs(tmp_path):
    snap, results, divergences = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_a.csv",  # same log twice == doubled sample count
    ])
    corrected = [r for r in results if r.status == "corrected"]
    assert corrected and all(r.sample_count % 2 == 0 for r in corrected)


def test_analyze_flags_snapshot_divergence_between_logs():
    _, _, divergences = analyze([
        "ve_autotune/tests/fixtures/ride_a.csv",
        "ve_autotune/tests/fixtures/ride_b.csv",
    ])
    assert len(divergences) == 1


def test_analyze_raises_when_closed_loop_was_active(tmp_path):
    # ride_c fixture's .msq has fuelClosedLoopCorrectionEnabled = "enabled"
    with pytest.raises(ClosedLoopActive):
        analyze(["ve_autotune/tests/fixtures/ride_c.csv"])
