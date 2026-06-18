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
from pathlib import Path
import sys
import time
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[2]
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))

from java_console.mcp_python import McpClient

DEFAULT_JAR = r"java_console\mcp_ecu\build\libs\mcp_ecu-all.jar"
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


def wait_for_format_done(mcp: McpClient, timeout_s: float, poll_s: float) -> tuple[float, bool]:
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

        print(
            f"  t={elapsed:6.1f}s  sd_formating={'NaN' if math.isnan(formatting) else f'{formatting:.3f}'}",
            flush=True,
        )
        time.sleep(poll_s)


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Query/format SD card via rusEFI ECU MCP")
    p.add_argument("--jar", default=DEFAULT_JAR, help="Path to mcp_ecu all-in-one jar")
    p.add_argument("--timeout", type=float, default=180.0, help="Format completion timeout in seconds")
    p.add_argument("--poll", type=float, default=1.0, help="Polling interval in seconds")
    return p.parse_args()


def main():
    args = parse_args()
    with McpClient(args.jar, client_name="sd_mcp_test") as mcp:
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
