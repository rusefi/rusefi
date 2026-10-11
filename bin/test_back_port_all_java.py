import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


REPOSITORY = Path(__file__).resolve().parents[1]
SCRIPT = REPOSITORY / "bin/_back_port_all_java.sh"
SUBMODULE = "java_console/peak-can-basic"


class BackPortJavaTest(unittest.TestCase):
    def git(self, directory, *args):
        return subprocess.check_output(
            ["git", "-C", str(directory), *args], text=True,
            stderr=subprocess.PIPE,
        ).strip()

    def setUp(self):
        if not shutil.which("rsync"):
            self.skipTest("rsync is required")
        upstream = REPOSITORY / SUBMODULE
        if not (upstream / ".git").exists():
            self.skipTest("initialize java_console/peak-can-basic first")
        # Reuse existing history in local clones; never commit or fetch remotely.
        self.new_revision = self.git(upstream, "rev-parse", "HEAD")
        self.old_revision = self.git(upstream, "rev-parse", "HEAD^")
        temporary = tempfile.TemporaryDirectory(prefix="back-port-java-")
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name)
        self.source = self.root / "source"
        self.target = self.root / "target with spaces"
        for directory, revision in [(self.source, self.new_revision),
                                    (self.target, self.old_revision)]:
            directory.mkdir()
            self.git(directory, "init", "-q")
            (directory / "java_console").mkdir()
            (directory / "java_tools").mkdir()
            self.git(directory, "clone", "--shared", str(upstream), SUBMODULE)
            self.git(directory / SUBMODULE, "checkout", "--detach", revision)
            (directory / ".gitmodules").write_text(
                '[submodule "peak-can-basic"]\n'
                f'\tpath = {SUBMODULE}\n\turl = {upstream}\n'
            )
            self.git(directory, "add", ".gitmodules")
            self.git(directory, "update-index", "--add", "--cacheinfo",
                     f"160000,{revision},{SUBMODULE}")
        (self.source / "java_tools/New.java").write_text("new Java\n")
        (self.target / "java_tools/Old.java").write_text("old Java\n")
        self.resource = Path("java_console/ui/src/main/resources/icon.txt")
        (self.source / self.resource).parent.mkdir(parents=True)
        (self.source / self.resource).write_text("resource\n")
        # These uncommitted submodule files must never be copied by the parent.
        (self.source / SUBMODULE / "DoNotCopy.java").write_text("local Java\n")
        resource = self.source / SUBMODULE / "src/main/resources/local.txt"
        resource.parent.mkdir(parents=True, exist_ok=True)
        resource.write_text("local resource\n")
        data = self.source / SUBMODULE / "src/test/java/local.ini"
        data.parent.mkdir(parents=True, exist_ok=True)
        data.write_text("local test data\n")

    def run_script(self):
        environment = dict(os.environ, GIT_ALLOW_PROTOCOL="file")
        return subprocess.run(
            ["bash", str(SCRIPT), str(self.target)], cwd=self.source,
            env=environment, text=True, stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
        )

    def test_updates_revision_without_copying_submodule_files(self):
        result = self.run_script()
        self.assertEqual(0, result.returncode, result.stdout)
        self.assertEqual("new Java\n", (self.target / "java_tools/New.java").read_text())
        self.assertFalse((self.target / "java_tools/Old.java").exists())
        self.assertEqual("resource\n", (self.target / self.resource).read_text())
        self.assertEqual(self.new_revision, self.git(self.target / SUBMODULE, "rev-parse", "HEAD"))
        self.assertEqual("", self.git(self.target / SUBMODULE, "status", "--porcelain"))
        self.assertFalse((self.target / SUBMODULE / "DoNotCopy.java").exists())
        self.assertFalse((self.target / SUBMODULE / "src/main/resources/local.txt").exists())
        self.assertFalse((self.target / SUBMODULE / "src/test/java/local.ini").exists())

    def test_initializes_only_the_requested_submodule(self):
        shutil.rmtree(self.target / SUBMODULE)
        # An unrelated uninitialized submodule must not be fetched or changed.
        with (self.target / ".gitmodules").open("a") as modules:
            modules.write('[submodule "embedded"]\n\tpath = firmware/embedded\n'
                          '\turl = /nonexistent/embedded-repository\n')
        self.git(self.target, "add", ".gitmodules")
        self.git(self.target, "update-index", "--add", "--cacheinfo",
                 f"160000,{self.old_revision},firmware/embedded")
        result = self.run_script()
        self.assertEqual(0, result.returncode, result.stdout)
        self.assertEqual(self.new_revision, self.git(self.target / SUBMODULE, "rev-parse", "HEAD"))
        self.assertEqual("", self.git(self.target / SUBMODULE, "status", "--porcelain"))
        self.assertFalse((self.target / "firmware/embedded").exists())

    def test_uses_pinned_revision_not_source_checkout(self):
        self.git(self.source / SUBMODULE, "checkout", "--detach", self.old_revision)
        result = self.run_script()
        self.assertEqual(0, result.returncode, result.stdout)
        self.assertEqual(self.new_revision, self.git(self.target / SUBMODULE, "rev-parse", "HEAD"))
        self.assertEqual(self.old_revision, self.git(self.source / SUBMODULE, "rev-parse", "HEAD"))

    def test_dirty_target_aborts_before_deleting_java(self):
        local = self.target / SUBMODULE / "Keep.java"
        local.write_text("local changes\n")
        result = self.run_script()
        self.assertNotEqual(0, result.returncode, result.stdout)
        self.assertIn("has local changes", result.stdout)
        self.assertEqual("local changes\n", local.read_text())
        self.assertEqual(self.old_revision, self.git(self.target / SUBMODULE, "rev-parse", "HEAD"))
        self.assertTrue((self.target / "java_tools/Old.java").exists())
        self.assertFalse((self.target / "java_tools/New.java").exists())

    def test_plain_directory_is_not_treated_as_a_submodule(self):
        self.git(self.target, "update-index", "--force-remove", SUBMODULE)
        result = self.run_script()
        self.assertNotEqual(0, result.returncode, result.stdout)
        self.assertIn("as a Git submodule", result.stdout)
        self.assertTrue((self.target / "java_tools/Old.java").exists())


if __name__ == "__main__":
    unittest.main()
