#include "global.h"
#include "engine.h"
#include "maf.h"

static void fillTheRest(persistent_config_s *e, int i) {
	/**
	 * unrealistic values just to make binary search happy
	 */
	while (i < MAF_DECODING_COUNT) {
		e->mafDecoding[i] = 200;
		e->mafDecodingBins[i] = 10 + i;
		i++;
	}
}

static int addMafPoint(persistent_config_s *e, int i, float kgHrValue, float voltage) {
	e->mafDecoding[i] = kgHrValue;
	e->mafDecodingBins[i] = voltage;
	return i + 1;
}

/**
 * Hot-film air-mass meter, Type HFM 5
 */
void setBosch0280218037(persistent_config_s *e) {
	int i = 0;
	i = addMafPoint(e, i, -34.5, 0);
	i = addMafPoint(e, i, -6, 0.78125);

	e->mafDecoding[i] = 10.5;
	e->mafDecodingBins[i++] = 1.38671875;

	e->mafDecoding[i] = 105.3;
	e->mafDecodingBins[i++] = 2.91015625;

	e->mafDecoding[i] = 387.5;
	e->mafDecodingBins[i++] = 4.2578125;

	e->mafDecoding[i] = 738;
	e->mafDecodingBins[i++] = 4.98046875;

	fillTheRest(e, i);
}

void setBosch0280218004(persistent_config_s *e) {
	int i = 0;

	fillTheRest(e, i);
}

void setDensoTODO(persistent_config_s *e) {
	int i = 0;

	i = addMafPoint(e, i, 116.3, 0.8);
	i = addMafPoint(e, i, 76.3, 1.04);
	i = addMafPoint(e, i, 40.5, 1.4);
	i = addMafPoint(e, i, 32.5, 1.60);
	i = addMafPoint(e, i, 23.8, 1.83);
	i = addMafPoint(e, i, 0, 3);

	while (i < MAF_DECODING_COUNT) {
		e->mafDecoding[i] = 0;
		e->mafDecodingBins[i] = 10 + i;
		i++;
	}
}

void setMazdaMiataNAMaf(persistent_config_s *e) {
  int i = 0;
  i= addMafPoint(e, i, 0.000000, 0.000000);
  i= addMafPoint(e, i, 0.000028, 0.100000);
  i= addMafPoint(e, i, 0.000387, 0.200000);
  i= addMafPoint(e, i, 0.001809, 0.300000);
  i= addMafPoint(e, i, 0.005409, 0.400000);
  i= addMafPoint(e, i, 0.012648, 0.500000);
  i= addMafPoint(e, i, 0.025320, 0.600000);
  i= addMafPoint(e, i, 0.045532, 0.700000);
  i= addMafPoint(e, i, 0.075698, 0.800000);
  i= addMafPoint(e, i, 0.118525, 0.900000);
  i= addMafPoint(e, i, 0.177011, 1.000000);
  i= addMafPoint(e, i, 0.254434, 1.100000);
  i= addMafPoint(e, i, 0.354347, 1.200000);
  i= addMafPoint(e, i, 0.480576, 1.300000);
  i= addMafPoint(e, i, 0.637210, 1.400000);
  i= addMafPoint(e, i, 0.828604, 1.500000);
  i= addMafPoint(e, i, 1.059369, 1.600000);
  i= addMafPoint(e, i, 1.334371, 1.700000);
  i= addMafPoint(e, i, 1.658728, 1.800000);
  i= addMafPoint(e, i, 2.037809, 1.900000);
  i= addMafPoint(e, i, 2.477225, 2.000000);
  i= addMafPoint(e, i, 2.982835, 2.100000);
  i= addMafPoint(e, i, 3.560736, 2.200000);
  i= addMafPoint(e, i, 4.217265, 2.300000);
  i= addMafPoint(e, i, 4.958995, 2.400000);
  i= addMafPoint(e, i, 5.792737, 2.500000);
  i= addMafPoint(e, i, 6.725531, 2.600000);
  i= addMafPoint(e, i, 7.764648, 2.700000);
  i= addMafPoint(e, i, 8.917592, 2.800000);
  i= addMafPoint(e, i, 10.192092, 2.900000);
  i= addMafPoint(e, i, 11.596102, 3.000000);
  i= addMafPoint(e, i, 13.137802, 3.100000);
  i= addMafPoint(e, i, 14.825595, 3.200000);
  i= addMafPoint(e, i, 16.668105, 3.300000);
  i= addMafPoint(e, i, 18.674177, 3.400000);
  i= addMafPoint(e, i, 20.852874, 3.500000);
  i= addMafPoint(e, i, 23.213478, 3.600000);
  i= addMafPoint(e, i, 25.765485, 3.700000);
  i= addMafPoint(e, i, 28.518610, 3.800000);
  i= addMafPoint(e, i, 31.482778, 3.900000);
  i= addMafPoint(e, i, 34.668130, 4.000000);
  i= addMafPoint(e, i, 38.085019, 4.100000);
  i= addMafPoint(e, i, 41.744006, 4.200000);
  i= addMafPoint(e, i, 45.655865, 4.300000);
  i= addMafPoint(e, i, 49.831577, 4.400000);
  i= addMafPoint(e, i, 54.282334, 4.500000);
  i= addMafPoint(e, i, 59.019531, 4.600000);
  i= addMafPoint(e, i, 64.054771, 4.700000);
  i= addMafPoint(e, i, 69.399864, 4.800000);
  i= addMafPoint(e, i, 75.066822, 4.900000);
  i= addMafPoint(e, i, 81.067862, 5.000000);
  
  fillTheRest(e, i);
}
