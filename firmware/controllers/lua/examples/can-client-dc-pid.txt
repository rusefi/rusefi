startPwm(0, 800, 0.1)
-- direction
startPwm(1, 80, 1.0)
-- disable
startPwm(2, 80, 0.0)

-- p, i, d, min, max
pid = Pid.new(2, 0, 0, -100, 100)

biasCurveIndex = findCurveIndex("bias")

canRxAdd(0x600)

voltageFromCan = 0

function onCanRx(bus, id, dlc, data)
  print('got CAN id=' .. id .. ' dlc='  .. dlc)
  voltageFromCan =   data[2] / 256.0 + data[1]
end

function onTick()
  local targetVoltage = getAuxAnalog(0)

--  local target = interpolate(1, 0, 3.5, 100, targetVoltage)
  local target = interpolate(1, 0, 3.5, 100, voltageFromCan)
-- clamp 0 to 100
  target = math.max(0, target)
  target = math.min(100, target)

  print('Decoded target: ' .. target)

  local tps = getSensor("TPS1")
  tps = (tps == nil and 'invalid TPS' or tps)
  print('Tps ' .. tps)

  local output = pid:get(target, tps)

  local bias = curve(biasCurveIndex, target)
  print('bias ' .. bias)

  local duty = (bias + output) / 100
  isPositive = duty > 0;
  pwmValue = isPositive and duty or -duty
  setPwmDuty(0, pwmValue)

  dirValue = isPositive and 1 or 0;
  setPwmDuty(1, dirValue)

  print('pwm ' .. pwmValue .. ' dir ' .. dirValue)
  print('')
end