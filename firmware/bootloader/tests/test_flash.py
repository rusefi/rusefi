#!/usr/bin/env python3
"""Compile the production flash adapter against a word-programming flash fake."""
from pathlib import Path
import os
import subprocess
import tempfile

here = Path(__file__).resolve().parent
firmware = here.parents[1]
with tempfile.TemporaryDirectory(prefix="openblt-flash-test-") as tmp:
    for platform in ("AT32F435xx", "STM32H7XX"):
        exe = Path(tmp) / platform
        subprocess.run([
            os.environ.get("CXX", "g++"), "-std=c++17", "-Wall", "-Wextra", "-Werror",
            f"-D{platform}",
            f"-I{here / 'stubs'}", f"-I{firmware / 'hw_layer'}",
            f"-I{firmware / 'bootloader/openblt_chibios'}",
            str(here / "flash_test.cpp"),
            str(firmware / "bootloader/openblt_chibios/openblt_flash.cpp"),
            "-o", str(exe),
        ], check=True)
        subprocess.run([str(exe)], check=True)

    exe = Path(tmp) / "backend"
    subprocess.run([
        os.environ.get("CXX", "g++"), "-std=c++17", "-Wall", "-Wextra", "-Werror",
        "-DAT32F435xx", "-DHAL_USE_EFL=1",
        f"-I{here / 'stubs'}", f"-I{firmware / 'hw_layer'}",
        str(here / "at32_backend_test.cpp"),
        str(firmware / "hw_layer/ports/at32/at32f4/flash_int.cpp"),
        "-o", str(exe),
    ], check=True)
    subprocess.run([str(exe)], check=True)
print("AT32 backend checks passed")
