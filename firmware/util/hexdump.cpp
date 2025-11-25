#include <ctype.h>
#include <stdio.h>

#include "efiprintf.h"

#include "hexdump.h"

#ifndef HEXDUMP_COLS
	#define HEXDUMP_COLS 16
#endif

void hexdump(void *mem, size_t len, unsigned int prefix, bool printChar) {
	char buffer[128];
	size_t offset = 0;

	for (unsigned i = 0; i < len + ((len % HEXDUMP_COLS) ? (HEXDUMP_COLS - len % HEXDUMP_COLS) : 0); i++) {
		/* print offset */
		if (i % HEXDUMP_COLS == 0) {
			offset += sprintf(buffer + offset, "%08x: ", prefix + i);
		} else if ((i + HEXDUMP_COLS / 2) % HEXDUMP_COLS == 0) {
			offset += sprintf(buffer + offset, " ");
		}

		/* print hex data */
		if (i < len) {
			offset += sprintf(buffer + offset, "%02x ", 0xFF & ((unsigned char *)mem)[i]);
		} else if (printChar) {
			/* end of block, just aligning for ASCII dump */
			offset += sprintf(buffer + offset, "   ");
		}

		if (i % HEXDUMP_COLS == (HEXDUMP_COLS - 1)) {
			if (printChar) {
				/* print ASCII dump */
				offset += sprintf(buffer + offset, "|");
				for (unsigned j = i - (HEXDUMP_COLS - 1); j <= i; j++)
				{
					if (j >= len) {
						/* end of block, not really printing */
						offset += sprintf(buffer + offset, " ");
					} else if (isprint(((unsigned char *)mem)[j])) {
						/* printable char */
						offset += sprintf(buffer + offset, "%c", 0xFF & ((char *)mem)[j]);
					} else {
						/* other char */
						offset += sprintf(buffer + offset, ".");
					}
				}
			}

			offset += sprintf(buffer + offset, "|");
			buffer[offset] = '\0';
			efiPrintf("%s", buffer);
			offset = 0;
		}
	}
}
