from ve_autotune.windows_recorder.bad_frame_tracker import BadFrameTracker


def test_not_dead_until_threshold():
    tracker = BadFrameTracker(5)
    for expected_count in (1, 2, 3, 4):
        dead = tracker.record_failure()
        assert dead is False
        assert tracker.consecutive_failures == expected_count
    dead = tracker.record_failure()
    assert dead is True
    assert tracker.consecutive_failures == 5


def test_reset_on_success():
    tracker = BadFrameTracker(5)
    for _ in range(3):
        tracker.record_failure()
    tracker.record_success()
    assert tracker.consecutive_failures == 0

    dead = tracker.record_failure()
    assert dead is False
    assert tracker.consecutive_failures == 1


def test_single_failure_does_not_trip():
    tracker = BadFrameTracker(5)
    assert tracker.record_failure() is False
