"""Byte-pipe transport over a serial (COM) port, backing the rusEFI binary
protocol connection. Ported from the Android app's bt/SocketTransport.kt --
this is a dumb byte pipe: it knows nothing about the rusEFI wire protocol
(framing, CRC, command bytes). All protocol semantics live in
ve_autotune.windows_recorder.protocol and .connection; this module only
moves bytes in and out of a serial port.

On Windows, a microRusEFI's Bluetooth module (classic SPP/RFCOMM) is paired
through Windows' own Bluetooth settings, which then exposes it as an
ordinary COM port -- no native Bluetooth API code is needed here.
"""

from typing import Protocol

import serial

#: Confirmed from firmware/controllers/algo/engine_configuration.cpp's
#: `#define TS_DEFAULT_SPEED 38400`, the default speed of microRusEFI's
#: Bluetooth-via-TTL secondary TunerStudio channel.
_BAUD_RATE = 38400
_READ_TIMEOUT_SECONDS = 2.0


class ByteTransport(Protocol):
    def write(self, data: bytes) -> None: ...

    def read_fully(self, n: int) -> bytes: ...


class SerialTransport:
    """ByteTransport backed by anything exposing pyserial's
    write(bytes)/read(n) -> bytes interface (a real serial.Serial, or a
    fake with the same shape for tests)."""

    def __init__(self, ser):
        self._ser = ser

    def write(self, data: bytes) -> None:
        self._ser.write(data)

    def read_fully(self, n: int) -> bytes:
        buf = bytearray()
        while len(buf) < n:
            chunk = self._ser.read(n - len(buf))
            if not chunk:
                raise TimeoutError(
                    f"serial read timed out after {len(buf)} of {n} requested bytes"
                )
            buf.extend(chunk)
        return bytes(buf)


def open_port(port_name: str) -> serial.Serial:
    """Opens the COM port Windows exposed for the paired Bluetooth SPP
    device. The only place these serial parameters are specified."""
    return serial.Serial(
        port=port_name,
        baudrate=_BAUD_RATE,
        bytesize=serial.EIGHTBITS,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        timeout=_READ_TIMEOUT_SECONDS,
    )
