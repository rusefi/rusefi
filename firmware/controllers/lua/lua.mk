LUA_DIR=$(CONTROLLERS_DIR)/lua
LUA_EXT=$(PROJECT_DIR)/ext/lua

ALLCPPSRC += $(LUA_DIR)/lua.cpp \
			 $(LUA_DIR)/lua_hooks.cpp \

ALLINC += $(LUA_DIR) $(LUA_DIR)/luaaa $(LUA_EXT)
ALLCSRC += \
	$(LUA_EXT)/lapi.c \
	$(LUA_EXT)/lcode.c \
	$(LUA_EXT)/lctype.c \
	$(LUA_EXT)/ldebug.c \
	$(LUA_EXT)/ldo.c \
	$(LUA_EXT)/ldump.c \
	$(LUA_EXT)/lfunc.c \
	$(LUA_EXT)/lgc.c \
	$(LUA_EXT)/llex.c \
	$(LUA_EXT)/lmem.c \
	$(LUA_EXT)/lobject.c \
	$(LUA_EXT)/lopcodes.c \
	$(LUA_EXT)/lparser.c \
	$(LUA_EXT)/lstate.c \
	$(LUA_EXT)/lstring.c \
	$(LUA_EXT)/ltable.c \
	$(LUA_EXT)/ltm.c \
	$(LUA_EXT)/lundump.c \
	$(LUA_EXT)/lvm.c \
	$(LUA_EXT)/lzio.c \
	$(LUA_EXT)/lauxlib.c \
	$(LUA_EXT)/lbaselib.c \
	$(LUA_EXT)/lcorolib.c \
	$(LUA_EXT)/ldblib.c \
	$(LUA_EXT)/liolib.c \
	$(LUA_EXT)/lmathlib.c \
	$(LUA_EXT)/loadlib.c \
	$(LUA_EXT)/lstrlib.c \
	$(LUA_EXT)/ltablib.c \
	$(LUA_EXT)/lutf8lib.c \
	$(LUA_EXT)/linit.c \
