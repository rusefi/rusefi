from ve_autotune.pipeline.msq import load_tune, read_all_constants, write_tune_with_ve_table

FIXTURE = "ve_autotune/tests/fixtures/sample_tune.msq"

# TuneSnapshot models exactly these 7 fields' worth of .msq constants.
_TUNE_SNAPSHOT_CONSTANT_NAMES = {
    "veTable",
    "veRpmBins",
    "veLoadBins",
    "lambdaTable",
    "lambdaRpmBins",
    "lambdaLoadBins",
    "fuelClosedLoopCorrectionEnabled",
}


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


def test_write_tune_rounds_ve_values_to_declared_digits(tmp_path):
    snap = load_tune(FIXTURE)
    new_ve = [row[:] for row in snap.ve_table]
    new_ve[0][0] = 29.9773
    new_ve[0][1] = 34.1121
    out = tmp_path / "out.msq"
    write_tune_with_ve_table(FIXTURE, new_ve, str(out))
    text = out.read_text(encoding="latin-1")
    # veTable declares digits="1", so values are written with one decimal place
    assert "30.0 34.1 " in text
    assert "29.9773" not in text
    assert "34.1121" not in text


def test_read_all_constants_includes_veTable_and_closed_loop_flag():
    constants = read_all_constants(FIXTURE)
    assert constants["veTable"].startswith("29.0 29.0")
    assert constants["fuelClosedLoopCorrectionEnabled"] == '"disabled"'


def test_load_tune_treats_unexpected_closed_loop_value_as_enabled(tmp_path):
    # Fail closed: only the literal "disabled" means inactive. An unexpected
    # value must not silently decode as "inactive" (the previous
    # `== "enabled"` check did exactly that for anything it didn't recognize).
    text = open(FIXTURE, encoding="latin-1").read()
    text = text.replace(
        '<constant name="fuelClosedLoopCorrectionEnabled">"disabled"</constant>',
        '<constant name="fuelClosedLoopCorrectionEnabled">"unknown"</constant>',
    )
    out = tmp_path / "weird.msq"
    out.write_text(text, encoding="latin-1")
    snap = load_tune(str(out))
    assert snap.fuel_closed_loop_enabled is True


def test_read_all_constants_is_not_limited_to_tune_snapshot_fields():
    constants = read_all_constants(FIXTURE)
    # Proves genuine comprehensiveness (every <constant> tag), not just the
    # 7 fields TuneSnapshot models -- e.g. the fixture's unrelated tag.
    assert "someOtherField" in constants
    assert constants["someOtherField"] == '"keep me"'
    assert set(constants) > _TUNE_SNAPSHOT_CONSTANT_NAMES
