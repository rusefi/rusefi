#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Used to trick the linker to include bootloader code into the binary.
*/
int initBootloader(void);

#ifdef __cplusplus
}
#endif
