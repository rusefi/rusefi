/*
 * tle6240.cpp
 *
 * TLE6240GP Smart 16-Channel Low-Side Switch
 *
 * All 16 channels can be controlled via the serial interface (SPI). In addition to the serial control it is possible to control channel 1 to 4 and 9 to 12 direct in parallel with a separate input pin.
 *
 * Looks like 3.3v SI and SCLK are NOT possible (H above 0.7Vs required, that's 3.5v for 5.0Vs)
 * 5 MHz SPI
 *
 * @date Dec 29, 2018
 * @author Andrey Belomutskiy, (c) 2012-2018
 */




