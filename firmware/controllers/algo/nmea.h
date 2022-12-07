/**
 *
 * https://github.com/wdalmut/libgps/tree/develop/src
 *
 */

#pragma once

#include "rusefi_types.h"

#define GPS_MAX_STRING 256

typedef enum {
	NMEA_UNKNOWN = 0x00,
	NMEA_GPRMC = 0x01,
	NMEA_GPGGA = 0x02
} nmea_message_type;

#define _EMPTY 0x00
#define NMEA_GPRMC_STR "$GPRMC"
#define NMEA_GPGGA_STR "$GPGGA"
#define _COMPLETED 0x03

#define NMEA_CHECKSUM_ERR 0x80
#define NMEA_MESSAGE_ERR 0xC0

struct GPSlocation {
	float latitude;
	float longitude;
	float speed;
	float altitude;
	float course;
	efidatetime_t time;
	nmea_message_type type;
	int quality;
	int satellites;
	char lat;		// Northing Indicator			N=North, S=South
	char lon;		// Easting Indicator			E=East, W=West
};
typedef struct GPSlocation loc_t;

nmea_message_type nmea_get_message_type(const char *);
int nmea_valid_checksum(const char *);
void nmea_parse_gpgga(char const * const, loc_t *);
void nmea_parse_gprmc(char const * const, loc_t *);
void gps_location(loc_t *, char const * const);

static int str2int(const char * a, const int len) {
	int i = 0,  k = 0;
	while (i < len) {
		k = (k << 3) + (k << 1) + (*a) - '0';
		a++;
		i++;
	}
	return k;
}
