import hashlib
import json
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch
import zipfile

from zip_firmware_source import archive_index, payload_hash, repository_identity, write_archive


class KnowledgeArchiveTest(unittest.TestCase):
    def test_archive_preserves_text_and_records_actual_bytes(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            guide = root / "guide.md"
            guide.write_text("# Cranking\nCheck voltage.\n", encoding="utf-8")
            license_file = root / "LICENSE"
            license_file.write_text("License terms\n", encoding="utf-8")
            output = root / "source.zip"
            repositories = {name: {"revision": "a" * 40, "dirty": name == "firmware"}
                            for name in ("firmware", "libfirmware", "wiki")}
            write_archive(output, [("docs/AI/cranking.md", guide), ("docs/licenses/wiki.txt", license_file)], repositories)
            with zipfile.ZipFile(output) as archive:
                self.assertIsNone(archive.testzip())
                manifest = json.loads(archive.read("knowledge-manifest.json"))
                self.assertEqual(repositories, manifest["repositories"])
                self.assertEqual(set(archive.namelist()) - {"knowledge-manifest.json"}, set(manifest["files"]))
                for name, metadata in manifest["files"].items():
                    content = archive.read(name)
                    self.assertEqual(len(content), metadata["size"])
                    self.assertEqual(hashlib.sha256(content).hexdigest(), metadata["sha256"])
                self.assertEqual(payload_hash(manifest["files"]), manifest["payload_sha256"])
                self.assertIn(b"[cranking.md](AI/cranking.md)", archive.read("docs/knowledge-index.md"))
                self.assertIn(b"[Firmware](../firmware/)", archive.read("docs/knowledge-index.md"))
            original = output.read_bytes()
            with self.assertRaises(FileExistsError):
                write_archive(output, [], repositories)
            self.assertEqual(original, output.read_bytes())

    def test_symlinks_are_not_followed(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            source = root / "link.md"
            try:
                source.symlink_to("missing-or-private-file")
            except (OSError, NotImplementedError):
                self.skipTest("Symlinks unavailable")
            write_archive(root / "source.zip", [("firmware/link.md", source)], {})
            with zipfile.ZipFile(root / "source.zip") as archive:
                self.assertEqual(b"missing-or-private-file", archive.read("firmware/link.md"))

    def test_identity_marks_tracked_local_edits(self):
        with patch("zip_firmware_source.subprocess.check_output", side_effect=["a" * 40 + "\n", b" M file.cpp\n"]):
            self.assertEqual({"revision": "a" * 40, "dirty": True}, repository_identity(Path(".")))

    def test_index_covers_licenses_and_board_reference(self):
        index = archive_index(["docs/AI/one.md"])
        self.assertIn("docs/licenses/rusefi.txt", index)
        self.assertIn("docs/licenses/wiki.txt", index)
        self.assertIn("docs/hellen-board-mapping.md", index)
        self.assertIn("not proof of matching firmware source", index)


if __name__ == "__main__":
    unittest.main()
