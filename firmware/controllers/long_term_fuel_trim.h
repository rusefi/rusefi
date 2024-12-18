#pragma once

struct LtftState {
	int ecuRestartCounter = 0;
	void save() {
		ecuRestartCounter++;
	}
	void load();
};
