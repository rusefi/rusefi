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

# Cranking filter (design.md "Decided design": reject rows during cranking).
# Below this RPM the engine is being cranked, not running -- without this
# filter a cranking sample aliases into the lowest RPM bin and biases it
# lean, since cranking fueling is intentionally very different from the
# running VE table. Adjustable; 500 rpm is comfortably above starter-motor
# cranking speed and below any realistic idle for this engine.
MIN_RUNNING_RPM = 500.0  # rpm

# DFCO (decel fuel cut) filter (design.md "Decided design": reject rows
# during decel fuel cut). The injector is cut during DFCO, so any AFR
# reading then reflects unburned residual mixture / exhaust-gas dilution,
# not the VE table's fueling. Closed throttle combined with low manifold
# pressure is the signature of a decel event. Both thresholds are
# adjustable starting points, not measured from a real engine yet.
DFCO_TPS_MAX = 2.0  # % throttle; at/below this is "closed throttle"
DFCO_MAP_MAX = 35.0  # kPa; at/below this with closed throttle looks like DFCO

_AFR_FLATLINE_TOL = 1e-6


class FlatlinedAfr(Exception):
    """The wideband AFR channel never changed across the whole log."""


def _rate(row: dict, prev: dict, key: str) -> float:
    dt = row["t"] - prev["t"]
    return abs(row[key] - prev[key]) / dt


def reject_transient_rows(rows: list[dict]) -> list[dict]:
    """Keep only rows whose rpm/map/tps rate of change (against the
    preceding row in ``rows``) doesn't exceed the matching MAX_*_RATE.

    The first row is dropped because there is no prior sample to diff
    against.
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
        kept.append(row)
    return kept


def reject_cold_rows(rows: list[dict]) -> list[dict]:
    """Drop rows where clt is below MIN_CLT (engine still warming up)."""
    return [r for r in rows if r["clt"] >= MIN_CLT]


def steady_state_rows(rows: list[dict]) -> list[dict]:
    """Keep only rows that are steady-state and warm enough to tune on.

    Composes `reject_transient_rows` then `reject_cold_rows`; kept as a
    single entry point for existing callers/tests, since most of this
    module's history treated "steady-state" as one combined concept.
    """
    return reject_cold_rows(reject_transient_rows(rows))


def reject_cranking_rows(rows: list[dict]) -> list[dict]:
    """Drop rows below MIN_RUNNING_RPM -- cranking, not running."""
    return [r for r in rows if r["rpm"] >= MIN_RUNNING_RPM]


def reject_dfco_rows(rows: list[dict]) -> list[dict]:
    """Drop rows that look like decel fuel cut: closed throttle (tps at or
    below DFCO_TPS_MAX) combined with low manifold pressure (map at or below
    DFCO_MAP_MAX).
    """
    return [r for r in rows if not (r["tps"] <= DFCO_TPS_MAX and r["map"] <= DFCO_MAP_MAX)]


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
