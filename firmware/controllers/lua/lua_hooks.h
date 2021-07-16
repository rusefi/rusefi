#pragma once

struct lua_State;
void configureRusefiLuaHooks(lua_State*);
void luaDeInitPins();

struct AirmassModelBase;
AirmassModelBase& getLuaAirmassModel();
