"""Tkinter GUI for the VE-autotune Windows BT recorder.

Wires serial_transport/connection/csv_logger/recorder together into a
Start/Stop window: pick a COM port and an output CSV path, watch live
channel values while recording, same wire protocol and CSV format as the
Android app. This is the PyInstaller entry point (see windows_recorder.spec).
"""

import os
import queue
import threading
import time
import tkinter as tk
from tkinter import filedialog, messagebox, ttk

import serial.tools.list_ports

from ve_autotune.windows_recorder.connection import RusefiBtConnection
from ve_autotune.windows_recorder.csv_logger import CsvLogger
from ve_autotune.windows_recorder.recorder import run_session
from ve_autotune.windows_recorder.serial_transport import SerialTransport, open_port

_QUEUE_POLL_INTERVAL_MS = 100


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("VE Autotune Recorder")

        self._stop_event: threading.Event | None = None
        self._queue: "queue.Queue[dict]" = queue.Queue()

        self._build_widgets()
        self._refresh_ports()
        self.protocol("WM_DELETE_WINDOW", self._on_close)
        self.after(_QUEUE_POLL_INTERVAL_MS, self._poll_queue)

    def _build_widgets(self) -> None:
        port_frame = ttk.Frame(self)
        port_frame.pack(fill="x", padx=8, pady=4)
        ttk.Label(port_frame, text="COM port:").pack(side="left")
        self._port_var = tk.StringVar()
        self._port_combo = ttk.Combobox(port_frame, textvariable=self._port_var, width=20)
        self._port_combo.pack(side="left", padx=4)
        ttk.Button(port_frame, text="Refresh", command=self._refresh_ports).pack(side="left")

        path_frame = ttk.Frame(self)
        path_frame.pack(fill="x", padx=8, pady=4)
        ttk.Label(path_frame, text="Output CSV:").pack(side="left")
        self._path_var = tk.StringVar()
        ttk.Entry(path_frame, textvariable=self._path_var, width=40).pack(side="left", padx=4)
        ttk.Button(path_frame, text="Browse...", command=self._browse_output_path).pack(side="left")

        channels_frame = ttk.Frame(self)
        channels_frame.pack(fill="x", padx=8, pady=8)
        self._channel_vars = {}
        for name in ("RPM", "MAP", "TPS", "CLT", "AFR"):
            cell = ttk.Frame(channels_frame)
            cell.pack(side="left", expand=True, fill="x")
            ttk.Label(cell, text=name).pack()
            var = tk.StringVar(value="--")
            ttk.Label(cell, textvariable=var, font=("", 14, "bold")).pack()
            self._channel_vars[name] = var

        button_frame = ttk.Frame(self)
        button_frame.pack(fill="x", padx=8, pady=4)
        self._start_button = ttk.Button(button_frame, text="Start", command=self._on_start)
        self._start_button.pack(side="left")
        self._stop_button = ttk.Button(
            button_frame, text="Stop", command=self._on_stop, state="disabled"
        )
        self._stop_button.pack(side="left", padx=4)

        self._status_var = tk.StringVar(value="Idle")
        ttk.Label(self, textvariable=self._status_var).pack(fill="x", padx=8, pady=4)

    def _refresh_ports(self) -> None:
        ports = [p.device for p in serial.tools.list_ports.comports()]
        self._port_combo["values"] = ports
        if ports and not self._port_var.get():
            self._port_var.set(ports[0])

    def _browse_output_path(self) -> None:
        path = filedialog.asksaveasfilename(defaultextension=".csv", filetypes=[("CSV", "*.csv")])
        if path:
            self._path_var.set(path)

    def _on_start(self) -> None:
        port_name = self._port_var.get().strip()
        output_path = self._path_var.get().strip()
        if not port_name or not output_path:
            messagebox.showerror("VE Autotune Recorder", "Select a COM port and an output CSV path first.")
            return

        if os.path.exists(output_path):
            # Re-pressing Start after a dropped connection keeps the same
            # output path in the entry box, and opening it with "w" would
            # silently truncate the previous ride's log -- ask first,
            # rather than lose data the user may not realize is gone.
            overwrite = messagebox.askyesno(
                "VE Autotune Recorder",
                f"{output_path} already exists. Overwrite it?",
            )
            if not overwrite:
                return

        self._start_button.config(state="disabled")
        self._stop_button.config(state="normal")
        self._status_var.set("Connecting...")

        self._stop_event = threading.Event()
        worker = threading.Thread(
            target=self._run_worker,
            args=(port_name, output_path, self._stop_event, self._queue),
            daemon=True,
        )
        worker.start()

    def _on_stop(self) -> None:
        if self._stop_event is None:
            return
        self._stop_event.set()
        self._stop_button.config(state="disabled")

    def _on_close(self) -> None:
        if self._stop_event is not None:
            self._stop_event.set()
            # Give the worker thread a bounded chance to actually close the
            # CSV file and serial port before the window (and this process,
            # since the worker is a daemon thread) goes away -- otherwise
            # the session's teardown races the process exit instead of
            # always running, as the design requires.
            deadline = time.monotonic() + 5.0
            while self._stop_event is not None and time.monotonic() < deadline:
                try:
                    message = self._queue.get(timeout=0.1)
                except queue.Empty:
                    continue
                self._handle_message(message)
        self.destroy()

    @staticmethod
    def _run_worker(port_name: str, output_path: str, stop_event: threading.Event, out_queue: "queue.Queue[dict]") -> None:
        try:
            csv_file = open(output_path, "w", newline="")
        except OSError as e:
            out_queue.put({"type": "stopped", "error": f"could not open output file: {e}"})
            return

        try:
            ser = open_port(port_name)
        except OSError as e:
            csv_file.close()
            out_queue.put({"type": "stopped", "error": f"could not open {port_name}: {e}"})
            return

        # Everything from here on must still reach the "stopped" message in
        # the finally block below -- an uncaught exception from, say, a
        # full disk during logger.append_row (raised inside run_session)
        # must not leave the GUI's Start/Stop buttons wedged with no error
        # shown, which a narrower try around just run_session's known
        # exception types would do.
        error_reason = None
        try:
            logger = CsvLogger(csv_file)
            logger.write_header()
            out_queue.put({"type": "status", "text": f"Recording to {os.path.basename(output_path)}..."})
            connection = RusefiBtConnection(SerialTransport(ser))
            error_reason = run_session(
                connection,
                logger,
                stop_event=stop_event,
                on_status=lambda text: out_queue.put({"type": "status", "text": text}),
                on_channels=lambda c: out_queue.put({"type": "channels", "channels": c}),
                # stop_event.wait() both pauses between polls AND returns
                # immediately once Stop is clicked, instead of blocking the
                # full second regardless (plain time.sleep).
                sleep=stop_event.wait,
            )
        except Exception as e:
            error_reason = f"unexpected error: {e}"
        finally:
            try:
                csv_file.close()
            except OSError:
                pass
            try:
                ser.close()
            except OSError:
                pass
            out_queue.put({"type": "stopped", "error": error_reason})

    def _poll_queue(self) -> None:
        try:
            while True:
                message = self._queue.get_nowait()
                self._handle_message(message)
        except queue.Empty:
            pass
        self.after(_QUEUE_POLL_INTERVAL_MS, self._poll_queue)

    def _handle_message(self, message: dict) -> None:
        if message["type"] == "status":
            self._status_var.set(message["text"])
        elif message["type"] == "channels":
            c = message["channels"]
            self._channel_vars["RPM"].set(f"{c.rpm:.0f}")
            self._channel_vars["MAP"].set(f"{c.map:.1f}")
            self._channel_vars["TPS"].set(f"{c.tps:.1f}")
            self._channel_vars["CLT"].set(f"{c.clt:.1f}")
            self._channel_vars["AFR"].set(f"{c.afr:.2f}")
        elif message["type"] == "stopped":
            self._stop_event = None
            self._start_button.config(state="normal")
            self._stop_button.config(state="disabled")
            error = message["error"]
            if error is not None:
                self._status_var.set(f"Stopped: {error}")
                messagebox.showerror("VE Autotune Recorder", error)
            else:
                self._status_var.set("Stopped")


if __name__ == "__main__":
    App().mainloop()
