/**
 * @file	signature.cpp
 * @brief A special file which is recompiled every time the .ini file changes.
 *
 * This is a minimalistic fast-compiling cpp-file. Any additional massive includes are not welcomed.
 *
 * @date Jul 2, 2020
 * @author andreika (c) 2020
 */

#include "pch.h"

#include "signature.h"

// We take these precautions to avoid redefinition of signature in generated .h files
// todo: do not code generate SIGNATURE_HASH and TS_SIGNATURE so that we do not have to hack here
#undef SIGNATURE_HASH
#undef TS_SIGNATURE

#pragma message ("SHORT_BOARD_NAME: " QUOTE(SHORT_BOARD_NAME))

#define SIGNATURE_NAME signature_
#define SIGNATURE_EXT .h

#if !EFI_UNIT_TEST
#define SIGNATURE_H_FILENAME QUOTE(SIGNATURE_NAME SHORT_BOARD_NAME SIGNATURE_EXT)
#include SIGNATURE_H_FILENAME

#pragma message ("TS_SIGNATURE: " TS_SIGNATURE)

const char *getTsSignature() {
	return TS_SIGNATURE;
}
#endif
