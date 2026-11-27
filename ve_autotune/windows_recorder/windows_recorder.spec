# -*- mode: python ; coding: utf-8 -*-
#
# PyInstaller spec for the VE-Autotune Windows BT Recorder GUI.
#
# Build on your own Windows machine (see BUILD_WINDOWS.md) from this
# directory:
#   pip install -r requirements.txt
#   pyinstaller windows_recorder.spec
# The finished app appears at dist/VeAutotuneRecorder.exe
#
# gui.py imports via the absolute package path
# `ve_autotune.windows_recorder...`, so the repository root (two
# directories up from this spec file -- windows_recorder -> ve_autotune ->
# repo root) must be on the import path. `pathex` below adds it.
#
# If the built .exe fails to start with a "no module named serial.tools"
# (or similar) error, the first thing to try is adding
# hiddenimports=["serial.tools.list_ports"] to the Analysis(...) call below
# -- PyInstaller's automatic dependency detection normally finds this, but
# submodules reached only via a dotted-import-at-call-time can be missed.

a = Analysis(
    ["gui.py"],
    pathex=["../.."],
    hiddenimports=[],
    hookspath=[],
    hooksconfig={},
    runtime_hooks=[],
    excludes=[],
)
pyz = PYZ(a.pure)

exe = EXE(
    pyz,
    a.scripts,
    a.binaries,
    a.datas,
    [],
    name="VeAutotuneRecorder",
    console=False,
    onefile=True,
)
