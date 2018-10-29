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
	void MouseEvents();
	void KeyEvents();
	void Events();
	void Update();
	void FixedUpdate();
	void Draw();
	sf::RenderWindow* GetWindow();
private:
	std::thread keyThread;
	std::thread mouseThread;
	std::vector<sf::Keyboard::Key> keys;
	short click;
	sf::RenderWindow window;
	std::vector<sf::Sprite>* tiles;
	std::vector<sf::Sprite>* sprites;
	Scene mainScene;
	Time timeObject;
	sf::View view;
	sf::View fixedView;
	sf::Vector2i mousePos;
	float idleTimer = 60;
};

