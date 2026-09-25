#!/usr/bin/env python3
"""Exercise the production CAN adapter with fake time, CAN and reset operations."""
from pathlib import Path
import os
import subprocess
import tempfile
here = Path(__file__).resolve().parent
firmware = here.parents[1]
with tempfile.TemporaryDirectory(prefix="openblt-can-test-") as tmp:
    for fdcan in (False, True):
        exe = Path(tmp) / str(fdcan)
        subprocess.run([
            os.environ.get("CXX", "g++"), "-std=c++17", "-Wall", "-Wextra", "-Werror",
            *(["-DTEST_FDCAN"] if fdcan else []),
            f"-I{here / 'can_stubs'}", f"-I{here / 'stubs'}",
            f"-I{firmware / 'bootloader/openblt_chibios'}",
            f"-I{firmware / 'hw_layer/openblt'}",
            str(here / "can_test.cpp"), str(firmware / "bootloader/openblt_chibios/openblt_can.cpp"),
            "-o", str(exe),
        ], check=True)
        subprocess.run([str(exe)], check=True)
print("CAN transport checks passed")
