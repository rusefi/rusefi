#include "pch.h"

#include "rusefi_lua.h"
#include "lua_hooks.h"

#include "lua_biquad.h"
#include "fuel_math.h"
#include "airmass.h"
#include "lua_airmass.h"
#include "value_lookup.h"
#include "can_filter.h"
#include "tunerstudio.h"
#include "lua_pid.h"
#include "start_stop.h"
#include "tinymt32.h" // TL,DR: basic implementation of 'random'
#include "signaldebounce.h"

#if EFI_PROD_CODE && HW_HELLEN
#include "hellen_meta.h"
#endif

#if EFI_DAC
#include "dac.h"
#endif // EFI_DAC

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
		lua_pushinteger(l, value);
	}
	return 1;
}

static int lua_readpin(lua_State* l) {
#if EFI_PROD_CODE
	const char * msg = luaL_checkstring(l, 1);
	brain_pin_e pin = parseBrainPin(msg);
	if (!isBrainPinValid(pin)) {
	    efiPrintf("LUA: invalid pin [%s]", msg);
		lua_pushnil(l);
	} else {
		int physicalValue = palReadPad(getHwPort("read", pin), getHwPin("read", pin));
		lua_pushinteger(l, physicalValue);
	}
#else
    UNUSED(l);
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
	// todo: shall we use HUMAN_OFFSET since UI goes from 1 and Lua loves going from 1?
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
uint32_t getLuaArray(lua_State* l, int paramIndex, uint8_t *data, uint32_t size) {
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
		else {
			data[result - 1] = val;
		}
	}
	return result;
}

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST

static int validateCanChannelAndConvertFromHumanIntoZeroIndex(lua_State* l) {
	lua_Integer channel = luaL_checkinteger(l, 1);
	luaL_argcheck(l, channel >= 1 && channel <= EFI_CAN_BUS_COUNT, 1, "Invalid bus index");
	return channel - HUMAN_OFFSET;
}

static int lua_txCan(lua_State* l) {
	ScopePerf perf(PE::LuaOneCanTxFunction);
  int bus;
  int id;
  int ext;
  int dataIndex;
  if (lua_gettop(l) == 2) {
    bus = 0;
    id = luaL_checkinteger(l, 1);
    ext = 0;
    dataIndex = 2;
  } else {
    bus = validateCanChannelAndConvertFromHumanIntoZeroIndex(l);
    id = luaL_checkinteger(l, 2);
	  ext = luaL_checkinteger(l, 3);
    dataIndex = 4;
  }

	// Check that ID is valid based on std vs. ext
	if (ext == 0) {
		luaL_argcheck(l, id <= 0x7FF, 2, "ID specified is greater than max std ID");
	} else {
		luaL_argcheck(l, id <= 0x1FFF'FFFF, 2, "ID specified is greater than max ext ID");
	}

	// conform ext parameter to true/false
	CanTxMessage msg(CanCategory::LUA, id, 8, bus, ext == 0 ? false : true);

	// Unfortunately there is no way to inspect the length of a table,
	// so we have to just iterate until we run out of numbers
	uint8_t dlc = 0;

	// todo: reduce code duplication with getLuaArray
	luaL_checktype(l, dataIndex, LUA_TTABLE);
	while (true) {
		lua_pushnumber(l, dlc + 1);
		auto elementType = lua_gettable(l, dataIndex);
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

#ifndef PWM_FREQ_PWM
#define PWM_FREQ_PWM 1000
#endif

void startPwm(int index, float freq, float duty) {
	// clamp to 1..1000 hz, this line would turn 0hz on/off PWM into 1hz behind the scenes
	freq = clampF(1, freq, 1000);

  brain_pin_e pwmPin = engineConfiguration->luaOutputPins[index];

	startSimplePwmExt(
		&pwms[index], "lua", &engine->scheduler,
		pwmPin, &enginePins.luaOutputPins[index],
		freq, duty
	);

	efiPrintf("LUA PWM on %s at %f initial duty",
	  hwPortname(pwmPin),
	  PERCENT_MULT * duty);
}

static int lua_startPwm(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto freq = luaL_checknumber(l, 2);
	auto duty = luaL_checknumber(l, 3);

    if (duty < 0 || duty > PWM_MAX_DUTY) {
        luaL_error(l, "Duty parameter should be from 0 to 1 got %f", duty);
        return 0;
    }

  startPwm(p.idx, freq, duty);

	return 0;
}

void luaDeInitPins() {
	// Simply de-init all pins - when the script runs again, they will be re-init'd
	for (size_t i = 0; i < efi::size(enginePins.luaOutputPins); i++) {
		enginePins.luaOutputPins[i].deInit();
	}
}

void setPwmDuty(int index, float duty) {
	// clamp to 0..1
	duty = clampF(0, duty, 1);

	pwms[index].setSimplePwmDutyCycle(duty);
}

static int lua_setPwmDuty(lua_State* l) {
	auto p = luaL_checkPwmIndex(l, 1);
	auto duty = luaL_checknumber(l, 2);
	setPwmDuty(p.idx, duty);

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

bool getAuxDigital(int index) {
#if EFI_PROD_CODE
    return efiReadPin(engineConfiguration->luaDigitalInputPins[index]);
#else
    UNUSED(index);
    return false;
#endif
}

static int lua_getAuxDigital(lua_State* l) {
	auto idx = luaL_checkinteger(l, 1);
	if (idx < 0 || idx >= LUA_DIGITAL_INPUT_COUNT) {
		// Return nil to indicate invalid parameter
		lua_pushnil(l);
		return 1;
	}

	if (!isBrainPinValid(engineConfiguration->luaDigitalInputPins[idx])) {
		// Return nil to indicate invalid pin
		lua_pushnil(l);
		return 1;
	}

#if !EFI_SIMULATOR
	bool state = getAuxDigital(idx);
	lua_pushboolean(l, state);
#endif // !EFI_SIMULATOR

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

#if EFI_ENGINE_CONTROL
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
	auto result = airmass->getAirmass(rpm, false).CylinderAirmass;

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
#endif // EFI_ENGINE_CONTROL

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
	int argumentCount = lua_gettop(l);

	switch (argumentCount) {
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
			return luaL_error(l, "Wrong number of arguments to canRxAdd. Got %d, expected 1, 2, or 3.", argumentCount);
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

PUBLIC_API_WEAK void boardConfigureLuaHooks(lua_State* lState) {
    UNUSED(lState);
}

static tinymt32_t tinymt;

void configureRusefiLuaHooks(lua_State* lState) {
  boardConfigureLuaHooks(lState);
  configureRusefiLuaHooksExt(lState);

  tinymt32_init(&tinymt, 1534525); // todo: share instance with launch_control? probably not?
	lua_register(lState, "random", [](lua_State* l) {
	  auto random = tinymt32_generate_float(&tinymt);
		lua_pushnumber(l, random);
		return 1;
	});

	LuaClass<Timer> luaTimer(lState, "Timer");
	luaTimer
		.ctor()
		.fun("reset",             static_cast<void (Timer::*)()     >(&Timer::reset            ))
		.fun("getElapsedSeconds", static_cast<float(Timer::*)()const>(&Timer::getElapsedSeconds));

	LuaClass<LuaSensor> luaSensor(lState, "Sensor");
	luaSensor
		.ctor<lua_State*, const char*>()
		.fun("set", &LuaSensor::set)
		.fun("setRedundant", &LuaSensor::setRedundant)
		.fun("setTimeout", &LuaSensor::setTimeout)
		.fun("invalidate", &LuaSensor::invalidate);

#ifndef WITH_LUA_PID
#define WITH_LUA_PID TRUE
#endif

#if WITH_LUA_PID
  LuaClass<LuaBiQuad> biQuard(lState, "Biquad");
  biQuard
    .ctor()
		.fun("filter", &LuaBiQuad::filter)
		.fun("configureLowpass", &LuaBiQuad::configureLowpass);

	LuaClass<LuaPid> luaPid(lState, "Pid");
	luaPid
		.ctor<float, float, float, float, float>()
		.fun("get", &LuaPid::get)
		.fun("setOffset", &LuaPid::setOffset)
		.fun("reset", &LuaPid::reset);

	LuaClass<LuaIndustrialPid> luaIndustrialPid(lState, "IndustrialPid");
	luaIndustrialPid
		.ctor<float, float, float, float, float>()
		.fun("get", &LuaIndustrialPid::get)
		.fun("setOffset", &LuaIndustrialPid::setOffset)
		.fun("setDerivativeFilterLoss", &LuaIndustrialPid::setDerivativeFilterLoss)
		.fun("setAntiwindupFreq", &LuaIndustrialPid::setAntiwindupFreq)
		.fun("reset", &LuaIndustrialPid::reset);
#endif

	configureRusefiLuaUtilHooks(lState);

	lua_register(lState, "readPin", lua_readpin);
#if EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
	lua_register(lState, "startCrankingEngine", [](lua_State* l) {
		doStartCranking();
		return 0;
	});
#endif // EFI_PROD_CODE && EFI_SHAFT_POSITION_INPUT
	lua_register(lState, "vin", lua_vin);

	lua_register(lState, "getAuxAnalog", lua_getAuxAnalog);
	lua_register(lState, "getSensorByIndex", lua_getSensorByIndex);
	lua_register(lState, "getSensor", lua_getSensorByName);
	lua_register(lState, "getSensorRaw", lua_getSensorRaw);
	lua_register(lState, "hasSensor", lua_hasSensor);

#ifndef WITH_LUA_CONSUMPTION
#define WITH_LUA_CONSUMPTION TRUE
#endif

#if EFI_VEHICLE_SPEED && WITH_LUA_CONSUMPTION && defined (MODULE_ODOMETER)
	lua_register(lState, "getConsumedGrams", [](lua_State* l) {
		lua_pushnumber(l, engine->module<TripOdometer>()->getConsumedGrams());
		return 1;
	});
	lua_register(lState, "getConsumedGramsRemainder", [](lua_State* l) {
		lua_pushnumber(l, engine->module<TripOdometer>()->getConsumedGramsRemainder());
		return 1;
	});
	lua_register(lState, "getConsumptionGramPerSecond", [](lua_State* l) {
		lua_pushnumber(l, engine->module<TripOdometer>()->getConsumptionGramPerSecond());
		return 1;
	});
	lua_register(lState, "resetOdometer", [](lua_State*) {
		engine->module<TripOdometer>()->reset();
		return 0;
	});
#endif // EFI_VEHICLE_SPEED WITH_LUA_CONSUMPTION MODULE_ODOMETER
	lua_register(lState, "table3d", [](lua_State* l) {
		auto humanTableIdx = luaL_checkinteger(l, 1);
		auto x = luaL_checknumber(l, 2);
		auto y = luaL_checknumber(l, 3);

		// index table, compute table lookup
		auto result = getscriptTable(humanTableIdx - HUMAN_OFFSET)->getValue(x, y);

		lua_pushnumber(l, result);
		return 1;
	});
    // time since console or TunerStudio
	lua_register(lState, "secondsSinceTsActivity", [](lua_State* l) {
		lua_pushinteger(l, getSecondsSinceChannelsRequest());
		return 1;
	});

	lua_register(lState, "curve", [](lua_State* l) {
		// index starting from 1
		auto humanCurveIdx = luaL_checkinteger(l, 1);
		auto x = luaL_checknumber(l, 2);

		auto result = getCurveValue(humanCurveIdx - HUMAN_OFFSET, x);

		lua_pushnumber(l, result);
		return 1;
	});

#if EFI_PROD_CODE || EFI_SIMULATOR
extern int luaCommandCounters[LUA_BUTTON_COUNT];

	lua_register(lState, "getTsButtonCount",
			[](lua_State* l) {
			auto humanIndex = luaL_checkinteger(l, 1);
			if (humanIndex < 1 || humanIndex > LUA_BUTTON_COUNT) {
			  luaL_error(l, "Invalid button index: %d", humanIndex);
			  return 0;
			}
			lua_pushinteger(l, luaCommandCounters[humanIndex - 1]);
			return 1;
	});
#endif // EFI_PROD_CODE || EFI_SIMULATOR

#if EFI_PROD_CODE && EFI_SENT_SUPPORT
	lua_register(lState, "getSentValue",
			[](lua_State* l) {
			auto humanIndex = luaL_checkinteger(l, 1);
			auto value = getSentValue(static_cast<SentInput>(humanIndex));
			lua_pushnumber(l, value);
			return 1;
	});

	lua_register(lState, "getSentValues",
			[](lua_State* l) {
			uint16_t sig0;
			uint16_t sig1;
			auto humanIndex = luaL_checkinteger(l, 1);
			/*auto ret = */getSentValues(static_cast<SentInput>(humanIndex), &sig0, &sig1);
			lua_pushinteger(l, sig0);
			lua_pushinteger(l, sig1);
			return 2;
	});
#endif // EFI_SENT_SUPPORT

#if EFI_LAUNCH_CONTROL
	lua_register(lState, "setSparkSkipRatio", [](lua_State* l) {
		auto targetSkipRatio = luaL_checknumber(l, 1);
		engine->engineState.luaSoftSparkSkip = targetSkipRatio;
		engine->engineState.updateSparkSkip();
		return 0;
	});
	lua_register(lState, "setSparkHardSkipRatio", [](lua_State* l) {
		auto targetSkipRatio = luaL_checknumber(l, 1);
		engine->engineState.luaHardSparkSkip = targetSkipRatio;
		engine->engineState.updateSparkSkip();
		return 0;
	});
	lua_register(lState, "setLaunchTrigger", [](lua_State* l) {
		auto value = luaL_checkinteger(l, 1);
  	engine->launchController.luaLaunchState = value;
		return 0;
	});
#endif // EFI_LAUNCH_CONTROL
#if EFI_ANTILAG_SYSTEM
	lua_register(lState, "setRollingIdleTrigger", [](lua_State* l) {
		auto value = luaL_checkinteger(l, 1);
  	engine->antilagController.luaAntilagState = value;
		return 0;
	});
#endif // EFI_ANTILAG_SYSTEM

#if EFI_EMULATE_POSITION_SENSORS && !EFI_UNIT_TEST
	lua_register(lState, "selfStimulateRPM", [](lua_State* l) {
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
	lua_register(lState, "setLuaGauge", [](lua_State* l) {
		auto index = luaL_checkinteger(l, 1) - 1;
		auto value = luaL_checknumber(l, 2);
		if (index < 0 || index >= LUA_GAUGE_COUNT)
			return 0;
		extern StoredValueSensor luaGauges[LUA_GAUGE_COUNT];
		luaGauges[index].setValidValue(value, getTimeNowNt());
		return 0;
	});

	lua_register(lState, "enableCanTx", [](lua_State* l) {
		engine->allowCanTx = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(lState, "enableCanRxWorkaround", [](lua_State*) {
		// that's about global_can_data
		engineConfiguration->luaCanRxWorkaround = true;
		return 0;
	});
// high-performance CANbus should be done on F7+, let's preserve couple of priceless bytes on F4
#if !defined(STM32F4)
#if EFI_CAN_SUPPORT
	lua_register(lState, "getCanRxDropped", [](lua_State* l) {
	  auto count = getLuaCanRxDropped();
    lua_pushinteger(l, count);
		return 1;
	});
#endif // EFI_CAN_SUPPORT
	lua_register(lState, "disableExtendedCanBroadcast", [](lua_State*) {
		// that's about global_can_data
		engineConfiguration->enableExtendedCanBroadcast = false;
		return 0;
	});
	lua_register(lState, "getCanBaudRate", [](lua_State* l) {
	  auto index = luaL_checkinteger(l, 1);
	  if (index == 1) {
	    lua_pushinteger(l, engineConfiguration->canBaudRate);
	  } else {
	    lua_pushinteger(l, engineConfiguration->can2BaudRate);
	  }
		return 1;
	});

	lua_register(lState, "setEngineTorque", [](lua_State* l) {
		engine->engineState.lua.engineTorque = luaL_checknumber(l, 1);
		return 0;
	});
#endif // STM32F4

#if !defined(STM32F4) || defined(WITH_LUA_GET_GPPWM_STATE)
	lua_register(lState, "getGpPwm", [](lua_State* l) {
	  auto index = luaL_checkinteger(l, 1);
	  // this works due to updateGppwm being invoked from periodicSlowCallback
	  auto result = engine->outputChannels.gppwmOutput[index];
	  lua_pushnumber(l, result);
		return 1;
	});
#endif

#if EFI_ELECTRONIC_THROTTLE_BODY && EFI_PROD_CODE
  lua_register(lState, "getEtbTarget", [](lua_State* l) {
    auto controller = engine->etbControllers[0];
 		assertNotNull(controller, 0);
    auto result = controller->getCurrentTarget();
		lua_pushnumber(l, result);
		return 1;
  });
	lua_register(lState, "restartEtb", [](lua_State*) {
		// this is about Lua sensor acting in place of real analog PPS sensor
		// todo: smarter implementation
		doInitElectronicThrottle(true); // lame, we run with 'isStartupInit=true' in order to reset, NOT COOL
		return 0;
	});
#endif // EFI_ELECTRONIC_THROTTLE_BODY

    // checksum stuff
	lua_register(lState, "crc8_j1850", [](lua_State* l) {
		uint8_t data[8];
		uint32_t length = getLuaArray(l, 1, data, sizeof(data));
		auto trimLength = luaL_checkinteger(l, 2);
		int crc = crc8(data, minI(length, trimLength));

		lua_pushinteger(l, crc);
		return 1;
	});

#if EFI_BOOST_CONTROL
	lua_register(lState, "setBoostTargetAdd", [](lua_State* l) {
		engine->module<BoostController>().unmock().luaTargetAdd = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(lState, "setBoostTargetMult", [](lua_State* l) {
		engine->module<BoostController>().unmock().luaTargetMult = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(lState, "setBoostDutyAdd", [](lua_State* l) {
		engine->module<BoostController>().unmock().luaOpenLoopAdd = luaL_checknumber(l, 1);
		return 0;
	});
#endif // EFI_BOOST_CONTROL
#if EFI_IDLE_CONTROL
	lua_register(lState, "setIdleAdd", [](lua_State* l) {
		engine->module<IdleController>().unmock().luaAdd = luaL_checknumber(l, 1);
		return 0;
	});
	lua_register(lState, "setIdleRpm", [](lua_State* l) {
	  auto rpm = luaL_checknumber(l, 1);
    setLinearCurve(config->cltIdleRpm, rpm, rpm, 1);
		return 0;
	});
#endif
	lua_register(lState, "setTimingAdd", [](lua_State* l) {
		engine->ignitionState.luaTimingAdd = luaL_checknumber(l, 1);
		return 0;
	});
#ifndef DISABLE_LUA_SET_TIMING_MULT
	lua_register(lState, "setTimingMult", [](lua_State* l) {
		engine->ignitionState.luaTimingMult = luaL_checknumber(l, 1);
		return 0;
	});
#endif // !defined(DISABLE_LUA_SET_TIMING_MULT)
#ifndef DISABLE_LUA_SET_FUEL_ADD
	lua_register(lState, "setFuelAdd", [](lua_State* l) {
		engine->engineState.lua.fuelAdd = luaL_checknumber(l, 1);
		return 0;
	});
#endif // !defined(DISABLE_LUA_SET_FUEL_ADD)
	lua_register(lState, "setFuelMult", [](lua_State* l) {
		engine->engineState.lua.fuelMult = luaL_checknumber(l, 1);
		return 0;
	});
#if EFI_ELECTRONIC_THROTTLE_BODY && EFI_PROD_CODE
	lua_register(lState, "setEtbAdd", [](lua_State* l) {
		auto luaAdjustment = luaL_checknumber(l, 1);

		setEtbLuaAdjustment(luaAdjustment);

		return 0;
	});
	lua_register(lState, "setEwgAdd", [](lua_State* l) {
		auto luaAdjustment = luaL_checknumber(l, 1);

		setEwgLuaAdjustment(luaAdjustment);

		return 0;
	});
	lua_register(lState, "setEtbDisabled", [](lua_State* l) {
		engine->engineState.lua.luaDisableEtb = lua_toboolean(l, 1);
		return 0;
	});
#endif // EFI_ELECTRONIC_THROTTLE_BODY
#if EFI_PROD_CODE
	lua_register(lState, "setIgnDisabled", [](lua_State* l) {
		engine->engineState.lua.luaIgnCut = lua_toboolean(l, 1);
		return 0;
	});
	lua_register(lState, "setFuelDisabled", [](lua_State* l) {
		engine->engineState.lua.luaFuelCut = lua_toboolean(l, 1);
		return 0;
	});
	lua_register(lState, "setDfcoDisabled", [](lua_State* l) {
		engine->engineState.lua.disableDecelerationFuelCutOff = lua_toboolean(l, 1);
		return 0;
	});
#endif // EFI_PROD_CODE

	lua_register(lState, "setClutchUpState", [](lua_State* l) {
		engine->engineState.lua.clutchUpState = lua_toboolean(l, 1);
		return 0;
	});
	lua_register(lState, "setClutchDownState", [](lua_State* l) {
		engine->engineState.lua.clutchDownState = lua_toboolean(l, 1);
		return 0;
	});
	lua_register(lState, "setBrakePedalState", [](lua_State* l) {
		engine->engineState.lua.brakePedalState = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(lState, "setAcRequestState", [](lua_State* l) {
		engine->engineState.lua.acRequestState = lua_toboolean(l, 1);
		return 0;
	});

#if !defined(STM32F4)
	lua_register(lState, "getTorque", [](lua_State* l) {
		auto rpm = Sensor::getOrZero(SensorType::Rpm);
		auto tps = Sensor::getOrZero(SensorType::Tps1);

 	  // here we assume load is TPS
		auto result = interpolate3d(
                  		config->torqueTable,
                  		config->torqueLoadBins, tps,
                  		config->torqueRpmBins, rpm
                  	);
		lua_pushnumber(l, result);
		return 1;
	});
#endif

	lua_register(lState, "setTorqueReductionState", [](lua_State* l) {
		engine->engineState.lua.torqueReductionState = lua_toboolean(l, 1);
		return 0;
	});

	lua_register(lState, "getCalibration", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		auto result = getConfigValueByName(propertyName);
		lua_pushnumber(l, result);
		return 1;
	});

#if EFI_TUNER_STUDIO && (EFI_PROD_CODE || EFI_SIMULATOR)
	lua_register(lState, "getOutput", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		 // fresh values need to be requested explicitly, there is no periodic invocation of that method
        updateTunerStudioState();
		auto result = getOutputValueByName(propertyName);
		lua_pushnumber(l, result);
		return 1;
	});
#endif // EFI_PROD_CODE || EFI_SIMULATOR

#if EFI_SHAFT_POSITION_INPUT
	lua_register(lState, "getEngineState", [](lua_State* l) {
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
		lua_pushinteger(l, luaStateCode);
		return 1;
	});
#endif //EFI_SHAFT_POSITION_INPUT

	lua_register(lState, "setCalibration", [](lua_State* l) {
		auto propertyName = luaL_checklstring(l, 1, nullptr);
		auto value = luaL_checknumber(l, 2);
		auto incrementVersion = lua_toboolean(l, 3);
		bool isGoodName = setConfigValueByName(propertyName, value);
		if (isGoodName) {
		    efiPrintf("LUA: applying [%s][%f]", propertyName, value);
		} else {
		    efiPrintf("LUA: invalid calibration key [%s]", propertyName);
		}
		if (incrementVersion) {
			incrementGlobalConfigurationVersion("lua");
		}
		return 0;
	});
	lua_register(lState, CMD_BURNCONFIG, [](lua_State*) {
	  requestBurn();
		return 0;
	});

	lua_register(lState, "getGlobalConfigurationVersion", [](lua_State* l) {
		lua_pushinteger(l, engine->getGlobalConfigurationVersion());
		return 1;
	});

	lua_register(lState, "setAcDisabled", [](lua_State* l) {
		auto value = lua_toboolean(l, 1);
		engine->module<AcController>().unmock().isDisabledByLua = value;
		return 0;
	});
	lua_register(lState, "getTimeSinceAcToggleMs", [](lua_State* l) {
		float result = engine->module<AcController>().unmock().timeSinceStateChange.getElapsedSeconds() * 1000;
		lua_pushnumber(l, result);
		return 1;
	});

#if !EFI_UNIT_TEST
	lua_register(lState, "startPwm", lua_startPwm);
	lua_register(lState, "setPwmDuty", lua_setPwmDuty);
	lua_register(lState, "setPwmFreq", lua_setPwmFreq);
	lua_register(lState, "getFan", [](lua_State* l) {
                                 		lua_pushboolean(l, enginePins.fanRelay.getLogicValue());
                                 		return 1;
                                 	});
	lua_register(lState, "getFan2", [](lua_State* l) {
                                 		lua_pushboolean(l, enginePins.fanRelay2.getLogicValue());
                                 		return 1;
                                 	});
	lua_register(lState, "getAcRelay", [](lua_State* l) {
                                 		lua_pushboolean(l, enginePins.acRelay.getLogicValue());
                                 		return 1;
                                 	});
	lua_register(lState, "getDigital", lua_getDigital);
	lua_register(lState, "getAuxDigital", lua_getAuxDigital);
	lua_register(lState, "setDebug", lua_setDebug);
#if EFI_ENGINE_CONTROL
	lua_register(lState, "getAirmass", lua_getAirmass);
	lua_register(lState, "setAirmass", lua_setAirmass);
#endif // EFI_ENGINE_CONTROL


#ifndef WITH_LUA_STOP_ENGINE
#define WITH_LUA_STOP_ENGINE TRUE
#endif

#if WITH_LUA_STOP_ENGINE
	lua_register(lState, "isFirmwareError", [](lua_State* l) {
		lua_pushboolean(l, hasFirmwareError());
		return 1;
	});
#if EFI_SHAFT_POSITION_INPUT
	lua_register(lState, "stopEngine", [](lua_State*) {
		doScheduleStopEngine(StopRequestedReason::Lua);
		return 0;
	});
	lua_register(lState, "isEngineStopRequested", [](lua_State* l) {
		bool result = getLimpManager()->shutdownController.isEngineStop(getTimeNowNt());
		lua_pushboolean(l, result);
		return 1;
	});
	lua_register(lState, "getTimeSinceTriggerEventMs", [](lua_State* l) {
		int result = engine->triggerCentral.m_lastEventTimer.getElapsedUs() / 1000;
		lua_pushinteger(l, result);
		return 1;
	});
#endif // EFI_SHAFT_POSITION_INPUT
#endif // WITH_LUA_STOP_ENGINE

#if EFI_CAN_SUPPORT
	lua_register(lState, "canRxAdd", lua_canRxAdd);
	lua_register(lState, "canRxAddMask", lua_canRxAddMask);
#endif // EFI_CAN_SUPPORT
#endif // not EFI_UNIT_TEST

#if EFI_CAN_SUPPORT || EFI_UNIT_TEST
	lua_register(lState, "txCan", lua_txCan);
#endif

#if EFI_PROD_CODE
  /* todo: hasCriticalReportFile method #7291
	lua_register(lState, "hasCriticalReportFile", [](lua_State*) {
		// todo: actual method to scan SD card for error report files
		lua_pushinteger(l, hasCriticalReportFile());
	  return 1;
  }
*/
#endif // EFI_PROD_CODE

#if EFI_PROD_CODE && HW_HELLEN
	lua_register(lState, "hellenEnablePower", [](lua_State*) {
		hellenEnableEn("Lua");
		return 0;
	});
	lua_register(lState, "hellenDisablePower", [](lua_State*) {
		hellenDisableEn("Lua");
		return 0;
	});
#endif // HW_HELLEN

#if EFI_DAC
	lua_register(lState, "setDacVoltage", [](lua_State* l) {
		auto channel = luaL_checkinteger(l, 1);
		auto voltage = luaL_checknumber(l, 2);
		setDacVoltage(channel, voltage);
		return 0;
	});
#endif // EFI_DAC

    LuaClass<SignalDebounce> luaDebounce(lState, "SignalDebounce");
    luaDebounce
        .ctor<float>()
        .fun("set", &SignalDebounce::set)
        .fun("get", &SignalDebounce::get);

#if EFI_UNIT_TEST
    lua_register(lState, "advanceTimeUs", [](lua_State *l){
        auto us = luaL_checknumber(l, 1);
        advanceTimeUs(us);
        return 0;
    });
#endif // EFI_UNIT_TEST
}
