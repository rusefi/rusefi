echo "Compiling for Kinetis MKE16F512"
rem TODO: somehow this -DDUMMY is helping us to not mess up the parameters, why?!
rem https://github.com/rusefi/rusefi/issues/684
set EXTRA_PARAMS=-DDUMMY -DCPU_MKE16F512VLH16 -DCPU_MKE16F512VLH16_cm4 -D__USE_CMSIS^
 -DEFI_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_CHECKS=FALSE -DCH_DBG_ENABLE_ASSERTS=FALSE -DCH_DBG_ENABLE_STACK_CHECK=FALSE -DCH_DBG_FILL_THREADS=FALSE -DCH_DBG_THREADS_PROFILING=FALSE^
 -DDEFAULT_ENGINE_TYPE=MINIMAL_PINS
rem set BUILDDIR=build_kinetis
set BUILDDIR=build
set PROJECT_BOARD=kinetis
set PROJECT_CPU=kinetis
set DEBUG_LEVEL_OPT="-O2"
set USE_FATFS=no
set USE_BOOTLOADER=no
rem -DCH_DBG_ENABLE_CHECKS=FALSE

cd ../../..

rem rm -f ${BUILDDIR}/rusefi_kinetis.*

rem make -r -j4

call config/boards/common_make.bat
