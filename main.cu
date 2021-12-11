#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <cuComplex.h>

#include "Root.cuh"
#include "Scale.cuh"
#include "Polynomial.cuh"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080
#define MAX_FRAMERATE 60

#define NUMBER_OF_ITERATIONS 15
//#define BASE_DEGREE 3

cuFloatComplex XYtoComplex(int x, int y, Scale &scale);
void initScreen(Scale &scale, cuFloatComplex **complexScreen);
void getColorMap(Polynomial *P, cuFloatComplex **complexScreen, sf::Image *image);
__global__ void performNewtonStep(Polynomial *P, cuFloatComplex **complexScreen);

int main(){

	//Creating the complex screen, and making it accessible to the device
	cuFloatComplex ** complexScreen;
	cudaMallocManaged(&complexScreen, SCREEN_WIDTH*sizeof(cuFloatComplex&));
	for(int i = 0; i < SCREEN_WIDTH; i++){
		cudaMallocManaged(&(complexScreen[i]), SCREEN_HEIGHT*sizeof(cuFloatComplex));
	}

	//Creating the main window, in fullscreen WUXGA- mode
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
	Scale currentScale(make_cuFloatComplex(0, 0), 1*5, 1*2.8125, SCREEN_WIDTH, SCREEN_HEIGHT);

	//Test variables
	Root z1(make_cuFloatComplex(1, 0), currentScale, &window, sf::Color(0, 0, 255));
	Root z2(make_cuFloatComplex(-0.5, 0.86602540378443), currentScale, &window, sf::Color(255, 0, 0));
	Root z3(make_cuFloatComplex(-0.5, -0.86602540378443), currentScale, &window, sf::Color(0, 255, 0));
	Root z4(100, 400, currentScale, &window, sf::Color(61, 15, 97));
	Root z5(100, 500, currentScale, &window, sf::Color(97, 15, 72));

	Root liste[] = {z1, z2, z3};

	Polynomial *P;
	cudaMallocManaged(&P, sizeof(Polynomial));
	*P = Polynomial(3, liste);

	//Calculating variables for the number of blocks and threads
	int blockSize = 256;
	int numBlocks = (SCREEN_WIDTH*SCREEN_HEIGHT + blockSize - 1) / blockSize;

	while(window.isOpen()){
		sf::Event event{};
		while(window.pollEvent(event)){
			//If the user quits the app, we close it
			if(event.type == sf::Event::Closed) {
				window.close();
			}

			//If the user performs a left click, we set up the corresponding flag in the selected root, if any
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left){
				P->leftMouseButtonPressed(event);
			}
			//If the user releases the left mouse button, we clear the corresponding flag
			if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left){
				P->leftMouseButtonReleased();
			}

			//If the mouse moves while the left mouse button is pressed, we make each root handle the movement
			if(event.type == sf::Event::MouseMoved && P->getIsLeftMouseButtonPressed()){
				P->update(event);
			}

			//If we want to re-draw the background, on a right click
			if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right){
				initScreen(currentScale, complexScreen);

				for(int i = 0; i < SCREEN_WIDTH; i++){
					cudaMemPrefetchAsync(complexScreen[i], SCREEN_HEIGHT*sizeof(cuFloatComplex), 0);
				}
				cudaMemPrefetchAsync(complexScreen, SCREEN_WIDTH*sizeof(cuFloatComplex*), 0);

				cudaDeviceSynchronize();

				for(int i = 0; i < NUMBER_OF_ITERATIONS; i++){

					performNewtonStep<<<numBlocks,blockSize>>>(P, complexScreen);
					cudaDeviceSynchronize();

				}

				for(int i = 0; i < SCREEN_WIDTH; i++){
					cudaMemPrefetchAsync(complexScreen[i], SCREEN_HEIGHT*sizeof(cuFloatComplex), cudaCpuDeviceId);
				}
				cudaMemPrefetchAsync(complexScreen, SCREEN_WIDTH*sizeof(cuFloatComplex*), cudaCpuDeviceId);

				cudaDeviceSynchronize();

				getColorMap(P, complexScreen, &backgroundImage);
				backgroundTexture.update(backgroundImage);

			}
		}

		//Cleaning the window of any previous renderings
		window.clear();

		//Drawing the background image
		window.draw(backgroundSprite);

		//This is where the roots should be drawn
		P->drawRoots();

		//Once all the modifications have been done, the window is drawn again
		window.display();

	}

	return 0;
}

cuFloatComplex XYtoComplex(int x, int y, Scale &scale) {

	cuFloatComplex res = scale.getCenter();

	res = cuCaddf(res, make_cuFloatComplex((x - (scale.getScreenWidth() / 2)) * (2 * (double)scale.getWidth() / (double)scale.getScreenWidth()), 0));
	res = cuCaddf(res, make_cuFloatComplex(0, ((scale.getScreenHeight() / 2) - y) * (2 * (double)scale.getHeight() / (double)scale.getScreenHeight())));

	return res;
}

void initScreen(Scale &scale, cuFloatComplex **complexScreen){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			complexScreen[i][j] = XYtoComplex(i, j, scale);
		}
}

void getColorMap(Polynomial *P, cuFloatComplex **complexScreen, sf::Image *image){
	for (int i = 0; i < SCREEN_WIDTH; i++)
		for (int j = 0; j < SCREEN_HEIGHT; j++) {
			image->setPixel(i, j, P->findClosestRootColor(complexScreen[i][j]));
		}
}

__global__
void performNewtonStep(Polynomial *P, cuFloatComplex **complexScreen){

	unsigned int index = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int stride = blockDim.x * gridDim.x;

	for(unsigned int i = index; i < SCREEN_WIDTH*SCREEN_HEIGHT; i += stride){

		unsigned int x = i%SCREEN_WIDTH;
		unsigned int y = i/SCREEN_WIDTH;

		cuFloatComplex alpha = complexScreen[x][y];

		cuFloatComplex val = make_cuFloatComplex(0, 0);
		P->evaluate(alpha, &val);

		cuFloatComplex valD = make_cuFloatComplex(0, 0);
		P->evaluate_derivative(alpha, &valD);

		complexScreen[x][y] = cuCsubf(complexScreen[x][y], cuCdivf(val, valD));
	}

}