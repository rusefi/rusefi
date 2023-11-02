/*
 * board_id.cpp
 *
 * @date Aug 18, 2023
 * @author Andrey Belomutskiy, (c) 2012-2021
 * @author andreika <prometheus.pcb@gmail.com>
 */

#include "pch.h"
#include "board_id.h"
#include "boards_dictionary.h"


board_id_t getBoardId() {
#if HW_PROTEUS
    if (engineConfiguration->engineType == engine_type_e::MAVERICK_X3) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_CANAM;
    }
    if (engineConfiguration->engineType == engine_type_e::PROTEUS_HARLEY) {
        // a way to test harness patch cord
        return STATIC_BOARD_ID_PROTEUS_HARLEY;
    }
#endif

#if HW_HELLEN
	return (board_id_t)engine->engineState.hellenBoardId;
#elif STATIC_BOARD_ID
	return (board_id_t)STATIC_BOARD_ID;
#else
	return 0;
#endif
}
