#include "pch.h"
#include "rusefi_lua.h"


TEST(LuaDebounce, DebounceSignal) {

static const char code[] = R"(
button = SignalDebounce.new(0.1)

function delay(seconds)
    advanceTimeUs(seconds*1000000)
end

function testFunc()
    -- fast changes should be ignored
    local startVal = button:get()
    for i=0,10 do
        button:set(i % 2 == 1)
        if button:get() ~= startVal then
            return 1
        end
        delay(0.05)
    end

    -- slow changes - should be ok
    button:set(true)
    delay(0.15)
    button:set(true)
    if not button:get() then
        return 2
    end

    -- release button
    button:set(false)
    delay(0.15)
    button:set(false)
    if button:get() then
        return 3
    end

    return 0
end

)";

    EXPECT_EQ(testLuaReturnsInteger(code), 0);
}
