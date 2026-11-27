import struct

import pytest

from ve_autotune.windows_recorder.channels import decode_channels
from ve_autotune.windows_recorder.protocol import BadResponse


def test_decode_channels_reads_known_values():
    buf = bytearray(214)
    struct.pack_into("<H", buf, 4, 3500)  # RPM raw -> 3500.0
    struct.pack_into("<h", buf, 14, 8500)  # CLT raw -> 85.0
    struct.pack_into("<h", buf, 22, 2550)  # TPS raw -> 25.5
    struct.pack_into("<H", buf, 30, 3000)  # MAP raw -> ~100.0
    struct.pack_into("<H", buf, 210, 14700)  # AFR raw -> 14.7

    c = decode_channels(bytes(buf))

    assert c.rpm == 3500.0
    assert c.clt == 85.0
    assert c.tps == 25.5
    assert c.map == pytest.approx(100.0, abs=0.01)
    assert c.afr == pytest.approx(14.7)


def test_decode_channels_raises_on_short_payload():
    with pytest.raises(BadResponse):
        decode_channels(bytes(211))
