/**
 * @file    interpolation.h
 *
 * @date Oct 17, 2013
 * @author Andrey Belomutskiy, (c) 2012-2014
 */

#ifndef INTERPOLATION_3D_H_
#define INTERPOLATION_3D_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

int findIndex(float array[], int size, float value);
float interpolate(float x1, float y1, float x2, float y2, float x);
float interpolate2d(float value, float bin[], float values[], int size);
float interpolate3d(float x, float xBin[], int xBinSize, float y, float yBin[], int yBinSize, float* map[]);
void setTableValue(float bins[], float values[], int size, float key, float value);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INTERPOLATION_3D_H_ */
