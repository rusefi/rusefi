setTickRate(100)

local initialTimer = Timer.new()
initialTimer:reset()
local _10msPeriodTimer = Timer.new()
_10msPeriodTimer:reset()
local _40msPeriodTimer = Timer.new()
_40msPeriodTimer:reset()

local pps = 0
local ac_active_flag = false 

local global_0x358_byte1_status = 0 

local pnIdleAdderValue = findSetting("pnIdleAdder", 5)
print("Idle Adder Value for Not P/N: " .. pnIdleAdderValue .. "%")

local currentShifterStateIsPN = true 
local prevShifterStateIsPN_forIdleAdd = nil 

local counter_23D_B0_cycle = {0x03, 0x23, 0x43, 0x63}
local counter_23D_B0_idx = 1
local counter_23D_B5_AC = 0x0A

local counter_231_B1 = 0

local counter_23E_B7_values = {0x92,0x95,0x93,0x94,0x91}
local counter_23E_B7_idx = 1

local counter_794_B2 = 0

local payload_0x231 = {0xFC, 0x10, 0x00, 0xFC, 0xFC, 0xBF, 0x4D, 0x9E}
local payload_0x233 = {0x00, 0x8E, 0x00, 0x00, 0x02}
local payload_0x23D = {0x03, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x8B}
local payload_0x23E = {0x00, 0x00, 0xF3, 0xC2, 0x01, 0xCF, 0x0C, 0x92}
local payload_0x551 = {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80}
local payload_0x794 = {0x00, 0x00, 0x00}

function generate_and_send_0x1F9()
    local fanState = getFan()
    local fanState2 = getFan2()

    local fanPayloadOff_Data = { 0x00, 0x00 }
    local fanPayloadLo_Data  = { 0x40, 0x00 }
    local fanPayloadHi_Data  = { 0x60, 0x00 } 

    local selectedFanPayloadData = {fanPayloadOff_Data[1], fanPayloadOff_Data[2]} 

    if fanState2 then
        selectedFanPayloadData[1] = fanPayloadHi_Data[1]
        selectedFanPayloadData[2] = fanPayloadHi_Data[2]
    elseif fanState then
        selectedFanPayloadData[1] = fanPayloadLo_Data[1]
        selectedFanPayloadData[2] = fanPayloadLo_Data[2]
    end

    if ac_active_flag then
        selectedFanPayloadData[1] = selectedFanPayloadData[1] | 0x08 
    end

    txCan(1, 0x1F9, 0, selectedFanPayloadData)
end

function generate_and_send_0x231()
    payload_0x231[1] = 0xFC
    counter_231_B1 = (counter_231_B1 + 1) % 4
    payload_0x231[2] = counter_231_B1 * 0x10

    local pps_val_b2 = math.floor(pps / 0.5)
    payload_0x231[3] = math.min(pps_val_b2, 0xFF)

    payload_0x231[4] = 0xFC
    payload_0x231[5] = 0xFC
    payload_0x231[6] = 0xBF
    payload_0x231[7] = 0x4D
    payload_0x231[8] = 0x9E
    txCan(1, 0x231, 0, payload_0x231)
end

function generate_and_send_0x233()
    payload_0x233[1] = 0x00
    payload_0x233[2] = 0x8E 
    payload_0x233[3] = 0x00
    payload_0x233[4] = 0x00
    if ac_active_flag then
        payload_0x233[5] = 0xFD
    else
        payload_0x233[5] = 0x02
    end
    txCan(1, 0x233, 0, payload_0x233)
end

function generate_and_send_0x23D()
    local RPMread_scaled = (getSensor("RPM") or 0) / 3.15
    local CLTread = math.floor(getSensor("CLT") or 0 + 0.5)
    local acRelayEngagedByEnv = getAcRelay() == true
    local ecu_sends_ac_load_signal = (ac_active_flag and acRelayEngagedByEnv)
    
    -- Keep track of this state for internal logic, print removed from here
    local prev_ecu_sends_ac_load_signal_local = (payload_0x23D[6] ~= 0x00) -- More direct way to check previous state

    payload_0x23D[1] = counter_23D_B0_cycle[counter_23D_B0_idx]
    counter_23D_B0_idx = counter_23D_B0_idx + 1
    if counter_23D_B0_idx > #counter_23D_B0_cycle then counter_23D_B0_idx = 1 end

    if pps > 0 then
         local pps_byte1_val = math.floor(pps / 0.392)
         payload_0x23D[2] = math.min(pps_byte1_val, 0xFF)
    else
        payload_0x23D[2] = 0x00
    end

    payload_0x23D[3] = 0x0C

    local int_rpm = math.floor(RPMread_scaled)
    payload_0x23D[4] = int_rpm & 0xFF
    payload_0x23D[5] = (int_rpm >> 8) & 0xFF

    if ecu_sends_ac_load_signal then
        if not prev_ecu_sends_ac_load_signal_local then
            counter_23D_B5_AC = 0x0A 
        end
        payload_0x23D[6] = counter_23D_B5_AC
        counter_23D_B5_AC = (counter_23D_B5_AC + 0x0A) 
        if counter_23D_B5_AC > 0xFA then counter_23D_B5_AC = 0xFA end 
    else
        payload_0x23D[6] = 0x00
        counter_23D_B5_AC = 0x0A 
    end

    payload_0x23D[7] = 0x00

    local cltGauge_val = 0
    if CLTread < 120 then
        cltGauge_val = math.floor(CLTread * 1.35 + 0.5)
    else
        cltGauge_val = 0xF0
    end
    payload_0x23D[8] = math.min(cltGauge_val, 0xFF)

    txCan(1, 0x23D, 0, payload_0x23D)
end

function generate_and_send_0x23E()
    payload_0x23E[1] = 0x00
    payload_0x23E[2] = 0x00

    local val_b2 = math.floor((100 - pps) / 0.392)
    payload_0x23E[3] = math.max(0, math.min(val_b2, 0xFF))

    payload_0x23E[4] = 0xC2
    payload_0x23E[5] = 0x01
    payload_0x23E[6] = 0xCF

    local val_b6 = math.floor(pps / 0.392)
    payload_0x23E[7] = math.max(0, math.min(val_b6, 0xFF))

    payload_0x23E[8] = counter_23E_B7_values[counter_23E_B7_idx]
    counter_23E_B7_idx = counter_23E_B7_idx + 1
    if counter_23E_B7_idx > #counter_23E_B7_values then counter_23E_B7_idx = 1 end

    txCan(1, 0x23E, 0, payload_0x23E)
end

function generate_and_send_0x551()
    if ac_active_flag then
        payload_0x551[1] = 0x80
    else
        payload_0x551[1] = 0x81
    end
    payload_0x551[2] = 0x00
    payload_0x551[3] = 0x00
    payload_0x551[4] = 0x00
    payload_0x551[5] = 0x00
    payload_0x551[6] = 0x00
    payload_0x551[7] = 0x80
    txCan(1, 0x551, 0, payload_0x551)
end

function generate_and_send_0x794()
    payload_0x794[1] = 0x00
    payload_0x794[2] = 0x00
    payload_0x794[3] = counter_794_B2
    counter_794_B2 = (counter_794_B2 + 1) % 4
    txCan(1, 0x794, 0, payload_0x794)
end

function onTick()
    pps = getSensor("AcceleratorPedal") or 0
    pps = (pps == nil and 0 or pps)

    if _10msPeriodTimer:getElapsedSeconds() >= 0.01 then
        _10msPeriodTimer:reset()

        generate_and_send_0x1F9()
        generate_and_send_0x231()
        generate_and_send_0x233()
        generate_and_send_0x23D()
        generate_and_send_0x23E()
        generate_and_send_0x551()
    end

    if _40msPeriodTimer:getElapsedSeconds() >= 0.04 then
        _40msPeriodTimer:reset()
        generate_and_send_0x794()
    end
end

function onCanRxAc(bus, id, dlc, data)
    if id == 0x35D then
        local requested_ac_state = (data[1] == 0xC1)
        if requested_ac_state ~= ac_active_flag then
            ac_active_flag = requested_ac_state
        end
        setAcRequestState(ac_active_flag) 
    end
end

canRxAdd(0x35D, onCanRxAc)

function onCanRxMonitor0x358(bus, id, dlc, data)
    if id == 0x358 then
        if dlc >= 2 then
            global_0x358_byte1_status = data[2]
        end
    end
end
canRxAdd(0x358, onCanRxMonitor0x358)

function onCanRxShifterState(bus, id, dlc, data)
    if id == 0x2DE then
        if dlc >= 1 then
            local newShifterStateIsPN_local
            if data[1] == 0x82 then
                newShifterStateIsPN_local = true 
            elseif data[1] == 0x02 then
                newShifterStateIsPN_local = false 
            else
                return 
            end

            if newShifterStateIsPN_local ~= prevShifterStateIsPN_forIdleAdd then
                currentShifterStateIsPN = newShifterStateIsPN_local
                if not currentShifterStateIsPN then 
                    setIdleAdd(pnIdleAdderValue)
                    print("Shifter State: NOT Park/Neutral (0x2DE Byte 0 = " .. data[1] .. "). Adding " .. pnIdleAdderValue .. "% to idle.")
                else 
                    setIdleAdd(0)
                    print("Shifter State: IS Park/Neutral (0x2DE Byte 0 = " .. data[1] .. "). Setting idle adder to 0%.")
                end
                prevShifterStateIsPN_forIdleAdd = currentShifterStateIsPN
            end
        end
    end
end
canRxAdd(0x2DE, onCanRxShifterState)

print("G35 ECU CAN Simulation Script Loaded (Reduced Prints, Idle Comp).")
print("Monitoring AC requests on 0x35d. Silently monitoring 0x358 Byte 1.")
print("Monitoring shifter state on 0x2DE for idle compensation.")
print("ECU will transmit IDs: 0x1F9, 0x231, 0x233, 0x23D, 0x23E, 0x551, 0x794.")

if prevShifterStateIsPN_forIdleAdd == nil then 
    if currentShifterStateIsPN then
        print("Initial Idle State: Assumed Park/Neutral. Idle adder at 0%.")
        setIdleAdd(0)
    else
        print("Initial Idle State: Assumed NOT Park/Neutral. Adding " .. pnIdleAdderValue .. "% to idle.")
        setIdleAdd(pnIdleAdderValue)
    end
    prevShifterStateIsPN_forIdleAdd = currentShifterStateIsPN
end