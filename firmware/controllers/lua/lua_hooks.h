#pragma once

struct lua_State;
void configureRusefiLuaHooks(lua_State*);

struct AirmassModelBase;
AirmassModelBase& getLuaAirmassModel();
