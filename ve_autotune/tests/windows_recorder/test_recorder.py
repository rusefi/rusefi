import threading

from ve_autotune.windows_recorder.channels import Channels
from ve_autotune.windows_recorder.protocol import BadResponse
from ve_autotune.windows_recorder.recorder import run_session

_CHANNELS = Channels(rpm=1.0, map=1.0, tps=1.0, clt=1.0, afr=1.0)


class FakeConnection:
    """Each entry in `actions` is either a Channels to return, or an
    exception instance to raise, from read_channels()."""

    def __init__(self, actions):
        self._actions = list(actions)
        self.call_count = 0

    def read_channels(self):
        self.call_count += 1
        action = self._actions.pop(0)
        if isinstance(action, Exception):
            raise action
        return action


class FakeLogger:
    def __init__(self):
        self.rows = []

    def append_row(self, t_seconds, channels):
        self.rows.append((t_seconds, channels))


def _make_clock():
    """Returns a fake clock advancing by 1.0 on every call, starting at 0.0."""
    state = {"t": 0.0}

    def clock():
        value = state["t"]
        state["t"] += 1.0
        return value

    return clock


def test_clean_stop_returns_none():
    stop_event = threading.Event()
    connection = FakeConnection([_CHANNELS, _CHANNELS, _CHANNELS])
    logger = FakeLogger()
    sleep_calls = []

    def fake_sleep(seconds):
        sleep_calls.append(seconds)
        if len(sleep_calls) >= 2:
            stop_event.set()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=lambda msg: None,
        on_channels=lambda c: None,
        sleep=fake_sleep,
        clock=_make_clock(),
    )

    assert result is None
    assert len(logger.rows) >= 2
    t_values = [t for t, _ in logger.rows]
    assert t_values == sorted(t_values)


def test_bad_response_is_retried_not_fatal():
    stop_event = threading.Event()
    connection = FakeConnection([BadResponse("noise"), _CHANNELS])
    logger = FakeLogger()
    statuses = []
    sleep_calls = []

    def fake_sleep(_seconds):
        sleep_calls.append(_seconds)
        if len(sleep_calls) >= 2:
            stop_event.set()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=statuses.append,
        on_channels=lambda c: None,
        sleep=fake_sleep,
        clock=_make_clock(),
    )

    assert result is None
    assert len(logger.rows) == 1
    assert any("retry" in msg.lower() or "1/5" in msg for msg in statuses)


def test_timeout_error_is_retried_like_bad_response():
    # A read timeout (e.g. one dropped byte, or firmware staying silent
    # while out of sync) must NOT end the session on its own -- only a run
    # of MAX_CONSECUTIVE_BAD_FRAMES timeouts/bad-responses in a row should.
    stop_event = threading.Event()
    connection = FakeConnection([TimeoutError("read timed out"), _CHANNELS])
    logger = FakeLogger()
    sleep_calls = []

    def fake_sleep(_seconds):
        sleep_calls.append(_seconds)
        if len(sleep_calls) >= 2:
            stop_event.set()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=lambda msg: None,
        on_channels=lambda c: None,
        sleep=fake_sleep,
        clock=_make_clock(),
    )

    assert result is None
    assert len(logger.rows) == 1


def test_fifth_consecutive_bad_response_ends_session():
    stop_event = threading.Event()
    connection = FakeConnection([BadResponse("noise")] * 5)
    logger = FakeLogger()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=lambda msg: None,
        on_channels=lambda c: None,
        sleep=lambda _s: None,
        clock=_make_clock(),
        max_consecutive_bad_frames=5,
    )

    assert result is not None
    assert "bad response" in result.lower()
    assert logger.rows == []
    assert connection.call_count == 5


def test_one_good_frame_resets_bad_counter():
    stop_event = threading.Event()
    actions = [BadResponse("noise")] * 4 + [_CHANNELS] + [BadResponse("noise")] * 4
    connection = FakeConnection(actions)
    logger = FakeLogger()
    call_count = {"n": 0}

    def fake_sleep(_seconds):
        call_count["n"] += 1
        if call_count["n"] >= len(actions):
            stop_event.set()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=lambda msg: None,
        on_channels=lambda c: None,
        sleep=fake_sleep,
        clock=_make_clock(),
        max_consecutive_bad_frames=5,
    )

    assert result is None
    assert connection.call_count == len(actions)


def test_transport_error_ends_session_immediately_not_retried():
    stop_event = threading.Event()
    connection = FakeConnection([OSError("port gone")])
    logger = FakeLogger()

    result = run_session(
        connection,
        logger,
        stop_event=stop_event,
        on_status=lambda msg: None,
        on_channels=lambda c: None,
        sleep=lambda _s: None,
        clock=_make_clock(),
    )

    assert result is not None
    assert "port gone" in result
    assert connection.call_count == 1
