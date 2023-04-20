#include "pch.h"

#include "rusefi_lua.h"
#include "lua_hooks.h"

#include "fuel_math.h"
#include "airmass.h"
#include "lua_airmass.h"
#include "value_lookup.h"
#include "can_filter.h"
#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
#include "can_msg_tx.h"
#endif // EFI_CAN_SUPPORT
#include "settings.h"
#include <new>

// We don't want to try and use the STL on a microcontroller
#define LUAAA_WITHOUT_CPP_STDLIB
#include "luaaa.hpp"
#include "lua_hooks_util.h"
using namespace luaaa;

#include "script_impl.h"
#include "trigger_emulator_algo.h"

#if EFI_PROD_CODE
#include "electronic_throttle.h"
#endif // EFI_PROD_CODE

#if EFI_SENT_SUPPORT
#include "sent.h"
#endif // EFI_SENT_SUPPORT

static int lua_vin(lua_State* l) {
	auto zeroBasedCharIndex = luaL_checkinteger(l, 1);
	if (zeroBasedCharIndex < 0 || zeroBasedCharIndex > VIN_NUMBER_SIZE) {
		lua_pushnil(l);
	} else {
		char value = engineConfiguration->vinNumber[zeroBasedCharIndex];
		lua_pushnumber(l, value);
	}
	return 1;
}

static int lua_readpin(lua_State* l) {
	auto msg = luaL_checkstring(l, 1);
#if EFI_PROD_CODE
	brain_pin_e pin = parseBrainPin(msg);
	if (!isBrainPinValid(pin)) {
		lua_pushnil(l);
	} else {
		int physicalValue = palReadPad(getHwPort("read", pin), getHwPin("read", pin));
		lua_pushnumber(l, physicalValue);
	}
#endif
	return 1;
}

static int getSensor(lua_State* l, SensorType type) {
	auto result = Sensor::get(type);

	if (result) {
		// return value if valid
		lua_pushnumber(l, result.Value);
	} else {
		// return nil if invalid
		lua_pushnil(l);
	}

	return 1;
}

static int lua_getAuxAnalog(lua_State* l) {
	// todo: shall we use HUMAN_INDEX since UI goes from 1 and Lua loves going from 1?
	auto zeroBasedSensorIndex = luaL_checkinteger(l, 1);

	auto type = static_cast<SensorType>(zeroBasedSensorIndex + static_cast<int>(SensorType::AuxAnalog1));

	return getSensor(l, type);
}

static int lua_getSensorByIndex(lua_State* l) {
	auto zeroBasedSensorIndex = luaL_checkinteger(l, 1);

	return getSensor(l, static_cast<SensorType>(zeroBasedSensorIndex));
}

static SensorType findSensorByName(lua_State* l, const char* name) {
	SensorType type = findSensorTypeByName(name);

	if (l && type == SensorType::Invalid) {
		luaL_error(l, "Invalid sensor type: %s", name);
	}

	return type;
}

static int lua_getSensorByName(lua_State* l) {
	auto sensorName = luaL_checklstring(l, 1, nullptr);
	SensorType type = findSensorByName(l, sensorName);

	return getSensor(l, type);
}

static int lua_getSensorRaw(lua_State* l) {
	auto zeroBasedSensorIndex = luaL_checkinteger(l, 1);

	lua_pushnumber(l, Sensor::getRaw(static_cast<SensorType>(zeroBasedSensorIndex)));
	return 1;
}

static int lua_hasSensor(lua_State* l) {
	auto zeroBasedSensorIndex = luaL_checkinteger(l, 1);

	lua_pushboolean(l, Sensor::hasSensor(static_cast<SensorType>(zeroBasedSensorIndex)));
	return 1;
}

/**
 * @return number of elements
 */
static uint32_t getArray(lua_State* l, int paramIndex, uint8_t *data, uint32_t size) {
	uint32_t result = 0;

	luaL_checktype(l, paramIndex, LUA_TTABLE);
	while (true) {
		lua_pushnumber(l, result + 1);
		auto elementType = lua_gettable(l, paramIndex);
		auto val = lua_tonumber(l, -1);
		lua_pop(l, 1);

		if (elementType == LUA_TNIL) {
			// we're done, this is the end of the array.
			break;
		}

		if (elementType != LUA_TNUMBER) {
			// We're not at the end, but this isn't a number!
			luaL_error(l, "Unexpected data at position %d: %s", result, lua_tostring(l, -1));
		}

		// This element is valid, increment DLC
		result++;

		if (result > size) {
			luaL_error(l, "Input array longer than buffer");
		}

		data[result - 1] = val;
	}
	return result;
}

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST

static int validateCanChannelAndConvertFromHumanIntoZeroIndex(lua_State* l) {
	lua_Integer channel = luaL_checkinteger(l, 1);
	// TODO: support multiple channels
	luaL_argcheck(l, channel == 1 || channel == 2, 1, "only buses 1 and 2 currently supported");
	return channel - HUMAN_OFFSET;
}

static int lua_txCan(lua_State* l) {
	auto bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);

	auto id = luaL_checkinteger(l, 2);
	auto ext = luaL_checkinteger(l, 3);

	// Check that ID is valid based on std vs. ext
	if (ext == 0) {
		luaL_argcheck(l, id <= 0x7FF, 2, "ID specified is greater than max std ID");
	} else {
		luaL_argcheck(l, id <= 0x1FFF'FFFF, 2, "ID specified is greater than max ext ID");
	}

	// conform ext parameter to true/false
	CanTxMessage msg(CanCategory::LUA, id, 8, 0, ext == 0 ? false : true);
	msg.busIndex = bus;

	// Unfortunately there is no way to inspect the length of a table,
	// so we have to just iterate until we run out of numbers
	uint8_t dlc = 0;

	// todo: reduce code duplication with getArray
	luaL_checktype(l, 4, LUA_TTABLE);
	while (true) {
		lua_pushnumber(l, dlc + 1);
		auto elementType = lua_gettable(l, 4);
		auto val = lua_tonumber(l, -1);
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
#endif // EFI_CAN_SUPPORT

static LuaAirmass luaAirmass;

AirmassModelBase& getLuaAirmassModel() {
	return luaAirmass;
}

#if !EFI_UNIT_TEST
static SimplePwm pwms[LUA_PWM_COUNT];

struct P {
	SimplePwm& pwm;
	lua_Integer idx;
};

static P luaL_checkPwmIndex(lua_State* l, int pos) {
	auto channel = luaL_checkinteger(l, pos);

    // todo: what a mess :( CAN buses start at 1 and PWM channels start at 0 :(
	// Ensure channel is valid
	if (channel < 0 || channel >= LUA_PWM_COUNT) {
		luaL_error(l, "setPwmDuty invalid channel %d", channel);
	}

	return { pwms[channel], channel };
}

static int lua_startPwm(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);
	auto duty = luaL_checknumber(l, 3);

	// clamp to 1..1000 hz
	freq = clampF(1, freq, 1000);

	startSimplePwmExt(
		&p.pwm, "lua", &engine->executor,
		engineConfiguration->luaOutputPins[p.idx], &enginePins.luaOutputPins[p.idx],
		freq, duty
	);

	return 0;
}

void luaDeInitPins() {
	// Simply de-init all pins - when the script runs again, they will be re-init'd
	for (size_t i = 0; i < efi::size(enginePins.luaOutputPins); i++) {
		enginePins.luaOutputPins[i].deInit();
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
		case 0: state = engine->engineState.clutchDownState; break;
		case 1: state = engine->engineState.clutchUpState; break;
		case 2: state = engine->engineState.brakePedalState; break;
		case 3: state = engine->module<AcController>().unmock().acButtonState; break;
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
	if (engineConfiguration->debugMode != DBG_LUA) {
		return 0;
	}

	auto idx = luaL_checkinteger(l, 1);
	auto val = luaL_checknumber(l, 2);

	// invalid index, ignore
	if (idx < 1 || idx > 7) {
		return 0;
	}

	auto firstDebugField = &engine->outputChannels.debugFloatField1;
	firstDebugField[idx - 1] = val;

	return 0;
}

static auto lua_getAirmassResolveMode(lua_State* l) {
	if (lua_gettop(l) == 0) {
		// zero args, return configured mode
		return engineConfiguration->fuelAlgorithm;
	} else {
		return static_cast<engine_load_mode_e>(luaL_checkinteger(l, 1));
	}
}

static int lua_getAirmass(lua_State* l) {
	auto airmassMode = lua_getAirmassResolveMode(l);
	auto airmass = getAirmassModel(airmassMode);

	if (!airmass) {
		return luaL_error(l, "null airmass");
	}

	auto rpm = Sensor::getOrZero(SensorType::Rpm);
	auto result = airmass->getAirmass(rpm).CylinderAirmass;

	lua_pushnumber(l, result);
	return 1;
}

static int lua_setAirmass(lua_State* l) {
	float airmass = luaL_checknumber(l, 1);
	float engineLoadPercent = luaL_checknumber(l, 2);

	airmass = clampF(0, airmass, 10);
	engineLoadPercent = clampF(0, engineLoadPercent, 1000);

	luaAirmass.setAirmass({airmass, engineLoadPercent});

	return 0;
}

#endif // EFI_UNIT_TEST

// TODO: PR this back in to https://github.com/gengyong/luaaa
namespace LUAAA_NS {
    template<typename TCLASS, typename ...ARGS>
    struct PlacementConstructorCaller<TCLASS, lua_State*, ARGS...>
    {
        // this speciailization passes the Lua state to the constructor as first argument, as it shouldn't
        // participate in the index generation as it's not a normal parameter passed via the Lua stack.

        static TCLASS * Invoke(lua_State * state, void * mem)
        {
            return InvokeImpl(state, mem, typename make_indices<sizeof...(ARGS)>::type());
        }

    private:
        template<std::size_t ...Ns>
        static TCLASS * InvokeImpl(lua_State * state, void * mem, indices<Ns...>)
        {
            (void)state;
            return new(mem) TCLASS(state, LuaStack<ARGS>::get(state, Ns + 1)...);
        }
    };
}

struct LuaSensor final : public StoredValueSensor {
	LuaSensor() : LuaSensor(nullptr, "Invalid") { }

	~LuaSensor() {
		unregister();
	}

	LuaSensor(lua_State* l, const char* name)
		: StoredValueSensor(findSensorByName(l, name), MS2NT(100))
	{
		// do a soft collision check to avoid a fatal error from the hard check in Register()
		if (l && Sensor::hasSensor(type())) {
			luaL_error(l, "Tried to create a Lua sensor of type %s, but one was already registered.", getSensorName());
		} else {
			Register();
			efiPrintf("LUA registered sensor of type %s", getSensorName());
		}
	}

	bool isRedundant() const override {
		return m_isRedundant;
	}

	// do we need method defined exactly on LuaSensor for Luaa to be happy?
	void setTimeout(int timeoutMs) override {
	    StoredValueSensor::setTimeout(timeoutMs);
	}

	void setRedundant(bool value) {
		m_isRedundant = value;
	}

	void set(float value) {
		setValidValue(value, getTimeNowNt());
	}

	void invalidate() {
		StoredValueSensor::invalidate();
	}

	void showInfo(const char* sensorName) const override {
		const auto value = get();
		efiPrintf("Sensor \"%s\": Lua sensor: Valid: %s Converted value %.2f", sensorName, boolToString(value.Valid), value.Value);
	}

private:
	bool m_isRedundant = false;
};

struct LuaPid final {
	LuaPid() = default;

	LuaPid(float kp, float ki, float kd, float min, float max)
		: m_pid(&m_params)
	{
		m_params.pFactor = kp;
		m_params.iFactor = ki;
		m_params.dFactor = kd;

		m_params.offset = 0;
		m_params.periodMs = 0;
		m_params.minValue = min;
		m_params.maxValue = max;

		m_lastUpdate.reset();
	}

	float get(float target, float input) {
#if EFI_UNIT_TEST
		extern int timeNowUs;
		// this is how we avoid zero dt
		timeNowUs += 1000;
#endif

		float dt = m_lastUpdate.getElapsedSecondsAndReset(getTimeNowNt());

		return m_pid.getOutput(target, input, dt);
	}

	void setOffset(float offset) {
		m_params.offset = offset;
		reset();
	}

	void reset() {
		m_pid.reset();
	}

private:
	Pid m_pid;
	Timer m_lastUpdate;
	pid_s m_params;
};

static bool isFunction(lua_State* l, int idx) {
	return lua_type(l, idx) == LUA_TFUNCTION;
}

int getLuaFunc(lua_State* l) {
	if (!isFunction(l, 1)) {
		return luaL_error(l, "expected function");
	} else {
		return luaL_ref(l, LUA_REGISTRYINDEX);
	}
}

#if EFI_CAN_SUPPORT
int lua_canRxAdd(lua_State* l) {
	uint32_t eid;

	// defaults if not passed
	int bus = ANY_BUS;
	int callback = NO_CALLBACK;

	switch (lua_gettop(l)) {
		case 1:
			// handle canRxAdd(id)
			eid = luaL_checkinteger(l, 1);
			break;

		case 2:
			if (isFunction(l, 2)) {
				// handle canRxAdd(id, callback)
				eid = luaL_checkinteger(l, 1);
				lua_remove(l, 1);
				callback = getLuaFunc(l);
			} else {
				// handle canRxAdd(bus, id)
				bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);
				eid = luaL_checkinteger(l, 2);
			}

			break;
		case 3:
			// handle canRxAdd(bus, id, callback)
			bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);
			eid = luaL_checkinteger(l, 2);
			lua_remove(l, 1);
			lua_remove(l, 1);
			callback = getLuaFunc(l);
			break;
		default:
			return luaL_error(l, "Wrong number of arguments to canRxAdd. Got %d, expected 1, 2, or 3.");
	}

	addLuaCanRxFilter(eid, FILTER_SPECIFIC, bus, callback);

	return 0;
}

int lua_canRxAddMask(lua_State* l) {
	uint32_t eid;
	uint32_t mask;

	// defaults if not passed
	int bus = ANY_BUS;
	int callback = NO_CALLBACK;

	switch (lua_gettop(l)) {
		case 2:
			// handle canRxAddMask(id, mask)
			eid = luaL_checkinteger(l, 1);
			mask = luaL_checkinteger(l, 2);
			break;

		case 3:
			if (isFunction(l, 3)) {
				// handle canRxAddMask(id, mask, callback)
				eid = luaL_checkinteger(l, 1);
				mask = luaL_checkinteger(l, 2);
				lua_remove(l, 1);
				lua_remove(l, 1);
				callback = getLuaFunc(l);
			} else {
				// handle canRxAddMask(bus, id, mask)
		    	bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);
				eid = luaL_checkinteger(l, 2);
				mask = luaL_checkinteger(l, 3);
			}

			break;
		case 4:
			// handle canRxAddMask(bus, id, mask, callback)
			bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);
			eid = luaL_checkinteger(l, 2);
			mask = luaL_checkinteger(l, 3);
			lua_remove(l, 1);
			lua_remove(l, 1);
			lua_remove(l, 1);
			callback = getLuaFunc(l);
			break;
		default:
			return luaL_error(l, "Wrong number of arguments to canRxAddMask. Got %d, expected 2, 3, or 4.");
	}

	addLuaCanRxFilter(eid, mask, bus, callback);

	return 0;
}
#endif // EFI_CAN_SUPPORT

void configureRusefiLuaHooks(lua_State* l) {
	LuaClass<Timer> luaTimer(l, "Timer");
	luaTimer
		.ctor()
		.fun("reset",             static_cast<void (Timer::*)()     >(&Timer::reset            ))
		.fun("getElapsedSeconds", static_cast<float(Timer::*)()const>(&Timer::getElapsedSeconds));

	LuaClass<LuaSensor> luaSensor(l, "Sensor");
	luaSensor
		.ctor<lua_State*, const char*>()
		.fun("set", &LuaSensor::set)
		.fun("setRedundant", &LuaSensor::setRedundant)
		.fun("setTimeout", &LuaSensor::setTimeout)
		.fun("invalidate", &LuaSensor::invalidate);

	LuaClass<LuaPid> luaPid(l, "Pid");
	luaPid
		.ctor<float, float, float, float, float>()
		.fun("get", &LuaPid::get)
		.fun("setOffset", &LuaPid::setOffset)
		.fun("reset", &LuaPid::reset);

	configureRusefiLuaUtilHooks(l);

	lua_register(l, "readPin", lua_readpin);
	lua_register(l, "vin", lua_vin);
	lua_register(l, "getAuxAnalog", lua_getAuxAnalog);
	lua_register(l, "getSensorByIndex", lua_getSensorByIndex);
	lua_register(l, "getSensor", lua_getSensorByName);
	lua_register(l, "getSensorRaw", lua_getSensorRaw);
	lua_register(l, "hasSensor", lua_hasSensor);
	lua_register(l, "table3d", [](lua_State* l) {
		auto humanTableIdx = luaL_checkinteger(l, 1);
		auto x = luaL_checknumber(l, 2);
		auto y = luaL_checknumber(l, 3);

		// index table, compute table lookup
		auto result = getscriptTable(humanTableIdx - HUMAN_OFFSET)->getValue(x, y);

		lua_pushnumber(l, result);
		return 1;
	});

	lua_register(l, "curve", [](lua_State* l) {
		// index starting from 1
		auto humanCurveIdx = luaL_checkinteger(l, 1);
		auto x = luaL_checknumber(l, 2);

		auto result = getCurveValue(humanCurveIdx - HUMAN_OFFSET, x);

		lua_pushnumber(l, result);
		return 1;
	});

#if EFI_SENT_SUPPORT
	lua_register(l, "getSentValue",
			[](lua_State* l) {
			auto humanIndex = luaL_checkinteger(l, 1);
			auto value = getSentValue(humanIndex - 1);
			lua_pushnumber(l, value);
			return 1;
	});

	lua_register(l, "getSentValues",
			[](lua_State* l) {
			uint16_t sig0;
			uint16_t sig1;
			auto humanIndex = luaL_checkinteger(l, 1);
			auto ret = getSentValues(humanIndex - 1, &sig0, &sig1);
			lua_pushnumber(l, sig0);
			lua_pushnumber(l, sig1);
			return 2;
	});
#endif // EFI_SENT_SUPPORT

#if EFI_LAUNCH_CONTROL
	lua_register(l, "setSparkSkipRatio", [](lua_State* l) {
		auto targetSkipRatio = luaL_checknumber(l, 1);
		engine->softSparkLimiter.setTargetSkipRatio(targetSkipRatio);
		return 1;
	});
#endif // EFI_LAUNCH_CONTROL

#if EFI_SHAFT_POSITION_INPUT && !EFI_UNIT_TEST
	lua_register(l, "selfStimulateRPM", [](lua_State* l) {
		auto rpm = luaL_checkinteger(l, 1);
		if (rpm < 1) {
			disableTriggerStimulator();
			return 0;
		}
		if (!engine->triggerCentral.directSelfStimulation) {
		    enableTriggerStimulator();
		}
        setTriggerEmulatorRPM(rpm);
		return 0;
	});
#endif // EFI_UNIT_TEST

	/**
	 * same exact could be accomplished via LuaSensor just with more API
	 */
	lua_register(l, "setLuaGauge", [](lua_State* l) {
		auto index = luaL_checkinteger(l, 1) - 1;
		auto value = luaL_checknumber(l, 2);
		if (index < 0 || index >= LUA_GAUGE_COUNT)
			return 0;
		extern StoredValueSensor luaGauges[LUA_GAUGE_COUNT];
		luaGauges[index].setValidValue(value, getTimeNowNt());
		return 0;
	});

	lua_register(l, "enableCanTx", [](lua_State* l) {
		engine->allowCanTx = lua_toboolean(l, 1);
		return 0;
	});

#if EFI_PROD_CODE
	lua_register(l, "restartEtb", [](lua_State* l) {
		// this is about Lua sensor acting in place of real analog PPS sensor
		// todo: smarter implementation
		doInitElectronicThrottle();
		return 0;
	});
#endif // EFI_PROD_CODE

	lua_register(l, "crc8_j1850", [](lua_State* l) {
		uint8_t data[8];
		uint32_t length = getArray(l, 1, data, sizeof(data));
		auto trimLength = luaL_checkinteger(l, 2);
		int crc = crc8(data, minI(length, trimLength));

		lua_pushnumber(l, crc);
		return 1;
	});

#if EFI_BOOST_CONTROL
	lua_register(l, "setBoostTargetAdd", [](lua_State* l) {
		engine->boostController.luaTargetAdd = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(l, "setBoostTargetMult", [](lua_State* l) {
		engine->boostController.luaTargetMult = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(l, "setBoostDutyAdd", [](lua_State* l) {
		engine->boostController.luaOpenLoopAdd = luaL_checknumber(l, 1);
		return 0;
	});
#endif // EFI_BOOST_CONTROL
#if EFI_IDLE_CONTROL
	lua_register(l, "setIdleAdd", [](lua_State* l) {
		engine->module<IdleController>().unmock().luaAdd = luaL_checknumber(l, 1);
		return 0;
	});
#endif
	lua_register(l, "setTimingAdd", [](lua_State* l) {
		engine->ignitionState.luaTimingAdd = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(l, "setTimingMult", [](lua_State* l) {
		engine->ignitionState.luaTimingMult = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(l, "setFuelAdd", [](lua_State* l) {
		engine->engineState.lua.fuelAdd = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(l, "setFuelMult", [](lua_State* l) {
		engine->engineState.lua.fuelMult = luaL_checknumber(l, 1);
		return 0;
	});
#if EFI_PROD_CODE
	lua_register(l, "setEtbAdd", [](lua_State* l) {
		auto luaAdjustment = luaL_checknumber(l, 1);

		setEtbLuaAdjustment(luaAdjustment);

		return 0;
	});
	lua_register(l, "setEtbDisabled", [](lua_State* l) {
		engine->engineState.lua.luaDisableEtb = lua_toboolean(l, 1);
		return 0;
	});
	lua_register(l, "setIgnDisabled", [](lua_State* l) {
		engine->engineState.lua.luaIgnCut = lua_toboolean(l, 1);
		return 0;
	});
#endif // EFI_PROD_CODE

	lua_register(l, "setClutchUpState", [](lua_State* l) {
		engine->engineState.lua.clutchUpState = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(l, "setBrakePedalState", [](lua_State* l) {
		engine->engineState.lua.brakePedalState = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(l, "setAcRequestState", [](lua_State* l) {
		engine->engineState.lua.acRequestState = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(l, "getCalibration", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		auto result = getConfigValueByName(propertyName);
		lua_pushnumber(l, result);
		return 1;
	});

	lua_register(l, "getOutput", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		auto result = getOutputValueByName(propertyName);
		lua_pushnumber(l, result);
		return 1;
	});

#if EFI_SHAFT_POSITION_INPUT
	lua_register(l, "getEngineState", [](lua_State* l) {
		spinning_state_e state = engine->rpmCalculator.getState();
		int luaStateCode;
		if (state == STOPPED) {
			luaStateCode = 0;
		} else if (state == RUNNING) {
			luaStateCode = 2;
		} else {
			// spinning-up or cranking
			luaStateCode = 1;
		}
		lua_pushnumber(l, luaStateCode);
		return 1;
	});
#endif //EFI_SHAFT_POSITION_INPUT

	lua_register(l, "setCalibration", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		auto value = luaL_checknumber(l, 2);
		auto incrementVersion = lua_toboolean(l, 3);
		setConfigValueByName(propertyName, value);
		if (incrementVersion) {
			incrementGlobalConfigurationVersion("lua");
		}
		return 0;
	});

	lua_register(l, "getGlobalConfigurationVersion", [](lua_State* l) {
		lua_pushnumber(l, engine->getGlobalConfigurationVersion());
		return 1;
	});

	lua_register(l, "setAcDisabled", [](lua_State* l) {
		auto value = lua_toboolean(l, 1);
		engine->module<AcController>().unmock().isDisabledByLua = value;
		return 0;
	});
	lua_register(l, "getTimeSinceAcToggleMs", [](lua_State* l) {
		int result = US2MS(getTimeNowUs()) - engine->module<AcController>().unmock().acSwitchLastChangeTimeMs;
		lua_pushnumber(l, result);
		return 1;
	});

#if EFI_VEHICLE_SPEED
	lua_register(l, "getCurrentGear", [](lua_State* l) {
		lua_pushinteger(l, engine->module<GearDetector>()->getCurrentGear());
		return 1;
	});

	lua_register(l, "getRpmInGear", [](lua_State* l) {
		auto idx = luaL_checkinteger(l, 1);
		lua_pushinteger(l, engine->module<GearDetector>()->getRpmInGear(idx));
		return 1;
	});
#endif // EFI_VEHICLE_SPEED

#if !EFI_UNIT_TEST
	lua_register(l, "startPwm", lua_startPwm);
	lua_register(l, "setPwmDuty", lua_setPwmDuty);
	lua_register(l, "setPwmFreq", lua_setPwmFreq);

	lua_register(l, "getFan", lua_fan);
	lua_register(l, "getDigital", lua_getDigital);
	lua_register(l, "setDebug", lua_setDebug);
	lua_register(l, "getAirmass", lua_getAirmass);
	lua_register(l, "setAirmass", lua_setAirmass);

	lua_register(l, "stopEngine", [](lua_State*) {
		doScheduleStopEngine();
		return 0;
	});
#if EFI_SHAFT_POSITION_INPUT
	lua_register(l, "getTimeSinceTriggerEventMs", [](lua_State* l) {
		int result = engine->triggerCentral.m_lastEventTimer.getElapsedUs() / 1000;
		lua_pushnumber(l, result);
		return 1;
	});
#endif // EFI_SHAFT_POSITION_INPUT

#if EFI_CAN_SUPPORT
	lua_register(l, "canRxAdd", lua_canRxAdd);
	lua_register(l, "canRxAddMask", lua_canRxAddMask);
#endif // EFI_CAN_SUPPORT
#endif // not EFI_UNIT_TEST

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
	lua_register(l, "txCan", lua_txCan);
#endif
}
