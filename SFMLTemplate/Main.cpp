#include "Main.h"

Main::Main()
{
	window.create(sf::VideoMode(1920, 1080), "Title");
	sprites = new std::vector<sf::Sprite>;
	mousePos = sf::Mouse::getPosition();
	mouseTile = sf::Vector2i(0, 0);

	window.setFramerateLimit(60);
	mainScene.SetTileSize(window.getSize().y / Constants::boardSize);
}

Main::~Main()
{

}

float clamp(float val, float min, float max) {
	return std::max(std::min(val, max), min);
}
sf::Vector2i LimitToBoard(sf::Vector2i tile) {
	return tile.x < Constants::boardSize&&tile.x >= 0 && tile.y < Constants::boardSize&&tile.y >= 0 ? tile : sf::Vector2i(-1,-1);
}

void Main::Events()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (!mainScene.GetBoard().refresh) {
			sf::FloatRect b = tiles->at(0).getGlobalBounds();
			if (event.type == sf::Event::Resized) {
				window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
				mainScene.SetTileSize(window.getSize().y / Constants::boardSize);
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
				tiles->at(0).scale(sf::Vector2f(1 + event.mouseWheelScroll.delta / 40, 1 + event.mouseWheelScroll.delta / 40));
				float delta = tiles->at(0).getGlobalBounds().width - b.width;
				b = tiles->at(0).getGlobalBounds();
				tiles->at(0).move(sf::Vector2f(-delta * 0.5f, -delta * 0.5f));
			}
			if (event.type == sf::Event::MouseMoved) {
				sf::Vector2i mouseDelta = sf::Vector2i(event.mouseMove.x, event.mouseMove.y) - mousePos;
				mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
					tiles->at(0).move(sf::Vector2f(mouseDelta.x, mouseDelta.y));
				}
				else {
					mouseTile = LimitToBoard(sf::Vector2i(Constants::boardSize * (mousePos.x - b.left) / b.width, Constants::boardSize * (mousePos.y - b.top) / b.height));
					//printf("%d, %d\n", mouseTile.x, mouseTile.y);
				}
			}
		}
	}
}

void Main::Update()
{
	tiles = mainScene.Update();
	float tileSize = tiles->at(0).getGlobalBounds().width / Constants::boardSize;
	tiles->at(1).setPosition(tiles->at(0).getPosition() + sf::Vector2f(mouseTile.x * tileSize, mouseTile.y*tileSize));
	tiles->at(1).setScale(sf::Vector2f(tileSize / tiles->at(1).getLocalBounds().width, tileSize / tiles->at(1).getLocalBounds().height));
	for (unsigned short i = 0; i < tiles->size(); i++) {
		sprites->push_back(tiles->at(i));
	}
	Time::TakeTime();
}

void Main::FixedUpdate()
{
}

void Main::Draw()
{
	window.clear();
	for (unsigned int i = 0; i < sprites->size(); i++) {
		window.draw(sprites->at(i));
	}
	sprites->clear();
	window.display();
}

sf::RenderWindow * Main::GetWindow()
{
	return &window;
}
