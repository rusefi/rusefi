
/* Define STM32F4, STM32F7 or STM32F765 flash layout in dual bank mode. Only first bank is defined */

/* This file is included in another .c file. So it is ok to have static and no users here */
static const tFlashSector flashLayout[] =
{
  /* space is reserved for a bootloader configuration with all supported communication
   * interfaces enabled. when for example only UART is needed, then the space required
   * for the bootloader can be made a lot smaller here.
   */
/*{ 0x08000000,  16 * 1024, 0 },              flash sector  0 - 16kb - reserved for OpenBLT */
/*{ 0x08004000,  16 * 1024, 1 },              flash sector  1 - 16kb - reserved for OpenBLT */
  { 0x08008000,  16 * 1024, 2 },           /* flash sector  2 - 16kb                        */
  { 0x0800c000,  16 * 1024, 3 },           /* flash sector  3 - 16kb                        */
  { 0x08010000,  64 * 1024, 4 },           /* flash sector  4 - 64kb                        */
  { 0x08020000, 128 * 1024, 5 },           /* flash sector  5 - 128kb                       */
  { 0x08040000, 128 * 1024, 6 },           /* flash sector  6 - 128kb                       */
  { 0x08060000, 128 * 1024, 7 },           /* flash sector  7 - 128kb                       */
#if (BOOT_NVM_SIZE_KB > 512)
  { 0x08080000, 128 * 1024, 8 },           /* flash sector  8 - 128kb                       */
  { 0x080a0000, 128 * 1024, 9 },           /* flash sector  9 - 128kb                       */
  { 0x080c0000, 128 * 1024, 10},           /* flash sector 10 - 128kb                       */
  { 0x080e0000, 128 * 1024, 11},           /* flash sector 11 - 128kb                       */
#endif
  /* Bank #2 is not defined */
#if (BOOT_NVM_SIZE_KB > 2048)
#error "BOOT_NVM_SIZE_KB > 2048 is currently not supported."
#endif
};