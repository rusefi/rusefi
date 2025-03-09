# FATFS files.
FATFSSRC = ${CHIBIOS}/os/various/fatfs_bindings/fatfs_diskio.c \
           ${PROJECT_DIR}/ext/FatFS/ffsystem.c \
           ${PROJECT_DIR}/ext/FatFS/ff.c \
           ${PROJECT_DIR}/ext/FatFS/ffunicode.c


FATFSINC = ${PROJECT_DIR}/ext
