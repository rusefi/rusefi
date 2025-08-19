// not Generated yet
#pragma once
#include <cstdint>
namespace rusefi::generated::enums
{
template<typename T>
constexpr char const* Enum2String(T const e);

template<typename T>
T String2Enum(char const* const s);

/* This enum is used to select your desired Engine Load calculation algorithm */
enum engine_load_mode_e: uint8_t {
    /* Speed Density algorithm - Engine Load is a function of MAP, VE and target AFR
     * http://articles.sae.org/8539/ */
    LM_SPEED_DENSITY = 0,
    /* MAF with a known kg/hour function */
    LM_REAL_MAF = 1,
    LM_ALPHA_N = 2,
    LM_LUA = 3,
    /* This mode is for unit testing only, so that tests don't have to rely on a particular real airmass mode */
    LM_MOCK = 100,
    engine_load_mode_e_Unsupported_Value
};

template<> constexpr char const* Enum2String<engine_load_mode_e>(engine_load_mode_e const e) {
    switch(e) {
        case engine_load_mode_e::LM_SPEED_DENSITY: return "LM_SPEED_DENSITY";
        case engine_load_mode_e::LM_REAL_MAF: return "LM_REAL_MAF";
        case engine_load_mode_e::LM_ALPHA_N: return "LM_ALPHA_N";
        case engine_load_mode_e::LM_LUA: return "LM_LUA";
        case engine_load_mode_e::LM_MOCK: return "LM_MOCK";
        default: return "engine_load_mode_e_Unsupported_Value";
    }
};

template<> engine_load_mode_e String2Enum<engine_load_mode_e>(char const* const s){
    if (strncmp(Enum2String<engine_load_mode_e>(engine_load_mode_e::LM_SPEED_DENSITY), s, 16+1) == 0) return engine_load_mode_e::LM_SPEED_DENSITY;
    if (strncmp(Enum2String<engine_load_mode_e>(engine_load_mode_e::LM_REAL_MAF), s, 11+1) == 0) return engine_load_mode_e::LM_REAL_MAF;
    if (strncmp(Enum2String<engine_load_mode_e>(engine_load_mode_e::LM_ALPHA_N), s, 10+1) == 0) return engine_load_mode_e::LM_ALPHA_N;
    if (strncmp(Enum2String<engine_load_mode_e>(engine_load_mode_e::LM_LUA), s, 6+1) == 0) return engine_load_mode_e::LM_LUA;
    if (strncmp(Enum2String<engine_load_mode_e>(engine_load_mode_e::LM_MOCK), s, 7+1) == 0) return engine_load_mode_e::LM_MOCK;
    return engine_load_mode_e::engine_load_mode_e_Unsupported_Value;
};


}
