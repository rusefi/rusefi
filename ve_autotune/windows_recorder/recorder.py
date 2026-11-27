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
        except BadResponse as e:
            dead = bad_frames.record_failure()
            if dead:
                error_reason = f"bad response from ECU: {e}"
                break
            on_status(
                f"Recording... bad frame, retrying... "
                f"({bad_frames.consecutive_failures}/{max_consecutive_bad_frames})"
            )
            sleep(_POLL_INTERVAL_SECONDS)
            continue
        except (OSError, TimeoutError) as e:
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
