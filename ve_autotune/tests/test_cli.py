from ve_autotune.cli import main


def test_burn_proceeds_without_prompt_when_no_diff(monkeypatch, tmp_path, capsys):
    calls = []
    def fake_run_mcp(tool, args):
        calls.append((tool, args))
        return '{"success": true}'
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
