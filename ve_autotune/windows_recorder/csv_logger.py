"""Writes the recorded channel stream to CSV, one row per sample. Ported
from the Android app's logger/CsvLogger.kt.

Flushes after every write (header and each row) so that if the process
crashes or the serial link drops mid-session, whatever rows were already
captured are safely on disk rather than sitting in a buffer.
"""

from ve_autotune.windows_recorder.channels import Channels


class CsvLogger:
    def __init__(self, out):
        self._out = out

    def write_header(self) -> None:
        self._out.write("Time,RPM,MAP,TPS,CLT,Air/Fuel Ratio\n")
        self._out.flush()

    def append_row(self, t_seconds: float, c: Channels) -> None:
        self._out.write(f"{t_seconds},{c.rpm},{c.map},{c.tps},{c.clt},{c.afr}\n")
        self._out.flush()
