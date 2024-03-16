#pragma once

struct lua_State;
void configureRusefiLuaHooks(lua_State*);
void luaDeInitPins();

struct AirmassModelBase;
AirmassModelBase& getLuaAirmassModel();
bool getAuxDigital(int index);

struct LuaDashOverride {
  // this helps with all kinds of cool tachometer sweeps!
  int16_t rpm = -1;
  int16_t coolant = -1;
  float vbatt = -1;
};
