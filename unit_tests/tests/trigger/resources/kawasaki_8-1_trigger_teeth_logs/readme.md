Real cranking recordings from a Kawasaki engine with an 8-1 crank trigger wheel
(8 teeth minus one, i.e. 7 physical teeth -> 7 rising edges per revolution with a
missing-tooth gap every 7 teeth).

Captured for [issue #10106](https://github.com/rusefi/rusefi/issues/10106)
"crank by edge for low count triggers on high comp engines": with so few teeth on
a high-compression engine, crank speed fluctuates heavily within a single
revolution during cranking (tooth-to-tooth intervals vary by more than 3x), which
challenges missing-tooth gap detection.

### Folder layout

Two capture sessions of the same engine, recorded 2026-08-19 on firmware
v20260818:

* `with spark plug/` - spark plugs installed (full compression); ~300 RPM
  cranking with large intra-revolution speed swings.
* `without spark plug/` - spark plugs removed (no compression); smoother
  tooth-to-tooth intervals, includes pauses between cranking attempts.

### File formats

* `re_NN.teeth` - on-ECU SD-card trigger tooth logs written by the firmware
  tooth logger in CSV mode (`tooth_logger.cpp`, `sdTriggerLogCsv`). Header:
  `Time[s], Primary, Cam 1..4, Sync, TDC, Coils, Injectors, ACR, VBatt, ET,
  InstantMAP, TPS`. The `re_<index>` names mean the RTC had no valid date at
  capture time. Same format as e.g. `../hd-re-sd.teeth`, so they can be fed to
  tests via `CsvReader` (see `test_real_nissan_maxima.cpp` for the pattern).
* `2026-08-19_*.csv` - composite trigger log exported from the TunerStudio /
  rusEFI console high-speed tooth logger (`RefTime,PriLevel,SecLevel,...`
  format, ~2.6 s each).

### Recording summary

| File | Duration | Primary rising edges | ~avg RPM |
|------|----------|----------------------|----------|
| with spark plug/re_10.teeth | 6.3 s | 223 | 300 |
| with spark plug/re_11.teeth | 4.5 s | 156 | 300 |
| with spark plug/re_12.teeth | 4.6 s | 160 | 300 |
| without spark plug/re_18.teeth | 5.7 s | 122 | 180 |
| without spark plug/re_19.teeth | 5.7 s | 108 | 160 |
| without spark plug/re_20.teeth | 3.9 s | 132 | 290 |

See also `../readme.md` for general notes on trigger recordings used by unit
tests.
