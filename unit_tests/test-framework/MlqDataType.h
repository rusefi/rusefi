/*
 * MlqDataType.h
 */

#pragma once

#include <cstdint> // For int8_t

enum class MlqDataType : int8_t {
    MLQ_DATA_S8 = 0,
    MLQ_DATA_U8 = 1,
    MLQ_DATA_S16 = 2,
    MLQ_DATA_U16 = 3,
    MLQ_DATA_S32 = 4,
    MLQ_DATA_U32 = 5,
    MLQ_DATA_BIT = 6,
    MLQ_DATA_FLOAT = 7,
    // Add any other types if they exist in your Java MlqDataType enum
};

// Helper function to get record size based on type
inline int getRecordSize(MlqDataType type) {
    switch (type) {
        case MlqDataType::MLQ_DATA_S8:
        case MlqDataType::MLQ_DATA_U8:
        case MlqDataType::MLQ_DATA_BIT:
            return 1;
        case MlqDataType::MLQ_DATA_S16:
        case MlqDataType::MLQ_DATA_U16:
            return 2;
        case MlqDataType::MLQ_DATA_S32:
        case MlqDataType::MLQ_DATA_U32:
        case MlqDataType::MLQ_DATA_FLOAT:
            return 4;
        default:
            return 0; // Or throw an error for unknown types
    }
}

inline MlqDataType findByOrdinal(int8_t ordinal) {
    switch (ordinal) {
        case 0: return MlqDataType::MLQ_DATA_S8;
        case 1: return MlqDataType::MLQ_DATA_U8;
        case 2: return MlqDataType::MLQ_DATA_S16;
        case 3: return MlqDataType::MLQ_DATA_U16;
        case 4: return MlqDataType::MLQ_DATA_S32;
        case 5: return MlqDataType::MLQ_DATA_U32;
        case 6: return MlqDataType::MLQ_DATA_BIT;
        case 7: return MlqDataType::MLQ_DATA_FLOAT;
        default:
            throw std::runtime_error("Unknown MlqDataType ordinal: " + std::to_string(ordinal));
    }
}
