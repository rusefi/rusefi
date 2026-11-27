from ve_autotune.pipeline.msq import load_tune, write_tune_with_ve_table

FIXTURE = "ve_autotune/tests/fixtures/sample_tune.msq"


def test_load_tune_reads_ve_and_lambda_tables():
    snap = load_tune(FIXTURE)
    assert len(snap.ve_table) == 16 and len(snap.ve_table[0]) == 16
    assert len(snap.lambda_table) == 16 and len(snap.lambda_table[0]) == 16
    assert len(snap.ve_rpm_bins) == 16
    assert len(snap.ve_load_bins) == 16
    assert snap.fuel_closed_loop_enabled is False


def test_write_tune_with_ve_table_changes_only_ve_table(tmp_path):
    snap = load_tune(FIXTURE)
    new_ve = [[v + 1 for v in row] for row in snap.ve_table]
    out = tmp_path / "out.msq"
    write_tune_with_ve_table(FIXTURE, new_ve, str(out))
    reloaded = load_tune(str(out))
    assert reloaded.ve_table == new_ve
    assert reloaded.lambda_table == snap.lambda_table  # untouched
    # the unrelated fixture tag survived unchanged
    assert "someOtherField" in out.read_text(encoding="latin-1")
