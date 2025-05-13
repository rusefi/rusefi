# Wall Wetting Implementation with RPM Dependency

## Overview

This implementation enhances the Wall Wetting (fuel film) model by adding RPM dependency to the model. The previous implementation only considered CLT (coolant temperature) and MAP (manifold pressure) for the tau and beta parameters, but the new implementation adds RPM as another axis for both parameters.

## Changes Made

1. **Configuration Structure**:
   - Added a new `#define WWAE_RPM_SIZE 8` constant to define the size of the RPM bins
   - Added RPM bins array: `uint8_t[WWAE_RPM_SIZE] autoscale wwRpmBins`
   - Replaced previous 1D tables with 2D tables:
     - `uint8_t[WWAE_RPM_SIZE x WWAE_TABLE_SIZE] autoscale wwTauMapRpmValues`
     - `uint8_t[WWAE_RPM_SIZE x WWAE_TABLE_SIZE] autoscale wwBetaMapRpmValues`
   - Updated `FLASH_DATA_VERSION` to 250515 to reflect configuration structure changes

2. **Calculation Logic**:
   - Modified `WallFuelController::computeTau()` to use `interpolate3d` for the MAP x RPM lookup
   - Modified `WallFuelController::computeBeta()` similarly
   - Maintained the 2D interpolation for temperature (CLT) as originally implemented

3. **TunerStudio Integration**:
   - Added 3D table definitions in `tunerstudio.template.ini`
   - Replaced existing 2D MAP curves with 3D MAP x RPM tables
   - Updated the UI descriptions to indicate RPM dependency

4. **Default Values**:
   - Added initialization code in `default_engine_configuration.cpp`
   - Set up default values for the RPM bins (600 RPM increments)
   - Initialize 3D tables with reasonable default values (tau=100, beta=60)

## How It Works

The wall wetting model calculates how much fuel adheres to intake port walls and how much evaporates based on operating conditions. The model uses two key parameters:

- **Tau (τ)**: The evaporation time constant - how quickly fuel evaporates from the walls
- **Beta (β)**: The wall impact fraction - what percentage of injected fuel hits the walls

With the new implementation:

1. First, a base value for tau and beta is determined by coolant temperature (CLT)
2. Then, this value is modified by a multiplier from a 3D table based on both RPM and MAP
3. The resulting values are used to calculate alpha (fuel remaining on wall per cycle) and adjust fueling accordingly

## Benefits

- Better fueling accuracy across the entire engine operating range
- Improved transient response during rapid RPM changes
- More precise control during conditions where wall wetting characteristics change with engine speed
- Maintains backward compatibility with existing wall wetting configuration data

## Compilation and Update

To complete the implementation:

1. Run the included `firmware/build_update_ww.sh` script on Linux/Mac systems to generate all required configuration files
2. On Windows, run these commands:
   ```
   cd firmware/integration
   java -jar ../java_tools/ConfigDefinition.jar rusefi_config.txt
   cd ../tunerstudio
   ./gen_tunerstudio.sh
   ```
3. Recompile the firmware for your target board
4. Upload the new firmware to your ECU
5. Use TunerStudio to tune the new 3D tables

## Tuning Guide

1. Start with existing wwaeTau and wwaeBeta values if you have a working tune
2. Set the wwTauCltValues and wwBetaCltValues curves based on engine temperature response
3. Tune the 3D tables for different RPM and MAP combinations:
   - Higher MAP values generally require higher tau values (slower evaporation)
   - Higher RPM may require lower tau values (faster evaporation due to increased air velocity)
   - Beta values may need adjustment at different RPM points due to changing fuel atomization

Remember that wall wetting effects are most significant during transient conditions, so log and analyze rapid throttle changes at different RPM ranges for best results. 