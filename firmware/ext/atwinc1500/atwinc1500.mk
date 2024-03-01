ATWINC_DIR = $(PROJECT_DIR)/ext/atwinc1500

ALLINC += \
	$(ATWINC_DIR)


# $(ATWINC_DIR)/bsp/include \
# $(ATWINC_DIR)/bus_wrapper/include \
# $(ATWINC_DIR)/common/include \
# $(ATWINC_DIR)/driver/include \
# $(ATWINC_DIR)/common/include \

ALLCPPSRC += \
	$(ATWINC_DIR)/common/source/nm_common.cpp \
	$(ATWINC_DIR)/driver/source/m2m_ate_mode.cpp \
	$(ATWINC_DIR)/driver/source/m2m_crypto.cpp \
	$(ATWINC_DIR)/driver/source/m2m_hif.cpp \
	$(ATWINC_DIR)/driver/source/m2m_ota.cpp \
	$(ATWINC_DIR)/driver/source/m2m_periph.cpp \
	$(ATWINC_DIR)/driver/source/m2m_wifi.cpp \
	$(ATWINC_DIR)/driver/source/nmasic.cpp \
	$(ATWINC_DIR)/driver/source/nmbus.cpp \
	$(ATWINC_DIR)/driver/source/nmdrv.cpp \
	$(ATWINC_DIR)/driver/source/nmi2c.cpp \
	$(ATWINC_DIR)/driver/source/nmspi.cpp \
	$(ATWINC_DIR)/driver/source/nmuart.cpp \
	$(ATWINC_DIR)/socket/source/socket.cpp \
	$(ATWINC_DIR)/spi_flash/source/spi_flash.cpp \


#$(ATWINC_DIR)/driver/source/m2m_ssl.c
