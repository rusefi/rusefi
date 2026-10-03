"""Run the actual STM32/OpenBLT flash sources with a host register model.

Usage: python3 unit_tests/stm32_flash/test_flash.py
Set CXX to g++, clang++ or cl. No cross compiler or hardware is required.
"""
import os
from pathlib import Path
import subprocess
import tempfile
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[1]


class FlashDriverTest(unittest.TestCase):
    def compile_and_run(self, source, variant):
        with tempfile.TemporaryDirectory() as directory:
            executable = Path(directory) / "flash_test.exe"
            compiler = os.environ.get("CXX", "g++")
            if Path(compiler).stem.lower() == "cl":
                # /J matches ARM's unsigned plain char, just like -funsigned-char.
                arguments = ["/nologo", "/std:c++17", "/EHsc", "/J", "/D" + variant,
                             "/I" + str(HERE / "stubs"),
                             "/I" + str(ROOT / "firmware/hw_layer"),
                             str(HERE / source), "/Fe:" + str(executable)]
            else:
                arguments = ["-std=c++17", "-funsigned-char", "-Wall", "-Wextra", "-Werror",
                             "-D" + variant, "-I" + str(HERE / "stubs"),
                             "-I" + str(ROOT / "firmware/hw_layer"),
                             str(HERE / source), "-o", str(executable)]
            subprocess.run([compiler, *arguments], cwd=directory, check=True)
            subprocess.run([str(executable)], check=True)

    def test_h743(self):
        self.compile_and_run("test_driver.cpp", "STM32H743xx")

    def test_h723(self):
        self.compile_and_run("test_driver.cpp", "STM32H723xx")

    def test_openblt(self):
        self.compile_and_run("test_openblt.cpp", "STM32H743xx")


if __name__ == "__main__":
    unittest.main()
