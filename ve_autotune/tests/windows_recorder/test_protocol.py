import struct

import pytest

from ve_autotune.windows_recorder.protocol import (
    BadResponse,
    build_output_channel_request,
    crc32,
    parse_response,
)


def test_crc32_matches_known_value():
    assert crc32(b"123456789") == 0xCBF43926


def test_build_output_channel_request_frames_correctly():
    frame = build_output_channel_request(0, 214)
    inner = bytes([0x4F, 0x00, 0x00, 0xD6, 0x00])
    assert frame[:2] == b"\x00\x05"
    assert frame[2:7] == inner
    assert frame[7:] == struct.pack(">I", crc32(inner))


def test_parse_response_returns_payload_on_ok_status():
    inner = b"\x00" + bytes(214)
    frame = struct.pack(">H", len(inner)) + inner + struct.pack(">I", crc32(inner))
    assert parse_response(frame) == bytes(214)


def test_parse_response_raises_on_crc_mismatch():
    inner = b"\x00" + bytes(214)
    good_crc = struct.pack(">I", crc32(inner))
    bad_crc = bytes([good_crc[0] ^ 0xFF]) + good_crc[1:]
    frame = struct.pack(">H", len(inner)) + inner + bad_crc
    with pytest.raises(BadResponse):
        parse_response(frame)


def test_parse_response_raises_on_non_ok_status():
    inner = b"\x01" + bytes(214)
    frame = struct.pack(">H", len(inner)) + inner + struct.pack(">I", crc32(inner))
    with pytest.raises(BadResponse):
        parse_response(frame)
