//
// Created by BabdCatha on 19/10/2021.
//

#include <iostream>
#include "Polynomial.cuh"

__device__
cuDoubleComplex expRap(cuDoubleComplex x, int i){
	if(i==0){
		return make_cuDoubleComplex(1, 0);
	}else if(i == 1){
		return x;
	}else if(i%2 == 0){
		cuDoubleComplex temp = expRap(x, i/2);
		return cuCmul(temp, temp);
	}else{
		cuDoubleComplex temp = expRap(x, i/2);
		return cuCmul(x, cuCmul(temp, temp));
	}
}

Polynomial::Polynomial(int degree, Root * rootsList) {
	this->degree = degree;
	roots = new Root[degree];
	for(int i = 0; i < degree; i++){
		roots[i] = rootsList[i];
	}
	coefficients = new cuDoubleComplex[degree+1];
	computeCoefficients();
}

__device__
void Polynomial::evaluate(cuDoubleComplex x, cuDoubleComplex *res) {
	*res = make_cuDoubleComplex(0, 0);
	for(int i = 0; i < degree+1; i++){
		//res += coefficients[i] * expRap(x, i);
		*res = cuCadd(*res, cuCmul(coefficients[i], expRap(x, i)));
	}
}

__device__
void Polynomial::evaluate_derivative(cuDoubleComplex x, cuDoubleComplex *res) {
	*res = make_cuDoubleComplex(0, 0);
	for(int i = 1; i < degree+1; i++){
		//res += ((double)i*coefficients[i] * std::pow(x, i-1));
		*res = cuCadd(*res, cuCmul(make_cuDoubleComplex(i, 0), cuCmul(coefficients[i], expRap(x, i-1))));
	}
}

void Polynomial::computeCoefficients(){

	for(int i = 0; i < degree+1; i++){
		coefficients[i] = make_cuDoubleComplex(0, 0);
	}

	coefficients[1] = make_cuDoubleComplex(1, 0);
	//coefficients[0] = (double)-1*roots[0].getValue();
	coefficients[0] = cuCmul(make_cuDoubleComplex(-1, 0), roots[0].getValue());

	cuDoubleComplex tempList[degree+1];

	for(int i = 1; i < degree; i++){

		for(int j = 1; j < degree+1 ; j++){
			tempList[j] = coefficients[j-1];
		}
		tempList[0] = make_cuDoubleComplex(0, 0);

		for(int j = 0; j < degree+1; j++){
			//coefficients[j] *= (double)-1*roots[i].getValue();
			//coefficients[j] += tempList[j];

			coefficients[j] = cuCmul(coefficients[j],cuCmul(make_cuDoubleComplex(-1, 0), roots[i].getValue()));
			coefficients[j] = cuCadd(coefficients[j],tempList[j]);
		}
	}
}

bool Polynomial::getIsLeftMouseButtonPressed() const {
	return isLeftMouseButtonPressed;
}

//They first return a boolean indicating whether they are the one that was pressed. This is used to make
//sure that when the mouse is over several roots, only one is moved at any time.
void Polynomial::leftMouseButtonPressed(sf::Event event) {
	isLeftMouseButtonPressed = true;
	for(int i = 0; i < degree; i++){
		if(roots[i].overlaps(event.mouseButton.x, event.mouseButton.y)){
			roots[i].setSelected(true);
			break;
		}
	}
	//Debug code
	for(int i = 0; i < degree+1; i++){
		std::cout << i << " : " << cuCreal(coefficients[i]) << "+i" << cuCimag(coefficients[i]) << std::endl;
	}
}

void Polynomial::leftMouseButtonReleased() {
	isLeftMouseButtonPressed = false;
	//We release every root
	for(int i = 0; i < degree; i++){
		if(roots[i].isSelected()){
			roots[i].setSelected(false);
		}
	}
}

void Polynomial::update(sf::Event event) {
	for(int i = 0; i < degree; i++){
		if(roots[i].isSelected()){
			roots[i].updatePosition(event.mouseMove.x, event.mouseMove.y);
		}
	}
	computeCoefficients();
}

void Polynomial::drawRoots() {
	for(int i = 0; i < degree; i++){
		roots[i].draw();
	}
}

sf::Color Polynomial::findClosestRootColor(cuDoubleComplex z){
	double dist = HUGE_VAL;
	sf::Color res;
	for(int i = 0; i < degree; i++){
		//double new_dist = std::abs(z - roots[i].getValue());
		double new_dist = cuCabs(cuCsub(z, roots[i].getValue()));
		if(new_dist < dist){
			dist = new_dist;
			res = roots[i].getRootColor();
		}
	}

	//Variable to be adjusted to make sure that the value is actually close to the root
	if(dist > 100.0){
		res = sf::Color::Black;
	}

	return res;

}