#include "Main.h"

Main::Main()
{
	window.create(sf::VideoMode(1920, 1080), "Title");
	sprites = new std::vector<sf::Sprite>;
	mousePos = sf::Mouse::getPosition();
	view = window.getView();

	window.setFramerateLimit(55);
	mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
}

Main::~Main()
{

}

float clamp(float val, float min, float max) {
	return std::max(std::min(val, max), min);
}

void Main::Events()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::Resized) {
			view.setSize(sf::Vector2f(window.getSize()));
			window.setView(view);
			mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
		}

		if (event.type == sf::Event::MouseWheelScrolled) {
			view.zoom(1-event.mouseWheelScroll.delta/30);
			window.setView(view);
		}

		if (event.type == sf::Event::MouseMoved) {
			sf::Vector2i mouseDelta = sf::Vector2i(event.mouseMove.x, event.mouseMove.y) - mousePos;
			mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
				view.move(sf::Vector2f(-mouseDelta));
				window.setView(view);
			}
			else {
				mainScene.MouseHover(sf::Vector2i(window.mapPixelToCoords(mousePos)));
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
				mainScene.Click();
		}
	}
}

void Main::Update()
{
	mainScene.Update();
}

void Main::FixedUpdate()
{
}

void Main::Draw()
{
	window.clear();
	for (unsigned int i = 0; i < mainScene.drawTiles.size(); i++) {
		window.draw(mainScene.tiles.at(mainScene.drawTiles.at(i)));
	}
	for (unsigned int i = 0; i < mainScene.drawUnits.size(); i++) {
		window.draw(mainScene.units.at(mainScene.drawUnits.at(i)).sprite);
	}
	for (unsigned int i = 0; i < mainScene.ui.size(); i++) {
		window.draw(mainScene.ui.at(i));
	}
	window.display();
	sprites->clear();
}

sf::RenderWindow * Main::GetWindow()
{
	return &window;
}
