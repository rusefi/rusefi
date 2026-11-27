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
# repo root) must be on the import path. `pathex` below adds it, resolved
# relative to THIS SPEC FILE'S location (SPECPATH, provided by PyInstaller)
# rather than the current working directory -- `pyinstaller` resolves a
# relative pathex against the cwd, not the spec file, so a plain "../.."
# silently breaks (and still "succeeds") if you ever invoke it from a
# different directory. This also means the repository root -- not just
# this windows_recorder/ folder -- must actually be present on disk next
# to this file: copy the whole `ve_autotune/` package (or the whole repo),
# not only this folder, per BUILD_WINDOWS.md.
#
# If the built .exe fails to start with a "no module named serial.tools"
# (or similar) error, the first thing to try is adding
# hiddenimports=["serial.tools.list_ports"] to the Analysis(...) call below
# -- PyInstaller's automatic dependency detection normally finds this, but
# submodules reached only via a dotted-import-at-call-time can be missed.

import os

a = Analysis(
    ["gui.py"],
    pathex=[os.path.join(SPECPATH, "..", "..")],
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
