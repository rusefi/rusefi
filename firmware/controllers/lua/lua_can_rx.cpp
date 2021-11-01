#include "pch.h"

#if EFI_CAN_SUPPORT

#include "rusefi_lua.h"
#include "can.h"

static constexpr size_t maxFilterCount = 16;

size_t filterCount = 0;
int32_t luaCanRxIds[maxFilterCount] = {0};

static bool shouldRxCanFrame(const CANRxFrame& frame) {
	for (size_t i = 0; i < filterCount; i++) {
		if (CAN_EID(frame) == luaCanRxIds[i]) {
			return true;
		}
	}

	return false;
}

constexpr size_t canFrameCount = 32;
static CANRxFrame canFrames[canFrameCount];
// CAN frame buffers that are not in use
chibios_rt::Mailbox<CANRxFrame*, canFrameCount> freeBuffers;
// CAN frame buffers that are waiting to be processed by the lua thread
chibios_rt::Mailbox<CANRxFrame*, canFrameCount> filledBuffers;

void processLuaCan(const CANRxFrame& frame) {
	// Filter the frame if we aren't listening for it
	if (!shouldRxCanFrame(frame)) {
		return;
	}

	CANRxFrame* frameBuffer;
	msg_t msg;

	{
		// Acquire a buffer under lock
		chibios_rt::CriticalSectionLocker csl;
		msg = freeBuffers.fetchI(&frameBuffer);
	}

	if (msg != MSG_OK) {
		// all buffers are already in use, this frame will be dropped!
		// TODO: warn the user
		return;
	}

	// Copy the frame in to the buffer
	*frameBuffer = frame;

	{
		// Push the frame in to the queue under lock
		chibios_rt::CriticalSectionLocker csl;
		filledBuffers.postI(frameBuffer);
	}
}

static void handleCanFrame(LuaHandle& ls, CANRxFrame* frame) {
	lua_getglobal(ls, "onCanRx");
	if (lua_isnil(ls, -1)) {
		// no rx function, ignore
		efiPrintf("LUA CAN rx missing function onCanRx");
		lua_settop(ls, 0);
		return;
	}

	auto dlc = frame->DLC;

	// Push bus, ID and DLC
	lua_pushinteger(ls, 1);	// TODO: support multiple busses!
	lua_pushinteger(ls, CAN_EID(*frame));
	lua_pushinteger(ls, dlc);

	// Build table for data
	lua_newtable(ls);
	for (size_t i = 0; i < dlc; i++) {
		lua_pushinteger(ls, frame->data8[i]);

		// index is i+1 because Lua "arrays" (tables) are 1-indexed
		lua_rawseti(ls, -2, i + 1);
	}

	// Perform the actual function call
	int status = lua_pcall(ls, 4, 0, 0);

	if (0 != status) {
		// error calling CAN rx hook function
		auto errMsg = lua_tostring(ls, -1);
		efiPrintf("LUA CAN RX error %s", errMsg);
		lua_pop(ls, 1);
	}

	lua_settop(ls, 0);
}

bool doOneLuaCanRx(LuaHandle& ls) {
	CANRxFrame* frame;

	msg_t msg = filledBuffers.fetch(&frame, TIME_IMMEDIATE);

	if (msg == MSG_TIMEOUT) {
		// No new CAN messages rx'd, nothing more to do.
		return false;
	}

	if (msg != MSG_OK) {
		// Message was otherwise not OK
		// TODO: what do here?
		return false;
	}

	// We've accepted the frame, process it in Lua.
	handleCanFrame(ls, frame);

	// We're done, return this frame to the free list
	msg = freeBuffers.post(frame, TIME_IMMEDIATE);
	efiAssertVoid(OBD_PCM_Processor_Fault, msg == MSG_OK, "lua can post to free buffer fail");

	// We processed a frame so we should check again
	return true;
}

void doLuaCanRx(LuaHandle& ls) {
	// While it processed a frame, continue checking
	while (doOneLuaCanRx(ls)) ;
}

void initLuaCanRx() {
	// Push all CAN frames in to the free buffer
	for (size_t i = 0; i < canFrameCount; i++) {
		freeBuffers.post(&canFrames[i], TIME_INFINITE);
	}
}

void resetLuaCanRx() {
	// Clear all lua filters - reloading the script will reinit them
	memset(luaCanRxIds, 0, sizeof(luaCanRxIds));
	filterCount = 0;
}

void addLuaCanRxFilter(int32_t eid) {
	if (filterCount >= maxFilterCount) {
		firmwareError(OBD_PCM_Processor_Fault, "Too many Lua CAN RX filters");
	}

	efiPrintf("Added Lua CAN RX filter: %d", eid);

	luaCanRxIds[filterCount] = eid;
	filterCount++;
}

#endif // EFI_CAN_SUPPORT
