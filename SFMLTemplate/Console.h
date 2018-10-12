#pragma once
#include "Resources.h"
#include "Scene.h"
class Console
{
public:
	Console();
	~Console();
	static void init();
	static void Write(char c);
	static void Command(Scene* scene);
	static void GetPrevious();
	static sf::RectangleShape bg;
	static sf::Text input;
	static bool enable;
private:
	static std::string previous;
};

