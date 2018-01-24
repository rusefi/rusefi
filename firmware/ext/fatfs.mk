# FATFS files.
FATFSSRC = ${CHIBIOS}/os/various/fatfs_bindings/fatfs_diskio.c \
           ${CHIBIOS}/os/various/fatfs_bindings/fatfs_syscall.c \
           ${PROJECT_DIR}/ext/ff.c \
           ${PROJECT_DIR}/ext/ffunicode.c \
           ${PROJECT_DIR}/ext/option/unicode.c

FATFSINC = ${PROJECT_DIR}/ext
