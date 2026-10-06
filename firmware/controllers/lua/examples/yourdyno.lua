-- scriptname yourdyno.lua
--
-- until YourDyno supports rusEFI or we learn how to add DBC into YourDyno, we pretend to be another ECU
-- this might work
--
-- rusEFI -> YourDyno CAN telemetry.
-- Configure the selected CAN bus for 500 kbit/s and enable transmission in
-- the tune. Define YourDyno CAN inputs using the layout and scaling below.
-- https://yourdyno.com/YourDyno-Software-User-Manual.pdf (section 8.10)
--
-- Standard 11-bit IDs, 8-byte payloads, little-endian signed 16-bit values:
-- ID     Hz   Words at byte offsets 0 / 2 / 4 / 6 (physical units)
-- 0x520  50   RPM / throttle % / MAP kPa / average lambda
-- 0x521  50   lambda 1 / lambda 2 / corrected ignition deg BTDC / unused
-- 0x522  50   last injection ms / injector duty % / unused / speed km/h
-- 0x530  10   battery V / baro kPa / intake temp C / coolant temp C
-- 0x536  10   detected gear / boost solenoid duty % / oil kPa / oil temp C
-- Decode as raw * factor, with zero offset: RPM/gear factor 1, lambda 0.001,
-- injection time/battery voltage 0.01, all other mapped fields 0.1.
--
-- Missing/invalid readings are sent as ZERO, not held at their previous value.
-- The protocol has no validity bits for these fields: only log configured,
-- working sensors. Lambda average uses both valid banks, or the available bank.
-- Ignition/fuel cut percentages are not mapped (0x521 word 4 / 0x522 word 3
-- remain zero); do not log them.
-- getOutput lookups need firmware with EFI_LUA_LOOKUP; otherwise those fields
-- are zero. Ignition is correctedIgnitionAdvance, before per-cylinder trims.
-- Enable signed decoding for ignition and temperatures, including when
-- importing channel definitions. Avoid other transmitters using these CAN IDs.
-- Nominal rates assume an ACKing, terminated bus; failed txCan can delay ticks.

local CAN_BUS = 1 -- rusEFI uses 1-based bus numbers; change to 2 if needed.
setTickRate(50)

local frame = {0, 0, 0, 0, 0, 0, 0, 0}
local slowTick = 0

local function validValue(value)
    if value == nil or value ~= value or value == math.huge or value == -math.huge then
        return nil
    end
    return value
end

local function sensor(name)
    return validValue(getSensor(name))
end

local function output(name)
    if getOutput == nil then
        return nil
    end
    local value = validValue(getOutput(name))
    -- Unknown/disabled output lookup returns EFI_ERROR_CODE (uint32 max,
    -- rounded to 2^32 by the ECU's float32 Lua build), not nil.
    if value ~= nil and value >= 4294967295.0 then
        return nil
    end
    return value
end

local function setWord(byteOffset, value, multiplier)
    local raw = (validValue(value) or 0) * multiplier
    -- Saturate before converting to an integer: out-of-range readings must
    -- not wrap into plausible values. Round negative values symmetrically.
    raw = math.max(-32768, math.min(32767, raw))
    if raw < 0 then
        raw = math.ceil(raw - 0.5)
    else
        raw = math.floor(raw + 0.5)
    end
    frame[byteOffset + 1] = raw & 0xff
    frame[byteOffset + 2] = (raw >> 8) & 0xff
end

function onTick()
    local lambda1 = sensor("Lambda1")
    local lambda2 = sensor("Lambda2")
    local lambdaAverage = lambda1 or lambda2
    if lambda1 ~= nil and lambda2 ~= nil then
        lambdaAverage = (lambda1 + lambda2) / 2
    end

    setWord(0, sensor("Rpm"), 1)
    setWord(2, sensor("DriverThrottleIntent") or sensor("Tps1"), 10)
    setWord(4, sensor("Map"), 10)
    setWord(6, lambdaAverage, 1000)
    txCan(CAN_BUS, 0x520, 0, frame)

    setWord(0, lambda1, 1000)
    setWord(2, lambda2, 1000)
    setWord(4, output("correctedIgnitionAdvance"), 10)
    setWord(6, 0, 1) -- ignition cut percentage unavailable
    txCan(CAN_BUS, 0x521, 0, frame)

    setWord(0, output("actualLastInjection"), 100)
    setWord(2, output("injectorDutyCycle"), 10)
    setWord(4, 0, 1) -- fuel cut percentage unavailable
    setWord(6, sensor("VehicleSpeed"), 10)
    txCan(CAN_BUS, 0x522, 0, frame)

    -- Spread the two slow frames over different ticks (10 Hz each).
    if slowTick == 0 then
        setWord(0, sensor("BatteryVoltage"), 100)
        setWord(2, sensor("BarometricPressure"), 10)
        setWord(4, sensor("Iat"), 10)
        setWord(6, sensor("Clt"), 10)
        txCan(CAN_BUS, 0x530, 0, frame)
    elseif slowTick == 1 then
        setWord(0, sensor("DetectedGear"), 1)
        setWord(2, output("boostOutput"), 10)
        setWord(4, sensor("OilPressure"), 10)
        setWord(6, sensor("OilTemperature"), 10)
        txCan(CAN_BUS, 0x536, 0, frame)
    end
    slowTick = (slowTick + 1) % 5
end
