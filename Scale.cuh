//
// Created by BabdCatha on 19/10/2021.
//

//The scale is an object used to store data relative to the area of the complex plane currently drawn on the screen.
//It is notably used to convert the XY coordinates of points on the screen to their complex value.

#ifndef NEWTON_RAPHSON_SCALE_CUH
#define NEWTON_RAPHSON_SCALE_CUH

#include <cuComplex.h>

class Scale {
private:
	cuFloatComplex center;
	float width;
	float height;
	int screenWidth;
	int screenHeight;
public:
	const cuFloatComplex &getCenter() const;
	float getWidth() const;
	float getHeight() const;
	int getScreenWidth() const;
	int getScreenHeight() const;
	Scale(cuFloatComplex cent, float w, float h, int screenWidth, int screenHeight);

	__device__ const cuFloatComplex  &getCenterD() const;
	__device__ float getWidthD() const;
	__device__ float getHeightD() const;
	__device__ int getScreenWidthD() const;
	__device__ int getScreenHeightD() const;
};

#endif //NEWTON_RAPHSON_SCALE_CUH