// file lua_heap.h

#pragma once

void luaHeapInit();
void* luaHeapAlloc(void* /*ud*/, void* optr, size_t osize, size_t nsize);
size_t luaHeapUsed();
void luaHeapReset();

void luaHeapPrintInfo();
