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
	double width;
	double height;
	int screenWidth;
	int screenHeight;
public:
	const cuFloatComplex &getCenter() const;
	double getWidth() const;
	double getHeight() const;
	double getScreenWidth() const;
	double getScreenHeight() const;
	Scale(cuFloatComplex cent, double w, double h, int screenWidth, int screenHeight);
};

#endif //NEWTON_RAPHSON_SCALE_CUH