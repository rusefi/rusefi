"""CLI for the VE autotune pipeline: analyze drive logs, then burn a reviewed
tune to the ECU.

Talks to the ECU only through an injectable ``run_mcp`` callable so tests
never need a real connection. ``run_mcp(tool_name, args) -> json_str`` mirrors
calling one tool via ``mcp_drive.py`` (which takes a JSON list of
``[tool_name, args]`` pairs -- see that file's docstring).
"""

import argparse
import json
import os
import subprocess
import sys
import tempfile
from typing import Callable

from ve_autotune.pipeline.analyze import ClosedLoopActive, _sibling_msq_path, analyze, write_output
from ve_autotune.pipeline.burn_guard import diff_fields
from ve_autotune.pipeline.msq import load_tune

# Map TuneSnapshot's Python attribute names to the real .msq constant names
# that diff_fields()'s default `ignore` set (and the rest of the ECU tooling)
# uses, so e.g. the VE table is correctly ignored as "veTable", not
# "ve_table". TuneSnapshot only models the handful of fields VE autotune
# cares about (see msq.py) -- this is therefore a partial field diff, not the
# full-tune diff design.md's burn-safety guard describes; see Task 9 report.
_MSQ_FIELD_NAMES = {
    "ve_table": "veTable",
    "ve_rpm_bins": "veRpmBins",
    "ve_load_bins": "veLoadBins",
    "lambda_table": "lambdaTable",
    "lambda_rpm_bins": "lambdaRpmBins",
    "lambda_load_bins": "lambdaLoadBins",
    "fuel_closed_loop_enabled": "fuelClosedLoopCorrectionEnabled",
}

_REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_MCP_DRIVE = os.path.join(_REPO_ROOT, "mcp_drive.py")


def _default_run_mcp(tool: str, args: dict) -> str:
    """Shell out to mcp_drive.py for one tool call, exactly as used elsewhere
    this session. Not exercised by any test (every test replaces run_mcp)."""
    calls_json = json.dumps([[tool, args]])
    result = subprocess.run(
        [sys.executable, _MCP_DRIVE, calls_json],
        capture_output=True,
        text=True,
        check=True,
        cwd=_REPO_ROOT,
    )
    lines = result.stdout.strip().splitlines()
    return lines[-1] if lines else ""


def _read_msq_fields(path: str) -> dict:
    snapshot = load_tune(path)
    return {msq_name: getattr(snapshot, attr) for attr, msq_name in _MSQ_FIELD_NAMES.items()}


def _read_live_tune_fields(run_mcp: Callable[[str, dict], str]) -> dict:
    """Export the ECU's current live tune to a temp .msq file via `run_mcp`
    (the "read_tune" MCP tool, confirmed in
    java_console/mcp_ecu/src/test/java/com/rusefi/mcp/EcuMcpServerOutputChannelTest.java
    -- it writes the live tune out to the path given), then parse it the same
    way as any other .msq file."""
    fd, tmp_path = tempfile.mkstemp(suffix=".msq")
    os.close(fd)
    try:
        run_mcp("read_tune", {"path": tmp_path})
        return _read_msq_fields(tmp_path)
    finally:
        try:
            os.remove(tmp_path)
        except OSError:
            pass


def _cmd_analyze(args: argparse.Namespace, run_mcp: Callable[[str, dict], str]) -> int:
    try:
        baseline, results, divergences = analyze(args.csv)
    except ClosedLoopActive as exc:
        print(f"ERROR: closed-loop fuel correction is enabled ({exc}); refusing to analyze")
        return 1

    baseline_source_path = _sibling_msq_path(args.csv[-1])
    _, report_path = write_output(baseline, baseline_source_path, results, args.out)

    print(f"Report: {report_path}")
    for divergence in divergences:
        for name, earlier, later in divergence.fields:
            print(f"WARNING: {name} differs between logs: {earlier!r} -> {later!r}")

    not_enough = sum(1 for r in results if r.status == "not_enough_data")
    print(f"Cells with not enough data: {not_enough}")
    return 0


def _cmd_burn(args: argparse.Namespace, run_mcp: Callable[[str, dict], str]) -> int:
    live_fields = _read_live_tune_fields(run_mcp)
    msq_fields = _read_msq_fields(args.msq_path)
    diffs = diff_fields(msq_fields, live_fields)

    if diffs:
        print("The live ECU tune has changed since this tune was built:")
        for name, baseline_value, live_value in diffs:
            print(f"  {name}: baseline={baseline_value!r} live={live_value!r}")
        answer = input("Burn anyway? [yes/no]: ")
        if answer.strip().lower() != "yes":
            print("Aborted: burn declined.")
            return 1

    run_mcp("write_tune", {"path": args.msq_path})
    print(f"Wrote tune: {args.msq_path}")
    return 0


def main(argv: list[str], run_mcp: Callable[[str, dict], str] = _default_run_mcp) -> int:
    parser = argparse.ArgumentParser(prog="ve_autotune")
    subparsers = parser.add_subparsers(dest="command", required=True)

    analyze_parser = subparsers.add_parser("analyze", help="analyze drive logs into a corrected tune")
    analyze_parser.add_argument("csv", nargs="+", help="CSV log path(s); each needs a sibling .msq")
    analyze_parser.add_argument("--out", required=True, help="output directory for report + corrected .msq")

    burn_parser = subparsers.add_parser("burn", help="burn a reviewed .msq to the ECU")
    burn_parser.add_argument("msq_path", help="path to the .msq file to write to the ECU")

    args = parser.parse_args(argv)

    if args.command == "analyze":
        return _cmd_analyze(args, run_mcp)
    return _cmd_burn(args, run_mcp)


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
