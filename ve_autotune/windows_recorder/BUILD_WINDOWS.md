# Building the Windows .exe

This app is developed and tested on Linux, so the `.exe` itself has to be
built on your own Windows machine (PyInstaller does not reliably cross-build
Windows executables from Linux). Everything else — the protocol, the
logger, the GUI logic — is already fully implemented and tested; this is
just the final packaging step.

## 1. Install Python

Install Python 3.10 or newer from [python.org](https://www.python.org/downloads/)
if you don't already have it. During setup, tick "Add Python to PATH".

## 2. Get this folder onto the Windows machine

Copy the whole `ve_autotune/windows_recorder/` folder (this one) to your
Windows machine, keeping its contents together.

## 3. Build

Open a terminal (PowerShell or Command Prompt) in this folder and run:

```
pip install -r requirements.txt
pyinstaller windows_recorder.spec
```

The finished app appears at `dist\VeAutotuneRecorder.exe`. You can copy
that one file anywhere and run it — it doesn't need Python installed to run,
only to build.

## 4. Pair the microRusEFI's Bluetooth module

In Windows: **Settings → Bluetooth & devices → Add device**, and pair with
the microRusEFI's Bluetooth module like any other Bluetooth device.

Once paired, find the COM port Windows assigned it: **Settings → Bluetooth
& devices → Devices → More Bluetooth settings → COM Ports** tab. Note the
"Outgoing" port (e.g. `COM5`).

## 5. Run it

Launch `VeAutotuneRecorder.exe`, pick that COM port from the dropdown (or
click Refresh if it's not listed yet), choose where to save the CSV log,
and click Start.

## If the build fails

If `pyinstaller windows_recorder.spec` fails with a missing-module error
(for example something under `serial.tools`), open `windows_recorder.spec`
and add `hiddenimports=["serial.tools.list_ports"]` to the `Analysis(...)`
call near the top, then re-run the build command.
