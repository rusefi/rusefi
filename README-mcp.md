### MCP servers for rusEFI

This document covers the MCP (Model Context Protocol) servers shipped with the rusEFI Java console and how they locate the `.ini` configuration file required to connect to an ECU.

See also: `java_console/mcp_ecu/README.md`, `java_console/mcp_can/README.md`.

#### Available MCP servers

| Module | Transport | Purpose |
|--------|-----------|---------|
| `:mcp_ecu` | stdio JSON-RPC | Upload/get Lua scripts, reset Lua, read ECU messages |
| `:mcp_can` | stdio JSON-RPC | Read-only CAN bus sniffing via PCAN hardware |

#### .ini file resolution

When the MCP Lua server (or the console UI) connects to an ECU it obtains a **signature** string, e.g. `rusEFI master.2026.06.07.purple-gateway.753206531`. The numeric suffix (`753206531`) is the **signature hash**. An `.ini` file matching that hash is required before the connection can proceed.

The resolution chain (see `RealIniFileProvider` and `SignatureHelper`) tries these sources **in order**:

1. **Console cache** — `~/.rusEFI/ini_database/{hash}.ini`
   (e.g. `C:\Users\<user>\.rusEFI\ini_database\753206531.ini`).
   If the file exists and is > 10 KB it is used immediately.

2. **HTTP download** — `https://rusefi.com/online/ini/rusefi/{branch}/{year}/{month}/{day}/{target}/{hash}.ini`
   (e.g. `https://rusefi.com/online/ini/rusefi/master/2026/06/07/purple-gateway/753206531.ini`).
   Downloaded and cached into the console cache folder above.
   Returns 404 for custom / unreleased boards.

3. **TunerStudio cache** — `~/.efiAnalytics/TunerStudio/config/ecuDef/{signatureNoSpaces}.ini`
   (e.g. `C:\Users\<user>\.efiAnalytics\TunerStudio\config\ecuDef\rusEFImaster.2026.06.07.purple-gateway.753206531.ini`).
   This folder is populated by TunerStudio itself or by the console's `PrimeTunerStudioCache` after a successful connection.
   Note: this path is used for *priming* (writing), not as a read fallback in the default resolution chain.

4. **Local filesystem search** — scans the current directory (`.`) and one level up (`..`) for files matching `rusefi*.ini`.
   Useful when running from a firmware build tree that contains `firmware/tunerstudio/generated/rusefi_<board>.ini`.

5. **`extra_local_ini_file_name` system property** — pass `-Dextra_local_ini_file_name=/path/to/file.ini` on the Java command line to force a specific `.ini` file.

#### Troubleshooting "Failed to locate .ini"

If the MCP server reports `Connection failed: Failed to located .ini`:

- **Custom / dev board**: The `.ini` has not been published to `rusefi.com`. Copy or symlink the generated `.ini` (e.g. from `firmware/tunerstudio/generated/`) into the console cache folder as `{hash}.ini`.
- **Offline**: Download the `.ini` from another machine and place it in the console cache.
- **System property**: Launch the MCP jar with `-Dextra_local_ini_file_name=path/to/rusefi_board.ini`.

#### Python client library (`java_console/mcp_python`)

A small Python package in `java_console/mcp_python/` provides reusable helpers for driving MCP servers over stdio JSON-RPC. Scripts at the repo root (`lua_can_test.py`, `pcan_sniff.py`, `mcp_drive.py`) demonstrate usage. Run scripts from the repo root so that `java_console.mcp_python` is importable.

**`McpClient`** — generic context-manager that launches an MCP server JAR, performs the JSON-RPC initialize handshake, and exposes `send()`, `notify()`, and `call()` helpers. `server_args=` passes arguments after the jar (e.g. `["--port", "/dev/ttyACM0"]`); `quiet=True` suppresses the `>>>`/`<<<` chatter for machine-readable wrappers:

```python
from java_console.mcp_python import McpClient

with McpClient("java_console/mcp_ecu/build/libs/mcp_ecu-all.jar") as mcp:
    mcp.call("connect")
    mcp.call("set_lua", script="function onTick() end")
    mcp.call("read_messages", maxLines=20)
```

**`mcp_drive.py`** (repo root) — command-line driver on top of `McpClient`. Single tool calls or a JSON list of `[tool, args]` pairs run inside ONE server session (one ECU connection, one message ring buffer — required for multi-step flows like `luareset` → observe boot output). Supports `"sinceSeq": "AUTO"` threading so waits/reads only consider messages produced after the previous step, avoiding stale ring-buffer matches. One JSON line per call on stdout. See its docstring for field-tested patterns (marker warm-up, `LUA:` regex anchoring, live-data-triggered "armed runs"):

```bash
./mcp_drive.py --port /dev/ttyACM0 ecu_info
./mcp_drive.py '[["send_command",{"command":"lua print(\"warm\")"}],
                 ["wait_for_message",{"regex":"LUA: warm","timeoutMs":25000}],
                 ["read_messages",{"sinceSeq":"AUTO","maxLines":100}]]'
```

**`.ini` helpers** — pre-flight functions that mirror the Java `SignatureHelper` logic, letting scripts check `.ini` availability before launching a Java MCP server:

```python
from java_console.mcp_python import ensure_ini, find_ini, ini_url, parse_signature

sig = "rusEFI master.2026.06.07.purple-gateway.753206531"
parts = parse_signature(sig)    # dict with branch/year/month/day/target/hash
cached = find_ini(sig)          # Path if in local cache, else None
url = ini_url(sig)              # HTTP download URL
path = ensure_ini(sig)          # find or download; raises if unavailable
```

**`PcanSniffer`** — convenience wrapper for PCAN CAN-bus sniffing that handles the JNI library path and provides `read_packets()`, `status()`, and `sniff_ids()`:

```python
from java_console.mcp_python import PcanSniffer

with PcanSniffer() as pcan:
    packets = pcan.sniff_ids(["0x600", "0x601", "0x602"], wait_seconds=5)
    for can_id, pkts in packets.items():
        print(f"{can_id}: {len(pkts)} packets")
```

#### Connecting to a real ECU

Always target a real ECU connected via serial port. The Win32 simulator is not reliable for MCP workflows (socket binding issues, process instability). Use serial port autodetect (omit `--port`) or pass the explicit serial port name.
