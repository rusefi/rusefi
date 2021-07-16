#include "lua.hpp"
#include "lua_hooks.h"

#include "engine.h"
#include "loggingcentral.h"
#include "sensor.h"
#include "adc_inputs.h"
#include "efilib.h"
#include "tunerstudio_outputs.h"
#include "pwm_generator_logic.h"
#include "can_msg_tx.h"

// Some functions lean on existing FSIO implementation
#include "fsio_impl.h"

EXTERN_ENGINE;

static int lua_efi_print(lua_State* l) {
	auto msg = luaL_checkstring(l, 1);

	efiPrintf("LUA: %s", msg);

	return 0;
}

static int lua_getSensor(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	auto result = Sensor::get(static_cast<SensorType>(sensorIndex));

	if (result) {
		// return value if valid
		lua_pushnumber(l, result.Value);
	} else {
		// return nil if invalid
		lua_pushnil(l);
	}

	return 1;
}

static int lua_getSensorRaw(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	lua_pushnumber(l, Sensor::getRaw(static_cast<SensorType>(sensorIndex)));
	return 1;
}

static int lua_hasSensor(lua_State* l) {
	auto sensorIndex = luaL_checkinteger(l, 1);

	lua_pushboolean(l, Sensor::hasSensor(static_cast<SensorType>(sensorIndex)));
	return 1;
}

static int lua_table3d(lua_State* l) {
	auto tableIdx = luaL_checkinteger(l, 1);
	auto x = luaL_checknumber(l, 2);
	auto y = luaL_checknumber(l, 3);

	// index table, compute table lookup
	auto result = getFSIOTable(tableIdx)->getValue(x, y);

	lua_pushnumber(l, result);
	return 1;
}

static int lua_txCan(lua_State* l) {
	auto channel = luaL_checkinteger(l, 1);
	// TODO: support multiple channels
	luaL_argcheck(l, channel == 1, 1, "only channel 1 currently supported");

	auto id = luaL_checkinteger(l, 2);
	auto ext = luaL_checkinteger(l, 3);

	// Check that ID is valid based on std vs. ext
	if (ext == 0) {
		luaL_argcheck(l, id <= 0x7FF, 2, "ID specified is greater than max std ID");
	} else {
		luaL_argcheck(l, id <= 0x1FFF'FFFF, 2, "ID specified is greater than max ext ID");
	}

	luaL_checktype(l, 4, LUA_TTABLE);

	// conform ext parameter to true/false
	CanTxMessage msg(id, 8, ext == 0 ? false : true);

	// Unfortunately there is no way to inspect the length of a table,
	// so we have to just iterate until we run out of numbers
	uint8_t dlc = 0;

	while (true) {
		lua_pushnumber(l, dlc + 1);
		auto elementType = lua_gettable(l, 4);
		auto val = lua_tointeger(l, -1);
		lua_pop(l, 1);

		if (elementType == LUA_TNIL) {
			// we're done, this is the end of the array.
			break;
		}

		if (elementType != LUA_TNUMBER) {
			// We're not at the end, but this isn't a number!
			luaL_error(l, "Unexpected CAN data at position %d: %s", dlc, lua_tostring(l, -1));
		}

		// This element is valid, increment DLC
		dlc++;

		if (dlc > 8) {
			luaL_error(l, "CAN frame length cannot be longer than 8");
		}

		msg[dlc - 1] = val;
	}

	msg.setDlc(dlc);

	// no return value
	return 0;
}

#if !EFI_UNIT_TEST
static SimplePwm pwms[LUA_PWM_COUNT];
static OutputPin pins[LUA_PWM_COUNT];

struct P {
	SimplePwm& pwm;
	lua_Integer idx;
};

static P luaL_checkPwmIndex(lua_State* l, int pos) {
	auto channel = luaL_checkinteger(l, pos);

	// Ensure channel is valid
	if (channel < 0 || channel >= FSIO_COMMAND_COUNT) {
		luaL_error(l, "setPwmDuty invalid channel %d", channel);
	}

	return { pwms[channel], channel };
}

static int lua_startPwm(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);
	auto duty = luaL_checknumber(l, 2);

	// clamp to 1..1000 hz
	freq = clampF(1, freq, 1000);

	startSimplePwmExt(
		&p.pwm, "lua", &engine->executor,
		CONFIG(luaOutputPins[p.idx]), &pins[p.idx],
		freq, duty
	);

	return 0;
}

void luaDeInitPins() {
	// Simply de-init all pins - when the script runs again, they will be re-init'd
	for (size_t i = 0; i < efi::size(pins); i++) {
		pins[i].deInit();
	}
}

static int lua_setPwmDuty(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto duty = luaL_checknumber(l, 2);

	// clamp to 0..1
	duty = clampF(0, duty, 1);

	p.pwm.setSimplePwmDutyCycle(duty);

	return 0;
}

static int lua_setPwmFreq(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);

	// clamp to 1..1000 hz
	freq = clampF(1, freq, 1000);

	p.pwm.setFrequency(freq);

	return 0;
}

static int lua_fan(lua_State* l) {
	lua_pushboolean(l, enginePins.fanRelay.getLogicValue());
	return 1;
}

static int lua_getDigital(lua_State* l) {
	auto idx = luaL_checkinteger(l, 1);

	bool state = false;

	switch (idx) {
		case 0: state = engine->clutchDownState; break;
		case 1: state = engine->clutchUpState; break;
		case 2: state = engine->brakePedalState; break;
		case 3: state = engine->acSwitchState; break;
		default:
			// Return nil to indicate invalid parameter
			lua_pushnil(l);
			return 1;
	}

	lua_pushboolean(l, state);
	return 1;
}

static int lua_setDebug(lua_State* l) {
	// wrong debug mode, ignore
	if (CONFIG(debugMode) != DBG_LUA) {
		return 0;
	}

	auto idx = luaL_checkinteger(l, 1);
	auto val = luaL_checknumber(l, 2);

	// invalid index, ignore
	if (idx < 1 || idx > 7) {
		return 0;
	}

	auto firstDebugField = &tsOutputChannels.debugFloatField1;
	firstDebugField[idx - 1] = val;

	return 0;
}

static int lua_stopEngine(lua_State*) {
	doScheduleStopEngine();

	return 0;
}
#endif // EFI_UNIT_TEST

void configureRusefiLuaHooks(lua_State* l) {
	lua_register(l, "print", lua_efi_print);
	lua_register(l, "getSensor", lua_getSensor);
	lua_register(l, "getSensorRaw", lua_getSensorRaw);
	lua_register(l, "hasSensor", lua_hasSensor);
	lua_register(l, "table3d", lua_table3d);
	lua_register(l, "txCan", lua_txCan);

#if !EFI_UNIT_TEST
	lua_register(l, "startPwm", lua_startPwm);
	lua_register(l, "setPwmDuty", lua_setPwmDuty);
	lua_register(l, "setPwmFreq", lua_setPwmFreq);

	lua_register(l, "getFan", lua_fan);
	lua_register(l, "getDigital", lua_getDigital);
	lua_register(l, "setDebug", lua_setDebug);

	lua_register(l, "stopEngine", lua_stopEngine);
#endif
}
