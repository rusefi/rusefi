-- Requires firmware built with LUA_I2C_DAC=TRUE.
-- UAEFI pads: P4 = PD7/SCL, P3 = PE8/SDA, P8 = PD5/LDAC.
-- These pins must be free. See docs/mcp4728.md for one-time address programming.
local ready = initI2cDac("PD7", "PE8", "PD5")
if not ready then
    print("MCP4728: pin initialization failed")
end

setTickRate(10)
function onTick()
    if ready then
        -- Five chips at addresses 0..4 provide channels 1..20.
        -- VDD defaults to 5 V; pass the measured supply as the third argument.
        local first = setI2cDacVoltage(1, 2.5)
        local last = setI2cDac(20, 2048)
        -- Update all four channels on chip address 1 in one I2C transaction.
        local group = setI2cDacChannels(1, 0, 1024, 2048, 4095)
        if not (first and last and group) then
            print("MCP4728: I2C write failed")
        end
    end
end
