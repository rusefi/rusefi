"""Map a log sample onto the nearest VE table cell."""


def _nearest_index(value: float, bins: list[float]) -> int:
    return min(range(len(bins)), key=lambda i: abs(bins[i] - value))


def nearest_cell(
    rpm: float,
    map_kpa: float,
    rpm_bins: list[float],
    load_bins: list[float],
) -> tuple[int, int]:
    """Return ``(row, col)`` of the VE cell nearest to ``(rpm, map_kpa)``.

    ``row`` indexes ``load_bins`` (Y axis) and ``col`` indexes ``rpm_bins``
    (X axis). Out-of-range values clamp to the first or last bin.
    """
    col = _nearest_index(rpm, rpm_bins)
    row = _nearest_index(map_kpa, load_bins)
    return row, col
