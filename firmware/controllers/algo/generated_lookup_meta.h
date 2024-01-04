#include "efi_quote.h"

#pragma once

#define META_GENERATED_NAME rusefi_generated_
#define META_GENERATED_EXT .h

#if EFI_PROD_CODE

#define META_GENERATED_H_FILENAME QUOTE(META_GENERATED_NAME SHORT_BOARD_NAME META_GENERATED_EXT)
#include META_GENERATED_H_FILENAME

#else

#include "rusefi_generated_f407-discovery.h"

#endif
