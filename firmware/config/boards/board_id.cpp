/*
 * board_id.cpp
 *
 * @date Aug 18, 2023
 * @author Andrey Belomutskiy, (c) 2012-2021
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"
#include "board_id.h"
#include "boards_id.h"
#include "boards_dictionary.h"


board_id_t getBoardId() {
#if HW_PROTEUS
    if (engineConfiguration->engineType == engine_type_e::MAVERICK_X3) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_CANAM;
    }
    if (engineConfiguration->engineType == engine_type_e::ME17_9_MISC) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_SLINGSHOT;
    }
    if (engineConfiguration->engineType == engine_type_e::PROTEUS_BMW_M73) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_M73;
    }
    if (engineConfiguration->engineType == engine_type_e::HARLEY) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_HARLEY;
    }
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_SBC;
    }
#endif

#if HW_HELLEN_8CHAN
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PLATINUM_SBC;
    }
#endif

#if HW_HELLEN_UAEFI121
    if (engineConfiguration->engineType == engine_type_e::GM_SBC) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_UAEFU121_SBC;
    }
#endif

#if HW_MICRO_RUSEFI
    if (engineConfiguration->engineType == engine_type_e::MERCEDES_M111) {
        return STATIC_BOARD_ID_MRE_M111;
    }
#endif

#if defined(HW_HELLEN_SKIP_BOARD_TYPE)
	return (board_id_t)STATIC_BOARD_ID;
#elif HW_HELLEN
  int16_t hellenBoardId = engine->engineState.hellenBoardId;
  if (hellenBoardId != -1)
	  return (board_id_t)hellenBoardId;
	#if STATIC_BOARD_ID
	  return (board_id_t)STATIC_BOARD_ID;
	#else
	  return -2;
	#endif
#elif STATIC_BOARD_ID
// should STATIC_BOARD_ID simply have priority over hellen board id? what's the use-case of HW_HELLEN with STATIC_BOARD_ID?
	return (board_id_t)STATIC_BOARD_ID;
#else
	return 0;
#endif
}
