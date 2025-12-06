#include "pch.h"

#include "rusefi_lua.h"

#if EFI_LUA

#include "lua.hpp"
#include "lua_heap.h"

#if EFI_PROD_CODE || EFI_SIMULATOR

#ifndef MCU_HAS_CCM_RAM
	#define MCU_HAS_CCM_RAM FALSE
#endif

#ifndef LUA_EXTRA_HEAP
	#define LUA_EXTRA_HEAP 0
#endif

#if (LUA_EXTRA_HEAP > 0)
CH_HEAP_AREA(luaExtraHeapArea, LUA_EXTRA_HEAP)
	#ifdef EFI_HAS_EXT_SDRAM
		SDRAM_OPTIONAL
	#endif
	;
#endif

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
		init(buffer, TSize);
	}

	Heap()
	{
		init(nullptr, 0);
	}

	void init(uint8_t *buffer, size_t size) {
		criticalAssertVoid(used() == 0, "Too late to init Lua heap: already in use");

		m_size = size;
		m_buffer = buffer;

		reset();
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

		return m_size - heapFree;
	}

	// Use only in case of emergency - obliterates all heap objects and starts over
	void reset() {
		if (m_buffer && m_size) {
			chHeapObjectInit(&m_heap, m_buffer, m_size);
		}
	}
};

// see [tag:multi-step-lua-alloc] below
// this is a bit over-complicated at the moment, one argument would be that this supports multi-region RAM use-case
#if (LUA_EXTRA_HEAP > 0)
// Optional SDRAM
static Heap luaExtraHeap(luaExtraHeapArea);
#endif

#if defined(STM32F4)
// Optional RAM3 exist on STM32F42x
static Heap luaOptionalHeap;
#endif

#if MCU_HAS_CCM_RAM
// CCM RAM leftovers on STM32F4xx
static Heap luaCcmHeap;
#endif

void luaHeapInit()
{
	// stm32f4xx can have optional ram3 region
#if defined(STM32F4)
	// Some boads can be equiped with STM32F42x only, in this case we allow linker to take care of ram3
#if !defined(EFI_IS_F42x)
	// cute hack: let's check at runtime if you are a lucky owner of board with extra RAM and use that extra RAM for extra Lua
	// we need this on microRusEFI for sure
	// definitely should NOT have this on Proteus
	// on Hellen a bit of open question what's the best track
	if (isStm32F42x()) {
		// This is safe to use section base and end as we define ram3 for all F4 chips
		extern uint8_t __ram3_base__[];
		extern uint8_t __ram3_end__[];
		luaOptionalHeap.init(__ram3_base__, __ram3_end__ - __ram3_base__);
	}
#endif // !EFI_IS_F42x
#endif // STM32F4

	// stm32f4xx have CCM memory that may have some leftovers
#if MCU_HAS_CCM_RAM
	extern uint8_t __heap_ccm_base__[];
	extern uint8_t __heap_ccm_end__[];
	luaCcmHeap.init(__heap_ccm_base__, __heap_ccm_end__ - __heap_ccm_base__);
#endif
}

static size_t luaMemoryUsed = 0;

void* luaHeapAlloc(void* /*ud*/, void* optr, size_t osize, size_t nsize) {
    // If new size is zero, this is a free. Do not allocate.
    if (nsize == 0) {
        if (optr) {
            size_t oldSize = chHeapGetSize(optr);
            chHeapFree(optr);
            luaMemoryUsed -= oldSize;
        }
        return nullptr;
    }

    void *nptr = nullptr;

	// [tag:multi-step-lua-alloc]
	// First try dedicated Lua heap(s)
#if (LUA_EXTRA_HEAP > 0)
	if (nptr == nullptr) {
		nptr = luaExtraHeap.alloc(nsize);
	}
#endif
#if defined(STM32F4)
	if (nptr == nullptr) {
		nptr = luaOptionalHeap.alloc(nsize);
	}
#endif
#if MCU_HAS_CCM_RAM
	if (nptr == nullptr) {
		nptr = luaCcmHeap.alloc(nsize);
	}
#endif

    // [tag:multi-step-lua-alloc]
    // then try ChibiOS default heap
    if (nptr == nullptr) {
        nptr = chHeapAlloc(NULL, nsize);
    }

    size_t newSize = 0;
    if (nptr) {
        // Account for newly allocated memory by actual block size
        newSize = chHeapGetSize(nptr);
        chDbgAssert(newSize >= nsize, "Lua allocator returned smaller block than requested");
        luaMemoryUsed += newSize;
    }

    if (optr) {
        // An old pointer was passed in. Only free it if we successfully allocated a new one.
        size_t oldSize = chHeapGetSize(optr);
        if (nptr != nullptr) {
            // Copy the minimum of old and new block sizes
            size_t copySize = (oldSize < newSize) ? oldSize : newSize;
            memcpy(nptr, optr, copySize);
            // chHeapFree will find correct heap to return memory to
            chHeapFree(optr);
            luaMemoryUsed -= oldSize;
        } else {
            if (nsize <= oldSize) {
                return optr; // shrink must not fail per Lua's assumption
            }
            return nullptr; // grow failed
        }
    }

	if (engineConfiguration->debugMode == DBG_LUA) {
		engine->outputChannels.debugIntField1 = luaHeapUsed();
	}

	return nptr;
}

size_t luaHeapUsed()
{
	return luaMemoryUsed;
}

void luaHeapReset()
{
#if (LUA_EXTRA_HEAP > 0)
	luaExtraHeap.reset();
#endif
#if defined(STM32F4)
	luaOptionalHeap.reset();
#endif
#if MCU_HAS_CCM_RAM
	luaCcmHeap.reset();
#endif

	luaMemoryUsed = 0;
}

#if CH_CFG_MEMCORE_SIZE == 0
	extern uint8_t __heap_base__[];
	extern uint8_t __heap_end__[];
#endif

void luaHeapPrintInfo() {
	size_t chMemTotal =
		/* Chibios heap size */
		#if CH_CFG_MEMCORE_SIZE == 0
			(__heap_end__ - __heap_base__);
		#else
			CH_CFG_MEMCORE_SIZE;
		#endif
	auto totalHeapSize =
		chMemTotal +
	#if (LUA_EXTRA_HEAP > 0)
		luaExtraHeap.size() +
	#endif
	#if defined(STM32F4)
		luaOptionalHeap.size() +
	#endif
	#if MCU_HAS_CCM_RAM
		luaCcmHeap.size() +
	#endif
		0;

	if (totalHeapSize) {
		auto memoryUsed = luaHeapUsed();
		float pct = 100.0f * memoryUsed / totalHeapSize;
		efiPrintf("Lua total heap(s) usage: %d / %d bytes = %.1f%%", memoryUsed, totalHeapSize, pct);
	} else {
		efiPrintf("No heap available for Lua");
	}

	#if (LUA_EXTRA_HEAP > 0)
		efiPrintf("Lua extra heap usage: %d / %d", luaExtraHeap.used(), luaExtraHeap.size());
	#endif
	#if defined(STM32F4)
		efiPrintf("Lua optional heap usage: %d / %d", luaOptionalHeap.used(), luaOptionalHeap.size());
	#endif
	#if MCU_HAS_CCM_RAM
		efiPrintf("Lua CCM heap usage: %d / %d", luaCcmHeap.used(), luaCcmHeap.size());
	#endif

	size_t chHeapFree = 0;
	chHeapStatus(NULL, &chHeapFree, NULL);
	/* total available for ChibiOS minus left free, plus free in Chibios Heap */
	size_t chMemCoreUsed = chMemTotal - chCoreGetStatusX() - chHeapFree;
	efiPrintf("Common ChibiOS heap: %d bytes free", chHeapFree);
	efiPrintf("ChibiOS memcore usage: %d / %d", chMemCoreUsed, chMemTotal);
}

#else // not EFI_PROD_CODE
// Non-MCU code can use plain realloc function instead of custom implementation
void* luaHeapAlloc(void* /*ud*/, void* ptr, size_t /*osize*/, size_t nsize) {
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
