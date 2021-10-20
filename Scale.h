//
// Created by BabdCatha on 19/10/2021.
//

//The scale is an object used to store data relative to the area of the complex plane currently drawn on the screen.
//It is notably used to convert the XY coordinates of points on the screen to their complex value.

#ifndef NEWTON_RAPHSON_SCALE_H
#define NEWTON_RAPHSON_SCALE_H

#include <complex>

class Scale {
private:
	std::complex<double> center;
	double width;
	double height;
	int screenWidth;
	int screenHeight;
public:
	const std::complex<double> &getCenter() const;
	double getWidth() const;
	double getHeight() const;
	double getScreenWidth() const;
	double getScreenHeight() const;
	Scale(std::complex<double> cent, double w, double h, int screenWidth, int screenHeight);
};

#endif //NEWTON_RAPHSON_SCALE_H