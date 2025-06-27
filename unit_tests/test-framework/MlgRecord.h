#ifndef RECORD_H
#define RECORD_H

#include <string>
#include <fstream> // For std::ifstream
#include <stdexcept> // For std::runtime_error
#include <iostream>  // For std::cerr
#include "MlgDataType.h" // Include the MlgDataType enum

extern int32_t readSwappedInt(std::ifstream* ifs);
extern float readSwappedFloat(std::ifstream* ifs);
extern int16_t readSwappedShort(std::ifstream* ifs);
extern int8_t readByte(std::ifstream* ifs);

class Record {
private:
    std::string fieldName;
    MlgDataType type;
    float scale;

public:
    Record(const std::string& fieldName, MlgDataType type, float scale)
        : fieldName(fieldName), type(type), scale(scale) {}

    const std::string& getFieldName() const { return fieldName; }

    float read(std::ifstream& ifs) {
        float value = 0.0f;
        int32_t temp_s32;
        uint32_t temp_u32;
        int16_t temp_s16;
        uint16_t temp_u16;
        int8_t temp_s8;
        uint8_t temp_u8;
        float temp_float;

        switch (type) {
            case MlgDataType::MLG_DATA_S8:
                temp_s8 = readByte(&ifs);
                value = static_cast<float>(temp_s8) * scale;
                break;
            case MlgDataType::MLG_DATA_U8:
                temp_u8 = static_cast<uint8_t>(readByte(&ifs)); // Read as signed, cast to unsigned
                value = static_cast<float>(temp_u8) * scale;
                break;
            case MlgDataType::MLG_DATA_S16:
                temp_s16 = readSwappedShort(&ifs);
                value = static_cast<float>(temp_s16) * scale;
                break;
            case MlgDataType::MLG_DATA_U16:
                temp_u16 = static_cast<uint16_t>(readSwappedShort(&ifs)); // Read as signed, cast to unsigned
                value = static_cast<float>(temp_u16) * scale;
                break;
            case MlgDataType::MLG_DATA_S32:
                temp_s32 = readSwappedInt(&ifs);
                value = static_cast<float>(temp_s32) * scale;
                break;
            case MlgDataType::MLG_DATA_U32:
                temp_u32 = static_cast<uint32_t>(readSwappedInt(&ifs)); // Read as signed, cast to unsigned
                value = static_cast<float>(temp_u32) * scale;
                break;
            case MlgDataType::MLG_DATA_BIT:
                temp_u8 = static_cast<uint8_t>(readByte(&ifs)); // Assuming bit is stored as a byte (0 or 1)
                value = static_cast<float>(temp_u8);
                break;
            case MlgDataType::MLG_DATA_FLOAT:
                temp_float = readSwappedFloat(&ifs);
                value = temp_float * scale;
                break;
            default:
                std::cerr << "Unsupported MLG data type: " << static_cast<int>(type) << std::endl;
                throw std::runtime_error("Unsupported MLG data type");
        }
        return value;
    }
};

#endif // RECORD_H
