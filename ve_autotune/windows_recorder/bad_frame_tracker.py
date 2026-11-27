"""Pure consecutive-bad-frame counter, ported from the Android app's
RecordingService.kt's BadFrameTracker class."""


class BadFrameTracker:
    def __init__(self, max_consecutive: int):
        self._max_consecutive = max_consecutive
        self._consecutive_failures = 0

    @property
    def consecutive_failures(self) -> int:
        return self._consecutive_failures

    def record_failure(self) -> bool:
        """Records one more bad frame; returns True once the link should be
        treated as dead (i.e. consecutive_failures has reached max_consecutive)."""
        self._consecutive_failures += 1
        return self._consecutive_failures >= self._max_consecutive

    def record_success(self) -> None:
        """Records a good frame, resetting the streak."""
        self._consecutive_failures = 0
