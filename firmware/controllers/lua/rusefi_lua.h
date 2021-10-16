// This file is not named lua.h as it would collide with firmware/ext/lua/src/lua.h

#pragma once

void startLua();

#if EFI_UNIT_TEST
#include "expected.h"

expected<float> testLuaReturnsNumberOrNil(const char* script);
float testLuaReturnsNumber(const char* script);
int testLuaReturnsInteger(const char* script);
void testLuaExecString(const char* script);
#endif
