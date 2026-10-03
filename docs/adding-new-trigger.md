# Adding a New Trigger

Adding a trigger includes defining its waveform, registering it in firmware and
TunerStudio, testing synchronization, and documenting the required engine checks.
The GM Gen II LT1 Optispark low-resolution trigger is a worked example:

- [Waveform implementation](../firmware/controllers/trigger/decoders/trigger_gm.cpp): `initializeGmLt1Optispark8`.
- [Decoder tests](../unit_tests/tests/trigger/test_gm_lt1_optispark.cpp).
- [Configuration and engine-validation notes](triggers/gm-lt1-optispark-8.md).

## 1. Establish the waveform and its reference

Before coding, record or identify:

- Which crank/cam signals are used, their input channels and polarity.
- Whether the pattern repeats every crank revolution or every full engine cycle.
- Rising and falling edge angles, expressed in crank degrees, including the
  interval across the end of the cycle.
- Which feature identifies the synchronization point and whether it establishes
  the full 720-degree phase of a four-stroke engine.
- The relationship between the waveform's angle origin and cylinder #1
  compression TDC, or an explicit statement that this is still unknown.

Prefer actual cranking and running captures. Keep the original recording and
exported data with the test resources; see the
[capture contribution guide](../unit_tests/tests/trigger/resources/readme.md).
If starting from a signal-generator definition, record its exact revision and
label the resulting tests as synthetic.

Optispark uses the eight-slot low-resolution track at distributor/cam speed.
Its rising edges are 90 crank degrees apart, but its varying pulse widths
identify the 720-degree cycle. Both edges are therefore needed. The 360-slot
track is unused. Its reference waveform comes from a pinned Ardu-Stim revision;
the reference does not establish cylinder #1 compression TDC.

## 2. Register a stable trigger ID and TunerStudio label

1. In [engine_types.h](../firmware/controllers/algo/engine_types.h), add an
   explicitly numbered entry to `trigger_type_e`. Preserve existing numeric
   values because saved tunes use them. When appending, use the current
   `TT_UNUSED` value for the new type and advance `TT_UNUSED` to one past it.
   This sentinel is the exclusive upper bound for the all-trigger tests.
   Optispark added `TT_GM_LT1_OPTISPARK_8 = 99` and moved `TT_UNUSED` to 100;
   those are the example's IDs, not the next available IDs.
2. In [rusefi_config.txt](../firmware/integration/rusefi_config.txt), update
   `#define trigger_type_e_enum` at the matching zero-based index. Append a label
   when appending an ID; preserve existing label positions. An `INVALID` label
   does not identify a free ID: active cam-only/VVT types also use that label.
3. Declare the initialization function in the appropriate decoder header, such
   as [trigger_gm.h](../firmware/controllers/trigger/decoders/trigger_gm.h):

   ```cpp
   void initializeGmLt1Optispark8(TriggerWaveform *s);
   ```

4. Implement it in the corresponding decoder source and add a case to
   `TriggerWaveform::initializeTriggerWaveform` in
   [trigger_structure.cpp](../firmware/controllers/trigger/decoders/trigger_structure.cpp):

   ```cpp
   case trigger_type_e::TT_GM_LT1_OPTISPARK_8:
       initializeGmLt1Optispark8(this);
       break;
   ```

If creating a decoder source file, include its header where needed and register
the source in [trigger.mk](../firmware/controllers/trigger/trigger.mk).
Builds regenerate configuration headers, enum conversions and INI files from
their inputs. Follow [repository guidance](../CLAUDE.md): do not hand-edit or
stage generated files.

## 3. Define geometry and synchronization

Use the existing waveform and decoder APIs where they can describe the pattern.
Choose the operation mode and synchronization edges deliberately. Optispark's
complete waveform implementation is:

```cpp
void initializeGmLt1Optispark8(TriggerWaveform *s) {
    s->initialize(FOUR_STROKE_CAM_SENSOR, SyncEdge::Both);

    static constexpr angle_t widths[] = {14, 4, 24, 4, 34, 4, 44, 4};
    for (size_t i = 0; i < efi::size(widths); i++) {
        const angle_t rise = 86 + 90 * i;
        s->addEvent720(rise, TriggerValue::RISE);
        s->addEvent720(rise + widths[i], TriggerValue::FALL);
    }

    s->setTriggerSynchronizationGap2(0.12f, 0.21f);
    s->setSecondTriggerSynchronizationGap2(16.0f, 27.0f);
    s->setTriggerSynchronizationGap3(2, 0.065f, 0.11f);
}
```

Here `addEvent720` uses crank degrees over the 720-degree cycle and defaults to
the primary input. The shape has 16 events. Synchronization occurs at the
falling edge at 100 degrees, the end of the 14-degree pulse.

For this both-edge pattern, each synchronization ratio compares consecutive
edge intervals. The configured history at the synchronization point is:

| History index | Nominal ratio | Accepted window | API |
| --- | --- | --- | --- |
| 0: most recent | `14 / 86` | `(0.12, 0.21)` | `setTriggerSynchronizationGap2` |
| 1: preceding | `86 / 4` | `(16, 27)` | `setSecondTriggerSynchronizationGap2` |
| 2: third | `4 / 46` | `(0.065, 0.11)` | `setTriggerSynchronizationGap3(2, ...)` |

Check that the sequence identifies the intended phase throughout the cycle and
under expected speed changes. Also check reverse rotation: Optispark's reverse
sequence `14/76`, `76/4`, `4/86` fits the first two windows but fails the third.
The third check is necessary for this definition. Derive and test the windows
for each new pattern rather than copying these values.

Keep waveform phase and physical TDC calibration explicit. Optispark leaves
`tdcPosition` at its default zero because its engine TDC relationship is still
unverified. A unique software synchronization point alone does not establish
the correct physical ignition or injection phase.

## 4. Add independent tests

Add tests under `unit_tests/tests/trigger/` and register the source in
[tests.mk](../unit_tests/tests/tests.mk). Follow the
[Optispark tests](../unit_tests/tests/trigger/test_gm_lt1_optispark.cpp) for the
following checks:

| Area | What to assert |
| --- | --- |
| Geometry | Operation mode, channels, edge selection, event angles/count and expected synchronization point. |
| Acquisition | Correct event index/phase from every possible starting edge, with a bounded acquisition time. |
| Speed | Cranking and running speeds; acceleration and interval jitter. |
| Invalid input | Reverse rotation and plausible incorrect patterns do not establish false phase. |
| Signal faults | Missing and extra pulses produce expected errors/loss of sync, then clean input reacquires the correct phase. |
| Stop/restart | A long pause clears synchronization and new input reacquires it. |
| Input integration | Feeding the primary/secondary input path produces correct crankshaft RPM and phase status. |

Use edge timings derived independently from captures or the reference waveform.
Do not generate the test input from the production waveform being tested: the
same geometry error could otherwise pass both sides of the test. Assert phase
and error counters as well as the synchronized flag.

The Optispark suite feeds `TriggerDecoderBase` directly for detailed phase/fault
checks, then uses `EngineTestHelper::firePrimaryTriggerRise/Fall` to verify RPM
through the primary input path. This catches a possible cam/crank factor-of-two
error. Its extra-pulse test mocks a running RPM because overflow diagnostics
are suppressed at zero RPM.

For real recordings, use the existing `test_real_*.cpp` examples and
`RealTriggerHelper`. See [CLAUDE.md](../CLAUDE.md) for `.teeth` capture gaps and
per-test logging details. Synthetic coverage should be supplemented with engine
captures when available.

## 5. Build, run tests and inspect the exported shape

From `unit_tests/`, run the focused suite and then the full suite:

```bash
./test.sh Optispark
./test.sh
```

Replace `Optispark` with the new suite's name. `test.sh` builds and runs the
tests. Also build a relevant firmware target using its board compile script;
host tests alone do not validate the firmware build.

The [AllTriggersFixture](../unit_tests/tests/trigger/test_all_triggers.cpp)
exports `triggers.txt` in the test process's working directory. Running the full
suite from `unit_tests/` refreshes `unit_tests/triggers.txt`. A focused
`./test.sh Optispark` run does **not** execute that fixture or refresh the export.
Inspect the new entry's mode, channels, synchronization windows and angles.

The Java `TriggerImage` tool consumes this export to draw trigger images. The
[console CI workflow](../.github/workflows/build-rusEFI-console.yaml) shows the
current invocation and uploads the images as the `triggers` artifact. Inspect
the resulting image as an additional geometry check. Generated exports and
images do not replace independent decoder tests.

## 6. Document configuration and engine validation

Add a page under `docs/triggers/`, following the
[Optispark configuration notes](triggers/gm-lt1-optispark-8.md). Include:

- The exact TunerStudio trigger label, required input assignments, polarity,
  edge requirements and gap-override settings.
- Which tracks/signals are supported and any operating limitations.
- The waveform source, geometry, synchronization point and known TDC reference.
- Whether an engine preset exists. Selecting a trigger alone does not install
  cylinder count, firing order, ignition/injection modes or an engine tune.
- What has been validated by synthetic tests, recorded captures, bench tests
  and actual engine operation, with remaining checks stated explicitly.

Before describing support as engine-validated, confirm real cranking/running
polarity and geometry, acquisition/reacquisition under compression-induced
speed changes, cylinder #1 compression phase, and timing at cranking, idle and
higher RPM with no sync errors. A timing light alone cannot distinguish
compression from exhaust TDC. Sparse patterns also need engine checks of timing
accuracy because the ECU must interpolate over longer intervals.

Update the matching user-facing wiki page in `../rusefi_documentation` when that
checkout is available and the change affects documented user behavior, as
required by [repository guidance](../CLAUDE.md).
