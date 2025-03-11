industrialPid = IndustrialPid.new(1, 1, 0, 0, 0.5)
industrialControlLoopOutput = industrialPid:get(10, 1)
print('industrialControlLoopOutput: ' ..industrialControlLoopOutput)

pid = Pid.new(1, 1, 0, 0, 0.5)
controlLoopOutput = pid:get(10, 1)
print('Control Output: ' ..controlLoopOutput)
