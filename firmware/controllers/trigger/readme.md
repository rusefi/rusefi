# Trigger Decoding

rusEFI uses a flexible, programmatic approach to engine position sensing (trigger decoding).

## Core Concepts

### 1. Trigger Signals
An ECU typically receives signals from:
- **Crankshaft Position Sensor (CKP):** Primary source for engine speed and position within a single revolution.
- **Camshaft Position Sensor (CMP):** Used to identify the 720° engine cycle (which stroke the engine is on) and for Variable Valve Timing (VVT).

rusEFI represents these as `SHAFT_PRIMARY` and `SHAFT_SECONDARY` signals.

### 2. Trigger Waveform (`TriggerWaveform`)
The "shape" of a trigger wheel is defined by a sequence of events (Rising or Falling edges) at specific angles.
- **Cycle Duration:** 720° for 4-stroke engines, 360° for 2-stroke.
- **Events:** Defined using `addEventAngle`, `addSkippedToothTriggerEvents`, etc.
- **Synchronization Point:** A unique pattern in the waveform (e.g., a missing tooth or a specific gap ratio) used to align the software state with the physical engine position.

### 3. Synchronization Algorithm
The primary synchronization method is based on **Gap Ratios**. 
The decoder tracks the duration between consecutive edges (`toothDurations`). 
A synchronization point is identified when the ratio of the current gap to the previous gap falls within a specific range:

`Ratio = CurrentGap / PreviousGap`

For a 36-1 missing tooth wheel, the gap at the missing tooth is roughly 2x the normal gap. The sync ratio would be approximately 2.0.

### 4. Trigger Decoder (`TriggerDecoder`)
The `TriggerDecoder` class maintains the runtime state:
- **`decodeTriggerEvent`:** The main entry point called on every sensor edge.
- **`shaft_is_synchronized`:** Boolean flag indicating if the decoder has found the sync point.
- **`current_index`:** The current position within the `TriggerWaveform` event sequence.

## Adding a New Trigger

1.  **Define the Shape:** In `firmware/controllers/trigger/decoders/`, create or update a decoder file. Use `TriggerWaveform` methods to add teeth and set synchronization ratios.
2.  **Registration:** Add the new trigger type to `rusefi_config.txt` and `rusefi_enums.h`.
3.  **Initialization:** Update `TriggerCentral::initialize` to call your new shape configuration logic.
4.  **Verification:**
    - Use **Unit Tests** (`unit_tests/tests/trigger/`) with real logic analyzer traces.
    - Use the **Engine Sniffer** in TunerStudio/rusEFI Console to visualize the trigger edges and synchronization.

## Debugging

- **Verbose Trigger Logging:** Set `setVerboseTrigger(true)` in unit tests or enable `VerboseTriggerSynchDetails` in the configuration.
- **Gap Printing:** The firmware can print the detected gap ratios to the console when synchronization fails or when verbose logging is enabled.
- **Trigger Error Counters:** `totalTriggerErrorCounter` and `orderingErrorCounter` help identify wiring issues or electrical noise.

---
*For more detail, see the source code in `firmware/controllers/trigger/` and the Doxygen comments in `rusefi.cpp`.*
