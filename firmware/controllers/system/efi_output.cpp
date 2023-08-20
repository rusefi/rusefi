/*
 * @file efi_output.cpp
 *
 */

#include "efi_output.h"

void SwitchState::update(bool newState) {
    if (newState != *state) {
        *state = newState;
        counter++;
    }
}
