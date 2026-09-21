"""Compile production SD persistence code against an in-memory, fault-injecting FatFS.

This exercises file operations and the production LTFT load path, not SDIO/DMA
timing or physical FAT durability. Run with --cxx g++, clang++, or cl.
"""
import argparse
import json
import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[1]
CXX = os.environ.get("CXX", "c++")


def extract_block(source, signature):
    """Keep the actual implementation, including its production preprocessor guards."""
    start = source.index(signature)
    opening = source.index("{", start)
    depth, end = 1, opening + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


class StorageSdTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory = tempfile.TemporaryDirectory(prefix="rusefi-storage-")
        cls.addClassCleanup(cls.directory.cleanup)
        path = Path(cls.directory.name)
        controllers = ROOT / "firmware/controllers"
        source = (controllers / "storage_sd.cpp").read_text(encoding="utf-8")
        source = re.sub(r'^#include .*$', '', source, flags=re.MULTILINE)
        storage = (controllers / "storage.cpp").read_text(encoding="utf-8")
        ltft = (controllers / "long_term_fuel_trim.cpp").read_text(encoding="utf-8")
        declaration = (controllers / "long_term_fuel_trim.h").read_text(encoding="utf-8")
        config = (controllers / "generated/rusefi_generated_f407-discovery.h").read_text(encoding="utf-8")
        dimensions = "\n".join(re.search(rf'^#define {name} \d+$', config, re.M)[0]
                               for name in ("FT_BANK_COUNT", "VE_LOAD_COUNT", "VE_RPM_COUNT"))
        harness = (ROOT / "unit_tests/storage_sd_harness.cpp").read_text(encoding="utf-8")
        substitutions = {
            "SD_SOURCE": source,
            "READ_SOURCE": extract_block(storage, "StorageStatus storageRead("),
            "LTFT_DECLARATION": extract_block(declaration, "struct LtftState") + ";",
            "LTFT_LOAD": extract_block(ltft, "bool LtftState::load("),
            "LTFT_DIMENSIONS": dimensions,
            "REQUEST_HELPERS": extract_block(storage, "static uint32_t getPendingReads("),
            "REQUEST_READ": extract_block(storage, "bool storageReqestReadID("),
            "WAIT_READ": extract_block(storage, "bool storageWaitReadDone("),
            "WAIT_STUB": extract_block(storage[storage.index("#else // !EFI_CONFIGURATION_STORAGE"):],
                                       "bool storageWaitReadDone("),
            "AVAILABLE": extract_block(storage, "bool storageIsIdAvailableForId("),
            "READ_ID": extract_block(storage, "static bool storageReadID("),
            "POLL_READS": storage[storage.index("\t\tuint32_t reads = getPendingReads();"):
                                  storage.index("\t\t// check if we can write some of pending IDs")],
            "LTFT_MODULE": "\n".join(extract_block(ltft, signature) for signature in (
                "void LongTermFuelTrim::init(", "bool LongTermFuelTrim::load(",
                "void LongTermFuelTrim::onSlowCallback(", "void LongTermFuelTrim::onEngineStop(")),
            "SD_EXECUTOR": extract_block((ROOT / "firmware/hw_layer/mmc_card.cpp").read_text(encoding="utf-8"),
                                         "static int sdModeExecuter("),
        }
        for name, value in substitutions.items():
            harness = harness.replace("@" + name + "@", value)
        (path / "test.cpp").write_text(harness, encoding="utf-8")
        cls.executable = path / ("test.exe" if os.name == "nt" else "test")
        cls.without_msd = path / ("test_no_msd.exe" if os.name == "nt" else "test_no_msd")
        for executable, msd in ((cls.executable, 1), (cls.without_msd, 0)):
            if Path(CXX).name.lower() in ("cl", "cl.exe"):
                command = [CXX, "/nologo", "/std:c++17", "/EHsc", "/W4", "/WX",
                           f"/DHAL_USE_USB_MSD={msd}", "/I" + str(controllers), str(path / "test.cpp"),
                           "/Fe:" + str(executable), "/Fo:" + str(path / "test.obj")]
            else:
                command = [CXX, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                           f"-DHAL_USE_USB_MSD={msd}", "-I", str(controllers),
                           str(path / "test.cpp"), "-o", str(executable)]
            subprocess.run(command, check=True, cwd=path)

    def run_case(self, scenario, record="ltft", without_msd=False):
        executable = self.without_msd if without_msd else self.executable
        result = subprocess.run([str(executable), scenario, record], check=True,
                                capture_output=True, text=True, timeout=5)
        return json.loads(result.stdout)

    def test_failed_replacement_preserves_readable_record(self):
        for record in ("ltft", "second_tables", "lua_script"):
            for operation in ("open", "write", "short_write", "sync", "close", "inspect",
                              "inspect_close", "unlink", "rotate", "promote", "restore"):
                with self.subTest(record=record, operation=operation):
                    result = self.run_case(operation, record)
                    self.assertEqual(result["status"], 6)  # StorageStatus::Failed
                    self.assertEqual(result["read_status"], 0)
                    self.assertTrue(result["old"])
                    self.assertFalse(result["new"])

    def test_successful_replacement_retains_previous_primary(self):
        for record in ("ltft", "second_tables", "lua_script"):
            with self.subTest(record=record):
                result = self.run_case("success", record)
                self.assertEqual(result["status"], 0)
                self.assertTrue(result["new"])
                self.assertTrue(result["backup"])

    def test_unavailable_filesystem_does_not_modify_storage(self):
        result = self.run_case("closed")
        self.assertEqual(result["status"], 5)
        self.assertTrue(result["old"])

    def test_missing_or_invalid_primary_uses_backup(self):
        for scenario in ("missing", "truncated", "oversized", "read_error", "short_read", "read_close"):
            with self.subTest(scenario=scenario):
                result = self.run_case(scenario)
                self.assertEqual(result["read_status"], 0)
                self.assertTrue(result["old"])

    def test_missing_primary_does_not_discard_backup_on_failed_promotion(self):
        result = self.run_case("missing_promote")
        self.assertEqual(result["status"], 6)
        self.assertTrue(result["old"])
        self.assertTrue(result["backup"])

    def test_invalid_primary_keeps_good_backup_after_failed_promotion(self):
        for scenario in ("truncated_promote", "oversized_promote"):
            with self.subTest(scenario=scenario):
                result = self.run_case(scenario)
                self.assertEqual(result["status"], 6)
                self.assertEqual(result["read_status"], 0)
                self.assertTrue(result["old"])
                self.assertTrue(result["backup"])

    def test_invalid_primary_replacement_keeps_recovery_backup(self):
        for scenario in ("truncated_replace", "oversized_replace"):
            with self.subTest(scenario=scenario):
                result = self.run_case(scenario)
                self.assertEqual(result["status"], 0)
                self.assertTrue(result["new"])
                self.assertTrue(result["backup"])

    def test_backend_failure_cannot_overwrite_successful_read(self):
        self.assertEqual(self.run_case("priority"), {"status": 0, "value": 17})

    def test_production_ltft_load_preserves_active_trims_after_failure(self):
        for scenario in ("ltft_partial", "ltft_missing", "ltft_close"):
            with self.subTest(scenario=scenario):
                self.assertEqual(self.run_case(scenario), {"intact": True, "bytes": 2048})

    def test_production_ltft_load_commits_complete_primary_or_backup(self):
        for scenario in ("ltft_success", "ltft_backup"):
            with self.subTest(scenario=scenario):
                self.assertEqual(self.run_case(scenario), {"intact": True, "bytes": 2048})

    def test_startup_mount_completes_read_before_ecu_mode(self):
        self.assertEqual(self.run_case("startup"), {"done": True, "slept": 10, "pending": False})

    def test_closing_filesystem_is_not_ready_even_in_ecu_mode(self):
        self.assertEqual(self.run_case("ready_closed"), {"ready": False})

    def test_full_mailbox_keeps_read_pending_before_wakeup(self):
        self.assertEqual(self.run_case("queue_full"), {"accepted": True, "before_wakeup": True,
                                                     "done": True, "pending": False})

    def test_wait_observes_only_requested_read_and_times_out(self):
        self.assertEqual(self.run_case("wait_timeout"), {"done": False, "slept": 25, "unrelated": True})
        self.assertEqual(self.run_case("wait_unrelated"), {"done": True, "slept": 0, "unrelated": True})
        self.assertEqual(self.run_case("wait_complete"), {"done": True, "slept": 20, "unrelated": True})

    def test_invalid_wait_id_is_rejected(self):
        self.assertEqual(self.run_case("wait_invalid"), {"zero": False, "limit": False,
                                                        "stub_invalid": False, "stub_valid": True})

    def test_failed_ltft_read_finishes_attempt_with_error_and_retry_on_stop(self):
        for scenario in ("module_missing", "module_partial"):
            with self.subTest(scenario=scenario):
                self.assertEqual(self.run_case(scenario), {"done": True, "error": True,
                                                          "retry": True, "intact": True})

    def test_stopped_engine_keeps_initial_read_pending(self):
        self.assertEqual(self.run_case("module_stopped"), {"pending": True, "error": False})

    def test_late_read_defers_until_engine_stops(self):
        self.assertEqual(self.run_case("module_late"), {"deferred": True, "intact": True,
                                                      "retried": True, "loaded": True, "error": False})

    def test_usb_handoff_suppresses_logging_without_overriding_explicit_ownership(self):
        self.assertEqual(self.run_case("usb_handoff"), {"started": 0, "written": 0, "logging": False})
        for scenario in ("usb_always", "usb_requested"):
            with self.subTest(scenario=scenario):
                self.assertEqual(self.run_case(scenario), {"started": 1, "written": 1, "logging": True})
        self.assertEqual(self.run_case("usb_handoff", without_msd=True),
                         {"started": 1, "written": 1, "logging": True})


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cxx", default=CXX)
    args, remaining = parser.parse_known_args()
    CXX = args.cxx
    unittest.main(argv=[__file__] + remaining)
