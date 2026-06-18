#!/usr/bin/env python3
"""
SD-card MCP smoke script.

Sequence:
1) query SD output channels
2) request `sdmode ecu`
3) request `sdmode format`
4) measure format duration and wait until formatting is over
5) request `sdmode pc`

Run from repository root so `java_console.mcp_python` is importable.
"""

import argparse
import json
import math
import subprocess
from pathlib import Path
import sys
import time
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[2]
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))

from java_console.mcp_python import McpClient

DEFAULT_JAR_CANDIDATES = [
    r"java_console\mcp_ecu\build\libs\mcp_ecu-all.jar",
]
DEFAULT_CHANNELS = [
    "sd_present",
    "sd_error",
    "sd_formating",
    "sdLoggingState",
    "sd_logging_internal",
    "sd_active_wr",
    "sd_active_rd",
]


def _structured(resp: dict[str, Any]) -> dict[str, Any]:
    if "error" in resp:
        raise RuntimeError(f"MCP error: {resp['error']}")
    return resp.get("result", {}).get("structuredContent", {})


def read_channel(mcp: McpClient, name: str) -> float:
    body = _structured(mcp.call("read_output_channel", name=name))
    if not body.get("found", False):
        return math.nan
    value = body.get("value")
    return float(value) if value is not None else math.nan


def dump_status(mcp: McpClient, channels: list[str], title: str):
    print(f"\n=== {title} ===", flush=True)
    for ch in channels:
        value = read_channel(mcp, ch)
        display = "NaN" if math.isnan(value) else f"{value:.3f}"
        print(f"{ch:20s} {display}", flush=True)


def snapshot_status(mcp: McpClient, channels: list[str]) -> dict[str, float | None]:
    values: dict[str, float | None] = {}
    for ch in channels:
        value = read_channel(mcp, ch)
        values[ch] = None if math.isnan(value) else value
    return values


def send_command(mcp: McpClient, cmd: str):
    print(f"\n>>> command: {cmd}", flush=True)
    _structured(mcp.call("command", command=cmd))


def wait_for_format_done(
    mcp: McpClient,
    timeout_s: float,
    poll_s: float,
    no_activity_done_s: float = 5.0,
) -> tuple[float, bool]:
    print("\nWaiting for format to complete (watching output channel `sd_formating`)...", flush=True)
    start = time.monotonic()
    seen_active = False

    while True:
        elapsed = time.monotonic() - start
        if elapsed > timeout_s:
            raise TimeoutError(f"Timed out after {timeout_s:.1f}s waiting for format completion")

        formatting = read_channel(mcp, "sd_formating")
        if not math.isnan(formatting) and formatting > 0.5:
            seen_active = True

        if seen_active and not math.isnan(formatting) and formatting < 0.5:
            return elapsed, True

        if not seen_active and not math.isnan(formatting) and formatting < 0.5 and elapsed >= no_activity_done_s:
            return elapsed, False

        print(
            f"  t={elapsed:6.1f}s  sd_formating={'NaN' if math.isnan(formatting) else f'{formatting:.3f}'}",
            flush=True,
        )
        time.sleep(poll_s)


def wait_for_sd_present(
    mcp: McpClient,
    timeout_s: float,
    poll_s: float,
) -> float:
    print("\nChecking SD presence (watching output channel `sd_present`)...", flush=True)
    start = time.monotonic()
    last_value = math.nan

    while True:
        elapsed = time.monotonic() - start
        if elapsed > timeout_s:
            display = "NaN" if math.isnan(last_value) else f"{last_value:.3f}"
            raise RuntimeError(
                "SD card is not present after "
                f"{timeout_s:.1f}s (`sd_present` last value: {display})"
            )

        value = read_channel(mcp, "sd_present")
        last_value = value
        print(
            f"  t={elapsed:6.1f}s  sd_present={'NaN' if math.isnan(value) else f'{value:.3f}'}",
            flush=True,
        )
        if not math.isnan(value) and value >= 0.5:
            return value

        time.sleep(poll_s)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Query/format SD card via rusEFI ECU MCP")
    p.add_argument("--jar", default=None, help="Path to MCP server jar (auto-detected if omitted)")
    p.add_argument("--port", default=None, help="Optional explicit serial COM port for MCP connect (e.g. COM6)")
    p.add_argument("--timeout", type=float, default=180.0, help="Format completion timeout in seconds")
    p.add_argument("--poll", type=float, default=1.0, help="Polling interval in seconds")
    p.add_argument("--report", default="sd_mcp_report.json", help="Path to JSON report file")
    return p.parse_args()


def resolve_jar_path(cli_jar: str | None) -> str:
    if cli_jar:
        if Path(cli_jar).exists():
            return cli_jar
        raise FileNotFoundError(f"MCP jar does not exist: {cli_jar}")

    for candidate in DEFAULT_JAR_CANDIDATES:
        if Path(candidate).exists():
            return candidate

    print("MCP fat jar not found; building with Gradle task :mcp_ecu:fatJar ...", flush=True)
    subprocess.run([".\\gradlew", ":mcp_ecu:fatJar"], check=True)

    for candidate in DEFAULT_JAR_CANDIDATES:
        if Path(candidate).exists():
            return candidate

    candidates = "\n  - ".join(DEFAULT_JAR_CANDIDATES)
    raise FileNotFoundError(
        "Could not find MCP fat jar even after running `./gradlew :mcp_ecu:fatJar`; pass --jar explicitly.\n"
        f"Checked:\n  - {candidates}"
    )


def write_report(report_path: str, report: dict[str, Any]):
    path = Path(report_path)
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(json.dumps(report, indent=2), encoding="utf-8")
    print(f"Report written to: {path}", flush=True)


def main():
    args = parse_args()
    jar = resolve_jar_path(args.jar)
    print(f"Using MCP jar: {jar}", flush=True)

    report: dict[str, Any] = {
        "started_at_epoch_s": time.time(),
        "jar": jar,
        "connect_port": args.port,
        "timeout_s": args.timeout,
        "poll_s": args.poll,
        "status": "running",
        "error": None,
        "connection": {},
        "format_elapsed_s": None,
        "observed_active_formatting": None,
        "snapshots": {},
    }

    try:
        with McpClient(jar, client_name="sd_mcp_test") as mcp:
            print("Connecting to ECU...", flush=True)
            connect_args: dict[str, Any] = {}
            if args.port:
                connect_args["port"] = args.port
            connect_body = _structured(mcp.call("connect", **connect_args))
            report["connection"] = {
                "requested_port": args.port,
                "connected": bool(connect_body.get("connected", False)),
                "port": connect_body.get("port"),
                "signature": connect_body.get("signature"),
            }

            dump_status(mcp, DEFAULT_CHANNELS, "Initial SD status")
            report["snapshots"]["initial"] = snapshot_status(mcp, DEFAULT_CHANNELS)

            wait_for_sd_present(mcp, timeout_s=max(5.0, min(20.0, args.timeout)), poll_s=max(0.5, args.poll))
            report["snapshots"]["after_sd_presence_wait"] = snapshot_status(mcp, DEFAULT_CHANNELS)

            send_command(mcp, "sdmode ecu")
            time.sleep(2.0)
            dump_status(mcp, DEFAULT_CHANNELS, "After sdmode ecu")
            report["snapshots"]["after_sdmode_ecu"] = snapshot_status(mcp, DEFAULT_CHANNELS)

            send_command(mcp, "sdmode format")
            format_elapsed_s, saw_formatting_active = wait_for_format_done(mcp, args.timeout, args.poll)
            report["format_elapsed_s"] = format_elapsed_s
            report["observed_active_formatting"] = saw_formatting_active
            print(
                f"\nFormat finished in {format_elapsed_s:.2f}s"
                f" (observed active formatting state: {saw_formatting_active})",
                flush=True,
            )

            dump_status(mcp, DEFAULT_CHANNELS, "After format completion")
            report["snapshots"]["after_format_completion"] = snapshot_status(mcp, DEFAULT_CHANNELS)

            send_command(mcp, "sdmode pc")
            time.sleep(2.0)
            dump_status(mcp, DEFAULT_CHANNELS, "After sdmode pc")
            report["snapshots"]["after_sdmode_pc"] = snapshot_status(mcp, DEFAULT_CHANNELS)

            print("\nDone.", flush=True)
            report["status"] = "success"
    except Exception as e:
        report["status"] = "failed"
        report["error"] = str(e)
        write_report(args.report, report)
        raise
    else:
        write_report(args.report, report)


if __name__ == "__main__":
    main()
