CONSOLE_COMMON_SRC_CPP = 	$(PROJECT_DIR)/console/binary/tooth_logger.cpp \
	                        $(PROJECT_DIR)/console/binary_mlg_log/binary_mlg_logging.cpp \
                         	$(PROJECT_DIR)/console/status_loop.cpp \


CONSOLE_SRC_CPP = $(CONSOLE_COMMON_SRC_CPP) \
	$(PROJECT_DIR)/console/console_io.cpp \
	$(PROJECT_DIR)/console/eficonsole.cpp \
	$(PROJECT_DIR)/console/connector_uart_dma.cpp \
	$(PROJECT_DIR)/console/binary_mlg_log/usb_console.cpp \
	$(PROJECT_DIR)/console/wifi_console.cpp \


CONSOLE_INC=\
	$(PROJECT_DIR)/console/binary \
	$(PROJECT_DIR)/console \
	$(PROJECT_DIR)/console/binary_log \
	$(PROJECT_DIR)/console/binary_mlg_log

