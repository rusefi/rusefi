/**
 * @file    interpolation.h
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2015
 */

#ifndef INTERPOLATION_3D_H_
#define INTERPOLATION_3D_H_

#define INTERPOLATION_A(x1, y1, x2, y2) ((y1 - y2) / (x1 - x2))

int findIndex(const float array[], int size, float value);
float interpolate(float x1, float y1, float x2, float y2, float x);
float interpolate2d(float value, float bin[], float values[], int size);
float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, float* map[]);
void setTableValue(float bins[], float values[], int size, float key, float value);

class FastInterpolation {
public:
	FastInterpolation();
	FastInterpolation(float x1, float y1, float x2, float y2);
	void init(float x1, float y1, float x2, float y2);
	float getValue(float x);
private:
	float a, b;
};

#endif /* INTERPOLATION_3D_H_ */
