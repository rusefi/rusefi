#include <iostream>
#include <fstream>
#include <iomanip>

typedef struct __attribute__ ((packed)) {
	// the whole order of all packet bytes is reversed, not just the 'endian-swap' integers
	uint32_t timestamp;
	// unfortunately all these fields are required by TS...
	bool priLevel : 1;
	bool secLevel : 1;
	bool trigger : 1;
	bool sync : 1;
	bool coil : 1;
	bool injector : 1;
} composite_logger_s;

static_assert(sizeof(composite_logger_s) == 5);

static inline uint32_t SWAP_UINT32(uint32_t x)
{
	return (((x >> 24) & 0x000000ff) | ((x <<  8) & 0x00ff0000) |
			((x >>  8) & 0x0000ff00) | ((x << 24) & 0xff000000));
}

static constexpr double ticksPerSecond = 1e6;

int main(int argc, char** argv)
{
	std::ifstream src(argv[1], std::ios::binary);
	std::ofstream dst(argv[2], std::ios::binary);

	dst << std::setprecision(10);

	dst << "timestamp,pri,sec" << std::endl;

	while (!src.eof())
	{
		composite_logger_s entry;

		src.read(reinterpret_cast<char*>(&entry), sizeof(composite_logger_s));

		double sec = SWAP_UINT32(entry.timestamp) / ticksPerSecond;

		dst << sec << "," << entry.priLevel << "," << entry.secLevel << std::endl;
	}
}
