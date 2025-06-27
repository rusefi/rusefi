/*
 * MlgDataType.h
 */

#pragma once

#include <cstdint> // For int8_t

enum class MlgDataType : int8_t {
    MLG_DATA_S8 = 0,
    MLG_DATA_U8 = 1,
    MLG_DATA_S16 = 2,
    MLG_DATA_U16 = 3,
    MLG_DATA_S32 = 4,
    MLG_DATA_U32 = 5,
    MLG_DATA_BIT = 6,
    MLG_DATA_FLOAT = 7,
    // Add any other types if they exist in your Java MlgDataType enum
};

// Helper function to get record size based on type
inline int getRecordSize(MlgDataType type) {
    switch (type) {
        case MlgDataType::MLG_DATA_S8:
        case MlgDataType::MLG_DATA_U8:
        case MlgDataType::MLG_DATA_BIT:
            return 1;
        case MlgDataType::MLG_DATA_S16:
        case MlgDataType::MLG_DATA_U16:
            return 2;
        case MlgDataType::MLG_DATA_S32:
        case MlgDataType::MLG_DATA_U32:
        case MlgDataType::MLG_DATA_FLOAT:
            return 4;
        default:
            return 0; // Or throw an error for unknown types
    }
}

inline MlgDataType findByOrdinal(int8_t ordinal) {
    switch (ordinal) {
        case 0: return MlgDataType::MLG_DATA_S8;
        case 1: return MlgDataType::MLG_DATA_U8;
        case 2: return MlgDataType::MLG_DATA_S16;
        case 3: return MlgDataType::MLG_DATA_U16;
        case 4: return MlgDataType::MLG_DATA_S32;
        case 5: return MlgDataType::MLG_DATA_U32;
        case 6: return MlgDataType::MLG_DATA_BIT;
        case 7: return MlgDataType::MLG_DATA_FLOAT;
        default:
            throw std::runtime_error("Unknown MlgDataType ordinal: " + std::to_string(ordinal));
    }
}
