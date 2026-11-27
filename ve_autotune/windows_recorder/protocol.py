"""Request/response framing for the rusEFI binary protocol's output-channel
read command ('O'), ported from the Android app's protocol/Frame.kt and
protocol/Crc.kt (same algorithm, same byte order, new language).

Outer envelope (java_console/io/src/main/java/com/rusefi/binaryprotocol/
IoHelper.java#makeCrc32Packet):
    [2 bytes BIG-ENDIAN length of inner][inner bytes][4 bytes BIG-ENDIAN CRC32 of inner bytes]

Inner request command (java_console/io/src/main/java/com/rusefi/io/commands/
ByteRange.java#packOffsetAndSize -- offset/count are packed as swap16(value)
through a big-endian putShort, which is equivalent to writing them
little-endian on the wire):
    ['O' (0x4F)] + offset as LITTLE-ENDIAN 16-bit + count as LITTLE-ENDIAN 16-bit
This is the ONE place the inner payload's endianness differs from the outer
envelope's -- do not "fix" it to match big-endian.

Inner response (java_console/io/src/main/java/com/rusefi/io/tcp/
BinaryProtocolServer.java#getOutputCommandResponse):
    [1 byte: 0x00 = OK][count raw bytes]
A first byte other than 0x00 means the ECU rejected the request.

This module must never grow a generic "build any command" helper -- the only
frame this app is allowed to build is the output-channel read request.
"""

import struct
import zlib

_OUTPUT_COMMAND = 0x4F  # 'O'
_STATUS_OK = 0x00


class BadResponse(Exception):
    """Raised when a response frame fails CRC validation or the ECU
    reports a non-OK status."""


def crc32(data: bytes) -> int:
    """Standard CRC-32 (same algorithm as Java's java.util.zip.CRC32)."""
    return zlib.crc32(data) & 0xFFFFFFFF


def build_output_channel_request(offset: int, count: int) -> bytes:
    """Builds the full outer-enveloped request for an output-channel read
    of `count` bytes starting at `offset`. The only command this app ever
    sends."""
    inner = struct.pack("<BHH", _OUTPUT_COMMAND, offset, count)
    return _wrap_outer_envelope(inner)


def _wrap_outer_envelope(inner: bytes) -> bytes:
    """[2 bytes BE length of inner][inner][4 bytes BE CRC32 of inner]."""
    return struct.pack(">H", len(inner)) + inner + struct.pack(">I", crc32(inner))


def parse_response(frame: bytes) -> bytes:
    """Parses a full outer envelope (as read off the wire per the outer BE16
    length prefix) into the decoded output-channel payload.

    Verifies the trailing CRC32 against the inner bytes and that the first
    inner byte is the OK status; raises BadResponse otherwise.
    """
    inner_length = struct.unpack_from(">H", frame, 0)[0]
    inner = frame[2 : 2 + inner_length]
    crc_offset = 2 + inner_length

    expected_crc = struct.unpack_from(">I", frame, crc_offset)[0]
    actual_crc = crc32(inner)
    if actual_crc != expected_crc:
        raise BadResponse(
            f"CRC mismatch: expected 0x{expected_crc:08x}, got 0x{actual_crc:08x}"
        )

    status = inner[0]
    if status != _STATUS_OK:
        raise BadResponse(f"ECU rejected request: 0x{status:02x}")

    return inner[1:]
