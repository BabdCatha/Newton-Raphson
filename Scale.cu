//
// Created by BabdCatha on 19/10/2021.
//

#include "Scale.cuh"

Scale::Scale(cuFloatComplex cent, float w, float h, int screenWidth, int screenHeight) : center(cent) {
	width = w;
	height = h;
	this->screenWidth = screenWidth;
	this->screenHeight = screenHeight;
}

const cuFloatComplex &Scale::getCenter() const {
	return center;
}

__device__
const cuFloatComplex &Scale::getCenterD() const {
	return center;
}

float Scale::getWidth() const {
	return width;
}

float Scale::getHeight() const {
	return height;
}

int Scale::getScreenWidth() const {
	return screenWidth;
}

int Scale::getScreenHeight() const {
	return screenHeight;
}

__device__ float Scale::getWidthD() const {
	return width;
}

__device__ float Scale::getHeightD() const {
	return height;
}

__device__ int Scale::getScreenWidthD() const {
	return screenWidth;
}

__device__ int Scale::getScreenHeightD() const {
	return screenHeight;
}
