rem 
rem VAG-ME7 version of the firmware for F767ZI chip NOT on Red board, with a real oscilattor
rem

cd ../../..
set PROJECT_BOARD=me7_pnp
set PROJECT_CPU=ARCH_STM32F7
set EXTRA_PARAMS=-DDUMMY -DSTM32F767xx ^
 -DEFI_ENABLE_ASSERTS=FALSE ^
 -DEFI_USE_OSC=TRUE ^
 -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_TRACE=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE
set DEBUG_LEVEL_OPT="-O2"

call config/boards/common_make.bat

call config/boards/clean_env_variables.bat