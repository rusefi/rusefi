/**
 * @date Dec 20, 2013
 *
 * @author Andrey Belomutskiy, (c) 2012-2020
 * @author Kot_dnz
 *
 * This file is part of rusEfi - see http://rusefi.com
 *
 * rusEfi is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * rusEfi is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * see #testGpsParser
 */
#include "pch.h"
#include <time.h>
#include "nmea.h"

static long hex2int(const char *a, int len) {
	int i;
	long val = 0;

	for (i = 0; i < len; i++)
		if (a[i] <= 57)
			val += (a[i] - 48) * (1 << (4 * (len - 1 - i))); // it's number
		else
			val += (a[i] - 87) * (1 << (4 * (len - 1 - i))); // it's a-f -> work only with low case hex
	return val;
}

static int str2int(const char *a, int len) {
	 int i = 0,  k = 0;
	while (i<len) {
		k = (k<<3)+(k<<1)+(*a)-'0';
		a++;
		i++;
	}
	return k;
}

static float gps_deg_dec(float deg_point) {
	float ddeg;
	float sec = modff(deg_point, &ddeg) * 60;
	int deg = (int) (ddeg / 100);
	int min = (int) (deg_point - (deg * 100));

	float absdlat = round(deg * 1000000.);
	float absmlat = round(min * 1000000.);
	float absslat = round(sec * 1000000.);

	return round(absdlat + (absmlat / 60) + (absslat / 3600)) / 1000000;
}

// Convert lat e lon to decimals (from deg)
static void gps_convert_deg_to_dec(float *latitude, char ns, float *longitude, char we) {
	float lat = (ns == 'N') ? *latitude : -1 * (*latitude);
	float lon = (we == 'E') ? *longitude : -1 * (*longitude);

	*latitude = gps_deg_dec(lat);
	*longitude = gps_deg_dec(lon);
}

// in string collect all char till comma and convert to float
static int str_till_comma(char *a, char *dStr) {

	int i = 0, sLen = strlen(a);
	if (sLen > GPS_MAX_STRING)
		sLen = GPS_MAX_STRING;

	while (a[i] != 44 && i < sLen) {	// while not comma or end
		dStr[i] = a[i];
		i++;
	}
	dStr[i] = '\0';
	return i;
}

/*
GxGGA - name code
Parameter	Value	Unit	Description
UTC					hhmmss.sss	Universal time coordinated
Lat					ddmm.mmmm	Latitude
Northing Indicator			N=North, S=South
Lon					dddmm.mmmm	Longitude
Easting Indicator			E=East, W=West
Status				0			0=Invalid, 1=2D/3D, 2=DGPS, 6=Dead Reckoning
SVs Used			00			Number of SVs used for Navigation
HDOP				99.99		Horizontal Dilution of Precision
Alt (MSL)			m	Altitude (above means sea level)
Unit				M=Meters
Geoid Sep.			m			Geoid Separation = Alt(HAE) - Alt(MSL)
Unit				M=Meters
Age of DGPS Corr	s			Age of Differential Corrections
DGPS Ref Station				ID of DGPS Reference Station
*/
void nmea_parse_gpgga(char *nmea, loc_t *loc) {
	char *p = nmea;
	char dStr[GPS_MAX_STRING];

	p = strchr(p, ',') + 1; 				//skip time - we read date&time if Valid in GxRMC

	p = strchr(p, ',') + 1;					// in p string started with searching address
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	if (strlen(p) == 0) {
		return;				// if no data in field - empty data - we return
	}
	
	loc->latitude = atoff(dStr);				// fulfil data

	p = strchr(p, ',') + 1;					// see above
	switch (p[0]) {
	case 'N':
		loc->lat = 'N';
		break;
	case 'S':
		loc->lat = 'S';
		break;
	case ',':
		loc->lat = '\0';
		break;
	}

	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->longitude = atoff(dStr);

	p = strchr(p, ',') + 1;
	switch (p[0]) {
	case 'W':
		loc->lon = 'W';
		break;
	case 'E':
		loc->lon = 'E';
		break;
	case ',':
		loc->lon = '\0';
		break;
	}

	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->quality = atoi(dStr);

	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->satellites = atoi(dStr);

	p = strchr(p, ',') + 1;

	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->altitude = atoff(dStr);
}

/*
GxRMC - nmea code
Parameter	Value		Unit			Description
UTC						hhmmss.sss		Universal time coordinated
Status		V		A=Valid, V=Invalid
Lat			ddmm.mmmm					Latitude
Northing Indicator			N=North, S=South
Lon			dddmm.mmmm					Longitude
Easting Indicator			E=East, W=West
SOG						nots			Speed Over Ground
COG (true)				°				Course Over Ground (true)
Date					ddmmyy			Universal time coordinated
Magnetic Variation		°				Magnetic Variation
Magnetic Variation			E=East,W=West
Mode Indicator	N		A=Autonomous, D=Differential, E=Dead Reckoning, N=None
Navigational Status			S=Safe C=Caution U=Unsafe V=Not valid

*/
void nmea_parse_gprmc(char *nmea, loc_t *loc) {
	char *p = nmea;
	char dStr[GPS_MAX_STRING];
	struct tm timp;

	p = strchr(p, ',') + 1; 				//read time
	str_till_comma(p, dStr);
	if (strlen(dStr) > 5) {
		timp.tm_hour = str2int(dStr,2);
		timp.tm_min  = str2int(dStr+2,2);
		timp.tm_sec  = str2int(dStr+4,2);
	}
	
	p = strchr(p, ',') + 1; 				//read field Valid status
	str_till_comma(p, dStr);

	if (dStr[0] == 'V') {					// if field is invalid
		loc->quality = 0;
		return;
	}
	
	loc->quality = 4;						// this is declaration that last receive field VALID
	
	p = strchr(p, ',') + 1;					// latitude
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->latitude = atoff(dStr);

	p = strchr(p, ',') + 1;
	switch (p[0]) {
	case 'N':
		loc->lat = 'N';
		break;
	case 'S':
		loc->lat = 'S';
		break;
	case ',':
		loc->lat = '\0';
		break;
	}

	p = strchr(p, ',') + 1; 				// longitude
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->longitude = atoff(dStr);

	p = strchr(p, ',') + 1;
	switch (p[0]) {
	case 'W':
		loc->lon = 'W';
		break;
	case 'E':
		loc->lon = 'E';
		break;
	case ',':
		loc->lon = '\0';
		break;
	}

	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->speed = atoff(dStr);
	
	p = strchr(p, ',') + 1;
	str_till_comma(p, dStr);				// str to float till comma saved modified string
	loc->course = atoff(dStr);

	p = strchr(p, ',') + 1; 				//read date
	str_till_comma(p, dStr);
	if (strlen(dStr) > 5) {
		timp.tm_mday = str2int(dStr,2);
		timp.tm_mon  = str2int(dStr+2,2);
		timp.tm_year = str2int(dStr+4,2)+100;	// we receive -200, but standard wait -1900 = add correction
	}

	if (timp.tm_year > 0 ) {				// check if date field is valid
		memcpy(&loc->GPStm, &timp, sizeof(timp));
	}
}

/**
 * Get the message type (GPGGA, GPRMC, etc..)
 *
 * This function filters out also wrong packages (invalid checksum)
 *
 * @param message The NMEA message
 * @return The type of message if it is valid
 */
nmea_message_type nmea_get_message_type(const char *message) {
	int checksum = nmea_valid_checksum(message);
	if (checksum != _EMPTY) {
		return static_cast<nmea_message_type>(checksum);
	}

	if (strstr(message, NMEA_GPGGA_STR) != NULL) {
		return NMEA_GPGGA;
	}

	if (strstr(message, NMEA_GPRMC_STR) != NULL) {
		return NMEA_GPRMC;
	}

	return NMEA_UNKNOWN;
}

int nmea_valid_checksum(const char *message) {
	char p;
	int sum = 0;
	const char* starPtr = strrchr(message, '*');
	if (!starPtr) {
		return NMEA_CHECKSUM_ERR;
	}
	const char* int_message = starPtr + 1;
	long checksum = hex2int(int_message, 2);

	++message;
	while ((p = *message++) != '*') {
		sum ^= p;
	}

	if (sum != checksum) {
		return NMEA_CHECKSUM_ERR;
	}
	return _EMPTY;
}

// Compute the GPS location using decimal scale
void gps_location(loc_t *coord, char *buffer) {

	coord->type = nmea_get_message_type(buffer);

	switch (coord->type) {
	case NMEA_GPGGA:
		nmea_parse_gpgga(buffer, coord);
		gps_convert_deg_to_dec(&(coord->latitude), coord->lat, &(coord->longitude), coord->lon);
		break;
	case NMEA_GPRMC:
		nmea_parse_gprmc(buffer, coord);
		break;
	case NMEA_UNKNOWN:
		// unknown message type
		break;
	}

}
