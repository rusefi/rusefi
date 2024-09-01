/*
 * @file injection_gpio.h
 */

#pragma once

#include "efi_output.h"

void startSimultaneousInjection(void* = nullptr);
void endSimultaneousInjectionOnlyTogglePins();

class InjectorOutputPin final : public NamedOutputPin {
public:
	InjectorOutputPin();
	void reset();

	void open(efitick_t nowNt);
	void close(efitick_t nowNt);

	int8_t getOverlappingCounter() const { return m_overlappingCounter; }

	// todo: re-implement this injectorIndex via address manipulation to reduce memory usage?
	int8_t injectorIndex;

private:
	int8_t m_overlappingCounter;
};
