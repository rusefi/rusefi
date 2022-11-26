# This board uses bootloader
USE_BOOTLOADER = yes

# include Prometheus bootloader code
BOOTLOADERINC = $(PROJECT_DIR)/bootloader/prometheus/$(PROMETHEUS_BOARD)
