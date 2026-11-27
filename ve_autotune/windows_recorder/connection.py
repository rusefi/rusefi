"""Reads the ECU's output channels over a ByteTransport. Ported from the
Android app's bt/RusefiBtConnection.kt.

This is the only thing this module does: it sends exactly one
output-channel read request (built by protocol.build_output_channel_request,
the only command this app is allowed to build) and decodes exactly one
response. No other command can be constructed or sent through this class.
"""

import struct

from ve_autotune.windows_recorder.channels import Channels, decode_channels
from ve_autotune.windows_recorder.protocol import (
    BadResponse,
    build_output_channel_request,
    parse_response,
)
from ve_autotune.windows_recorder.serial_transport import ByteTransport

_OUTPUT_CHANNELS_OFFSET = 0
_OUTPUT_CHANNELS_COUNT = 214

#: Expected length of the inner response body: 1 status byte +
#: OUTPUT_CHANNELS_COUNT channel bytes. A response whose declared length
#: prefix is anything else is untrustworthy -- reading exactly that many
#: bytes could otherwise block forever if the length was corrupted in
#: transit and the ECU never sends that many (it only replies once per
#: request).
_EXPECTED_INNER_LENGTH = 1 + _OUTPUT_CHANNELS_COUNT


class RusefiBtConnection:
    def __init__(self, transport: ByteTransport):
        self._transport = transport

    def read_channels(self) -> Channels:
        self._transport.write(
            build_output_channel_request(_OUTPUT_CHANNELS_OFFSET, _OUTPUT_CHANNELS_COUNT)
        )

        length_prefix = self._transport.read_fully(2)
        inner_length = struct.unpack_from(">H", length_prefix, 0)[0]
        if inner_length != _EXPECTED_INNER_LENGTH:
            # Don't trust a corrupted length prefix enough to call
            # read_fully(inner_length) with it -- that could block forever
            # waiting for bytes the ECU will never send. Fail fast and
            # loudly instead, as an ordinary BadResponse the polling loop
            # already knows how to skip-and-retry.
            raise BadResponse(
                f"Unexpected frame length: {inner_length} (expected {_EXPECTED_INNER_LENGTH})"
            )
        inner = self._transport.read_fully(inner_length)
        crc = self._transport.read_fully(4)

        frame = length_prefix + inner + crc
        payload = parse_response(frame)
        return decode_channels(payload)
