import shutil

from ve_autotune.cli import main
from ve_autotune.pipeline.analyze import write_output
from ve_autotune.pipeline.correction import CellResult
from ve_autotune.pipeline.msq import load_tune

_RIDE_A_MSQ = "ve_autotune/tests/fixtures/ride_a.msq"


def test_burn_proceeds_without_prompt_when_no_diff(monkeypatch, tmp_path, capsys):
    calls = []
    def fake_run_mcp(tool, args):
        calls.append((tool, args))
        return '{"success": true, "verified": true}'
    # live tune export + msq_path both resolve to identical field dicts in this fake
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"veTable": [[1]]})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"veTable": [[2]]})
    rc = main(["burn", "whatever.msq"], run_mcp=fake_run_mcp)
    assert rc == 0
    assert any(tool == "write_tune" for tool, _ in calls)


def test_burn_requires_confirmation_when_fields_diverge(monkeypatch, capsys):
    monkeypatch.setattr("builtins.input", lambda _: "no")
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"globalTriggerAngleOffset": -5.0})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"globalTriggerAngleOffset": -2.0})
    calls = []
    rc = main(["burn", "whatever.msq"], run_mcp=lambda tool, args: calls.append((tool, args)))
    assert not calls  # declined -> write_tune never called
    assert rc != 0


def test_burn_reports_error_when_write_tune_fails(monkeypatch):
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"veTable": [[1]]})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"veTable": [[2]]})
    calls = []

    def fake_run_mcp(tool, args):
        calls.append((tool, args))
        return '{"success": false, "verified": false, "error": "ecu not responding"}'

    rc = main(["burn", "whatever.msq"], run_mcp=fake_run_mcp)
    assert rc != 0
    assert any(tool == "write_tune" for tool, _ in calls)  # it was attempted


def test_burn_reports_error_when_read_tune_fails(monkeypatch):
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"veTable": [[2]]})

    def fake_run_mcp(tool, args):
        if tool == "read_tune":
            return '{"success": false, "error": "not connected"}'
        return '{"success": true, "verified": true}'

    rc = main(["burn", "whatever.msq"], run_mcp=fake_run_mcp)
    assert rc != 0


def test_burn_wraps_run_mcp_exception_as_clean_error(monkeypatch, capsys):
    monkeypatch.setattr("ve_autotune.cli._read_live_tune_fields", lambda run_mcp: {"veTable": [[1]]})
    monkeypatch.setattr("ve_autotune.cli._read_msq_fields", lambda path: {"veTable": [[2]]})

    def exploding_run_mcp(tool, args):
        raise RuntimeError("subprocess exploded")

    rc = main(["burn", "whatever.msq"], run_mcp=exploding_run_mcp)
    assert rc != 0
    out = capsys.readouterr().out
    assert "subprocess exploded" in out
    assert "Traceback" not in out


def test_burn_warns_and_requires_confirmation_when_ve_table_differs_from_baseline(monkeypatch, tmp_path):
    # write_output copies baseline.msq next to corrected.msq; a hand-edit to
    # the live ECU's VE table since recording must be caught even though
    # veTable is otherwise excluded from the general field diff. This test
    # exercises the real _read_live_tune_fields/_read_msq_fields code paths
    # (only run_mcp is faked) so the comparison is real text vs. real text.
    baseline = load_tune(_RIDE_A_MSQ)
    results = [
        CellResult(row=0, col=0, old_ve=baseline.ve_table[0][0], new_ve=baseline.ve_table[0][0],
                   delta_pct=0.0, sample_count=0, measured_afr=0.0, target_afr=14.7, status="not_enough_data"),
    ]
    out_dir = str(tmp_path / "out")
    msq_path, _ = write_output(baseline, _RIDE_A_MSQ, results, out_dir)

    def fake_run_mcp(tool, args):
        if tool == "read_tune":
            # Simulate the live ECU's exported tune: baseline.msq with its
            # first VE cell hand-edited.
            text = open(_RIDE_A_MSQ, encoding="latin-1").read()
            text = text.replace(
                "29.0 29.0 29.0 29.0 32.0 32.0 34.0 36.0",
                "31.0 29.0 29.0 29.0 32.0 32.0 34.0 36.0",
                1,
            )
            with open(args["path"], "w", encoding="latin-1") as f:
                f.write(text)
            return '{"success": true}'
        raise AssertionError(f"unexpected tool call: {tool}")

    monkeypatch.setattr("builtins.input", lambda _: "no")
    rc = main(["burn", msq_path], run_mcp=fake_run_mcp)
    assert rc != 0


def test_burn_proceeds_when_ve_table_matches_baseline(monkeypatch, tmp_path):
    baseline = load_tune(_RIDE_A_MSQ)
    results = [
        CellResult(row=0, col=0, old_ve=baseline.ve_table[0][0], new_ve=99.0,
                   delta_pct=0.0, sample_count=25, measured_afr=14.0, target_afr=14.7, status="corrected"),
    ]
    out_dir = str(tmp_path / "out")
    msq_path, _ = write_output(baseline, _RIDE_A_MSQ, results, out_dir)

    calls = []

    def fake_run_mcp(tool, args):
        calls.append((tool, args))
        if tool == "read_tune":
            # Live VE table matches baseline.msq exactly (no hand-edit);
            # only corrected.msq's veTable differs, which is expected and
            # must not be flagged.
            shutil.copyfile(_RIDE_A_MSQ, args["path"])
            return '{"success": true}'
        return '{"success": true, "verified": true}'

    rc = main(["burn", msq_path], run_mcp=fake_run_mcp)
    assert rc == 0
    assert any(tool == "write_tune" for tool, _ in calls)
