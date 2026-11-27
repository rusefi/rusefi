import struct

import pytest

from ve_autotune.windows_recorder.connection import RusefiBtConnection
from ve_autotune.windows_recorder.protocol import (
    BadResponse,
    build_output_channel_request,
    crc32,
)


class FakeTransport:
    def __init__(self, read_chunks):
        self._read_chunks = list(read_chunks)
        self.written = None
        self.reset_input_buffer_calls = 0

    def write(self, data: bytes) -> None:
        self.written = data

    def read_fully(self, n: int) -> bytes:
        chunk = self._read_chunks.pop(0)
        assert len(chunk) == n
        return chunk

    def reset_input_buffer(self) -> None:
        self.reset_input_buffer_calls += 1


def _framed_response(payload: bytes) -> tuple[bytes, bytes, bytes]:
    """Returns (length_prefix, inner, crc) for a correctly framed response
    carrying `payload` (214 bytes) after an OK status byte."""
    inner = b"\x00" + payload
    length_prefix = struct.pack(">H", len(inner))
    crc = struct.pack(">I", crc32(inner))
    return length_prefix, inner, crc


def test_read_channels_sends_exact_request():
    length_prefix, inner, crc = _framed_response(bytes(214))
    transport = FakeTransport([length_prefix, inner, crc])
    RusefiBtConnection(transport).read_channels()
    assert transport.written == build_output_channel_request(0, 214)


def test_read_channels_resets_input_buffer_before_writing():
    # Guards against a previous request's short error-reply frame (the
    # ECU's own error-code path on a corrupted request) leaving trailing
    # bytes in the OS buffer that would otherwise desync every later read.
    length_prefix, inner, crc = _framed_response(bytes(214))
    transport = FakeTransport([length_prefix, inner, crc])
    RusefiBtConnection(transport).read_channels()
    assert transport.reset_input_buffer_calls == 1


def test_read_channels_decodes_response():
    payload = bytearray(214)
    struct.pack_into("<H", payload, 4, 3500)  # RPM -> 3500.0
    length_prefix, inner, crc = _framed_response(bytes(payload))
    transport = FakeTransport([length_prefix, inner, crc])

    channels = RusefiBtConnection(transport).read_channels()

    assert channels.rpm == 3500.0


def test_read_channels_raises_on_wrong_length_prefix():
    bogus_length_prefix = struct.pack(">H", 99)
    transport = FakeTransport([bogus_length_prefix])
    with pytest.raises(BadResponse):
        RusefiBtConnection(transport).read_channels()


def test_read_channels_propagates_crc_mismatch_as_bad_response():
    length_prefix, inner, crc = _framed_response(bytes(214))
    bad_crc = bytes([crc[0] ^ 0xFF]) + crc[1:]
    transport = FakeTransport([length_prefix, inner, bad_crc])
    with pytest.raises(BadResponse):
        RusefiBtConnection(transport).read_channels()
