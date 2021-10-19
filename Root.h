//
// Created by BabdCatha on 19/10/2021.
//

//The Root class is used for two things:
//1) Be used as a zero in a polynomial expression.
//2) Be used to display on the screen the different roots of the current polynomial expression, and allow the user to
//   Modify them using the mouse.

#ifndef NEWTON_RAPHSON_ROOT_H
#define NEWTON_RAPHSON_ROOT_H

#include <complex>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Scale.h"

#define ROOT_RADIUS 10

class Root {
private:
	//(X,Y) = (0,0) corresponds to top left corner of the screen
	int coordX, coordY;
	std::complex<double> value;
	Scale scale;
	sf::RenderWindow * window;
	sf::CircleShape sprite;
	bool selected;
public:
	bool isSelected() const;
	void setSelected(bool n_selected);
	Root(int coordX, int coordY, const Scale &scale, sf::RenderWindow * window);
	void draw();
	bool overlaps(int e_x, int e_y) const;
	void updatePosition(int n_x, int n_y);
};

#endif //NEWTON_RAPHSON_ROOT_H