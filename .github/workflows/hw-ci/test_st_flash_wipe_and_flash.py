"""Exercise the flash wrapper without accessing USB or a real target.

Run with: python3 .github/workflows/hw-ci/test_st_flash_wipe_and_flash.py
"""

import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest


SCRIPT = Path(__file__).with_name("st_flash_wipe_and_flash.sh")
MOCK = r'''
import json
import os
from pathlib import Path
import sys

args = sys.argv[1:]
with open(os.environ["MOCK_CALLS"], "a") as log:
    log.write(json.dumps([Path(sys.argv[0]).name, *args]) + "\n")
if Path(sys.argv[0]).name == "st-info":
    print("Found 1 stlink programmers")
    sys.exit(int(os.environ.get("MOCK_PROBE_STATUS", "0")))
if args == ["--version"]:
    print("v1.8.0")
    sys.exit(0)
operation = "erase" if "erase" in args else "write"
counter = Path(os.environ["MOCK_CALLS"] + operation)
attempt = int(counter.read_text()) if counter.exists() else 0
counter.write_text(str(attempt + 1))
responses = json.loads(os.environ["MOCK_RESPONSES"]).get(operation, [[0, "success"]])
status, output = responses[min(attempt, len(responses) - 1)]
print(output, file=sys.stderr)
sys.exit(status)
'''


class FlashDiagnosticsTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        for name in ("st-flash", "st-info"):
            tool = self.root / name
            tool.write_text(f"#!{sys.executable}\n{MOCK}")
            tool.chmod(0o755)
        self.image = self.root / "image with spaces.bin"
        self.image.write_bytes(b"mock image")

    def run_flash(self, responses, **env):
        calls = self.root / "calls"
        result = subprocess.run(
            ["bash", str(SCRIPT), str(self.image)],
            env={**os.environ, "PATH": f"{self.root}:{os.environ['PATH']}",
                 "ST_FLASH_ATTEMPTS": "2", "ST_FLASH_RETRY_DELAY": "0",
                 "HARDWARE_CI_STLINK_SERIAL": "test-serial",
                 "MOCK_CALLS": str(calls), "MOCK_RESPONSES": json.dumps(responses),
                 **env},
            stdout=subprocess.PIPE, stderr=subprocess.STDOUT, text=True, timeout=10,
        )
        self.calls = [json.loads(line) for line in calls.read_text().splitlines()]
        return result

    def test_preserves_exit_status(self):
        result = self.run_flash({"write": [[42, "Unknown memory region"]]})
        self.assertEqual(result.returncode, 1)
        self.assertIn("write: failed (st-flash exit 42", result.stdout)
        self.assertIn("st-flash rejected the address", result.stdout)
        self.assertEqual(sum(call[0] == "st-info" for call in self.calls), 2)

    def test_reset_failure_rejects_zero_exit_status(self):
        result = self.run_flash({"erase": [[0, "Mass erase completed successfully.\nFailed to reset device"]]})
        self.assertEqual(result.returncode, 1)
        self.assertIn("erase: failed (st-flash exit 0, reported failure: true)", result.stdout)
        self.assertNotIn("erase: OK", result.stdout)
        self.assertFalse(any("write" in call for call in self.calls))

    def test_zero_flash_size(self):
        result = self.run_flash({"erase": [[0, "STM32F4x5_F4x7: 192 KiB SRAM, 0 KiB flash in at least 16 KiB pages."]]})
        self.assertEqual(result.returncode, 1)
        self.assertIn("target reports zero flash size", result.stdout)
        self.assertNotIn("Done!", result.stdout)

    def test_soft_reset_timeout(self):
        result = self.run_flash({"write": [[0, "ERROR common.c: Soft reset failed: timeout"]]})
        self.assertEqual(result.returncode, 1)
        self.assertIn("target reset failed despite connect-under-reset", result.stdout)

    def test_retry_recovers_and_clears_previous_diagnostics(self):
        result = self.run_flash({"erase": [[0, "Failed to reset device"], [0, "Mass erase completed successfully."]]})
        self.assertEqual(result.returncode, 0)
        self.assertIn("erase: attempt 2 of 2", result.stdout)
        self.assertIn("Done!", result.stdout)
        writes = [call for call in self.calls if "write" in call]
        self.assertEqual(len(writes), 1)
        self.assertEqual(writes[0], ["st-flash", "--serial", "test-serial",
                                    "--connect-under-reset", "--reset", "--format=binary",
                                    "write", str(self.image), "0x08000000"])

    def test_probe_failure_does_not_hide_original_failure(self):
        result = self.run_flash({"erase": [[17, "USB connection failed"]]}, MOCK_PROBE_STATUS="9")
        self.assertEqual(result.returncode, 1)
        self.assertIn("st-flash exit 17", result.stdout)
        self.assertIn("st-info --probe failed (exit 9)", result.stdout)
        self.assertIn("giving up after 2 attempts", result.stdout)

    def test_success(self):
        result = self.run_flash({"erase": [[0, "1024 KiB flash"]]})
        self.assertEqual(result.returncode, 0)
        self.assertIn("Done!", result.stdout)
        self.assertNotIn("failed", result.stdout)
        self.assertFalse(any(call[0] == "st-info" for call in self.calls))


if __name__ == "__main__":
    unittest.main()
