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
from ve_autotune.pipeline.csv_log import MissingChannel
from ve_autotune.pipeline.filters import FlatlinedAfr
from ve_autotune.pipeline.msq import read_all_constants

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
    """Every <constant> field in the .msq, keyed by its real .msq name (e.g.
    "veTable", "globalTriggerAngleOffset") -- a full-tune field dict, not
    just the handful TuneSnapshot models, so the burn-safety diff below can
    catch a change to *any* live ECU field, not only VE/lambda-table ones."""
    return read_all_constants(path)


def _parse_mcp_result(raw: str) -> dict:
    try:
        parsed = json.loads(raw)
    except (TypeError, ValueError) as exc:
        raise RuntimeError(f"could not parse MCP tool response as JSON: {raw!r}") from exc
    if not isinstance(parsed, dict):
        raise RuntimeError(f"unexpected MCP tool response shape: {parsed!r}")
    return parsed


def _read_tune_ok(result: dict) -> tuple[bool, str]:
    """read_tune's own response has no "verified" field (see
    EcuMcpServer.doReadTune) -- only success matters here."""
    if result.get("success") is not True:
        return False, f"read_tune did not report success: {result!r}"
    return True, ""


def _write_tune_ok(result: dict) -> tuple[bool, str]:
    """write_tune's response always carries success/verified/failedFields
    (see EcuMcpServer.doWriteTune / tuneWriteBase) -- anything other than
    success=true, verified=true, and an empty failedFields (if present) is
    a failure, even if no exception was raised."""
    if result.get("success") is not True:
        return False, f"success was not true: {result!r}"
    if result.get("verified") is not True:
        return False, f"verified was not true: {result!r}"
    failed_fields = result.get("failedFields")
    if failed_fields:
        return False, f"failedFields was non-empty: {failed_fields!r}"
    return True, ""


def _read_live_tune_fields(run_mcp: Callable[[str, dict], str]) -> dict:
    """Export the ECU's current live tune to a temp .msq file via `run_mcp`
    (the "read_tune" MCP tool, confirmed in
    java_console/mcp_ecu/src/test/java/com/rusefi/mcp/EcuMcpServerOutputChannelTest.java
    -- it writes the live tune out to the path given), then parse it the same
    way as any other .msq file.

    Raises RuntimeError (never a raw subprocess/JSON exception) if the
    underlying `run_mcp` call fails or reports anything other than success.
    """
    fd, tmp_path = tempfile.mkstemp(suffix=".msq")
    os.close(fd)
    try:
        try:
            raw_result = run_mcp("read_tune", {"path": tmp_path})
        except Exception as exc:
            raise RuntimeError(f"read_tune call failed: {exc}") from exc
        ok, reason = _read_tune_ok(_parse_mcp_result(raw_result))
        if not ok:
            raise RuntimeError(reason)
        return _read_msq_fields(tmp_path)
    finally:
        try:
            os.remove(tmp_path)
        except OSError:
            pass


def _cmd_analyze(args: argparse.Namespace, run_mcp: Callable[[str, dict], str]) -> int:
    try:
        baseline, results, divergences, filter_counts = analyze(args.csv)
    except ClosedLoopActive as exc:
        print(f"ERROR: closed-loop fuel correction is enabled ({exc}); refusing to analyze")
        return 1
    except FlatlinedAfr as exc:
        # Wording matches design.md addendum #3.
        print(f"ERROR: AFR channel looks flatlined/railed -- check wideband wiring ({exc})")
        return 1
    except MissingChannel as exc:
        print(f"ERROR: expected channel '{exc}' not found in a CSV log (check the header names)")
        return 1

    baseline_source_path = _sibling_msq_path(args.csv[-1])
    _, report_path = write_output(baseline, baseline_source_path, results, args.out, filter_counts)

    print(f"Report: {report_path}")
    for divergence in divergences:
        for name, baseline_value, log_value in divergence.fields:
            print(
                f"WARNING: {name} differs: {divergence.log_path} has {log_value!r}, "
                f"baseline {divergence.baseline_path} has {baseline_value!r}"
            )

    not_enough = sum(1 for r in results if r.status == "not_enough_data")
    print(f"Cells with not enough data: {not_enough}")
    return 0


def _cmd_burn(args: argparse.Namespace, run_mcp: Callable[[str, dict], str]) -> int:
    try:
        live_fields = _read_live_tune_fields(run_mcp)
    except Exception as exc:
        print(f"ERROR: could not read the live ECU tune: {exc}")
        return 1

    try:
        msq_fields = _read_msq_fields(args.msq_path)
    except Exception as exc:
        print(f"ERROR: could not read {args.msq_path}: {exc}")
        return 1

    # Finding #5: veTable is excluded from the general diff below by design
    # (it's the field this tool intentionally changes), but that means a
    # hand-edit to VE on the live ECU made *after* recording and *before*
    # burning would otherwise be silently overwritten with no warning.
    # write_output() (analyze.py) writes baseline.msq next to the corrected
    # .msq specifically so this check has something to compare against --
    # the corrected .msq's own veTable is expected to differ, so it can't be
    # used here.
    baseline_msq_path = os.path.join(os.path.dirname(os.path.abspath(args.msq_path)), "baseline.msq")
    if os.path.exists(baseline_msq_path):
        try:
            baseline_fields = _read_msq_fields(baseline_msq_path)
        except Exception as exc:
            print(f"WARNING: could not read {baseline_msq_path} to check for a manual VE edit: {exc}")
        else:
            live_ve = live_fields.get("veTable")
            baseline_ve = baseline_fields.get("veTable")
            if live_ve is not None and baseline_ve is not None and live_ve != baseline_ve:
                print(
                    "WARNING: VE table changed since recording -- the live ECU's veTable differs "
                    "from baseline.msq (the tune this correction was computed against)."
                )
                print("  Someone edited VE on the live ECU after the drive was recorded; burning now")
                print("  will overwrite that hand-edit with this tool's corrected table.")
                answer = input("Burn anyway and overwrite the hand-edited VE table? [yes/no]: ")
                if answer.strip().lower() != "yes":
                    print("Aborted: burn declined.")
                    return 1
    else:
        print(
            f"NOTE: no baseline.msq next to {args.msq_path}; cannot check whether VE was "
            "hand-edited on the live ECU since recording."
        )

    diffs = diff_fields(msq_fields, live_fields)
    if diffs:
        print("The live ECU tune has changed since this tune was built:")
        for name, baseline_value, live_value in diffs:
            print(f"  {name}: baseline={baseline_value!r} live={live_value!r}")
        answer = input("Burn anyway? [yes/no]: ")
        if answer.strip().lower() != "yes":
            print("Aborted: burn declined.")
            return 1

    try:
        raw_result = run_mcp("write_tune", {"path": args.msq_path})
    except Exception as exc:
        print(f"ERROR: write_tune call failed: {exc}")
        return 1

    try:
        ok, reason = _write_tune_ok(_parse_mcp_result(raw_result))
    except RuntimeError as exc:
        print(f"ERROR: burn failed: {exc}")
        return 1
    if not ok:
        print(f"ERROR: burn failed: {reason}")
        return 1

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
