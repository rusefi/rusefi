rd /s /q merged2layer
mkdir merged2layer


c:\Python26\python.exe ki2gm.py ../brain_board/gerber/brain_board_STM32F407_R0.3/brain_board_STM32F407.drl ../brain_board/gerber/brain_board_STM32F407_R0.3/brain_board_STM32F407_gm.drl

c:\Python26\python.exe ki2gm.py ../176122-6-connector/176122-6-mfg/176122-6-mfg_R0.2/176122-6.drl ../176122-6-connector/176122-6-mfg/176122-6-mfg_R0.2/176122-6_gm.drl

c:\Python26\python.exe gerbmerge.py brain2layer.cfg