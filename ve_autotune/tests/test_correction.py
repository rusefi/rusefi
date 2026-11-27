from types import SimpleNamespace

import pytest

from ve_autotune.pipeline.correction import (
    MAX_LEAN_PCT,
    MAX_RICH_PCT,
    compute_corrections,
)


def _snapshot(ve_table, lambda_table, bins):
    # minimal stand-in with just the fields compute_corrections reads
    return SimpleNamespace(ve_table=ve_table, ve_rpm_bins=bins, ve_load_bins=bins,
                            lambda_table=lambda_table, lambda_rpm_bins=bins, lambda_load_bins=bins)


def test_cell_with_enough_samples_gets_corrected_and_capped():
    ve = [[80.0]]  # 1x1 for a focused test
    lam = [[13.0]]
    snap = _snapshot(ve, lam, [3000])
    samples = {(0, 0): [15.0] * 25}  # measured way leaner than target -> wants +15.4%, capped at MAX_RICH_PCT
    results = compute_corrections(samples, snap)
    r = results[0]
    assert r.status == "corrected"
    assert r.sample_count == 25
    assert r.delta_pct == pytest.approx(15.0)
    assert r.new_ve == pytest.approx(80.0 * 1.15)


def test_cell_without_enough_samples_is_unchanged():
    ve = [[80.0]]
    lam = [[13.0]]
    snap = _snapshot(ve, lam, [3000])
    samples = {(0, 0): [15.0] * 5}
    r = compute_corrections(samples, snap)[0]
    assert r.status == "not_enough_data"
    assert r.new_ve == 80.0


def test_lean_cap_is_never_looser_than_rich_cap():
    assert MAX_LEAN_PCT <= MAX_RICH_PCT


def test_not_enough_data_still_reports_real_target_afr():
    ve = [[80.0]]
    lam = [[13.0]]
    snap = _snapshot(ve, lam, [3000])
    samples = {(0, 0): [15.0] * 5}
    r = compute_corrections(samples, snap)[0]
    assert r.status == "not_enough_data"
    assert r.target_afr == pytest.approx(13.0)
