#include "pch.h"

#include "rusefi_lua.h"
#include "thread_controller.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_hooks.h"

#if EFI_PROD_CODE || EFI_SIMULATOR

#ifndef LUA_USER_HEAP
// At least one heap_header_t should fit
// see [tag:multi-step-lua-alloc] below
// this is a bit over-complicated at the moment, one argument would be that this supports multi-region RAM use-case
#define LUA_USER_HEAP 16
#endif // LUA_USER_HEAP

//#ifdef PERSISTENT_LOCATION_TODO
//#define LUA_HEAP_RAM_SECTION CCM_OPTIONAL
//#endif

CH_HEAP_AREA(luaUserHeap, LUA_USER_HEAP)
#ifdef EFI_HAS_EXT_SDRAM
SDRAM_OPTIONAL
#endif
#ifdef LUA_HEAP_RAM_SECTION
LUA_HEAP_RAM_SECTION
#endif
;

class Heap {
public:
	memory_heap_t m_heap;

	size_t m_size = 0;
	uint8_t* m_buffer = nullptr;

	void* alloc(size_t n) {
		if (m_buffer && m_size) {
			return chHeapAlloc(&m_heap, n);
		}

		return nullptr;
	}

	void free(void* obj) {
		chHeapFree(obj);
	}

public:
	template<size_t TSize>
	Heap(uint8_t (&buffer)[TSize])
	{
		reinit(buffer, TSize);
	}

	void reinit(uint8_t *buffer, size_t size) {
		criticalAssertVoid(used() == 0, "Too late to reinit Lua heap");

		m_size = size;
		m_buffer = buffer;

		reset();
	}

	void* realloc(void* ptr, size_t osize, size_t nsize) {
		if (nsize == 0) {
			// requested size is zero, free if necessary and return nullptr
			if (ptr) {
				free(ptr);
			}

			return nullptr;
		}

		void *new_mem = nullptr;

		// try this heap first
		new_mem = alloc(nsize);

    // [tag:multi-step-lua-alloc]
		// then try ChibiOS default heap
		if (new_mem == nullptr) {
			new_mem = chHeapAlloc(NULL, nsize);
		}

		if (!ptr) {
			// No old pointer passed in, simply return allocated block
			return new_mem;
		}

		// An old pointer was passed in, copy the old data in, then free
		if (new_mem != nullptr) {
			memcpy(new_mem, ptr, chHeapGetSize(ptr) > nsize ? nsize : chHeapGetSize(ptr));
			free(ptr);
		}

		return new_mem;
	}

	size_t size() const {
		return m_size;
	}

	size_t used() {
		if (size() == 0) {
			return 0;
		}

		size_t heapFree = 0;
		size_t lagestFree = 0;
		chHeapStatus(&m_heap, &heapFree, &lagestFree);
		// hack to return zero when heap is totaly free
		// this is for leak detector
		// if all free memory is in one chunk this means heap is totaly free
		if (heapFree == lagestFree) {
			return 0;
		}
		return m_size - heapFree;
	}

	// Use only in case of emergency - obliterates all heap objects and starts over
	void reset() {
		if (m_buffer && m_size) {
			chHeapObjectInit(&m_heap, m_buffer, m_size);
		}
	}
};

static Heap userHeap(luaUserHeap);

void* luaHeapAlloc(void* /*ud*/, void* optr, size_t osize, size_t nsize) {
	void *nptr = userHeap.realloc(optr, osize, nsize);

	if (engineConfiguration->debugMode == DBG_LUA) {
		engine->outputChannels.debugIntField1 = userHeap.used();
	}

	return nptr;
}

size_t luaHeapUsed()
{
	return userHeap.used();
}

void luaHeapReset()
{
	userHeap.reset();
}

void luaHeapPrintInfo() {
	auto heapSize = userHeap.size();

	if (heapSize) {
		auto memoryUsed = userHeap.used();
		float pct = 100.0f * memoryUsed / heapSize;
		efiPrintf("Dedicated Lua memory heap usage: %d / %d bytes = %.1f%%", memoryUsed, heapSize, pct);
	} else {
		efiPrintf("No dedicated Lua heap, using ChibiOS default heap");
	}

	size_t chHeapFree = 0;
	chHeapStatus(NULL, &chHeapFree, NULL);
	efiPrintf("Common ChibiOS heap: %d bytes free", chHeapFree);
	efiPrintf("ChibiOS memcore: %d bytes free", chCoreGetStatusX());
}

#else // not EFI_PROD_CODE
// Non-MCU code can use plain realloc function instead of custom implementation
void* myAlloc(void* /*ud*/, void* ptr, size_t /*osize*/, size_t nsize) {
	if (!nsize) {
		free(ptr);
		return nullptr;
	}

	if (!ptr) {
		return malloc(nsize);
	}

	return realloc(ptr, nsize);
}
#endif // EFI_PROD_CODE

#endif // EFI_LUA
