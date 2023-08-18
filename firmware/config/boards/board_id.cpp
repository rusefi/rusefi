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
#if HW_HELLEN
	return (board_id_t)engine->engineState.hellenBoardId;
#elif STATIC_BOARD_ID
	return (board_id_t)STATIC_BOARD_ID;
#else
	return 0;
#endif
}
