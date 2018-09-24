#pragma once
#include <SFML/Graphics.hpp>
#include "Scene.h"
#include "Time.h"
#include <stdio.h>

class Main
{
public:
	Main();
	~Main();
	static void Events();
	static void Update();
	static void FixedUpdate();
	static void Draw();
	static sf::RenderWindow* GetWindow();
private:
	static sf::RenderWindow window;
	static std::vector<sf::Sprite>* sprites;
	static Scene mainScene;
	static Time timeObject;
};

