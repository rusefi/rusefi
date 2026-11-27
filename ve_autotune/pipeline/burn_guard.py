"""Burn-safety diff: detect ECU settings changed since a drive was recorded.

Operates on plain field maps so it needs no tune object. Callers build the
``baseline`` and ``live`` dicts; keys absent from ``live`` are not compared.
"""

from typing import Any


def diff_fields(
    baseline: dict[str, Any],
    live: dict[str, Any],
    ignore: set[str] = {"veTable", "vinNumber"},
) -> list[tuple[str, Any, Any]]:
    """Return ``(key, baseline_value, live_value)`` for each changed field.

    Only keys present in ``baseline`` (and not in ``ignore``) that also exist
    in ``live`` with a differing value are reported.
    """
    changes = []
    for key, baseline_value in baseline.items():
        if key in ignore or key not in live:
            continue
        live_value = live[key]
        if live_value != baseline_value:
            changes.append((key, baseline_value, live_value))
    return changes
