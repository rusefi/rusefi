#pragma once

inline constexpr unsigned L9779_KEY_ON_MAX_ATTEMPTS = 3;

template <typename Chip>
void l9779RequestInitialization(Chip& chip) {
	chip.need_init = true;
	chip.init_attempts = 0;
	chip.init_error = 0;
}

// Return true only when the worker may service watchdog and normal outputs.
// Exhaustion stays pending until another key-on requests a fresh attempt budget.
template <typename Chip>
bool l9779InitializeOnKeyOn(Chip& chip) {
	if (!chip.power_stage_on) {
		return false;
	}
	if (!chip.need_init) {
		return true;
	}
	if (chip.init_attempts >= L9779_KEY_ON_MAX_ATTEMPTS) {
		return false;
	}

	// A surviving watchdog timer must not feed during reset or partial setup.
	chip.stop_watchdog();
	chip.init_attempts++;
	chip.init_error = chip.chip_reset();
	if (chip.init_error == 0) {
		chip.init_error = chip.chip_init();
	}
	if (chip.init_error == 0) {
		chip.init_error = chip.update_output();
	}
	if (chip.init_error != 0) {
		return false;
	}

	chip.need_init = false;
	return true;
}
