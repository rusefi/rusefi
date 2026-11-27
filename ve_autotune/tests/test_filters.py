import pytest

from ve_autotune.pipeline.filters import (
    AFR_RAIL_LOW,
    FlatlinedAfr,
    check_afr_plausible,
    reject_cranking_rows,
    reject_dfco_rows,
    reject_railed_afr,
    steady_state_rows,
)


def _row(t, rpm, map_, tps, clt, afr):
    return {"t": t, "rpm": rpm, "map": map_, "tps": tps, "clt": clt, "afr": afr}


def test_steady_state_drops_rpm_transient():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3600, 50, 10, 80, 13.0)]
    assert steady_state_rows(rows) == []  # 6000 rpm/s > MAX_RPM_RATE


def test_steady_state_keeps_stable_row():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3005, 50.1, 10, 80, 13.0)]
    assert steady_state_rows(rows) == [rows[1]]


def test_steady_state_drops_cold_row():
    rows = [_row(0.0, 3000, 50, 10, 40, 13.0), _row(0.1, 3001, 50, 10, 40, 13.0)]
    assert steady_state_rows(rows) == []


def test_steady_state_drops_map_transient():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3000, 53, 10, 80, 13.0)]
    assert steady_state_rows(rows) == []  # 30 kPa/s > MAX_MAP_RATE (20)


def test_steady_state_drops_tps_transient():
    rows = [_row(0.0, 3000, 50, 10, 80, 13.0), _row(0.1, 3000, 50, 16, 80, 13.0)]
    assert steady_state_rows(rows) == []  # 60 %/s > MAX_TPS_RATE (50)


def test_reject_cranking_rows_drops_low_rpm():
    rows = [_row(0.0, 200, 50, 10, 80, 13.0), _row(0.1, 600, 50, 10, 80, 13.0)]
    assert reject_cranking_rows(rows) == [rows[1]]


def test_reject_dfco_rows_drops_closed_throttle_low_map():
    rows = [_row(0.0, 3000, 20, 0.0, 80, 13.0), _row(0.1, 3000, 50, 10, 80, 13.0)]
    assert reject_dfco_rows(rows) == [rows[1]]


def test_reject_dfco_rows_keeps_closed_throttle_high_map():
    # closed throttle but MAP isn't low (e.g. coasting downhill at altitude
    # or a sensor quirk) -- only the combination of both is DFCO-like.
    rows = [_row(0.0, 3000, 90, 0.0, 80, 13.0)]
    assert reject_dfco_rows(rows) == rows


def test_check_afr_plausible_raises_on_flatline():
    rows = [_row(i * 0.1, 3000, 50, 10, 80, 7.35) for i in range(10)]
    with pytest.raises(FlatlinedAfr):
        check_afr_plausible(rows)


def test_reject_railed_afr_drops_boundary_samples():
    rows = [_row(0, 3000, 50, 10, 80, 7.35), _row(0.1, 3000, 50, 10, 80, 13.2)]
    assert reject_railed_afr(rows) == [rows[1]]


def test_reject_railed_afr_drops_exact_low_rail_boundary():
    # AFR_RAIL_LOW is 7.4 (not the 7.35 calibration endpoint itself) -- a
    # sample reading exactly 7.4 must still be rejected.
    rows = [_row(0, 3000, 50, 10, 80, AFR_RAIL_LOW), _row(0.1, 3000, 50, 10, 80, 13.2)]
    assert reject_railed_afr(rows) == [rows[1]]
