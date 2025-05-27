/*
 * @file engine_cylinder.hpp
 *
 * @date: may 25, 2025
 * @author Matthew Kennedy, FDSoftware
 */

#ifndef ENGINE_CILINDER_HPP
#define ENGINE_CILINDER_HPP
#include "rusefi_types.h"

class OneCylinder final {
public:
    void updateCylinderNumber(uint8_t index, uint8_t cylinderNumber);
    void invalidCylinder();

    // Get this cylinder's offset, in positive degrees, from cylinder 1
    angle_t getAngleOffset() const;

#if EFI_UNIT_TEST
    bool getIsValid() const {
      return m_valid;
    }
#endif

private:
    int m_index = 0;
    bool m_valid = false;

    // This cylinder's position in the firing order (0-based)
    uint8_t m_cylinderIndex = 0;
    // This cylinder's physical cylinder number (0-based)
    uint8_t m_cylinderNumber = 0;

    // This cylinder's mechanical TDC offset in degrees after #1
    angle_t m_baseAngleOffset;
};

namespace EngineCylinders {
    void updateCylinders(void);
};

#endif //ENGINE_CILINDER_HPP
