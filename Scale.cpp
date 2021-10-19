//
// Created by BabdCatha on 19/10/2021.
//

#include "Scale.h"

Scale::Scale(std::complex<double> cent, double w, double h) {
	center = cent;
	width = w;
	height = h;
}

const std::complex<double> &Scale::getCenter() const {
	return center;
}

double Scale::getWidth() const {
	return width;
}

double Scale::getHeight() const {
	return height;
}

double Scale::getScreenWidth() const {
	return screenWidth;
}

double Scale::getScreenHeight() const {
	return screenHeight;
}

