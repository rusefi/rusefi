import io

from ve_autotune.windows_recorder.channels import Channels
from ve_autotune.windows_recorder.csv_logger import CsvLogger


def test_write_header_writes_exact_header_line():
    buf = io.StringIO()
    CsvLogger(buf).write_header()
    assert buf.getvalue() == "Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio\n"


def test_append_row_writes_comma_joined_values():
    buf = io.StringIO()
    c = Channels(rpm=3500.0, map=100.0, tps=25.5, clt=85.0, afr=14.7)
    CsvLogger(buf).append_row(1.5, c)
    assert buf.getvalue() == "1.5,3500.0,100.0,25.5,85.0,14.7\n"


class _FlushTrackingWriter:
    def __init__(self):
        self.flushed = False

    def write(self, _text):
        pass

    def flush(self):
        self.flushed = True


def test_append_row_flushes():
    fake = _FlushTrackingWriter()
    c = Channels(rpm=1.0, map=1.0, tps=1.0, clt=1.0, afr=1.0)
    CsvLogger(fake).append_row(0.0, c)
    assert fake.flushed is True
