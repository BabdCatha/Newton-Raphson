#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>

#include "Root.h"
#include "Scale.h"
#include "Polynomial.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_FRAMERATE 60

#define NUMBER_OF_ITERATIONS 1
#define BASE_DEGREE 3

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
	Scale currentScale(0, 1, 1, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Test variables
	Root z1(960, 540, currentScale, &window);
	Root z2(100, 100, currentScale, &window);

	Root liste[] = {z1, z2};

	Polynomial P(2, liste);

	while(window.isOpen()){
		sf::Event event{};
		while(window.pollEvent(event)){
			//If the user quits the app, we close it
			if(event.type == sf::Event::Closed) {
				window.close();
			}

			//If the user performs a left click, we set up the corresponding flag in the selected root, if any
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				P.leftMouseButtonPressed(event);
			}
			//If the user releases the left mouse button, we clear the corresponding flag
			if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
				P.leftMouseButtonReleased();
			}

			//If the mouse moves while the left mouse button is pressed, we make each root handle the movement
			if(event.type == sf::Event::MouseMoved && P.getIsLeftMouseButtonPressed()){
				P.update(event);
			}
		}

		//Cleaning the window of any previous renderings
		window.clear();

		//Drawing the background image
		window.draw(backgroundSprite);

		//This is where the roots should be drawn
		P.drawRoots();

		//Once all the modifications have been done, the window is drawn again
		window.display();
	}

	return 0;
}
