#include "Main.h"

sf::RenderWindow Main::window(sf::VideoMode(1280, 720), "Title");
std::vector<sf::Sprite>* Main::sprites = new std::vector<sf::Sprite>;
Scene Main::mainScene = Scene();
Time Main::timeObject = Time();

Main::Main()
{
	window.setFramerateLimit(60);
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
	}
}

void Main::Update()
{
	std::vector<sf::Sprite>* tiles = mainScene.Update();
	for (int i = 0; i < tiles->size(); i++) {
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
