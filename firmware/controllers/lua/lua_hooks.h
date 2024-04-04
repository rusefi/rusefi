#pragma once

struct lua_State;
void configureRusefiLuaHooks(lua_State*);
void luaDeInitPins();

struct AirmassModelBase;
AirmassModelBase& getLuaAirmassModel();
bool getAuxDigital(int index);

struct LuaOverrideSensor final : public Sensor {
  LuaOverrideSensor(SensorType type, SensorType underlyingType) : Sensor(type) {
    m_underlyingType = underlyingType;
    reset();
  }

  void reset() {
    overrideValue = -1;
  }

  void setOverrideValue(float value) {
    overrideValue = value;
  }

	SensorResult get() const final override {
	  if (overrideValue < 0)
	    return Sensor::get(m_underlyingType);
	  return overrideValue;
	}

	void showInfo(const char* sensorName) const override {
		efiPrintf("LuaOverrideSensor \"%s\": override value %.2f", sensorName, overrideValue);
	}

  float overrideValue = -1;
  SensorType m_underlyingType;
};

void startPwm(int index, float freq, float duty);
void setPwmDuty(int index, float duty);
