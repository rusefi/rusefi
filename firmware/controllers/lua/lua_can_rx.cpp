#include "pch.h"

#include "can_filter.h"


#if EFI_CAN_SUPPORT

#include "rusefi_lua.h"

extern "C" {
	#include "lapi.h"
	#include "ltable.h"
	#include "lgc.h"
}

// Stores information about one received CAN frame: which bus, plus the actual frame
struct CanFrameData {
	CanBusIndex BusIndex;
	int Callback;
	CANRxFrame Frame;
};

constexpr size_t canFrameCount = 32;
static CanFrameData canFrames[canFrameCount];
// CAN frame buffers that are not in use
chibios_rt::Mailbox<CanFrameData*, canFrameCount> freeBuffers;
// CAN frame buffers that are waiting to be processed by the lua thread
chibios_rt::Mailbox<CanFrameData*, canFrameCount> filledBuffers;

void processLuaCan(CanBusIndex busIndex, const CANRxFrame& frame) {
	auto filter = getFilterForId(busIndex, CAN_ID(frame));

	// Filter the frame if we aren't listening for it
	if (!filter) {
		return;
	}

	CanFrameData* frameBuffer;
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
	frameBuffer->BusIndex = busIndex;
	frameBuffer->Frame = frame;
	frameBuffer->Callback = filter->Callback;

	{
		// Push the frame in to the queue under lock
		chibios_rt::CriticalSectionLocker csl;
		filledBuffers.postI(frameBuffer);
	}
}

// From lapi.c:756, modified slightly
static void lua_createtable_noGC(lua_State *L, int narray) {
	Table *t;
	lua_lock(L);
	t = luaH_new(L);
	sethvalue2s(L, L->top, t);
	api_incr_top(L);
	luaH_resize(L, t, narray, 0);

	// This line is commented out - no need to do a GC every time in
	// this hot path when we'll do it shortly and have plenty of memory available.
	// luaC_checkGC(L);

	lua_unlock(L);
}

static void handleCanFrame(LuaHandle& ls, CanFrameData* data) {
	if (data->Callback == NO_CALLBACK) {
		// No callback, use catch-all function
		lua_getglobal(ls, "onCanRx");
	} else {
		// Push the specified callback on to the stack
		lua_rawgeti(ls, LUA_REGISTRYINDEX, data->Callback);
	}

	if (lua_isnil(ls, -1)) {
		// no rx function, ignore
		efiPrintf("LUA CAN rx missing function onCanRx");
		lua_settop(ls, 0);
		return;
	}

	auto dlc = data->Frame.DLC;

	// Push bus, ID and DLC
	lua_pushinteger(ls, static_cast<int>(data->BusIndex));
	lua_pushinteger(ls, CAN_ID(data->Frame));
	lua_pushinteger(ls, dlc);

	// Build table for data
	lua_createtable_noGC(ls, dlc);
	for (size_t i = 0; i < dlc; i++) {
		lua_pushinteger(ls, data->Frame.data8[i]);

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
	CanFrameData* data;

	msg_t msg = filledBuffers.fetch(&data, TIME_IMMEDIATE);

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
	handleCanFrame(ls, data);

	// We're done, return this frame to the free list
	msg = freeBuffers.post(data, TIME_IMMEDIATE);
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, msg == MSG_OK, "lua can post to free buffer fail", false);

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

#endif // EFI_CAN_SUPPORT
