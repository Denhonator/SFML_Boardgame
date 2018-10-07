#include "Main.h"

Main::Main()
{
	window.create(sf::VideoMode(1920, 1080), "Title");
	sprites = new std::vector<sf::Sprite>;
	mousePos = sf::Mouse::getPosition();
	view = window.getView();
	fixedView = sf::View(sf::FloatRect(0, 0, window.getSize().x*1.25f, window.getSize().y*1.25f));

	window.setFramerateLimit(55);
	mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
	Console::init();
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
			fixedView = sf::View(sf::FloatRect(0, 0, window.getSize().x*1.25f, window.getSize().y*1.25f));
			window.setView(view);
			mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::F1) {
				Console::enable = !Console::enable;
			}
			else if (event.key.code == sf::Keyboard::Return && Console::enable) {
				Console::Command(&mainScene);
			}
			else {
				mainScene.KeyPress(event.key.code);
			}
		}

		if (event.type == sf::Event::TextEntered) {
			if (Console::enable) {
				Console::Write(event.text.unicode);
			}
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
	window.setView(fixedView);
	for (unsigned int i = 0; i < mainScene.texts.size(); i++) {
		window.draw(mainScene.texts.at(i));
	}
	if (Console::enable) {
		Console::bg.setSize(sf::Vector2f(fixedView.getSize().x, fixedView.getSize().y/4));
		window.draw(Console::bg);
		window.draw(Console::input);
	}
	window.display();
	sprites->clear();
	window.setView(view);
}

sf::RenderWindow * Main::GetWindow()
{
	return &window;
}
