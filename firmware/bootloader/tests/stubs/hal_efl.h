#pragma once
struct flash_descriptor_t {
    uint8_t* address;
    size_t size;
    size_t sectors_size;
    size_t sectors_count;
};
struct EFlashDriver { int state; };
extern EFlashDriver EFLD1;
#define FLASH_STOP 0
#define MSG_OK 0
#define FLASH_NO_ERROR 0
#define FLASH_BUSY_ERASING 1
#define AT32_FLASH_WAIT_TIME_MS 5
const flash_descriptor_t* flashGetDescriptor(EFlashDriver*);
int eflStart(EFlashDriver*, const void*);
int flashStartEraseSector(EFlashDriver*, size_t);
int flashQueryErase(EFlashDriver*, uint32_t*);
int flashProgram(EFlashDriver*, size_t, size_t, const uint8_t*);
void chThdSleepMilliseconds(uint32_t);
