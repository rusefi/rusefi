"""
Reusable Python helpers for driving rusEFI MCP servers over stdio JSON-RPC.

Typical usage::

    from java_console.mcp_python import McpClient

    with McpClient("java_console/mcp_lua/build/libs/mcp_lua-all.jar",
                    client_name="my_script") as mcp:
        mcp.call("connect")
        mcp.call("set_lua", script="function onTick() end")
        msgs = mcp.call("read_messages", maxLines=20)
"""

from java_console.mcp_python.client import McpClient
from java_console.mcp_python.ini import ensure_ini, find_ini, ini_url, parse_signature
from java_console.mcp_python.pcan import PcanSniffer

__all__ = ["McpClient", "PcanSniffer", "ensure_ini", "find_ini", "ini_url", "parse_signature"]
