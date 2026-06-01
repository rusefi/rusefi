-- Cruise Control + Check Engine LED
--
-- Hardware:
--   LED          : PWM output 0, 50 Hz
--   Buttons      : AuxAnalog 0, resistor-ladder
--     Set        : 0.75-1.40 V   -- engage / disengage cruise 4x4.7k in parallel
--     Decrease   : 1.80-2.70 V   -- target RPM - RPM_STEP 2x4.7k in parallel
--     Increase   : 2.90-3.55 V   -- target RPM + RPM_STEP 1x4.7k 
--     No button  : all other voltages
--   Brake pedal  : getOutput("brakePedalState")
--
-- LED priority (highest first):
--   1. Fault (CLT > 100)        -- blink 1 s period, 100 %
--   2. Engine stopped (RPM = 0) -- 100 % steady
--   3. Cruise active            -- 50 % steady
--   4. Normal run               -- off

-- ========= tuneable constants =========
local CRUISE_MIN_RPM   = 1200  -- minimum RPM to allow engagement
local CRUISE_MAX_RPM   = 4000  -- RPM above this forces disengagement
local RPM_STEP         = 100   -- RPM per increase / decrease press
local CLT_FAULT_TEMP   = 100   -- degrees C for fault LED blink
local PID_KP           = 0.02  -- %ETB per RPM of error
local PID_KI           = 0.003 -- integrator gain
local PID_MIN          = 0.0   -- don't wont to decrease ETB below baseline
local PID_MAX          = 50.0  -- limit max ETB increase to avoid big throttle jumps
local BLINK_HALF       = 0.5   -- fault blink half-period [s]
local DEBOUNCE         = 20    -- ticks of stable reading (~100 ms at 200 Hz)
-- ======================================

-- button state constants
local BTN_NONE     = 0
local BTN_SET      = 1
local BTN_DECREASE = 2
local BTN_INCREASE = 3

startPwm(0, 50, 0)  -- LED: PWM output 0, 50 Hz, off

local pid = Pid.new(PID_KP, PID_KI, 0, PID_MIN, PID_MAX)

local cruiseActive = false
local targetRpm    = 0

-- debounce state
local btnCandidate = BTN_NONE
local btnCount     = 0
local btnStable    = BTN_NONE
local btnPrev      = BTN_NONE

-- LED blink state
local blinkTimer = Timer.new()
local blinkOn    = false

-- ---- helpers ----

local function readBtn()
    local v = getAuxAnalog(0)
    if v == nil                then return BTN_NONE     end
    if v >= 0.75 and v <= 1.40 then return BTN_SET      end
    if v >= 1.80 and v <= 2.70 then return BTN_DECREASE end
    if v >= 2.90 and v <= 3.55 then return BTN_INCREASE end
    return BTN_NONE
end

local function disengageCruise()
    cruiseActive = false
    pid:reset()
    setEtbAdd(0)
    setLuaGauge(1, 0)
    setLuaGauge(2, 0)
    print("Cruise OFF")
end

-- ---- main loop ----

function onTick()
    local rpm = getSensor("RPM") or 0

    -- ---- button debounce ----
    local raw = readBtn()
    if raw == btnCandidate then
        btnCount = btnCount + 1
        if btnCount >= DEBOUNCE then btnStable = btnCandidate end
    else
        btnCandidate = raw
        btnCount     = 1
    end

    -- ---- single-fire edge detection (none -> button) ----
    if btnStable ~= BTN_NONE and btnPrev == BTN_NONE then
        if btnStable == BTN_SET then
            if not cruiseActive then
                if rpm >= CRUISE_MIN_RPM then
                    cruiseActive = true
                    targetRpm    = rpm
                    pid:reset()
                    print("Cruise ON target=" .. math.floor(rpm) .. " RPM")
                else
                    print("Cruise: RPM too low (" .. math.floor(rpm) .. ")")
                end
            else
                disengageCruise()
            end
        elseif cruiseActive then
            if btnStable == BTN_INCREASE then
                targetRpm = targetRpm + RPM_STEP
            elseif btnStable == BTN_DECREASE then
                targetRpm = math.max(CRUISE_MIN_RPM, targetRpm - RPM_STEP)
            end
            pid:reset()
            print("Cruise target=" .. math.floor(targetRpm) .. " RPM")
        end
    end
    btnPrev = btnStable

    -- ---- auto-disengage ----
    if cruiseActive then
        local brake = getOutput("brakePedalState")
        if (brake ~= nil and brake > 0) then
            print("Cruise OFF: brake")
            disengageCruise()
        elseif rpm > CRUISE_MAX_RPM then
            print("Cruise OFF: RPM " .. math.floor(rpm))
            disengageCruise()
        end
    end

    -- ---- ETB PI control ----
    if cruiseActive then
        local adj = pid:get(targetRpm, rpm)
        setEtbAdd(adj)
        setLuaGauge(1, adj)
        setLuaGauge(2, targetRpm)
    else
        setEtbAdd(0)
    end

    -- ---- LED ----
    local clt = getSensor("Clt")
    local fault = clt ~= nil and clt > CLT_FAULT_TEMP
    local stopped = rpm == 0

    local duty
    if fault then
        if blinkTimer:getElapsedSeconds() >= BLINK_HALF then
            blinkOn = not blinkOn
            blinkTimer:reset()
        end
        duty = blinkOn and 1.0 or 0.0
    elseif stopped then
        blinkOn = false ; blinkTimer:reset()
        duty = 1.0
    elseif cruiseActive then
        blinkOn = false ; blinkTimer:reset()
        duty = 0.5
    else
        blinkOn = false ; blinkTimer:reset()
        duty = 0.0
    end

    setPwmDuty(0, duty)
end
