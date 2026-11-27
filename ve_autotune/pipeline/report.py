"""Render VE correction results as a Markdown report."""

from ve_autotune.pipeline.correction import CellResult

HEADERS = [
    "row",
    "col",
    "old VE",
    "new VE",
    "Δ%",
    "samples",
    "measured AFR",
    "target AFR",
    "status",
]


def _format_row(r: CellResult) -> str:
    cells = [
        str(r.row),
        str(r.col),
        f"{r.old_ve:.1f}",
        f"{r.new_ve:.1f}",
        f"{r.delta_pct:.1f}",
        str(r.sample_count),
        f"{r.measured_afr:.2f}" if r.status == "corrected" else "-",
        f"{r.target_afr:.2f}",
        r.status,
    ]
    return "| " + " | ".join(cells) + " |"


_FILTER_HEADERS = ["log", "total rows", "transient", "cold", "cranking", "dfco", "railed afr", "kept"]


def _format_filter_row(fc) -> str:
    # Duck-typed on purpose (log_path/total_rows/rejected_*/kept attributes)
    # to avoid importing analyze.py's FilterCounts here, which would create
    # a circular import (analyze.py already imports render_report).
    cells = [
        fc.log_path,
        fc.total_rows,
        fc.rejected_transient,
        fc.rejected_cold,
        fc.rejected_cranking,
        fc.rejected_dfco,
        fc.rejected_railed,
        fc.kept,
    ]
    return "| " + " | ".join(str(c) for c in cells) + " |"


def render_report(results: list[CellResult], filter_counts: list | None = None) -> str:
    """Return a Markdown report: a one-line summary, then a table of cells.

    If ``filter_counts`` is given (one entry per input log, duck-typed with
    ``log_path``/``total_rows``/``rejected_transient``/``rejected_cold``/
    ``rejected_cranking``/``rejected_dfco``/``rejected_railed``/``kept``
    attributes -- see `analyze.FilterCounts`), a second table is appended
    explaining *why* a log contributed few or no samples, so a report
    showing e.g. "0/256 corrected" isn't silent about the cause.
    """
    corrected = sum(1 for r in results if r.status == "corrected")
    not_enough = sum(1 for r in results if r.status == "not_enough_data")

    lines = [
        f"{corrected}/{len(results)} cells corrected, {not_enough} not enough data",
        "",
        "| " + " | ".join(HEADERS) + " |",
        "| " + " | ".join("---" for _ in HEADERS) + " |",
    ]
    lines.extend(_format_row(r) for r in results)

    if filter_counts:
        lines.append("")
        lines.append("## Rows filtered per log")
        lines.append("")
        lines.append("| " + " | ".join(_FILTER_HEADERS) + " |")
        lines.append("| " + " | ".join("---" for _ in _FILTER_HEADERS) + " |")
        lines.extend(_format_filter_row(fc) for fc in filter_counts)

    return "\n".join(lines) + "\n"
