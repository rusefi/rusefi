import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

from java_console.mcp_python.client import McpClient


class _FakeStdin:
    def __init__(self, events):
        self._events = events

    def close(self):
        self._events.append("stdin.close")


class _FakeProcess:
    def __init__(self, wait_outcomes=(0,)):
        self.events = []
        self.stdin = _FakeStdin(self.events)
        self.stdout = None
        self._wait_outcomes = iter(wait_outcomes)

    def terminate(self):
        self.events.append("terminate")

    def kill(self):
        self.events.append("kill")

    def wait(self, timeout=None):
        self.events.append(f"wait({timeout})")
        outcome = next(self._wait_outcomes)
        if outcome == "timeout":
            raise subprocess.TimeoutExpired("java", timeout)
        return outcome


class McpClientCloseTest(unittest.TestCase):
    def test_close_allows_real_subprocess_eof_cleanup_to_finish(self):
        with tempfile.TemporaryDirectory() as directory:
            marker = Path(directory) / "cleanup-complete"
            cleanup_script = (
                "import pathlib, sys, time; "
                "sys.stdin.buffer.read(); "
                "time.sleep(0.05); "
                "pathlib.Path(sys.argv[1]).write_text('done')"
            )
            process = subprocess.Popen(
                [sys.executable, "-c", cleanup_script, str(marker)],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
            )
            client = McpClient("unused.jar")
            client._proc = process

            client.close()

            self.assertEqual("done", marker.read_text())
            self.assertIsNone(client._proc)

    def test_close_waits_for_eof_cleanup_before_terminating(self):
        client = McpClient("unused.jar")
        process = _FakeProcess()
        client._proc = process

        client.close()

        self.assertEqual(
            ["stdin.close", "wait(10)"],
            process.events,
        )
        self.assertIsNone(client._proc)

    def test_close_terminates_after_graceful_timeout(self):
        client = McpClient("unused.jar")
        process = _FakeProcess(("timeout", 0))
        client._proc = process

        client.close()

        self.assertEqual(
            ["stdin.close", "wait(10)", "terminate", "wait(5)"],
            process.events,
        )
        self.assertIsNone(client._proc)

    def test_close_kills_after_terminate_timeout(self):
        client = McpClient("unused.jar")
        process = _FakeProcess(("timeout", "timeout", 0))
        client._proc = process

        client.close()

        self.assertEqual(
            [
                "stdin.close",
                "wait(10)",
                "terminate",
                "wait(5)",
                "kill",
                "wait(None)",
            ],
            process.events,
        )
        self.assertIsNone(client._proc)


if __name__ == "__main__":
    unittest.main()
