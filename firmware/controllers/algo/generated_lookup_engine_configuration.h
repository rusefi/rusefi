#include "efi_quote.h"

#pragma once

#define META_ENGINE_GENERATED_NAME engine_configuration_generated_structures_
#define META_ENGINE_GENERATED_EXT h

// todo: sad technical debt: failing to define SHORT_BOARD_NAME for unit_tests and concatenate specifically for mac os?!
#if defined(SHORT_BOARD_NAME)

#define META_ENGINE_GENERATED_H_FILENAME QUOTE(CONCATENATE(META_ENGINE_GENERATED_NAME, SHORT_BOARD_NAME)) "." QUOTE(META_ENGINE_GENERATED_EXT)
#include META_ENGINE_GENERATED_H_FILENAME

#else

#include "engine_configuration_generated_structures_f407-discovery.h"

#endif
