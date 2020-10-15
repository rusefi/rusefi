#include <cstddef>

struct Writer;
void writeHeader(Writer& buffer);
size_t writeBlock(char* buffer);
