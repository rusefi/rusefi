# Connecting to rusEFI over Ethernet

This describes the firmware's Ethernet console: what address the ECU uses, what
port to connect to, and how to set up your PC and TunerStudio.

## TL;DR

| Parameter   | Value                                        |
|-------------|----------------------------------------------|
| ECU IP      | 192.168.10.239 (static, no DHCP)             |
| TCP port    | 29001                                        |
| Netmask     | 255.255.255.0                                |
| Gateway     | 192.168.10.1                                 |
| MAC address | 72:75:73:12:34:56 (ascii "rus" + 12:34:56)   |
| Protocol    | TunerStudio serial protocol over raw TCP     |
| Clients     | one by default; board-configurable                     |

There is NO DHCP client in the firmware: the ECU will not request an address
from your router, and it will not answer discovery. It always boots with the
fixed address 192.168.10.239.

## PC setup

A direct PC-to-ECU cable works fine (no switch or crossover cable needed; the
PHY handles auto-MDIX). Because the ECU does not do DHCP, your PC's Ethernet
interface must be given a static address on the same subnet:

- IP address: 192.168.10.1 (anything in 192.168.10.x except .239)
- Netmask:    255.255.255.0
- Gateway:    leave empty

Then verify the link:

```
ping 192.168.10.239
```

## TunerStudio / console setup

Connect using a network (TCP/socket) connection type, not a serial port:

- Host/IP: 192.168.10.239
- Port:    29001

The generated .ini already carries `defaultIpPort = 29001`. The wire protocol
is the exact same TunerStudio protocol as USB/serial, just framed over TCP, so
anything that can talk to a rusEFI TCP port (TunerStudio, rusEFI console,
simulator tooling) works unchanged.

The number of simultaneous clients on this TCP port is selected at build time
with `Ethernet_Connection_Count`, which defaults to 1. The network hub overrides
it to 2. Each client has its own TunerStudio worker, socket, protocol state and
response buffers. With multiple clients enabled, an idle client or a reconnect
does not stop another client. All connections access the same ECU configuration;
changes made through any connection apply to that ECU.

A dropped client is detected via TCP keepalive (5 s idle, 1 s probe interval,
3 probes -> roughly 8 s), after which the ECU goes back to accepting a new
connection in that slot. If all slots are occupied, wait for one client to
disconnect. If you cannot reconnect, wait a few seconds for the keepalive to
reap the stale connection.

## Which boards have this

Ethernet support is compile-time: the `ethernet_console` module is built only
when the board's `board.mk` sets `EFI_ETHERNET = yes`. As of 2026-07 that is:

- nucleo_f429, nucleo_f767, nucleo_h743
- f746-discovery
- hellen/premium-quick-test

There is no runtime tune setting to enable/disable Ethernet - if the firmware
was built with it, the TCP server is always listening whenever the link is up.

## Where the values live (for developers)

- IP / gateway / MAC: `firmware/console/lwipopts.h`
  (`LWIP_IPADDR`, `LWIP_GATEWAY`, `LWIP_ETHADDR_*`). Netmask falls back to the
  ChibiOS default 255.255.255.0 in
  `firmware/ChibiOS/os/various/lwip_bindings/lwipthread.h` (`LWIP_NETMASK`).
- TCP port: `#define ETHERNET_PORT 29001` in
  `firmware/integration/rusefi_config.txt`, propagated to generated headers and
  to the TunerStudio .ini (`defaultIpPort`).
- Server implementation:
  `firmware/controllers/modules/ethernet_console/ethernet_console.cpp` - binds
  to INADDR_ANY:ETHERNET_PORT and provides `Ethernet_Connection_Count` independent
  `TsChannelBase` instances to standard TunerStudio threads. A mutex serializes
  listener setup and accepts; established client I/O does not take that mutex. The default
  lwIP pools (4 netconns and 5 active TCP PCBs) accommodate the listener and
  two clients.
- Client count: `Ethernet_Connection_Count` defaults to 1 in the server source.
  Set `DDEFS += -DEthernet_Connection_Count=2` in a board's `board.mk` to serve two
  clients. Worker storage and listener backlog both use this count. Counts must
  be positive; larger counts also require sufficient RAM and lwIP pool capacity.
- Addressing mode: `lwipInit(nullptr)` uses the compile-time settings above.
  `LWIP_DHCP` is never defined, so lwIP's default (0) applies and ChibiOS's
  `lwipthread.c` picks `NET_ADDRESS_STATIC`.

Changing the IP or port therefore requires editing those files and recompiling;
there is no runtime configuration. DHCP could be enabled by defining
`LWIP_DHCP 1` in `lwipopts.h` (ChibiOS's lwipthread supports it), but then the
ECU's address would depend on the DHCP server and you would need some way to
discover it - the fixed-address scheme is deliberate for direct PC-to-ECU use.
