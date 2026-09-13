# CAN sniffer MCP

`:mcp_can` exposes read-only CAN capture through stdio JSON-RPC. Build from the repository root:

```sh
./gradlew :mcp_can:fatJar
```

## Built-in SLCAN

Use the ECU's secondary USB serial port on firmware with SLCAN support:

```sh
java -jar java_console/mcp_can/build/libs/mcp_can-all.jar --backend slcan
java -jar java_console/mcp_can/build/libs/mcp_can-all.jar --backend slcan --port /dev/ttyACM1
```

On Windows, use a port such as `COM5`. Without `--port`, the server probes serial ports,
rejects the primary TunerStudio console, and opens the first SLCAN port found. With
`--port`, only that port is probed. Use an explicit port when several ECUs are attached;
USB port numbering does not reliably identify the sniffer interface.

Close the console's SLCAN tab or other applications holding the sniffer port before
connecting. The primary ECU console and the secondary SLCAN port can be used separately.
PCAN hardware and its native driver are not required for SLCAN.

The ECU tune controls CAN bus selection and bitrate. Enable `canSnifferN_read` for each
bus whose received traffic you want to capture. With default settings, only the ECU's
own transmitted frames may appear. The server sends the required `C` / `S6` / `O`
initialization sequence; `S6` does not change the ECU's bitrate. `--channel` applies only
to PCAN. No CAN transmit tool is exposed.

## PCAN

Existing launches keep using PCAN USB bus 1 at 500 kbit/s:

```sh
java -jar java_console/mcp_can/build/libs/mcp_can-all.jar
java -jar java_console/mcp_can/build/libs/mcp_can-all.jar --backend pcan --channel 2
```

PCAN requires its driver and PCANBasic JNI library on the Java library path. A channel
can also be specified as `PCAN_USBBUS2`. Run one server per adapter connection.

## Tools and packet format

- `connect`: opens the backend selected on the command line. Repeated calls reuse an active connection.
- `read_packets`: reads the ring buffer; supports `sinceSeq`, `maxPackets`, and hexadecimal `idFilter`.
- `wait_for_packet`: waits for a matching packet using `sinceSeq`, `idFilter`, `dataRegex`, and `timeoutMs`.
- `status`: returns connection state, backend, port (SLCAN) or channel (PCAN), and buffer statistics.

Both packet tools connect implicitly when needed. SLCAN read/write failures close the
serial connection and are reported as tool errors; a subsequent connect or packet tool
can retry. Periodic SLCAN status commands detect serial write failures even on a quiet bus.
Closing MCP stdin shuts down the reader and closes the logical SLCAN channel.

Packet fields remain `seq`, `timestamp` (host Unix time in milliseconds), `id`, `idDec`,
`length`, and `data` (hex). SLCAN packets additionally include `extended` and `rtr`.
RTR packets retain their requested DLC in `length` and have empty `data`. When supplied
by the device, `slcanTimestamp` contains its four hexadecimal timestamp characters;
it does not replace the host timestamp. Non-frame acknowledgments and status replies
are excluded from the packet buffer. Sequence numbers and buffered packets survive
reconnection; use `sinceSeq` to exclude earlier traffic.

## Python example

```python
from java_console.mcp_python import McpClient

with McpClient(
    "java_console/mcp_can/build/libs/mcp_can-all.jar",
    server_args=["--backend", "slcan", "--port", "/dev/ttyACM1"],
) as mcp:
    mcp.call("connect")
    print(mcp.call("wait_for_packet", idFilter="0x123", timeoutMs=10000))
    print(mcp.call("read_packets", maxPackets=200))
```

The existing `PcanSniffer` Python wrapper remains specific to PCAN; use `McpClient`
with `server_args` for SLCAN.
