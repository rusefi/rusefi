#pragma once

#include <cstdint>
#include <concepts>
#include <type_traits>

// NOTE THAT Ordinals are part of logging serialization as described in
// https://www.efianalytics.com/TunerStudio/docs/MLG_Binary_LogFormat_2.0.pdf
namespace MLG::Types{

/*
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Field Name        | Offset | Length | Required | Value Description                                                        |
    |                   | (bytes)| (bytes)|          |                                                                          |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | File Format       | 0      | 6      | Yes      | "MLVLG" padded by 0; Hex: x4D x4C x56 x4C x47 x00                        |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Format Version    | 6      | 2      | Yes      | Currently x00 x02 (future versions may increase)                         |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Time Stamp        | 8      | 4      | No       | Unix 32-bit timestamp (seconds since epoch). If unavailable: x00 x00 x00|
    |                   |        |        |          | x00                                                                      |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Info Data Start   | 12     | 4      | No       | Start offset after LoggerField[] and before Data Begin Index; set to 0   |
    |                   |        |        |          | if no Log Info Data                                                      |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Data Begin Index  | 16     | 4      | Yes      | Address of 1st byte containing Type-Data pairs                           |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Record Length     | 20     | 2      | Yes      | Length of a single data record (excluding Type-Pair overhead)            |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Num Logger Fields | 22     | 2      | Yes      | Number of expected Logger Fields                                         |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Logger Field []   | 24     | 89*n   | Yes      | Array of (Num Logger Fields * 89) in length                              |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Bit Field Names   |24+n*89 | varies | No       | Available if Logger Fields contain Bit Info                              |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Info Data         | varies | varies | No       | Optional null-terminated string with informational data (firmware, date) |
    +-------------------+--------+--------+----------+--------------------------------------------------------------------------+
*/
namespace Header {
    constexpr size_t Size = 24;
}

/*
    Only Scalar is currently supported and BitField is emulated via 1-byte Scalar:

    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Field Name    | Offset | Length | Required | Value Description                                                        |
    |               | (bytes)| (bytes)|          |                                                                          |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Type          | 0      | 1      | Yes      | Value type:                                                              |
    |               |        |        |          | 0=U08, 1=S08, 2=U16, 3=S16, 4=U32, 5=S32, 6=S64, 7=F32                   |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Name          | 1      | 34     | Yes      | ASCII string, null terminated                                            |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Units         | 35     | 10     | Yes      | ASCII string, null terminated                                            |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Display Style | 45     | 1      | Yes      | Display format:                                                          |
    |               |        |        |          | 0=Float, 1=Hex, 2=Bits, 3=Date, 4=On/Off, 5=Yes/No, 6=High/Low,          |
    |               |        |        |          | 7=Active/Inactive, 8=True/False                                          |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Scale         | 46     | 4      | Yes      | IEEE 754 float representing the scale applied to (raw + transform)       |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Transform     | 50     | 4      | Yes      | IEEE 754 float representing any shift of raw value before scaling        |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Digits        | 54     | 1      | Yes      | S08 representing the number of decimal places to display to the right    |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
    | Category      | 55     | 34     | No       | Optional category for logical grouping in MLV                            |
    +---------------+--------+--------+----------+--------------------------------------------------------------------------+
*/
namespace Field {
    constexpr size_t DescriptorSize = 89;

    enum class Scalar : uint8_t {
        U08 = 0,
        S08 = 1,
        U16 = 2,
        S16 = 3,
        U32 = 4,
        S32 = 5,
        S64 = 6,
        F32 = 7,
        unsupported = static_cast<uint8_t>(-1)
    };

    template<typename T>
    consteval Scalar resolveBuiltInNumberType() {
        using enum Scalar;
        using CleanType = std::remove_const_t<T>;
        constexpr auto resolvedType{[](){
            if constexpr (std::is_integral_v<CleanType>) {
                if constexpr (std::is_signed_v<CleanType>) {
                    if      constexpr (sizeof(CleanType) == 1) { return S08; }
                    else if constexpr (sizeof(CleanType) == 2) { return S16; }
                    else if constexpr (sizeof(CleanType) == 4) { return S32; }
                    else if constexpr (sizeof(CleanType) == 8) { return S64; }
                } else {
                    if      constexpr (sizeof(CleanType) == 1) { return U08; }
                    else if constexpr (sizeof(CleanType) == 2) { return U16; }
                    else if constexpr (sizeof(CleanType) == 4) { return U32; }
                }
            }
            else if constexpr (std::same_as<CleanType, float>) { return F32; }

            return unsupported;
        }()};
        static_assert(resolvedType != unsupported, "Scalar type was not recognized as supported built in numeric type");
        return resolvedType;
    }

    template<Scalar t>
    consteval size_t sizeForType() {
        constexpr auto s{[]{
            switch (t) {
                using enum Scalar;
                case U08: case S08: return 1;
                case U16: case S16: return 2;
                case U32: case S32: case F32: return 4;
                case S64: return 8;
                default: return 0;
            }
        }()};
        static_assert(s != 0, "Can not resolve Scalar type, check enum for new values that were left unhandled");
        return s;
    }
}
}
