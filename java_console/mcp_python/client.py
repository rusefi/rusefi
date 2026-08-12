"""
McpClient — a lightweight Python client for rusEFI MCP servers over stdio JSON-RPC.

The client launches a Java MCP server JAR as a subprocess, sends JSON-RPC 2.0
requests over stdin, and reads responses from stdout.
"""

import json
import subprocess
import time
from typing import Any


class McpClient:
    """Context-manager wrapper around a stdio JSON-RPC MCP server process.

    Example::

        with McpClient(jar, client_name="demo") as mcp:
            mcp.call("connect")
            result = mcp.call("read_messages", maxLines=10)
    """

    def __init__(
        self,
        jar_path: str,
        *,
        client_name: str = "mcp_python",
        extra_java_args: list[str] | None = None,
        server_args: list[str] | None = None,
        quiet: bool = False,
    ):
        self._jar = jar_path
        self._client_name = client_name
        self._extra_java_args = extra_java_args or []
        # arguments AFTER the jar, e.g. ["--port", "/dev/ttyACM0"]
        self._server_args = server_args or []
        # quiet=True suppresses the >>> / <<< chatter on stdout, for CLI
        # wrappers that need machine-readable output
        self._quiet = quiet
        self._next_id = 0
        self._proc: subprocess.Popen | None = None

    # ── lifecycle ─────────────────────────────────────────────────────────

    def start(self) -> "McpClient":
        """Launch the MCP server subprocess and perform the MCP initialize handshake."""
        cmd = ["java", "-Djava.awt.headless=true", *self._extra_java_args, "-jar", self._jar,
               *self._server_args]
        self._proc = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=None,  # let stderr pass through for diagnostics
        )
        self.send("initialize", {
            "protocolVersion": "2024-11-05",
            "capabilities": {},
            "clientInfo": {"name": self._client_name, "version": "1.0"},
        })
        self.notify("notifications/initialized")
        time.sleep(0.5)
        return self

    def close(self):
        """Shut down the MCP server subprocess."""
        if self._proc is not None:
            self._proc.stdin.close()
            self._proc.terminate()
            self._proc.wait(timeout=10)
            self._proc = None

    def __enter__(self) -> "McpClient":
        return self.start()

    def __exit__(self, *exc):
        self.close()

    # ── JSON-RPC primitives ───────────────────────────────────────────────

    def send(self, method: str, params: dict | None = None) -> dict:
        """Send a JSON-RPC request and wait for the matching response."""
        self._next_id += 1
        req_id = self._next_id
        msg: dict[str, Any] = {"jsonrpc": "2.0", "id": req_id, "method": method}
        if params is not None:
            msg["params"] = params
        line = json.dumps(msg) + "\n"
        if not self._quiet:
            print(f">>> {method}", flush=True)
        self._proc.stdin.write(line.encode())
        self._proc.stdin.flush()
        while True:
            resp_line = self._proc.stdout.readline()
            if not resp_line:
                raise RuntimeError("MCP server closed stdout")
            resp_line = resp_line.strip()
            if not resp_line:
                continue
            try:
                resp = json.loads(resp_line)
            except json.JSONDecodeError:
                if not self._quiet:
                    print(f"  (non-json) {resp_line.decode(errors='replace')}", flush=True)
                continue
            if resp.get("id") == req_id:
                if not self._quiet:
                    print(f"<<< {json.dumps(resp, indent=2)}", flush=True)
                return resp

    def notify(self, method: str, params: dict | None = None):
        """Send a JSON-RPC notification (no id, no response expected)."""
        msg: dict[str, Any] = {"jsonrpc": "2.0", "method": method}
        if params is not None:
            msg["params"] = params
        line = json.dumps(msg) + "\n"
        self._proc.stdin.write(line.encode())
        self._proc.stdin.flush()

    def call(self, tool_name: str, **arguments) -> dict:
        """Convenience: invoke ``tools/call`` with the given tool name and arguments."""
        return self.send("tools/call", {"name": tool_name, "arguments": arguments})
