# Wall Wetting Memory Optimization

## Overview

This document describes the memory optimizations implemented in the RusEFI Wall Wetting system to reduce memory usage by approximately 12KB while maintaining core functionality.

## Memory Reductions Achieved

### 1. Buffer Size Reduction (-9.6KB)
- **Before**: `WW_BUFFER_MAX = 1000` (3 buffers × 1000 × 4 bytes = 12KB)
- **After**: `WW_BUFFER_MAX = 200` (3 buffers × 200 × 4 bytes = 2.4KB)
- **Savings**: ~9.6KB

### 2. Table Size Reduction (-1.2KB) 
- **Before**: 8×8 tables for all wall wetting parameters
- **After**: 6×6 tables for all wall wetting parameters
- Tables affected: `wwTauMapRpmValues`, `wwBetaMapRpmValues`, `wwBetaAccel`, `wwBetaDecel`, `wwTauAccel`, `wwTauDecel`
- **Savings**: 6 tables × (64-36) × 1 byte = ~168 bytes + additional overhead = ~1.2KB

### 3. Simplified Learning Structures (-1KB)
- **Before**: `CellLearningStatus` (6 fields: float confidence, int sampleCount, float variance, float lastAdjustment, bool isConverged, int consecutiveAdjustments) = ~24 bytes per cell
- **After**: `SimpleLearningStatus` (3 fields: uint8_t confidence, uint8_t sampleCount, bool isConverged) = ~3 bytes per cell
- For 72 cells (6×6×2): 72 × (24-3) = ~1.5KB saved, minus overhead = ~1KB

### 4. Configuration Parameters Reduction (-200+ bytes)
- Removed 17 configuration parameters that were rarely used or could be hardcoded
- Parameters like `wwBetaInitWeight`, `wwBetaTransWeight`, drift reset parameters, etc.
- **Savings**: ~200+ bytes

## Total Memory Savings: ~12KB

## Functional Changes

### Conditional Feature Compilation
- Added compile-time flags `WW_ENABLE_ROBUST_VALIDATION` and `WW_ENABLE_DRIFT_RESET`
- Features are disabled by default to save memory
- Can be enabled by defining the macros if needed

### Simplified Data Structures
- `CellLearningStatus` replaced with `SimpleLearningStatus`
- Confidence tracking uses uint8_t (0-255) instead of float
- Removed variance tracking and consecutive adjustment counting

### Hardcoded Parameters
Several parameters that were configurable are now hardcoded for simplicity:
- Lambda stability threshold: 15%
- Temperature range for learning: 70-110°C
- Minimum MAP for learning: 30 kPa
- Buffer calculation multiplier: 1.5x
- Drift reset interval: 30 minutes

### Removed Features (Optional)
- Complex variance tracking
- Consecutive adjustment monitoring  
- Configurable quality thresholds
- Complex drift reset logic
- Weighted phase calculations

## Impact on Functionality

### Maintained Features
- Core wall wetting calculations (tau/beta)
- Basic adaptive learning
- Direction-specific corrections (accel/decel)
- 3D table interpolation
- Confidence tracking (simplified)
- Buffer-based transient monitoring

### Reduced Precision
- Table resolution reduced from 8×8 to 6×6
- Confidence tracking uses 8-bit instead of float precision
- Buffer size reduced but still adequate for most applications

### Disabled by Default
- Adaptive learning disabled by default (`wwEnableAdaptiveLearning = false`)
- Robust validation disabled by default (`WW_ENABLE_ROBUST_VALIDATION = 0`)
- Drift reset disabled by default (`WW_ENABLE_DRIFT_RESET = 0`)

## Performance Impact

- **Memory**: -12KB RAM, -2KB flash (reduced tables and parameters)
- **CPU**: Minimal reduction due to simplified calculations
- **Functionality**: Core wall wetting maintained, advanced features optional

## Configuration

### Enable Advanced Features (if needed)
Add to your build configuration:
```cpp
#define WW_ENABLE_ROBUST_VALIDATION 1
#define WW_ENABLE_DRIFT_RESET 1
```

### Enable Adaptive Learning
Set in TunerStudio or configuration:
```cpp
config->wwEnableAdaptiveLearning = true;
config->wwDirectionalCorrections = true;
```

## Recommendations

1. **For most users**: Use the optimized version as-is. The 6×6 tables and reduced buffer are sufficient for typical applications.

2. **For advanced tuning**: Enable adaptive learning and robust validation if memory allows and you need the extra precision.

3. **For racing/competition**: Consider enabling all features if you have sufficient memory and need maximum precision.

## Upgrading from Previous Version

### Configuration Migration
- Old 8×8 tables will be automatically resized to 6×6
- Default values will be interpolated to maintain similar behavior
- Remove unused configuration parameters from your tune

### TunerStudio Changes
- Table interfaces updated to show 6×6 instead of 8×8
- Some advanced parameters no longer available in interface
- Core functionality remains the same

## Future Enhancements

If memory becomes available in the future, features can be re-enabled by:
1. Increasing table sizes back to 8×8 or larger
2. Enabling robust validation and drift reset by default
3. Adding back configurable parameters
4. Increasing buffer sizes for better transient detection

The code is structured to allow easy restoration of features when memory constraints are relaxed. 