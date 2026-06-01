# Calibration Compatibility Guidance

When adding new calibration parameters to rusEFI, it is important to ensure that existing tunes (saved in older versions of the firmware) remain compatible and functional after a firmware update.

## The Problem
When a new parameter is added to the `engine_configuration_s` struct, it will initially have a value of 0 in existing user tunes. If 0 is not a sensible or safe default for that parameter, the engine might not start or might behave unexpectedly after an update.

## The Solution: `defaultsOrFixOnBurn()`
The `defaultsOrFixOnBurn()` function in `firmware/controllers/algo/defaults/default_base_engine.cpp` is specifically designed to handle these migrations. This function is called during startup and whenever a "burn" (save configuration) operation occurs.

### Implementation Pattern
Use the following pattern to provide a default value for a new parameter if it hasn't been initialized yet:

```cpp
if (engineConfiguration->someNewParameter == 0) {
  engineConfiguration->someNewParameter = DEFAULT_VALUE;
}
```

### Example: Engine Shutdown Period
In the codebase, you can find this example:

```cpp
if (engineConfiguration->engineShutDownPeriod == 0) {
  engineConfiguration->engineShutDownPeriod = 5;
}
```

This ensures that even if an old tune doesn't have `engineShutDownPeriod` set, it will default to 5 seconds instead of 0 (which might cause immediate shutdown or other issues).

## When to use `defaultsOrFixOnBurn()` vs `setDefaultBaseEngine()`
- **`setDefaultBaseEngine()`**: Use this to set the initial values for a *brand new* configuration (e.g., when "Reset to Defaults" is clicked).
- **`defaultsOrFixOnBurn()`**: Use this for *migration* logic. It ensures that *existing* configurations get reasonable values for newly added fields that they don't yet have values for.

## Best Practices
1. **Choose 0 carefully**: If possible, design new parameters so that 0 is a safe/neutral value. If 0 must have a specific meaning, consider if a different "uninitialized" value is needed or if `defaultsOrFixOnBurn()` migration is required.
2. **Avoid breaking changes**: If you must change the meaning of an existing field, you may need more complex migration logic in `defaultsOrFixOnBurn()` to translate old values to new ones.
3. **Validation**: Use `validateConfigOnStartUpOrBurn()` in `engine_controller.cpp` for hard errors if a configuration is truly invalid and cannot be safely fixed automatically.
