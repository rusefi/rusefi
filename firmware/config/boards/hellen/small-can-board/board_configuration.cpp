#include "pch.h"
#include "hellen_meta.h"

#include "hellen_leds_64.cpp"

void setBoardConfigOverrides() {
	setHellen64Can();
	setHellen64MegaEnPin();
	setHellen64SdCardSpi1();
}
