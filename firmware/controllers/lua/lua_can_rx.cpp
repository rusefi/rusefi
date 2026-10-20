/**
 * @file lua_can_rx.cpp
 * @brief Lua CAN receive support.
 *
 * Implements the CAN frame filtering and buffering that lets Lua scripts subscribe
 * to and read incoming CAN messages via the onCanRx hook.
 */

#include "pch.h"

#include "can_filter.h"

#if EFI_CAN_SUPPORT && EFI_LUA

#include "rusefi_lua.h"

extern "C" {
	#include "lapi.h"
	#include "ltable.h"
	#include "lgc.h"
}

// Stores information about one received CAN frame: which bus, plus the actual frame
struct CanFrameData {
	uint8_t BusIndex;
	int Callback;
	CANRxFrame Frame;
};

#ifndef LUA_canFrameCount
#if defined(STM32F7) || defined(STM32H7)
#define LUA_canFrameCount 256
#else
#define LUA_canFrameCount 32
#endif
#endif

static CanFrameData canFrames[LUA_canFrameCount];
// CAN frame buffers that are not in use
static chibios_rt::Mailbox<CanFrameData*, LUA_canFrameCount> freeBuffers;
// CAN frame buffers that are waiting to be processed by the lua thread
static chibios_rt::Mailbox<CanFrameData*, LUA_canFrameCount> filledBuffers;

static size_t dropRxCount = 0;

void processLuaCan(const size_t busIndex, const CANRxFrame& frame) {
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
		dropRxCount++;
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

// From lapi.c:762 lua_createtable, modified slightly
// Suppressing the routine GC check does not make this allocation-free:
// luaH_new/luaH_resize can still exhaust memory (and trigger emergency GC).
// Call only inside the protected CAN operation below.
static void lua_createtable_noGC(lua_State *L, int narray) {
	Table *t;
	lua_lock(L);
	t = luaH_new(L);
	sethvalue2s(L, L->top.p, t);
	api_incr_top(L);
	luaH_resize(L, t, narray, 0);

	// This line is commented out - no need to do a GC every time in
	// this hot path when we'll do it shortly and have plenty of memory available.
	// luaC_checkGC(L);

	lua_unlock(L);
}

static int handleCanFrameCallback(lua_State* ls) {
	// Borrow the mailbox buffer through luaProtectedCall's argument. Ownership
	// stays with doOneLuaCanRx so a Lua longjmp cannot skip returning the buffer.
	auto* data = static_cast<CanFrameData*>(lua_touserdata(ls, 1));
	if (data->Callback == NO_CALLBACK) {
		// No callback, use catch-all function
		lua_getglobal(ls, "onCanRx");
	} else {
		// Push the specified callback on to the stack
		lua_rawgeti(ls, LUA_REGISTRYINDEX, data->Callback);
	}

  auto frameCanId = CAN_ID(data->Frame);

	if (lua_isnil(ls, -1)) {
		// no rx function, ignore
		efiPrintf("LUA CAN rx missing function onCanRx ID=%ld", frameCanId);
		return 0;
	}

	auto dlc = data->Frame.DLC;

	// Push bus, ID and DLC
	lua_pushinteger(ls, HUMAN_OFFSET + data->BusIndex);
	lua_pushinteger(ls, frameCanId);
	lua_pushinteger(ls, dlc);

  if (engineConfiguration->luaCanRxWorkaround) {
    // todo: https://github.com/rusefi/rusefi/issues/6041
    if (lua_getglobal(ls, "global_can_data") != LUA_TTABLE) {
      criticalError("luaCanRxWorkaround without global_can_data");
    }
  } else {
  	// Build table for data, custom implementation without explicit GC but still garbage
	  lua_createtable_noGC(ls, dlc);
	}
	for (size_t i = 0; i < dlc; i++) {
		lua_pushinteger(ls, data->Frame.data8[i]);

		// index is i+1 because Lua "arrays" (tables) are 1-indexed
		lua_rawseti(ls, -2, i + 1);
	}

	// The outer lua_pcall covers lookup, table construction and script errors.
	// Protecting only this call would leave argument-allocation failures fatal.
	lua_call(ls, 4, 0);
	return 0;
}

static void handleCanFrame(LuaHandle& ls, CanFrameData* data) {
	// [tag:lua_protected_call] ScopePerf must survive the Lua error jump so
	// its destructor runs even when lookup or payload-table allocation fails.
	ScopePerf perf(PE::LuaOneCanRxCallback);
	// Failure abandons this callback and returns normally to doOneLuaCanRx.
	// Do not replay the frame: the script may already have performed side effects.
	luaProtectedCall(ls, handleCanFrameCallback, data, "CAN RX failed");
	lua_settop(ls, 0);
}

static bool doOneLuaCanRx(LuaHandle& ls) {
	ScopePerf perf(PE::LuaOneCanRxFunction);
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

	// Release on both callback success and caught Lua error. Losing a buffer
	// on each OOM would eventually exhaust this fixed pool and drop all CAN RX.
	msg = freeBuffers.post(data, TIME_IMMEDIATE);
	efiAssert(ObdCode::OBD_PCM_Processor_Fault, msg == MSG_OK, "lua can post to free buffer fail", false);

	// We processed a frame so we should check again
	return true;
}

int doLuaCanRx(LuaHandle& ls) {
	ScopePerf perf(PE::LuaAllCanRxFunction);
  int counter = 0;
	// While it processed a frame, continue checking
	while (doOneLuaCanRx(ls)) {
	  counter++;
	}
	return counter;
}

void initLuaCanRx() {
	// Push all CAN frames in to the free buffer
	for (size_t i = 0; i < LUA_canFrameCount; i++) {
		freeBuffers.post(&canFrames[i], TIME_INFINITE);
	}
}

size_t getLuaCanRxDropped() {
	return dropRxCount;
}

#endif // EFI_CAN_SUPPORT && EFI_LUA
