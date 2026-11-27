import csv


class MissingChannel(Exception):
    """A mapped channel header was not found in the CSV log."""


def _normalize(header: str) -> str:
    return header.strip().casefold()


def load_csv_rows(path: str, channel_map: dict[str, str]) -> list[dict[str, float]]:
    """Read a CSV log and return one dict per row keyed by logical channel name.

    ``channel_map`` maps a logical name (e.g. "rpm") to the CSV header text for
    that channel. Header matching ignores case and surrounding whitespace.
    Raises MissingChannel(logical_name) if a mapped header is absent.
    """
    with open(path, newline="") as f:
        reader = csv.DictReader(f)
        lookup = {_normalize(h): h for h in (reader.fieldnames or [])}

        columns: dict[str, str] = {}
        for logical, header_text in channel_map.items():
            actual = lookup.get(_normalize(header_text))
            if actual is None:
                raise MissingChannel(logical)
            columns[logical] = actual

        return [
            {logical: float(row[actual]) for logical, actual in columns.items()}
            for row in reader
        ]
