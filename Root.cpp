//
// Created by BabdCatha on 19/10/2021.
//

#include "Root.h"

using namespace std::complex_literals;

Root::Root(int coordX, int coordY, const Scale &scale, sf::RenderWindow * window) : coordX(coordX), coordY(coordY), scale(scale){
	value = scale.getCenter();
	value += (coordX - scale.getScreenWidth()/2) * (double)((scale.getWidth() / scale.getScreenWidth()))
			+ 1i * (double)((scale.getScreenHeight()/2 - coordY) * (scale.getHeight() / scale.getScreenHeight()));
	this->window = window;
	sprite = sf::CircleShape(ROOT_RADIUS);
	sprite.setPosition((float)coordX, (float)coordY);
	sprite.setFillColor(sf::Color::Blue);

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
	value = scale.getCenter();
	value += (coordX - scale.getScreenWidth()/2) * (double)((scale.getWidth() / scale.getScreenWidth()))
			 + 1i * (double)((scale.getScreenHeight()/2 - coordY) * (scale.getHeight() / scale.getScreenHeight()));
	sprite.setPosition((float)coordX, (float)coordY);
}

bool Root::isSelected() const {
	return selected;
}

void Root::setSelected(bool n_selected) {
	Root::selected = n_selected;
}
