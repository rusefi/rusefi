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
            "WRITE_SOURCE": extract_block(storage, "StorageStatus storageWrite("),
            "LTFT_DECLARATION": extract_block(declaration, "struct LtftState") + ";",
            "LTFT_LOAD": extract_block(ltft, "void LtftState::load("),
            "LTFT_SAVE": extract_block(ltft, "bool LtftState::save("),
            "LTFT_DIMENSIONS": dimensions,
        }
        for name, value in substitutions.items():
            harness = harness.replace("@" + name + "@", value)
        (path / "test.cpp").write_text(harness, encoding="utf-8")
        cls.executable = path / ("test.exe" if os.name == "nt" else "test")
        if Path(CXX).name.lower() in ("cl", "cl.exe"):
            command = [CXX, "/nologo", "/std:c++17", "/EHsc", "/W4", "/WX",
                       "/I" + str(controllers), str(path / "test.cpp"),
                       "/Fe:" + str(cls.executable), "/Fo:" + str(path / "test.obj")]
        else:
            command = [CXX, "-std=c++17", "-Wall", "-Wextra", "-Werror",
                       "-I", str(controllers), str(path / "test.cpp"), "-o", str(cls.executable)]
        subprocess.run(command, check=True, cwd=path)

    def run_case(self, scenario, record="ltft"):
        result = subprocess.run([str(self.executable), scenario, record], check=True,
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

    def test_production_ltft_save_exposes_active_table_to_storage(self):
        # Reproduce the direct-storage dependency before moving active trims to CCM.
        self.assertEqual(self.run_case("save_mutation"), {"saved": True, "snapshot": False,
                                                        "active_source": True, "changed": True})

    def test_production_ltft_save_reports_failure_without_losing_previous_record(self):
        for scenario in ("save_write", "save_sync", "save_close"):
            with self.subTest(scenario=scenario):
                self.assertEqual(self.run_case(scenario), {"saved": False, "old": True})


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--cxx", default=CXX)
    args, remaining = parser.parse_known_args()
    CXX = args.cxx
    unittest.main(argv=[__file__] + remaining)
