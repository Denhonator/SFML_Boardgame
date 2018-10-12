#pragma once
#include "Resources.h"
#include "Scene.h"
#include "Time.h"
#include <stdio.h>
#include "Console.h"
#include <thread>

class Main
{
public:
	Main();
	~Main();
	 void Events();
	 void Update();
	 void FixedUpdate();
	 void Draw();
	 sf::RenderWindow* GetWindow();
private:
	std::thread keyThread;
	std::thread mouseThread;
	sf::RenderWindow window;
	std::vector<sf::Sprite>* tiles;
	std::vector<sf::Sprite>* sprites;
	Scene mainScene;
	Time timeObject;
	sf::View view;
	sf::View fixedView;
	sf::Vector2i mousePos;
};

