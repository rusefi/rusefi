"""
PcanSniffer — convenience wrapper around McpClient for PCAN CAN-bus sniffing.

Usage::

    from java_console.mcp_python import PcanSniffer

    with PcanSniffer() as pcan:
        packets = pcan.sniff_ids(["0x600", "0x601", "0x602"], max_packets=5)
        for can_id, pkts in packets.items():
            print(f"{can_id}: {len(pkts)} packets")
"""

import time
from typing import Any

from java_console.mcp_python.client import McpClient

PCAN_JAR = r"java_console\mcp_can\build\libs\mcp_can-all.jar"
PCAN_JNI_PATH = r"java_console"


class PcanSniffer:
    """Context-manager that launches the PCAN MCP server and connects.

    Example::

        with PcanSniffer() as pcan:
            time.sleep(3)
            result = pcan.read_packets(idFilter="0x600", maxPackets=10)
    """

    def __init__(
        self,
        jar_path: str = PCAN_JAR,
        jni_path: str = PCAN_JNI_PATH,
        client_name: str = "pcan_sniffer",
    ):
        self._mcp = McpClient(
            jar_path,
            client_name=client_name,
            extra_java_args=[f"-Djava.library.path={jni_path}"],
        )

    def __enter__(self) -> "PcanSniffer":
        self._mcp.start()
        self._mcp.call("connect")
        return self

    def __exit__(self, *exc):
        self._mcp.close()

    def read_packets(self, **kwargs) -> dict:
        """Call ``read_packets`` with arbitrary keyword arguments."""
        return self._mcp.call("read_packets", **kwargs)

    def status(self) -> dict:
        """Return PCAN connection status."""
        return self._mcp.call("status")

    def sniff_ids(
        self,
        can_ids: list[str],
        *,
        wait_seconds: float = 5,
        max_packets: int = 5,
    ) -> dict[str, list[dict[str, Any]]]:
        """Wait, then read packets filtered by each CAN ID.

        Returns a dict mapping each CAN ID string to its list of packet dicts.
        """
        print(f"\nWaiting {wait_seconds} s for CAN frames …", flush=True)
        time.sleep(wait_seconds)

        results: dict[str, list[dict[str, Any]]] = {}
        for can_id in can_ids:
            resp = self.read_packets(idFilter=can_id, maxPackets=max_packets)
            # extract packets list from the MCP response
            content = resp.get("result", {}).get("content", [])
            packets = []
            for item in content:
                if isinstance(item, dict) and "text" in item:
                    import json
                    try:
                        data = json.loads(item["text"])
                        packets = data.get("packets", [])
                    except (json.JSONDecodeError, AttributeError):
                        pass
            results[can_id] = packets
            print(f"  {can_id}: {len(packets)} packets", flush=True)

        return results
