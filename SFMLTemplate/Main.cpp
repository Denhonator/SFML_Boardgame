#include "Main.h"

sf::RenderWindow Main::window(sf::VideoMode(1920, 1080), "Title");
std::vector<sf::Sprite>* Main::sprites = new std::vector<sf::Sprite>;
Scene Main::mainScene = Scene();
Time Main::timeObject = Time();
sf::View Main::view = sf::View();

Main::Main()
{
	window.setFramerateLimit(60);
	mainScene.SetTileSize(window.getSize().y / Constants::boardSize);
	//window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
}

Main::~Main()
{

}

void Main::Events()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
		if (event.type == sf::Event::Resized) {
			window.setView(sf::View(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y)));
			mainScene.SetTileSize(window.getSize().y / Constants::boardSize);
		}
	}
}

void Main::Update()
{
	std::vector<sf::Sprite>* tiles = mainScene.Update();
	for (unsigned short i = 0; i < tiles->size(); i++) {
		sprites->push_back(tiles->at(i));
	}
	float boardScale = window.getSize().y / sprites->at(0).getGlobalBounds().height;
	sprites->at(0).setScale(sf::Vector2f(boardScale, boardScale));
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
