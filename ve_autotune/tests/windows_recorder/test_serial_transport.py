import pytest

from ve_autotune.windows_recorder.serial_transport import SerialTransport


class FakeSerial:
    def __init__(self, chunks):
        self._chunks = list(chunks)
        self.written = b""

    def write(self, data: bytes) -> None:
        self.written += data

    def read(self, n: int) -> bytes:
        return self._chunks.pop(0) if self._chunks else b""


def test_write_passes_bytes_through():
    fake = FakeSerial([])
    SerialTransport(fake).write(b"abc")
    assert fake.written == b"abc"


def test_read_fully_accumulates_across_short_reads():
    fake = FakeSerial([b"ab", b"cde"])
    result = SerialTransport(fake).read_fully(5)
    assert result == b"abcde"


def test_read_fully_raises_timeout_on_empty_read():
    fake = FakeSerial([b"ab", b""])
    with pytest.raises(TimeoutError):
        SerialTransport(fake).read_fully(5)
