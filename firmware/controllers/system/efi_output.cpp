/*
 * @file efi_output.cpp
 *
 */

#include "pch.h"
#include "efi_output.h"


bool SwitchedState::update(bool newState) {
    if (newState != *state) {
        *state = newState ? 1 : 0;
        counter++;
        return true;
    }
    return false;
}

uint16_t SwitchedState::getCounter() {
	return counter;
}

