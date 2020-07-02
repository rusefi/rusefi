/**
 * @file	signature.cpp
 * @brief A special file which is recompiled every time the .ini file changes.
 *
 * @date Jul 2, 2020
 * @author andreika (c) 2020
 */

#include "signature.h"

#ifndef SHORT_BOARD_NAME
#define SHORT_BOARD_NAME all
#endif /* SHORT_BOARD_NAME */

#define Q(x) #x
#define QUOTE(x) Q(x)

#pragma message ("SHORT_BOARD_NAME: " QUOTE(SHORT_BOARD_NAME))

#define SIGNATURE_NAME signature_
#define SIGNATURE_EXT .h

#define SIGNATURE_H QUOTE(SIGNATURE_NAME SHORT_BOARD_NAME SIGNATURE_EXT)

#include SIGNATURE_H

#pragma message ("TS_SIGNATURE: " TS_SIGNATURE)

const char *getTsSignature() {
	return TS_SIGNATURE;
}
