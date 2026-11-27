"""Read and write rusEFI .msq tune files (XML), limited to the tables VE autotune uses."""

import re
import xml.etree.ElementTree as ET
from dataclasses import dataclass

ENCODING = "latin-1"  # rusEFI .msq files are ISO-8859-1; latin-1 round-trips bytes exactly
GRID = 16


@dataclass
class TuneSnapshot:
    ve_table: list[list[float]]
    ve_rpm_bins: list[float]
    ve_load_bins: list[float]
    lambda_table: list[list[float]]
    lambda_rpm_bins: list[float]
    lambda_load_bins: list[float]
    fuel_closed_loop_enabled: bool


def _constant_values(root: ET.Element, name: str) -> list[float]:
    for el in root.iter():
        if el.tag.split("}")[-1] == "constant" and el.get("name") == name:
            return [float(v) for v in el.text.split()]
    raise ValueError(f"constant '{name}' not found")


def _grid(values: list[float]) -> list[list[float]]:
    return [values[i : i + GRID] for i in range(0, len(values), GRID)]


def load_tune(path: str) -> TuneSnapshot:
    root = ET.parse(path).getroot()
    closed_loop_text = None
    for el in root.iter():
        if el.tag.split("}")[-1] == "constant" and el.get("name") == "fuelClosedLoopCorrectionEnabled":
            closed_loop_text = el.text.strip().strip('"')
            break
    if closed_loop_text is None:
        raise ValueError("constant 'fuelClosedLoopCorrectionEnabled' not found")
    return TuneSnapshot(
        ve_table=_grid(_constant_values(root, "veTable")),
        ve_rpm_bins=_constant_values(root, "veRpmBins"),
        ve_load_bins=_constant_values(root, "veLoadBins"),
        lambda_table=_grid(_constant_values(root, "lambdaTable")),
        lambda_rpm_bins=_constant_values(root, "lambdaRpmBins"),
        lambda_load_bins=_constant_values(root, "lambdaLoadBins"),
        fuel_closed_loop_enabled=closed_loop_text == "enabled",
    )


def _constant_block_pattern(name: str) -> re.Pattern:
    return re.compile(r"(<constant\b[^>]*\bname=\"%s\"[^>]*>)(.*?)(</constant>)" % re.escape(name), re.S)


def write_tune_with_ve_table(source_path: str, ve_table: list[list[float]], dest_path: str) -> None:
    if len(ve_table) != GRID or any(len(row) != GRID for row in ve_table):
        raise ValueError(f"ve_table must be {GRID}x{GRID}")
    with open(source_path, "rb") as f:
        text = f.read().decode(ENCODING)
    match = _constant_block_pattern("veTable").search(text)
    if match is None:
        raise ValueError("constant 'veTable' not found")
    body = match.group(2)
    # Keep the original leading newline and closing indentation; replace only the value rows.
    rows = "".join(
        "         " + " ".join(repr(float(v)) for v in row) + " \n" for row in ve_table
    )
    new_body = "\n" + rows + body.rsplit("\n", 1)[-1]
    text = text[: match.start(2)] + new_body + text[match.end(2) :]
    with open(dest_path, "wb") as f:
        f.write(text.encode(ENCODING))
