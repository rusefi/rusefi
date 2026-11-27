"""Poll loop driving a recording session, ported from the Android app's
RecordingService.kt's polling coroutine.

Takes an injected connection/logger/sleep/clock so the retry policy is
testable without a display, a real serial port, or real timing. The caller
(the GUI's worker thread) owns opening/closing the serial port and CSV
file; this function only runs the read-decode-log-repeat loop.
"""

import threading
import time
from typing import Callable

from ve_autotune.windows_recorder.bad_frame_tracker import BadFrameTracker
from ve_autotune.windows_recorder.channels import Channels
from ve_autotune.windows_recorder.protocol import BadResponse

_POLL_INTERVAL_SECONDS = 1.0


def run_session(
    connection,
    logger,
    *,
    stop_event: threading.Event,
    on_status: Callable[[str], None],
    on_channels: Callable[[Channels], None],
    sleep: Callable[[float], None] = time.sleep,
    clock: Callable[[], float] = time.monotonic,
    max_consecutive_bad_frames: int = 5,
) -> str | None:
    """Runs the poll loop until stop_event is set or the link is judged
    dead. Returns an error reason string, or None for a clean stop."""
    start_time = clock()
    bad_frames = BadFrameTracker(max_consecutive_bad_frames)
    error_reason: str | None = None

    while not stop_event.is_set():
        try:
            channels = connection.read_channels()
        except (BadResponse, TimeoutError) as e:
            # A read timeout is treated the same as a bad frame (not a
            # fatal transport error): the 2-second read timeout exists
            # specifically so a dead link "surfaces promptly as a bad
            # frame" (one dropped byte, or firmware staying silent while
            # out of sync, shouldn't end the whole ride on its own -- only
            # MAX_CONSECUTIVE_BAD_FRAMES in a row should). A deeper OSError
            # (port physically gone, access denied) is NOT caught here --
            # it falls through to the except OSError branch below and ends
            # the session immediately, since retrying a broken port can't
            # recover. TimeoutError is itself an OSError subclass, so this
            # clause must be listed first to take priority over that one.
            dead = bad_frames.record_failure()
            if dead:
                error_reason = f"connection lost: bad response from ECU: {e}"
                break
            on_status(
                f"Recording... bad frame, retrying... "
                f"({bad_frames.consecutive_failures}/{max_consecutive_bad_frames})"
            )
            sleep(_POLL_INTERVAL_SECONDS)
            continue
        except OSError as e:
            if not stop_event.is_set():
                error_reason = f"serial I/O error: {e}"
            break

        bad_frames.record_success()
        t_seconds = clock() - start_time
        logger.append_row(t_seconds, channels)
        on_channels(channels)
        on_status(f"Recording... RPM = {int(channels.rpm)}")
        sleep(_POLL_INTERVAL_SECONDS)

    return error_reason
