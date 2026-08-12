#!/usr/bin/env python3
"""
Practical rusEFI USB MSD check.

Checks both:
1) MCP-level state (connect + `sdmode pc` + output channel assertions)
2) Windows-level storage visibility (USB/volume snapshot assertions)
"""

import argparse
import json
import math
import subprocess
import sys
import time
from pathlib import Path
from typing import Any

REPO_ROOT = Path(__file__).resolve().parents[2]
if str(REPO_ROOT) not in sys.path:
    sys.path.insert(0, str(REPO_ROOT))

from java_console.mcp_python import McpClient

DEFAULT_JAR_CANDIDATES = [
    r"java_console\mcp_ecu\build\libs\mcp_ecu-all.jar",
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


def send_command(mcp: McpClient, cmd: str):
    print(f">>> command: {cmd}", flush=True)
    _structured(mcp.call("command", command=cmd))


def wait_for_channel_at_least(mcp: McpClient, name: str, min_value: float, timeout_s: float, poll_s: float) -> float:
    start = time.monotonic()
    last = math.nan

    while True:
        elapsed = time.monotonic() - start
        if elapsed > timeout_s:
            display = "NaN" if math.isnan(last) else f"{last:.3f}"
            raise AssertionError(
                f"MCP assertion failed: `{name}` did not reach >= {min_value:.3f} in {timeout_s:.1f}s "
                f"(last={display})"
            )

        value = read_channel(mcp, name)
        last = value
        if not math.isnan(value) and value >= min_value:
            return value

        time.sleep(max(0.1, poll_s))


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

    raise FileNotFoundError(
        "Could not find MCP fat jar even after running `./gradlew :mcp_ecu:fatJar`; pass --jar explicitly."
    )


def run_powershell_json(script: str) -> Any:
    cp = subprocess.run(
        [
            "powershell",
            "-NoProfile",
            "-NonInteractive",
            "-ExecutionPolicy",
            "Bypass",
            "-Command",
            script,
        ],
        check=True,
        capture_output=True,
        text=True,
    )

    stdout = cp.stdout.strip()
    if not stdout:
        return {}

    return json.loads(stdout)


def to_list(value: Any) -> list[Any]:
    if value is None:
        return []
    if isinstance(value, list):
        return value
    return [value]


DRIVE_TYPE_MAP: dict[int, str] = {
    0: "Unknown",
    1: "NoRootDirectory",
    2: "Removable",
    3: "Fixed",
    4: "Network",
    5: "CDROM",
    6: "RAMDisk",
}


def humanize_drive_type(volumes: list[dict[str, Any]]) -> list[dict[str, Any]]:
    result: list[dict[str, Any]] = []

    for volume in volumes:
        item = dict(volume)
        drive_type_raw = item.get("DriveType", 0)
        drive_type = int(drive_type_raw or 0)
        item["DriveType"] = DRIVE_TYPE_MAP.get(drive_type, f"Unknown({drive_type})")
        item["DriveTypeCode"] = drive_type
        result.append(item)

    return result


def windows_storage_snapshot() -> dict[str, Any]:
    ps = r"""
$usbDisks = Get-CimInstance Win32_DiskDrive |
  Where-Object { $_.InterfaceType -eq 'USB' } |
  Select-Object DeviceID, Model, PNPDeviceID, Index, Size

$volumes = Get-CimInstance Win32_Volume |
  Where-Object { $_.DriveLetter -ne $null } |
  Select-Object DriveLetter, Label, FileSystem, DriveType

[pscustomobject]@{
  usb_disks = $usbDisks
  volumes = $volumes
} | ConvertTo-Json -Depth 4 -Compress
"""
    raw = run_powershell_json(ps)
    volumes = to_list(raw.get("volumes") if isinstance(raw, dict) else None)

    return {
        "usb_disks": to_list(raw.get("usb_disks") if isinstance(raw, dict) else None),
        "volumes": humanize_drive_type(volumes),
    }


def filter_removable_volumes(volumes: list[dict[str, Any]]) -> list[dict[str, Any]]:
    return [v for v in volumes if int(v.get("DriveTypeCode", v.get("DriveType", 0)) or 0) == 2]


def assert_windows_msd_visible(before: dict[str, Any], after: dict[str, Any], expected_label: str) -> dict[str, Any]:
    before_usb = to_list(before.get("usb_disks"))
    after_usb = to_list(after.get("usb_disks"))

    before_rem = filter_removable_volumes(to_list(before.get("volumes")))
    after_rem = filter_removable_volumes(to_list(after.get("volumes")))

    expected_label_lower = expected_label.strip().lower()
    labeled = [
        v for v in after_rem
        if str(v.get("Label") or "").strip().lower() == expected_label_lower
    ]

    usb_hint = [
        d for d in after_usb
        if "rusefi" in (str(d.get("Model") or "") + " " + str(d.get("PNPDeviceID") or "")).lower()
    ]

    before_letters = {str(v.get("DriveLetter")) for v in before_rem if v.get("DriveLetter")}
    after_letters = {str(v.get("DriveLetter")) for v in after_rem if v.get("DriveLetter")}
    new_letters = sorted(after_letters - before_letters)

    evidence = {
        "before_usb_count": len(before_usb),
        "after_usb_count": len(after_usb),
        "before_removable_count": len(before_rem),
        "after_removable_count": len(after_rem),
        "new_removable_drive_letters": new_letters,
        "matching_label_count": len(labeled),
        "rusefi_usb_hint_count": len(usb_hint),
    }

    if len(labeled) > 0 or len(usb_hint) > 0 or len(new_letters) > 0:
        return evidence

    raise AssertionError(
        "Windows-level assertion failed: no new removable drive, no expected label "
        f"`{expected_label}`, and no obvious rusEFI USB disk identity. Evidence: {evidence}"
    )


def parse_args() -> argparse.Namespace:
    p = argparse.ArgumentParser(description="Practical rusEFI USB MSD check (MCP + Windows assertions)")
    p.add_argument("--jar", default=None, help="Path to MCP server jar (auto-detected if omitted)")
    p.add_argument("--port", default=None, help="Optional explicit COM port for MCP connect (e.g. COM6)")
    p.add_argument("--settle", type=float, default=4.0, help="Wait after `sdmode pc` before host checks")
    p.add_argument("--label", default="rusEFI logs", help="Expected SD card volume label")
    p.add_argument("--report", default="sd_msd_practical_report.json", help="Path to JSON report file")
    return p.parse_args()


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
        "requested_port": args.port,
        "expected_volume_label": args.label,
        "settle_s": args.settle,
        "status": "running",
        "error": None,
        "connection": {},
        "mcp": {},
        "windows": {},
    }

    before = windows_storage_snapshot()
    report["windows"]["before"] = before

    try:
        with McpClient(jar, client_name="sd_msd_practical_check") as mcp:
            connect_args: dict[str, Any] = {}
            if args.port:
                connect_args["port"] = args.port

            print("Connecting to ECU...", flush=True)
            connect_body = _structured(mcp.call("connect", **connect_args))
            connected = bool(connect_body.get("connected", False))
            report["connection"] = {
                "connected": connected,
                "port": connect_body.get("port"),
                "signature": connect_body.get("signature"),
            }
            if not connected:
                raise AssertionError("MCP assertion failed: ECU is not connected")

            sd_present = wait_for_channel_at_least(mcp, "sd_present", min_value=0.5, timeout_s=15.0, poll_s=0.5)

            send_command(mcp, "sdmode pc")
            time.sleep(max(0.5, args.settle))

            sd_msd = wait_for_channel_at_least(mcp, "sd_msd", min_value=0.5, timeout_s=10.0, poll_s=0.5)

            report["mcp"] = {
                "sd_present": None if math.isnan(sd_present) else sd_present,
                "sd_msd_after_sdmode_pc": None if math.isnan(sd_msd) else sd_msd,
            }

        after = windows_storage_snapshot()
        report["windows"]["after"] = after
        report["windows"]["evidence"] = assert_windows_msd_visible(before, after, expected_label=args.label)
        report["status"] = "success"
        print("Practical check passed: MCP and Windows assertions succeeded.", flush=True)
    except Exception as e:
        report["status"] = "failed"
        report["error"] = str(e)
        try:
            report["windows"]["after"] = windows_storage_snapshot()
        except Exception as second_error:
            report["windows"]["after_error"] = str(second_error)
        write_report(args.report, report)
        raise
    else:
        write_report(args.report, report)


if __name__ == "__main__":
    main()
