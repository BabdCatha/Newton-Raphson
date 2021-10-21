#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <complex>

#include "Root.h"
#include "Scale.h"
#include "Polynomial.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_FRAMERATE 60

#define NUMBER_OF_ITERATIONS 20
#define BASE_DEGREE 3

std::complex<double> complexScreen[SCREEN_WIDTH][SCREEN_HEIGHT];
sf::Color screen[SCREEN_WIDTH][SCREEN_HEIGHT];

std::complex<double> XYtoComplex(int x, int y, Scale &scale);
int * complextoXY(std::complex<double> z, Scale &scale);
void initScreen(Scale &scale);
void getColorMap(Polynomial &P);
void updateBackgroundImage(sf::Image &image);
void performNewtonStep(Polynomial &P);

using namespace std::complex_literals;

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
	Scale currentScale(0, 1*5, 1*2.8125, SCREEN_WIDTH, SCREEN_HEIGHT);

	int * r1 = complextoXY(1, currentScale);
	int * r2 = complextoXY(std::exp(1i*3.1415926535/1.5), currentScale);
	int * r3 = complextoXY(std::exp(1i*3.1415926535/0.75), currentScale);

	//Test variables
	Root z1(r1[0], r1[1], currentScale, &window, sf::Color(97, 20, 15));
	Root z2(r2[0], r2[1], currentScale, &window, sf::Color(49, 97, 15));
	Root z3(r3[0], r3[1], currentScale, &window, sf::Color(15, 89, 97));
	Root z4(100, 400, currentScale, &window, sf::Color(61, 15, 97));
	Root z5(100, 500, currentScale, &window, sf::Color(97, 15, 72));

	Root liste[] = {z1, z2, z3};

	Polynomial P(3, liste);

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

			//If we want to re-draw the background, on a right click
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
				initScreen(currentScale);

				for(int i = 0; i < NUMBER_OF_ITERATIONS; i++){
					performNewtonStep(P);
				}

				getColorMap(P);
				updateBackgroundImage(backgroundImage);
				backgroundTexture.update(backgroundImage);
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

std::complex<double> XYtoComplex(int x, int y, Scale &scale) {
	std::complex<double> res = scale.getCenter();
	res += (x - (scale.getScreenWidth() / 2)) * (2 * (double)scale.getWidth() / (double)scale.getScreenWidth());
	res += (1i * ((scale.getScreenHeight() / 2) - y) * (2 * (double)scale.getHeight() / (double)scale.getScreenHeight()));
	return res;
}

void initScreen(Scale &scale){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			complexScreen[i][j] = XYtoComplex(i, j, scale);
		}
}

void getColorMap(Polynomial &P){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			screen[i][j] = P.findClosestRootColor(complexScreen[i][j]);
		}
}

void updateBackgroundImage(sf::Image &image){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			image.setPixel(i, j, screen[i][j]);
		}
}

void performNewtonStep(Polynomial &P){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			std::complex<double> alpha = complexScreen[i][j];
			complexScreen[i][j] -= (P.evaluate(alpha)/P.evaluate_derivative(alpha));
		}
}

int * complextoXY(std::complex<double> z, Scale &scale){
	int * res = (int*)malloc(2*sizeof(int));
	res[0] = ((z.real()/scale.getWidth())*scale.getScreenWidth()) + scale.getScreenWidth()/2;
	res[1] = ((z.imag()/scale.getHeight())*scale.getScreenHeight()) + scale.getScreenHeight()/2;

	return res;
}