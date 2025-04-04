-- rusEFI receiving lambda from MoTeC
-- https://www.motec.com.au/products/LTC?catId=10#SPECIFICATIONS
-- https://www.motec.com.au/hessian/uploads/LTC_LTCD_LTCN_User_Manual_252ca77b3d.pdf
-- Appendix E – LTC CAN Messages, page 39

LTC_MASTER_1120_460 = 0x460

sensor = Sensor.new("Lambda1")
sensor : setTimeout(3000)

function onMotecLtc(bus, id, dlc, data)

end

canRxAdd(LTC_MASTER_1120_460, onMotecLtc)
