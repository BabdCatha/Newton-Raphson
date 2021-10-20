//
// Created by BabdCatha on 19/10/2021.
//
//The Polynomial class is used to represent a polynomial. It is created using its degree and a list of its roots.
//It is therefore represented at first in a factorized way. It is expanded in the computeCoefficients method, in order
//to simplify the calculation and evaluation of its derivative.

#ifndef NEWTON_RAPHSON_POLYNOMIAL_H
#define NEWTON_RAPHSON_POLYNOMIAL_H

#include "Root.h"

class Polynomial {
private:
	int degree;
	Root * roots;
	std::complex<double> * coefficients;
	bool isLeftMouseButtonPressed = false;

	void computeCoefficients();
public:
	Polynomial(int degree, Root * rootsList);
	std::complex<double> evaluate(std::complex<double> x);
	std::complex<double> evaluate_derivative(std::complex<double> x);
	bool getIsLeftMouseButtonPressed() const;
	void leftMouseButtonPressed(sf::Event event);
	void leftMouseButtonReleased();
	void update(sf::Event event);
	void drawRoots();
};

#endif //NEWTON_RAPHSON_POLYNOMIAL_H