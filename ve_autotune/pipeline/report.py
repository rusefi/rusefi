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


def render_report(results: list[CellResult]) -> str:
    """Return a Markdown report: a one-line summary, then a table of cells."""
    corrected = sum(1 for r in results if r.status == "corrected")
    not_enough = sum(1 for r in results if r.status == "not_enough_data")

    lines = [
        f"{corrected}/{len(results)} cells corrected, {not_enough} not enough data",
        "",
        "| " + " | ".join(HEADERS) + " |",
        "| " + " | ".join("---" for _ in HEADERS) + " |",
    ]
    lines.extend(_format_row(r) for r in results)
    return "\n".join(lines) + "\n"
