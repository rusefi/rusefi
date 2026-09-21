"""Exercise bundle.mk's SREC recipe with recording tools, without ARM/Java builds.

Run with: python3 firmware/bin/test_bundle_checksum.py
Requires the same GNU make and POSIX shell environment as the firmware build.
"""

import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


BUNDLE_MK = Path(__file__).resolve().parents[1] / "bundle.mk"


class BundleChecksumTest(unittest.TestCase):
    def run_recipe(self, base_address):
        with tempfile.TemporaryDirectory() as directory:
            work = Path(directory)
            (work / "build").mkdir()
            (work / "build/rusefi.hex").touch()
            (work / "tools").mkdir()
            # Detect any dependency on bc, including when it exists on the host.
            bc = work / "tools/bc"
            bc.write_text("#!/bin/sh\ntouch bc-called\nexit 127\n")
            bc.chmod(0o755)
            (work / "record-hex2dfu.sh").write_text(
                '#!/bin/sh\nprintf "%s\\n" "$@" > hex2dfu-args\n'
            )
            env = dict(os.environ)
            env["PATH"] = str(work / "tools") + os.pathsep + env["PATH"]
            # Parent make flags must not interfere with this isolated build.
            env.pop("MAKEFLAGS", None)
            env.pop("MFLAGS", None)
            result = subprocess.run(
                [shutil.which("make"), "--no-print-directory", "-f", str(BUNDLE_MK),
                 "build/rusefi.srec", "PROJECT=rusefi", "BUILDDIR=build",
                 "HEX_BASE_ADDRESS=" + base_address, "SIGNATURE_HASH=nohash",
                 "UNAME_S=Linux", "H2D=sh ./record-hex2dfu.sh", "CP=true"],
                cwd=work, env=env, capture_output=True, text=True,
            )
            args_file = work / "hex2dfu-args"
            args = args_file.read_text().splitlines() if args_file.exists() else None
            return result, args, (work / "bc-called").exists()

    def test_missing_bc(self):
        result, args, bc_called = self.run_recipe("08008000")
        self.assertEqual(0, result.returncode, result.stdout + result.stderr)
        self.assertFalse(bc_called)
        self.assertEqual("0x800801C", args[args.index("-c") + 1])

    def test_valid_addresses(self):
        for base, expected in [("08000000", "0x800001C"), ("0", "0x1C"),
                               ("0800abcd", "0x800ABE9"), ("FFFFFFE3", "0xFFFFFFFF")]:
            with self.subTest(base=base):
                result, args, bc_called = self.run_recipe(base)
                self.assertEqual(0, result.returncode, result.stdout + result.stderr)
                self.assertFalse(bc_called)
                self.assertEqual(expected, args[args.index("-c") + 1])

    def test_invalid_addresses_stop_before_converter(self):
        for base in ["", "not-hex", "0x08000000", "0800 8000", "100000000", "FFFFFFE4"]:
            with self.subTest(base=base):
                result, args, bc_called = self.run_recipe(base)
                self.assertNotEqual(0, result.returncode)
                self.assertIn("Invalid .vectors base address", result.stderr)
                self.assertIsNone(args, "hex2dfu must not run with an invalid checksum address")
                self.assertFalse(bc_called)


if __name__ == "__main__":
    unittest.main()
