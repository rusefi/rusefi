#include "flash_storage.h"

#include <cstdio>
#include <fstream>
#include <sstream>

std::string makeFileName(flashaddr_t addr) {
	std::stringstream ss;

	ss << "flash" << addr << ".bin";

	return ss.str();
}

bool intFlashCompare(flashaddr_t address, const char* buffer, size_t size) {
	auto fileName = makeFileName(address);

	printf("Simulator: comparing with config from %s\n", fileName.c_str());

	std::ifstream flash;
	flash.open(fileName, std::ios::binary);

	if (!flash.is_open()) {
		return false;
	}

	char ch;
	bool same = true;
	size_t checked = 0;
	while (same && checked < size && flash.get(ch)) {
		if (static_cast<unsigned char>(ch) != buffer[checked]) {
			same = false;
		}
		checked++;
	}

	flash.close();

	return same;
}

