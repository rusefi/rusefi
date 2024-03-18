#include "efi_quote.h"

#pragma once

#define META_ENGINE_GENERATED_NAME engine_configuration_generated_structures_
#define META_ENGINE_GENERATED_EXT .h

#if EFI_PROD_CODE || EFI_SIMULATOR

#define META_ENGINE_GENERATED_H_FILENAME QUOTE(META_ENGINE_GENERATED_NAME SHORT_BOARD_NAME META_ENGINE_GENERATED_EXT)
#include META_ENGINE_GENERATED_H_FILENAME

#else

#include "engine_configuration_generated_structures_f407-discovery.h"

#endif
