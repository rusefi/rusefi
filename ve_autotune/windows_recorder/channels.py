"""Decoded subset of the ECU's output channels response, as used by the VE
autotune recorder. Field offsets/scales ported verbatim from the Android
app's protocol/OutputChannels.kt (byte offsets into the 214-byte output
channels payload, with the leading 0x00 status byte already stripped by
the caller).
"""

import struct
from dataclasses import dataclass

from ve_autotune.windows_recorder.protocol import BadResponse


@dataclass
class Channels:
    rpm: float
    map: float
    tps: float
    clt: float
    afr: float


def _read_u16(payload: bytes, offset: int) -> int:
    return struct.unpack_from("<H", payload, offset)[0]


def _read_s16(payload: bytes, offset: int) -> int:
    return struct.unpack_from("<h", payload, offset)[0]


def decode_channels(payload: bytes) -> Channels:
    """Decodes `payload` (the 214-byte output channels buffer, status byte
    already stripped by the caller) into a Channels.

    Offsets/scales, copied verbatim from the .ini source lines cited below,
    not re-derived:
     - rpm: U16 @ 4,   scale 1.0
     - map: U16 @ 30,  scale 0.0333333333333333
     - tps: S16 @ 22,  scale 0.01
     - clt: S16 @ 14,  scale 0.01
     - afr: U16 @ 210, scale 0.001 (already the ECU's computed AFR)
    """
    # Guard against an undersized payload before reading any field -- the
    # highest byte offset used below is 210 (AFR's U16 read needs bytes
    # 210-211), so anything shorter than 212 bytes would otherwise raise an
    # uncaught struct.error instead of a handleable BadResponse.
    if len(payload) < 212:
        raise BadResponse(f"Channel payload too short: {len(payload)} bytes")

    # RPMValue: U16 offset 4, scale 1.0 (425490096.ini:3336 "RPMValue = scalar, U16, 4, \"RPM\", 1, 0")
    rpm = _read_u16(payload, 4) * 1.0
    # MAPValue: U16 offset 30, scale 0.03333333333333333 (425490096.ini:3349 "MAPValue = scalar, U16, 30, \"kPa\", 0.03333333333333333, 0")
    map_ = _read_u16(payload, 30) * 0.0333333333333333
    # TPSValue: S16 offset 22, scale 0.01 (425490096.ini:3345 "TPSValue = scalar, S16, 22, \"%\", 0.01, 0")
    tps = _read_s16(payload, 22) * 0.01
    # coolant: S16 offset 14, scale 0.01 (425490096.ini:3341 "coolant = scalar, S16, 14, \"deg C\", 0.01, 0")
    clt = _read_s16(payload, 14) * 0.01
    # AFRValue: U16 offset 210, scale 0.001 (425490096.ini:3425 "AFRValue = scalar, U16, 210, \"AFR\", 0.001, 0")
    afr = _read_u16(payload, 210) * 0.001
    return Channels(rpm=rpm, map=map_, tps=tps, clt=clt, afr=afr)
