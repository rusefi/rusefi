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
    # Fail closed: only the literal "disabled" means closed-loop fuel
    # correction is off. Any other value -- "enabled", something unexpected,
    # a future ini variant -- must be treated as active, since the whole
    # point of this flag is to refuse computing a correction when the ECU
    # might already be trimming fuel live (design.md addendum #2). The
    # previous `== "enabled"` check defaulted to "inactive" for anything it
    # didn't recognize, which is the unsafe direction.
    fuel_closed_loop_enabled = closed_loop_text.strip().lower() != "disabled"
    return TuneSnapshot(
        ve_table=_grid(_constant_values(root, "veTable")),
        ve_rpm_bins=_constant_values(root, "veRpmBins"),
        ve_load_bins=_constant_values(root, "veLoadBins"),
        lambda_table=_grid(_constant_values(root, "lambdaTable")),
        lambda_rpm_bins=_constant_values(root, "lambdaRpmBins"),
        lambda_load_bins=_constant_values(root, "lambdaLoadBins"),
        fuel_closed_loop_enabled=fuel_closed_loop_enabled,
    )


def read_all_constants(path: str) -> dict[str, str]:
    """Return every ``<constant name="X">...</constant>`` tag in the file as
    ``{name: raw_text}``, not just the handful ``TuneSnapshot`` models.

    Values are only whitespace-stripped, not quote-stripped (unlike the
    closed-loop-flag lookup in ``load_tune``): raw-text equality is
    intentionally conservative for a field-by-field diff -- a quoted vs.
    unquoted mismatch would just mean "report it as different," which is the
    safe direction.
    """
    root = ET.parse(path).getroot()
    constants: dict[str, str] = {}
    for el in root.iter():
        if el.tag.split("}")[-1] == "constant":
            name = el.get("name")
            if name is not None and el.text is not None:
                constants[name] = el.text.strip()
    return constants


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
    digits_match = re.search(r"\bdigits=\"(\d+)\"", match.group(1))
    digits = int(digits_match.group(1)) if digits_match else 1
    body = match.group(2)
    # Keep the original leading newline and closing indentation; replace only the value rows.
    rows = "".join(
        "         " + " ".join(f"{float(v):.{digits}f}" for v in row) + " \n" for row in ve_table
    )
    new_body = "\n" + rows + body.rsplit("\n", 1)[-1]
    text = text[: match.start(2)] + new_body + text[match.end(2) :]
    with open(dest_path, "wb") as f:
        f.write(text.encode(ENCODING))
