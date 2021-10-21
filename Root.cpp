//
// Created by BabdCatha on 19/10/2021.
//

#include <iostream>
#include "Root.h"

using namespace std::complex_literals;

Root::Root(int coordX, int coordY, const Scale &scale, sf::RenderWindow * window, sf::Color color) : coordX(coordX), coordY(coordY), scale(scale){
	value = XYtoComplex(coordX, coordY);
	this->window = window;
	sprite = sf::CircleShape(ROOT_RADIUS);
	rootColor = color;
	sprite.setPosition((float)coordX, (float)coordY);
	sprite.setFillColor(sf::Color::Blue);
	selected = false;
}

Root::Root() : scale(0, 1, 1, 1920, 1080){
	value = 0;
	coordX = 0;
	coordY = 0;
	this->window = nullptr;
	sprite = sf::CircleShape(ROOT_RADIUS);
	sprite.setPosition((float)coordX, (float)coordY);
	sprite.setFillColor(sf::Color::Blue);
	rootColor = sf::Color::Blue;
	selected = false;
}

void Root::draw() {
	window->draw(sprite);
}

bool Root::overlaps(int e_x, int e_y) const {
	if(abs(e_x - ROOT_RADIUS - coordX) < ROOT_RADIUS && abs(e_y - ROOT_RADIUS - coordY) < ROOT_RADIUS){
		return true;
	}
	return false;
}

void Root::updatePosition(int n_x, int n_y) {
	coordX = n_x - ROOT_RADIUS;
	coordY = n_y - ROOT_RADIUS;
	value = XYtoComplex(coordX, coordY);
	sprite.setPosition((float)coordX, (float)coordY);
}

bool Root::isSelected() const {
	return selected;
}

void Root::setSelected(bool n_selected) {
	Root::selected = n_selected;
}

std::complex<double> Root::getValue() {
	return value;
}

std::complex<double> Root::XYtoComplex(int x, int y) {
	std::complex<double> res = scale.getCenter();
	res += (x - (scale.getScreenWidth() / 2)) * (2 * (double)scale.getWidth() / (double)scale.getScreenWidth());
	res += 1i * ((scale.getScreenHeight() / 2) - y) * (2 * (double)scale.getHeight() / (double)scale.getScreenHeight());
	return res;
}

sf::Color Root::getRootColor() {
	return rootColor;
}