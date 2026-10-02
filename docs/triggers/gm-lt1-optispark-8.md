# GM Gen II LT1 Optispark: 8-slot track only

This trigger reads the low-resolution optical track of the GM Gen II LT1
Optispark distributor. It does not read the 360-slot track and is unrelated to
the later GM LS 24x/58x or Gen V LT1 patterns.

## Configuration

- Select **GM LT1 Optispark (8-slot only)** as Trigger Type.
- Assign the low-resolution signal to the **primary trigger input**.
- Leave the secondary trigger input unassigned. No separate cam input is needed
  to identify the 720-degree cycle with this pattern.
- Keep Trigger Gap Override disabled: this definition supplies its own gaps.
- Both edges are required. Use the trigger input inversion setting if necessary
  so the decoded waveform has evenly spaced rising edges and the high widths
  listed below. Reading only the evenly spaced edges loses cylinder phase.
- Set engine displacement, eight cylinders, firing order, ignition and injection
  modes separately. Selecting a trigger does not install an LT1 base tune.

## Reference waveform and synchronization

Geometry is based on the `optispark_lt1` signal-generator definition in
[Ardu-Stim, revision 51f624a](https://github.com/speeduino/Ardu-Stim/blob/51f624a9ae369035211952862befe099c2b93a58/ardustim/ardustim/wheel_defs.h).
Its 720 samples describe one distributor revolution, i.e. 720 crank degrees.
The high-resolution track is discarded; the remaining waveform is:

| Rising edge (crank degrees) | Falling edge | High width |
| --- | --- | --- |
| 86 | 100 | 14 |
| 176 | 180 | 4 |
| 266 | 290 | 24 |
| 356 | 360 | 4 |
| 446 | 480 | 34 |
| 536 | 540 | 4 |
| 626 | 670 | 44 |
| 716 | 720 | 4 |

The ordinary rusEFI decoder evaluates both edges. Synchronization occurs at the
end of the 14-degree pulse: the most recent interval ratio must be in
`(0.12, 0.21)` (nominally `14/86`), the preceding ratio in `(16, 27)`
(nominally `86/4`), and the third in `(0.065, 0.11)` (nominally `4/46`).
This sequence is unique within the reference cycle. The third ratio rejects
reverse rotation: `14/76`, `76/4`, `4/86` would match only the first two windows.
There are only 16 events per 720 degrees; no enlarged trigger buffers or
Optispark-specific code in the central decoder is required.

The angle origin above is a signal-generator reference, **not a verified #1
compression TDC reference**. The default `tdcPosition` remains zero. Determine
the trigger offset and verify cylinder #1 compression phase on the actual
engine before enabling sequential ignition/injection. A timing light alone
cannot distinguish compression from exhaust TDC. Do not assume that a timing
offset from another ECU or a different GM trigger is interchangeable.

## Validation and remaining engine checks

The automated tests feed independent low-resolution edge timings through the
rusEFI decoder. They exercise all 16 starting edges, cranking/running speeds,
changing speed with interval jitter, reverse rotation, invalid equal-width
pulses, missing/extra pulses, stop/restart, waveform geometry, and crankshaft RPM
through the primary trigger input path. These are **synthetic tests, not a recorded engine capture**.

Before treating this as engine-validated support:

1. Record the low-resolution signal during cranking and running; confirm edge
   polarity, window order/widths and the relationship to cylinder #1 compression.
2. Check acquisition/reacquisition with real compression-induced speed changes.
3. Verify timing at cranking, idle and higher RPM, and confirm no sync errors.

Without the 360-slot track the ECU receives fewer position updates and must
interpolate over longer intervals, particularly during cranking. The software
can identify a complete cycle; synthetic tests do not establish its timing
accuracy on an engine.

Background: [Haltech LT1 Optispark description](https://support.haltech.com/portal/en/kb/articles/lt1-350-optispark-engine).
