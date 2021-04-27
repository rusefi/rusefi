LUA_DIR=$(CONTROLLERS_DIR)/lua
LUA_EXT=$(PROJECT_DIR)/ext/lua

ALLCPPSRC += $(LUA_DIR)/lua.cpp \
			 $(LUA_DIR)/lua_hooks.cpp \

ALLINC += $(LUA_DIR) $(LUA_EXT)/src $(LUA_EXT)/etc
ALLCSRC += 	$(LUA_EXT)/src/lapi.c \
	$(LUA_EXT)/src/lcode.c \
	$(LUA_EXT)/src/lctype.c \
	$(LUA_EXT)/src/ldebug.c \
	$(LUA_EXT)/src/ldo.c \
	$(LUA_EXT)/src/ldump.c \
	$(LUA_EXT)/src/lfunc.c \
	$(LUA_EXT)/src/lgc.c \
	$(LUA_EXT)/src/llex.c \
	$(LUA_EXT)/src/lmem.c \
	$(LUA_EXT)/src/lobject.c \
	$(LUA_EXT)/src/lopcodes.c \
	$(LUA_EXT)/src/lparser.c \
	$(LUA_EXT)/src/lstate.c \
	$(LUA_EXT)/src/lstring.c \
	$(LUA_EXT)/src/ltable.c \
	$(LUA_EXT)/src/ltm.c \
	$(LUA_EXT)/src/lundump.c \
	$(LUA_EXT)/src/lvm.c \
	$(LUA_EXT)/src/lzio.c \
	$(LUA_EXT)/src/lauxlib.c \
	$(LUA_EXT)/src/lbaselib.c \
	$(LUA_EXT)/src/lcorolib.c \
	$(LUA_EXT)/src/ldblib.c \
	$(LUA_EXT)/src/liolib.c \
	$(LUA_EXT)/src/lmathlib.c \
	$(LUA_EXT)/src/loadlib.c \
	$(LUA_EXT)/src/loslib.c \
	$(LUA_EXT)/src/lstrlib.c \
	$(LUA_EXT)/src/ltablib.c \
	$(LUA_EXT)/src/lutf8lib.c \
	$(LUA_EXT)/src/linit.c \
