/*
** $Id: linit.c,v 1.14.1.1 2007/12/27 13:02:25 roberto Exp $
** Initialization of libraries for lua.c
** See Copyright Notice in lua.h
*/


#define linit_c
#define LUA_LIB

#include "lua.h"

#include "lualib.h"
#include "lauxlib.h"
#include "lrotable.h"

static const luaL_Reg lualibs[] = {
    {"", luaopen_base},
    {LUA_LOADLIBNAME, luaopen_package},
    {LUA_STRLIBNAME, luaopen_string},
#if LUA_OPTIMIZE_MEMORY == 0
    {LUA_MATHLIBNAME, luaopen_math},
    //{LUA_OSLIBNAME, luaopen_os},
    {LUA_TABLIBNAME, luaopen_table},
    {LUA_DBLIBNAME, luaopen_debug},
    {LUA_BITLIBNAME, luaopen_bit},
#endif
    {NULL, NULL}
};

/* The read-only tables are defined here */
extern const luaL_Reg mathlib[];
extern const luaR_value_entry mathlib_vals[];
extern const luaL_Reg syslib[];
extern const luaL_Reg tab_funcs[];
extern const luaL_Reg dblib[];
extern const luaL_Reg co_funcs[];
const luaR_table lua_rotable[] = {
#if LUA_OPTIMIZE_MEMORY > 0
    {LUA_MATHLIBNAME, mathlib, mathlib_vals},
    {LUA_OSLIBNAME, syslib, NULL},
    {LUA_TABLIBNAME, tab_funcs, NULL},
    {LUA_DBLIBNAME, dblib, NULL},
    {LUA_COLIBNAME, co_funcs, NULL},
#endif
    {NULL, NULL, NULL}
};


LUALIB_API void luaL_openlibs (lua_State *L)
{
    const luaL_Reg *lib = lualibs;
    for (; lib->func; lib++) {
        lua_pushcfunction(L, lib->func);
        lua_pushstring(L, lib->name);
        lua_call(L, 1, 0);
    }
}
