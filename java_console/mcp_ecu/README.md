# rusEFI Lua MCP server

An [MCP (Model Context Protocol)](https://modelcontextprotocol.io/) server that lets an
LLM client (Claude Desktop, JetBrains AI, Cursor, etc.) iterate on rusEFI Lua scripts:
write a candidate script, upload it to the ECU, reset Lua, and observe the resulting
`print(...)` / `efiPrintf` output.

## Architecture

```
LLM client  <-- stdio JSON-RPC (MCP) -->  LuaMcpServer
                                              |
                                              +-- LuaService (in :ecu_io)
                                              |       |
                                              |       +-- LinkManager / BinaryProtocol
                                              |       +-- BurnCommand
                                              |       +-- StringIniField (LUASCRIPT)
                                              |
                                              +-- MessagesCentral listener
                                                      (same hook as Swing MessagesView,
                                                       captures Lua print() + efiPrintf)
```

`LuaService` (in `:ecu_io`) is the headless reusable core that `SetLuaTool` and
`LuaMcpServer` both depend on. It owns connecting to the ECU, locating the `LUASCRIPT`
ini field, writing/burning, and `luareset`.

## Tools exposed over MCP

| Tool | Purpose |
|---|---|
| `connect` | Connect (autodetect or supplied port). Called implicitly by other tools. |
| `ecu_info` | Signature + `LUASCRIPT` offset / max size. |
| `set_lua` | Upload Lua: write + burn + luareset. Accepts inline `script` or file `path`. |
| `get_lua` | Read currently-flashed Lua from the cached image. |
| `lua_reset` | Send `luareset` to restart the Lua VM. |
| `send_command` | Send any text command via the standard queue (e.g. `lua 1+2`). |
| `command` | Alias for `send_command` (useful for clients that prefer shorter tool names). |
| `read_output_channel` | Read latest output-channel value by name (case-insensitive). |
| `read_messages` | Pull recent ECU messages from the in-memory ring buffer (Lua `print` included). |
| `wait_for_message` | Block until a regex matches an ECU message, or timeout. |

A typical LLM session:
1. `ecu_info` to confirm signature and script size budget.
2. `set_lua { script: "..." }` to upload.
3. `wait_for_message { regex: "MY_MARKER:" }` or `read_messages` to observe output.
4. Iterate.

## Run

```
./gradlew :mcp_ecu:runMcp          # run on stdio
./gradlew :mcp_ecu:fatJar          # produce build/libs/mcp_ecu-*-all.jar
java -jar build/libs/mcp_ecu-all.jar [--port /dev/ttyACM0]
```

### Example client config (Claude Desktop)

```json
{
  "mcpServers": {
    "rusefi-lua": {
      "command": "java",
      "args": ["-jar", "/abs/path/to/mcp_ecu-all.jar"]
    }
  }
}
```

## Gotchas

- **stdio is the transport.** All logging must go to file (`Logging`) or stderr, never
  stdout.
- **One ECU connection at a time.** Don't run this alongside the rusEFI console
  pointed at the same port. All ECU access is serialized via `LinkManager#submit(...)`.
- **ASCII Lua only.** The controller's `LUASCRIPT` field is ASCII; non-ASCII characters
  are not transmitted.
- **`MessagesCentral` runs on the AWT EDT.** A headless JRE is fine, but you can pass
  `-Djava.awt.headless=true` to be explicit.
