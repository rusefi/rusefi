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


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Query/format SD card via rusEFI ECU MCP")
    p.add_argument("--jar", default=None, help="Path to MCP server jar (auto-detected if omitted)")
    p.add_argument("--timeout", type=float, default=180.0, help="Format completion timeout in seconds")
    p.add_argument("--poll", type=float, default=1.0, help="Polling interval in seconds")
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


def main():
    args = parse_args()
    jar = resolve_jar_path(args.jar)
    print(f"Using MCP jar: {jar}", flush=True)

    with McpClient(jar, client_name="sd_mcp_test") as mcp:
        print("Connecting to ECU...", flush=True)
        _structured(mcp.call("connect"))

        dump_status(mcp, DEFAULT_CHANNELS, "Initial SD status")

        send_command(mcp, "sdmode ecu")
        time.sleep(2.0)
        dump_status(mcp, DEFAULT_CHANNELS, "After sdmode ecu")

        send_command(mcp, "sdmode format")
        format_elapsed_s, saw_formatting_active = wait_for_format_done(mcp, args.timeout, args.poll)
        print(
            f"\nFormat finished in {format_elapsed_s:.2f}s"
            f" (observed active formatting state: {saw_formatting_active})",
            flush=True,
        )

        dump_status(mcp, DEFAULT_CHANNELS, "After format completion")

        send_command(mcp, "sdmode pc")
        time.sleep(2.0)
        dump_status(mcp, DEFAULT_CHANNELS, "After sdmode pc")

        print("\nDone.", flush=True)


if __name__ == "__main__":
    main()
