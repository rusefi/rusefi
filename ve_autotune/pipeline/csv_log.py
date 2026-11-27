import csv
import re

# The real convert_log_to_csv tool (MslToCsv.java's `heading(name, units)`)
# names each CSV column "<datalog label>" when the field has no units, or
# "<datalog label> (<units>)" when it does (e.g. "RPM (RPM)", "MAP (kPa)").
# Strip a trailing " (...)" unit suffix before matching so a bare channel
# name and a "Label (units)" header both resolve to the same logical name.
_UNIT_SUFFIX_RE = re.compile(r"\s*\([^()]*\)\s*$")


class MissingChannel(Exception):
    """A mapped channel header was not found in the CSV log."""


def _normalize(header: str) -> str:
    header = _UNIT_SUFFIX_RE.sub("", header.strip())
    return header.strip().casefold()


def load_csv_rows(path: str, channel_map: dict[str, str]) -> list[dict[str, float]]:
    """Read a CSV log and return one dict per row keyed by logical channel name.

    ``channel_map`` maps a logical name (e.g. "rpm") to the CSV header text for
    that channel. Header matching ignores case, surrounding whitespace, and a
    trailing " (units)" suffix on the CSV's own header (so "RPM" in
    ``channel_map`` matches a CSV column named either "RPM" or "RPM (RPM)").
    Raises MissingChannel(logical_name) if a mapped header is absent, or
    ValueError if two CSV headers normalize to the same name (ambiguous).
    """
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        headers_by_normalized: dict[str, list[str]] = {}
        for h in reader.fieldnames or []:
            headers_by_normalized.setdefault(_normalize(h), []).append(h)

        duplicates = {norm: hs for norm, hs in headers_by_normalized.items() if len(hs) > 1}
        if duplicates:
            detail = "; ".join(f"{hs!r} all normalize to {norm!r}" for norm, hs in duplicates.items())
            raise ValueError(f"ambiguous CSV headers after normalization: {detail}")
        lookup = {norm: hs[0] for norm, hs in headers_by_normalized.items()}

        columns: dict[str, str] = {}
        for logical, header_text in channel_map.items():
            actual = lookup.get(_normalize(header_text))
            if actual is None:
                raise MissingChannel(logical)
            columns[logical] = actual

        rows: list[dict[str, float]] = []
        for row_num, row in enumerate(reader, start=1):
            parsed: dict[str, float] = {}
            for logical, actual in columns.items():
                raw_value = row[actual]
                try:
                    parsed[logical] = float(raw_value)
                except (TypeError, ValueError) as exc:
                    raise ValueError(
                        f"row {row_num}: could not parse channel '{logical}' "
                        f"(CSV column {actual!r}) value {raw_value!r} as a number"
                    ) from exc
            rows.append(parsed)
        return rows
