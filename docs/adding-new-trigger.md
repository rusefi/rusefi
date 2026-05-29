# Adding a New Trigger Skeleton

To add a new trigger decoder to rusEFI, follow these steps:

1.  **Identify an unused trigger type index**:
    In `firmware/controllers/algo/engine_types.h`, find the first available `TT_UNUSED_X` entry in the `trigger_type_e` enum.

2.  **Rename the unused entry**:
    Replace the chosen `TT_UNUSED_X` with your new trigger name (e.g., `TT_VVT_MITSUBISHI_6G75`).

3.  **Add a declaration for the initialization method**:
    In the appropriate header file (e.g., `firmware/controllers/trigger/decoders/trigger_mitsubishi.h`), add a declaration for your new initialization function:
    ```cpp
    void initializeMitsubishi6G75Cam(TriggerWaveform *s);
    ```

4.  **Add an empty implementation**:
    In the corresponding source file (e.g., `firmware/controllers/trigger/decoders/trigger_mitsubishi.cpp`), add an empty implementation of the method:
    ```cpp
    void initializeMitsubishi6G75Cam(TriggerWaveform *s) {
        // To be implemented
    }
    ```

5.  **Hook up the new trigger in the structure switch**:
    In `firmware/controllers/trigger/decoders/trigger_structure.cpp`, add a new case to the switch statement within `initializeTriggerStructure`:
    ```cpp
    case trigger_type_e::TT_VVT_MITSUBISHI_6G75:
        initializeMitsubishi6G75Cam(this);
        break;
    ```

6.  **Update the TunerStudio configuration**:
    In `firmware/integration/rusefi_config.txt`, find the `#define trigger_type_e_enum` line. Update the string at the corresponding index to match your new trigger's name.

7.  **Verify**:
    Compile the firmware and run unit tests (e.g., using `unit_tests/test.sh`). Unit tests execution produces `triggers.txt` (a snapshot of all trigger definitions), which is then used to generate trigger images.
