
/* Define STM32F4 or STM32F7 flash layout in dual bank mode. Only first bank is defined */

static const tFlashSector flashLayout[] =
{
  /* space is reserved for a bootloader configuration with all supported communication
   * interfaces enabled. when for example only UART is needed, then the space required
   * for the bootloader can be made a lot smaller here.
   */
  /* { 0x08000000, 0x04000, 0 },         flash sector 0 - reserved for bootloader      */
  /* { 0x08004000, 0x04000, 1 },         flash sector 1 - reserved for bootloader      */
  { 0x08008000, 0x08000, 2},            /* flash sector  2 - 16kb                      */
  { 0x0800c000, 0x08000, 3},            /* flash sector  3 - 16kb                      */
  { 0x08010000, 0x10000, 4},            /* flash sector  4 - 64kb                      */
  { 0x08020000, 0x20000, 5},            /* flash sector  5 - 128kb                     */
  { 0x08040000, 0x20000, 6},            /* flash sector  6 - 128kb                     */
  { 0x08060000, 0x20000, 7},            /* flash sector  7 - 128kb                     */
#if (BOOT_NVM_SIZE_KB > 1024)
  { 0x08080000, 0x20000, 8},            /* flash sector  8 - 128kb                     */
  { 0x080a0000, 0x20000, 9},            /* flash sector  9 - 128kb                     */
  { 0x080c0000, 0x20000, 10},           /* flash sector 10 - 128kb                     */
  { 0x080e0000, 0x20000, 11},           /* flash sector 11 - 128kb                     */
#endif
  /* Bank #2 is not defined */
#if (BOOT_NVM_SIZE_KB > 2048)
#error "BOOT_NVM_SIZE_KB > 2048 is currently not supported."
#endif
};