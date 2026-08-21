/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * AT32F435ZMxx with default settings memory setup.
 * Note: Use of ram1 and ram2 is mutually exclusive with use of ram0.
 * bl section is where we link the rusefi bootloader.
 */

/* change address & length if bootloader */
bootloader_size = 32k;
flash_start = 0x08000000 + (DEFINED(HAS_BOOTLOADER) ? bootloader_size : 0);
flash_size = DEFINED(IS_BOOTLOADER) ? bootloader_size : 4032k;
image_size = DEFINED(HAS_BOOTLOADER) ? (flash_size - bootloader_size) : flash_size;

/* OpenBLT <-> main FW shared area */
_OpenBLT_Shared_Params_Size = DEFINED(HAS_BOOTLOADER) || DEFINED(IS_BOOTLOADER) ? 16 : 0;

MEMORY
{
    bl      : org = 0x08000000, len = bootloader_size     /* bootloader section */
    flash0 (rx) : org = flash_start, len = image_size
    flash1 (rx) : org = 0x00000000, len = 0
    flash2 (rx) : org = 0x00000000, len = 0
    flash3 (rx) : org = 0x00000000, len = 0
    flash4 (rx) : org = 0x00000000, len = 0
    flash5 (rx) : org = 0x00000000, len = 0
    flash6 (rx) : org = 0x00000000, len = 0
    flash7 (rx) : org = 0x00000000, len = 0
    shared  : org = 0x20000000, len = _OpenBLT_Shared_Params_Size
    ram0   (wx) : org = 0x20000000 + _OpenBLT_Shared_Params_Size, len = 64k + 320k - _OpenBLT_Shared_Params_Size    /* SRAM1 + SRAM2 */
    /* 64 KB carved off the top of ram0 for the trigger fast-path code: the
       AT32 flash NZW area is too slow for the per-tooth ISR (measured ~25
       cycles per 7-instruction iteration with continue-read at 288 MHz),
       while SRAM is zero-wait. The heap shrinks by the same 64 KB (FatFs
       allocations are small, plenty of headroom). */
    ram_fast (wx) : org = 0x20050000, len = 64k
    ram_heap (wx) : org = 0x20000000 + _OpenBLT_Shared_Params_Size, len = 64k + 320k - 64k - _OpenBLT_Shared_Params_Size
    ram1   (wx) : org = 0x20000000 + _OpenBLT_Shared_Params_Size, len = 64k - _OpenBLT_Shared_Params_Size     /* SRAM1 */
    ram2   (wx) : org = 0x20010000, len = 320k    /* SRAM2 */
    ram3   (wx) : org = 0x20020000, len = 0
    ram4   (wx) : org = 0x10000000, len = 0
    ram5   (wx) : org = 0x40024000, len = 0
    ram6   (wx) : org = 0x00000000, len = 0
    ram7   (wx) : org = 0x00000000, len = 0
}

/* For each data/text section two region are defined, a virtual region
   and a load region (_LMA suffix).*/

/* Flash region to be used for exception vectors.*/
REGION_ALIAS("VECTORS_FLASH", flash0);
REGION_ALIAS("VECTORS_FLASH_LMA", flash0);

/* Flash region to be used for constructors and destructors.*/
REGION_ALIAS("XTORS_FLASH", flash0);
REGION_ALIAS("XTORS_FLASH_LMA", flash0);

/* Flash region to be used for code text.*/
REGION_ALIAS("TEXT_FLASH", flash0);
REGION_ALIAS("TEXT_FLASH_LMA", flash0);

/* Flash region to be used for read only data.*/
REGION_ALIAS("RODATA_FLASH", flash0);
REGION_ALIAS("RODATA_FLASH_LMA", flash0);

/* Flash region to be used for various.*/
REGION_ALIAS("VARIOUS_FLASH", flash0);
REGION_ALIAS("VARIOUS_FLASH_LMA", flash0);

/* Flash region to be used for RAM(n) initialization data.*/
REGION_ALIAS("RAM_INIT_FLASH_LMA", flash0);

/* RAM region to be used for Main stack. This stack accommodates the processing
   of all exceptions and interrupts.*/
REGION_ALIAS("MAIN_STACK_RAM", ram0);

/* RAM region to be used for the process stack. This is the stack used by
   the main() function.*/
REGION_ALIAS("PROCESS_STACK_RAM", ram0);

/* RAM region to be used for data segment.*/
REGION_ALIAS("DATA_RAM", ram0);
REGION_ALIAS("DATA_RAM_LMA", flash0);

/* RAM region to be used for BSS segment.*/
REGION_ALIAS("BSS_RAM", ram0);

/* RAM region to be used for the default heap.*/
REGION_ALIAS("HEAP_RAM", ram_heap);

SECTIONS
{
  /* Bootloader section */
  .bl : ALIGN(4)
    {
        . = ALIGN(4);
        *(.bl)
        *(.bl.*)
        . = ALIGN(4);
    } > bl AT > bl

  /* shared between main FW and OpenBLT */
  .shared (NOLOAD) :
  {
    . = ALIGN(4);
    _sshared = .;
    __shared_start__ = _sshared;
    *(.shared)
    *(.shared.*)
    KEEP(*(.shared))
    . = ALIGN(4);
    _eshared = .;
    __shared_end__ = _eshared;
  } > shared
}

/* Generic rules inclusion.*/
INCLUDE rules.ld

/* Trigger fast path: code copied from flash to zero-wait SRAM at boot
   (see m74_9 board init). Note: rules.ld opens its own SECTIONS block, so
   this must be a separate SECTIONS block after the include. Its LMA
   continues the flash image and its VMA sits in the carved-out ram_fast
   region. */
SECTIONS
{
    .fast_text : ALIGN(4)
    {
        . = ALIGN(4);
        __fast_text_start__ = .;
        *(.fast_text)
        *(.fast_text.*)
        . = ALIGN(4);
        __fast_text_end__ = .;
    } > ram_fast AT > flash0
}

__fast_text_load__ = LOADADDR(.fast_text);
__fast_text_size__ = __fast_text_end__ - __fast_text_start__;
