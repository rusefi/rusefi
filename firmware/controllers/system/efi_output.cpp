/*
 * @file efi_output.cpp
 *
 */

#include "pch.h"
#include "efi_output.h"

bool SwitchedState::update(bool const newState) {
    if (state == nullptr) {
        return false;
    }

    if (newState != *state) {
        *state = newState ? 1 : 0;
        counter++;
        return true;
    }

    return false;
}

uint16_t SwitchedState::getCounter() const {
	return counter;
}
