//
// Created by BabdCatha on 19/10/2021.
//
//The Polynomial class is used to represent a polynomial. It is created using its degree and a list of its roots.
//It is therefore represented at first in a factorized way. It is expanded in the computeCoefficients method, in order
//to simplify the calculation and evaluation of its derivative.

#ifndef NEWTON_RAPHSON_POLYNOMIAL_CUH
#define NEWTON_RAPHSON_POLYNOMIAL_CUH

#include "Root.cuh"

class Polynomial {
private:
	int degree;
	Root * roots;
	cuDoubleComplex * coefficients;
	bool isLeftMouseButtonPressed = false;

	void computeCoefficients();
public:
	Polynomial(int degree, Root * rootsList);
	__device__ void evaluate(cuDoubleComplex x, cuDoubleComplex *res);
	__device__ void evaluate_derivative(cuDoubleComplex x, cuDoubleComplex *res);
	bool getIsLeftMouseButtonPressed() const;
	void leftMouseButtonPressed(sf::Event event);
	void leftMouseButtonReleased();
	void update(sf::Event event);
	void drawRoots();
	sf::Color findClosestRootColor(cuDoubleComplex z);
};

#endif //NEWTON_RAPHSON_POLYNOMIAL_CUH