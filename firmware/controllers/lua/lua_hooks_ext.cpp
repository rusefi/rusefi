#include "pch.h"

#include "rusefi_lua.h"
#include "lua_hooks.h"

#if EFI_ENGINE_CONTROL && EFI_LUA && (defined(STM32F7) || EFI_UNIT_TEST)
#include "closed_loop_fuel.h"

static InjectorDeadtimeAutotune* injectorTuningControls(lua_State* l, bool releasing) {
	// A lightuserdata pointer binds the controls to this VM; true means the VM
	// failed and cannot rearm. luaProtectedCall() owns the error transition.
	lua_getfield(l, LUA_REGISTRYINDEX, "rusefi.injectorTuning");
	const bool failed = lua_isboolean(l, -1) && lua_toboolean(l, -1);
	lua_pop(l, 1);
	if (failed && !releasing) {
		return nullptr;
	}
	auto* controls = &engine->module<InjectorDeadtimeAutotune>().unmock();
	if (!failed) {
		lua_pushlightuserdata(l, controls);
		lua_setfield(l, LUA_REGISTRYINDEX, "rusefi.injectorTuning");
	}
	return controls;
}

static const char* injectionModeName(injection_mode_e mode) {
	switch (mode) {
	case IM_SEQUENTIAL: return "sequential";
	case IM_BATCH: return "batch";
	case IM_SIMULTANEOUS: return "simultaneous";
	case IM_SINGLE_POINT: return "single-point";
	default: return "other";
	}
}

static int luaGetFuelTrim(lua_State* l) {
	const auto humanBank = luaL_checkinteger(l, 1);
	if (humanBank < 1 || humanBank > FT_BANK_COUNT) {
		return luaL_error(l, "fuel trim bank must be 1..%d", FT_BANK_COUNT);
	}
	const size_t bank = humanBank - 1;
	float correction;
	stft_state_e correctionState, learningState;
	int cell;
	bool saturated, enabled;
	{
		chibios_rt::CriticalSectionLocker lock;
		const auto& stft = engine->module<ShortTermFuelTrim>();
		correction = engine->engineState.stftCorrection[bank];
		correctionState = stft->stftCorrectionState;
		learningState = stft->stftLearningState[bank];
		cell = stft->stftCorrectionBinIdx;
		saturated = stft->isSaturated(bank);
		enabled = engine->rpmCalculator.isRunning()
			&& correctionState == stftEnabled && learningState == stftEnabled
			&& Sensor::get(bank == 0 ? SensorType::Lambda1 : SensorType::Lambda2).Valid
			&& std::isfinite(correction) && correction > 0;
	}
	// Allocate Lua objects outside the critical section.
	lua_createtable(l, 0, 6);
	lua_pushnumber(l, correction); lua_setfield(l, -2, "correction");
	lua_pushinteger(l, correctionState); lua_setfield(l, -2, "correctionState");
	lua_pushinteger(l, learningState); lua_setfield(l, -2, "learningState");
	lua_pushinteger(l, cell + 1); lua_setfield(l, -2, "cell");
	lua_pushboolean(l, saturated); lua_setfield(l, -2, "saturated");
	lua_pushboolean(l, enabled); lua_setfield(l, -2, "enabled");
	return 1;
}
#endif

void configureRusefiLuaHooksExt(lua_State* lState) {
#if EFI_ENGINE_CONTROL && EFI_LUA && (defined(STM32F7) || EFI_UNIT_TEST)
	// Preallocate the registry entry so even an out-of-memory error can revoke
	// controls without allocating a new key/table slot in luaProtectedCall().
	lua_pushboolean(lState, false);
	lua_setfield(lState, LUA_REGISTRYINDEX, "rusefi.injectorTuning");
	lua_register(lState, "setInjectionModeOverride", [](lua_State* l) {
		int mode = -1;
		if (!lua_isnil(l, 1)) {
			const char* name = luaL_checkstring(l, 1);
			if (strcmp(name, "sequential") == 0) {
				mode = IM_SEQUENTIAL;
			} else if (strcmp(name, "batch") == 0) {
				mode = IM_BATCH;
			} else {
				return luaL_error(l, "mode must be sequential, batch or nil");
			}
		}
		auto* controls = injectorTuningControls(l, mode == -1);
		lua_pushboolean(l, controls && controls->setInjectionMode(mode));
		return 1;
	});
	lua_register(lState, "getInjectionMode", [](lua_State* l) {
		injection_mode_e mode;
		bool transitioning;
		{
			chibios_rt::CriticalSectionLocker lock;
			mode = getCurrentInjectionMode();
			transitioning = engine->module<InjectorDeadtimeAutotune>()->isTransitioning();
		}
		lua_pushstring(l, injectionModeName(mode));
		lua_pushboolean(l, transitioning);
		return 2;
	});
	lua_register(lState, "setInjectorDeadtimeAdd", [](lua_State* l) {
		float milliseconds = luaL_checknumber(l, 1);
		if (!std::isfinite(milliseconds) || std::abs(milliseconds) > InjectorDeadtimeAutotune::MaxDeadtimeAddMs) {
			return luaL_error(l, "deadtime addition must be finite and within -2..2 ms");
		}
		auto* controls = injectorTuningControls(l, milliseconds == 0);
		lua_pushboolean(l, controls && controls->setDeadtimeAdd(milliseconds));
		return 1;
	});
	lua_register(lState, "getFuelTrim", luaGetFuelTrim);
#if EFI_LTFT_CONTROL
	lua_register(lState, "setLtftLearningDisabled", [](lua_State* l) {
		luaL_checktype(l, 1, LUA_TBOOLEAN);
		const bool disabled = lua_toboolean(l, 1);
		auto* controls = injectorTuningControls(l, !disabled);
		lua_pushboolean(l, controls && controls->setLtftLearningDisabled(disabled));
		return 1;
	});
#endif
#else
	(void)lState;
#endif
}
