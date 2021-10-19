#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Root.h"
#include "Scale.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_FRAMERATE 60

int main(){
	//Creating the main windows, in fullscreen WUXGA- mode
	sf::RenderWindow window(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Newton-Raphson fractal", sf::Style::Fullscreen);
	window.setFramerateLimit(MAX_FRAMERATE); //60Hz max

	//Creating the background image
	//It is stored in RAM, and this is where we will update each pixel's color
	sf::Image backgroundImage;
	backgroundImage.create(SCREEN_WIDTH, SCREEN_HEIGHT, sf::Color(255,0,0));

	//Creating the background texture
	//This texture is a VRAM copy of the image, and is applied to a 1920x1080 sprite, to be drawn as the background
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromImage(backgroundImage);

	//Creating the background sprite
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	//Creating the current scale
	Scale currentScale(0, 1, 1);
	Root z1(50, 50, currentScale, &window);

	bool isLeftMouseButtonPressed = false;

	while(window.isOpen()){
		sf::Event event{};
		while(window.pollEvent(event)){
			//If the user quits the app, we close it
			if(event.type == sf::Event::Closed)
				window.close();

			//If the user performs a left click, we set up the corresponding flag in the selected root, if any
			//They first return a boolean indicating whether they are the one that was pressed. This is used to make
			//sure that when the mouse is over several roots, only one is moved at any time.
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				isLeftMouseButtonPressed = true;

				//TODO: make it usable with a polynomial
				if(z1.overlaps(event.mouseButton.x, event.mouseButton.y)){
					z1.setSelected(true);
				}
			}
			//If the user releases the left mouse button, we clear the corresponding flag
			if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
				isLeftMouseButtonPressed = false;
				//We release every root
				if(z1.isSelected()){
					z1.setSelected(false);
				}
			}

			//If the mouse moves while the left mouse button is pressed, we make each root handle the movement
			if(event.type == sf::Event::MouseMoved && isLeftMouseButtonPressed){
				if(z1.isSelected()){
					z1.updatePosition(event.mouseMove.x, event.mouseMove.y);
				}
			}
		}

		//Cleaning the window of any previous renderings
		window.clear();

		//Drawing the background image
		window.draw(backgroundSprite);

		//This is where the roots should be drawn
		z1.draw();

		//Once all the modifications have been done, the window is drawn again
		window.display();
	}

	return 0;
}
