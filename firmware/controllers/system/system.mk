
SYSTEMSRC = \
	$(PROJECT_DIR)/controllers/system/signal_executor_sleep.c \
	$(PROJECT_DIR)/controllers/system/signal_executor_hw_timer.c

SYSTEMSRC_CPP =	$(PROJECT_DIR)/controllers/system/trigger_emulator_algo.cpp \
	$(PROJECT_DIR)/controllers/system/pwm_generator_logic.cpp \
	$(PROJECT_DIR)/controllers/system/SingleTimerExecutor.cpp