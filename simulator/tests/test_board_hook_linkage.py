"""Check board-hook defaults and overrides with ELF/LTO and MinGW/COFF."""
from pathlib import Path
import re
import shutil
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[2]
HOOKS = [
    "toothProfileStorageWrite", "toothProfileStorageRead",
    "vrModelStorageWrite", "vrModelStorageRead", "boardTriggerCallback",
    "boardRawTriggerEdge", "triggerGetToothProfileFactor",
    "boardTriggerSyncEvent", "triggerObserveGapShift",
]
PREAMBLE = r"""
#include <cstdint>
using efitick_t = int64_t;
#define PUBLIC_API_WEAK __attribute__((weak))
#define UNUSED(x) (void)(x)
bool toothProfileStorageWrite();
bool toothProfileStorageRead();
bool vrModelStorageWrite();
bool vrModelStorageRead();
void boardTriggerCallback(efitick_t, float);
void boardRawTriggerEdge(int, bool, efitick_t);
float triggerGetToothProfileFactor(int);
void boardTriggerSyncEvent(char, int, float, float);
void triggerObserveGapShift(float);
"""
DEFAULTS = r"""
#include "firmware/controllers/storage_board_hooks.inc"
#include "firmware/controllers/trigger/trigger_central_hooks.inc"
#include "firmware/controllers/trigger/trigger_decoder_hooks.inc"
"""
CALLER = r"""
int observed = 0;
int main() {
    if (toothProfileStorageWrite() != EXPECT_DEFAULT) return 1;
    if (toothProfileStorageRead() != EXPECT_DEFAULT) return 2;
    if (vrModelStorageWrite() != EXPECT_DEFAULT) return 3;
    if (vrModelStorageRead() != EXPECT_DEFAULT) return 4;
    boardTriggerCallback(0, 0);
    boardRawTriggerEdge(0, false, 0);
    boardTriggerSyncEvent('S', 0, 0, 0);
    triggerObserveGapShift(0);
    if (triggerGetToothProfileFactor(0) != (EXPECT_DEFAULT ? 1.0f : 2.0f)) return 5;
    return observed == (EXPECT_DEFAULT ? 0 : 4) ? 0 : 6;
}
"""
OVERRIDES = r"""
extern int observed;
bool toothProfileStorageWrite() { return false; }
bool toothProfileStorageRead() { return false; }
bool vrModelStorageWrite() { return false; }
bool vrModelStorageRead() { return false; }
void boardTriggerCallback(efitick_t, float) { observed++; }
void boardRawTriggerEdge(int, bool, efitick_t) { observed++; }
float triggerGetToothProfileFactor(int) { return 2.0f; }
void boardTriggerSyncEvent(char, int, float, float) { observed++; }
void triggerObserveGapShift(float) { observed++; }
"""


class BoardHookLinkage(unittest.TestCase):
    def check_linkage(self, windows, override):
        compiler = "i686-w64-mingw32-g++" if windows else "g++"
        if not shutil.which(compiler):
            self.skipTest(f"{compiler} is required")
        objdump = "i686-w64-mingw32-objdump"
        if windows and not shutil.which(objdump):
            self.skipTest(f"{objdump} is required")
        with tempfile.TemporaryDirectory(prefix="board-hook-linkage-") as temporary:
            directory = Path(temporary)
            caller = directory / "caller.cpp"
            caller.write_text(PREAMBLE + (DEFAULTS if windows else "") + CALLER)
            sources = [str(caller)]
            if not windows:
                defaults = directory / "defaults.cpp"
                defaults.write_text(PREAMBLE + DEFAULTS)
                sources.append(str(defaults))
            if override:
                overrides = directory / "overrides.cpp"
                overrides.write_text(PREAMBLE + OVERRIDES)
                sources.append(str(overrides))
            executable = directory / ("hooks.exe" if windows else "hooks")
            flags = ["-O0", "-ffunction-sections"] if windows else ["-O2", "-flto"]
            result = subprocess.run(
                [compiler, *flags, f"-DEXPECT_DEFAULT={int(not override)}",
                 "-I", str(ROOT), *sources, "-o", str(executable)],
                capture_output=True, text=True)
            self.assertEqual(result.returncode, 0, result.stderr)
            if not windows:
                subprocess.run([str(executable)], check=True)
                return
            # A successful COFF link can still emit calls to address zero.
            # Inspect the actual call targets without requiring Wine.
            assembly = subprocess.check_output(
                [objdump, "-d", "-C", str(executable)], text=True)
            main = assembly.split("<main>:\n", 1)[1].split("\n\n", 1)[0]
            calls = re.findall(r"\bcall\s+([0-9a-f]+)\s+<([^>]+)>", main)
            for hook in HOOKS:
                targets = [(address, name) for address, name in calls if hook in name]
                self.assertEqual(len(targets), 1, f"{hook}: {main}")
                address, name = targets[0]
                self.assertGreater(int(address, 16), 0)
                self.assertEqual(".weak." in name, not override, name)

    def test_elf_defaults(self):
        self.check_linkage(False, False)

    def test_elf_strong_overrides_with_lto(self):
        self.check_linkage(False, True)

    def test_coff_defaults_have_nonzero_targets(self):
        self.check_linkage(True, False)

    def test_coff_strong_overrides(self):
        self.check_linkage(True, True)


if __name__ == "__main__":
    unittest.main()
