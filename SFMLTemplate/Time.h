#pragma once
#include <SFML/Graphics.hpp>
#include <stdio.h>

class Time
{
public:
	Time();
	~Time();
	static void TakeTime();
	static float GetTime();
	static float Mult();
private:
	static sf::Clock clock;
	static float deltaTime;
};

