#!/usr/bin/env python3
"""mcp_drive.py — command-line driver for the rusEFI ECU MCP server.

One server process = one ECU connection + one message ring buffer, so
multi-step flows (luareset -> observe boot output, tap -> watch status lines)
MUST run inside a single invocation — pass a JSON list of calls.

Usage (run from the repo root so java_console.mcp_python is importable):

  # single tool call
  ./mcp_drive.py ecu_info
  ./mcp_drive.py read_messages '{"maxLines": 50}'

  # multi-call session: JSON list of [tool, args] pairs
  ./mcp_drive.py '[
      ["send_command", {"command": "lua print(\"warm\")"}],
      ["wait_for_message", {"regex": "LUA: warm", "timeoutMs": 25000}],
      ["read_messages", {"sinceSeq": "AUTO", "maxLines": 100}]
  ]'

  # options
  ./mcp_drive.py --port /dev/ttyACM0 --jar path/to/mcp_ecu-all.jar ecu_info

"sinceSeq": "AUTO" in any args is replaced by the newest message seq seen so
far in this session, so waits/reads only consider output produced after the
previous step. Without it, wait_for_message happily matches stale ring-buffer
history (see mcp_ecu/README.md).

Output: one JSON line per call — the parsed content text of the tool result.

Field-tested patterns (2026-07 Dodge 8HP bench, canbus-dodge-8hp
docs/report.md):
- Streaming warms up seconds after connect: send a marker
  (lua print("warm")) and wait for it before trusting reads; empty
  read_messages / "no data yet" right after connect is normal.
- wait_for_message regexes must anchor on "LUA: " or they match the
  command confirmation echo (confirmation_lua print(...)).
- Armed runs: wait_for_message on a live-data pattern (e.g. a status-line
  value crossing a threshold) as a trigger, then fire no-arg Lua globals
  via send_command "lua MyAction()" — the interactive snippet is ~99 chars
  max and flaky on spaces.
"""

import json
import sys

from java_console.mcp_python import McpClient

DEFAULT_JAR = "java_console/mcp_ecu/build/libs/mcp_ecu-all.jar"


def main() -> int:
    argv = sys.argv[1:]
    jar = DEFAULT_JAR
    server_args: list[str] = []
    while argv and argv[0].startswith("--"):
        opt = argv.pop(0)
        if opt == "--jar":
            jar = argv.pop(0)
        elif opt == "--port":
            server_args += ["--port", argv.pop(0)]
        else:
            raise SystemExit(f"unknown option {opt}")
    if not argv:
        raise SystemExit(__doc__)

    spec = argv.pop(0)
    if spec.lstrip().startswith("["):
        calls = json.loads(spec)
    else:
        args = json.loads(argv.pop(0)) if argv else {}
        calls = [[spec, args]]

    last_seq = -1
    with McpClient(jar, client_name="mcp_drive", server_args=server_args,
                   quiet=True) as mcp:
        for name, cargs in calls:
            if cargs.get("sinceSeq") == "AUTO":
                cargs["sinceSeq"] = last_seq
            resp = mcp.call(name, **cargs)
            try:
                body = json.loads(resp["result"]["content"][0]["text"])
            except (KeyError, IndexError, ValueError):
                body = resp
            if isinstance(body, dict):
                if isinstance(body.get("latestSeq"), int):
                    last_seq = max(last_seq, body["latestSeq"])
                match = body.get("match")
                if isinstance(match, dict) and isinstance(match.get("seq"), int):
                    last_seq = max(last_seq, match["seq"])
            print(json.dumps(body), flush=True)
    return 0


if __name__ == "__main__":
    sys.exit(main())
