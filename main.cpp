#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

int main(){
	//Creating the main windows, in fullscreen WUXGA- mode
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "SFML works!", sf::Style::Fullscreen);

	//Creating the background image
	//It is stored in RAM, and this is where wi will update each pixel's color
	sf::Image backgroundImage;
	backgroundImage.create(1920, 1080, sf::Color(255,0,0));

	//Creating the background texture
	//This texture is a VRAM copy of the image, and is applied to a 1920x1080 sprite, to be drawn as the background
	sf::Texture backgroundTexture;
	backgroundTexture.loadFromImage(backgroundImage);

	//Creating the background sprite
	sf::Sprite backgroundSprite;
	backgroundSprite.setTexture(backgroundTexture);

	while (window.isOpen()){
		sf::Event event{};
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		window.draw(backgroundSprite);
		window.display();
	}

	return 0;
}
