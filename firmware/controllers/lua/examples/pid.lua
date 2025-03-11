-- min/max are integers!
industrialPid = IndustrialPid.new(1, 1, 0, 0, 10)
industrialControlLoopOutput = industrialPid:get(10, 1)
print('industrialControlLoopOutput: ' ..industrialControlLoopOutput)

-- min/max are integers!
pid = Pid.new(1, 1, 0, 0, 10)
controlLoopOutput = pid:get(10, 1)
print('Control Output: ' ..controlLoopOutput)
