from ve_autotune.pipeline.burn_guard import diff_fields


def test_diff_fields_reports_changed_non_ignored_field():
    baseline = {"veTable": [[1]], "globalTriggerAngleOffset": -2.0, "vinNumber": "A"}
    live = {"veTable": [[9]], "globalTriggerAngleOffset": -5.0, "vinNumber": "B"}
    assert diff_fields(baseline, live) == [("globalTriggerAngleOffset", -2.0, -5.0)]


def test_diff_fields_empty_when_only_ignored_fields_changed():
    baseline = {"veTable": [[1]], "vinNumber": "A"}
    live = {"veTable": [[9]], "vinNumber": "B"}
    assert diff_fields(baseline, live) == []


def test_diff_fields_skips_key_missing_from_live():
    # Asymmetric by design: a key present in baseline but absent from live
    # has nothing to compare against, so it produces no entry.
    baseline = {"globalTriggerAngleOffset": -2.0, "onlyInBaseline": 42}
    live = {"globalTriggerAngleOffset": -2.0}
    assert diff_fields(baseline, live) == []
