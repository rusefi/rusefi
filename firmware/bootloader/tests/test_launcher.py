#!/usr/bin/env python3
"""Verify launcher argv/cwd without Gradle, a JVM or a CAN adapter."""
from pathlib import Path
import os
import shutil
import subprocess
import tempfile
repo = Path(__file__).resolve().parents[3]
with tempfile.TemporaryDirectory(prefix="openblt-launcher-") as tmp:
    root = Path(tmp)
    console = root / "repo with spaces/java_console"
    script = console / "bin/openblt_can.sh"
    script.parent.mkdir(parents=True)
    shutil.copyfile(repo / "java_console/bin/openblt_can.sh", script)
    gradle = console.parent / "gradlew"
    gradle.write_text('#!/bin/sh\n[ "$1" = ":openblt_can:fatJar" ]\n')
    gradle.chmod(0o755)
    tools = root / "tools"
    tools.mkdir()
    java = tools / "java"
    java.write_text('#!/bin/sh\npwd\nprintf "%s\\n" "$@"\n')
    java.chmod(0o755)
    caller = root / "caller"
    caller.mkdir()
    (caller / "1").mkdir()  # A numeric option is not a path argument.
    (caller / "firmware image.srec").touch()
    args = ["--channel", "1", "firmware image.srec"]
    result = subprocess.run(["bash", str(script), *args], cwd=caller,
                            env={**os.environ, "PATH": str(tools) + os.pathsep + os.environ["PATH"]},
                            text=True, capture_output=True, check=True)
    lines = result.stdout.splitlines()
    assert lines[0] == str(caller), lines
    assert lines[-3:] == args, lines
    assert lines[1] == "-Djava.library.path=" + str(console), lines
print("Launcher checks passed")
