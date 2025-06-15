/*
 * @file injection_gpio.h
 */

#pragma once

#include "efi_output.h"

void startSimultaneousInjection();
void endSimultaneousInjectionOnlyTogglePins();

class InjectorOutputPin final : public NamedOutputPin {
public:
	InjectorOutputPin();
	void reset();

	void open(efitick_t nowNt);
	void close(efitick_t nowNt);
	void setHigh() override;
	void setLow() override;

	int8_t getOverlappingCounter() const { return overlappingCounter; }

	// todo: re-implement this injectorIndex via address manipulation to reduce memory usage?
	int8_t injectorIndex;

private:
	int8_t overlappingCounter;
};
