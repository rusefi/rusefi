"""Steady-state and AFR-plausibility filters for VE autotune log rows.

Rows are dicts with keys ``t`` (seconds), ``rpm``, ``map``, ``tps``, ``clt``
and ``afr``. The thresholds below are adjustable defaults from design.md
("Defaults the builder should use").
"""

# Adjustable defaults (design.md "Defaults the builder should use").
MAX_RPM_RATE = 500.0  # rpm/s
MAX_MAP_RATE = 20.0  # kPa/s
MAX_TPS_RATE = 50.0  # %/s
MIN_CLT = 60.0  # degC
AFR_RAIL_LOW = 7.4  # just inside the 7.35 calibration endpoint
AFR_RAIL_HIGH = 22.3  # just inside the 22.39 calibration endpoint

_AFR_FLATLINE_TOL = 1e-6


class FlatlinedAfr(Exception):
    """The wideband AFR channel never changed across the whole log."""


def _rate(row: dict, prev: dict, key: str) -> float:
    dt = row["t"] - prev["t"]
    return abs(row[key] - prev[key]) / dt


def steady_state_rows(rows: list[dict]) -> list[dict]:
    """Keep only rows that are steady-state and warm enough to tune on.

    The first row is dropped because there is no prior sample to diff against.
    A row is dropped if its rpm, map or tps rate of change (against the
    preceding row in ``rows``) exceeds the matching MAX_*_RATE, or if clt is
    below MIN_CLT.
    """
    kept: list[dict] = []
    for i in range(1, len(rows)):
        row, prev = rows[i], rows[i - 1]
        if row["t"] <= prev["t"]:
            continue  # non-increasing timestamp: no valid rate, drop it
        if _rate(row, prev, "rpm") > MAX_RPM_RATE:
            continue
        if _rate(row, prev, "map") > MAX_MAP_RATE:
            continue
        if _rate(row, prev, "tps") > MAX_TPS_RATE:
            continue
        if row["clt"] < MIN_CLT:
            continue
        kept.append(row)
    return kept


def check_afr_plausible(rows: list[dict]) -> None:
    """Raise FlatlinedAfr if every AFR sample equals the first one.

    Checks the whole unfiltered log. An empty log has nothing to flatline.
    """
    if not rows:
        return
    first = rows[0]["afr"]
    if all(abs(r["afr"] - first) <= _AFR_FLATLINE_TOL for r in rows):
        raise FlatlinedAfr("AFR channel is constant across the whole log")


def reject_railed_afr(rows: list[dict]) -> list[dict]:
    """Drop rows whose AFR is at or beyond the sensor rails."""
    return [r for r in rows if AFR_RAIL_LOW < r["afr"] < AFR_RAIL_HIGH]
