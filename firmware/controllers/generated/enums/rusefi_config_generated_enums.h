// Not yet generated

#pragma once

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace rusefi::generated::enums
{
    template<typename T>
    constexpr auto Enum2Underlying(T const e) requires std::is_enum_v<T> {
        return static_cast<std::underlying_type_t<T>>(e);
    }

    template <typename T>
    struct EnumStringPair {
        T const enumValue;
        char const* const name;
    };

    template <typename T>
    constexpr EnumStringPair<T> EnumLookUp[]{};

    template <typename T>
    constexpr char const* Enum2String(T const e) {
        for (auto& entry : EnumLookUp<T>) {
            if (entry.enumValue == e) {
                return entry.name;
            }
        }
        return "UNSUPPORTED_ENUM_VALUE";
    }

    template <typename T>
    constexpr T String2Enum(char const* const s) {
        for (auto& entry : EnumLookUp<T>) {
            if (std::strncmp(entry.name, s, std::strlen(entry.name) + 1) == 0) {
                return entry.enumValue;
            }
        }
        return T::UNSUPPORTED_ENUM_VALUE; // Invalid value (may need adjustment)
    }

    /* This enum is used to select your desired Engine Load calculation algorithm */
    enum class engine_load_mode_e: uint8_t {
        /* Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR
         * http://articles.sae.org/8539/ */
        LM_SPEED_DENSITY = 0,
        /* MAF with a known kg/hour function */
        LM_REAL_MAF = 1,
        LM_ALPHA_N = 2,
        LM_LUA = 3,
        UNSUPPORTED_ENUM_VALUE
    };

    template<>
    inline constexpr EnumStringPair<engine_load_mode_e> EnumLookUp<engine_load_mode_e>[] = {
        { engine_load_mode_e::LM_SPEED_DENSITY, "LM_SPEED_DENSITY" },
        { engine_load_mode_e::LM_REAL_MAF, "LM_REAL_MAF" },
        { engine_load_mode_e::LM_ALPHA_N, "LM_ALPHA_N" },
        { engine_load_mode_e::LM_LUA, "LM_LUA" }
    };
}
