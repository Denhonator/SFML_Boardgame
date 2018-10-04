#include "Time.h"

sf::Clock Time::clock = sf::Clock();
sf::Time Time::time = sf::Time();
float Time::deltaTime = 0;

Time::Time()
{
	time = clock.getElapsedTime();
}

Time::~Time()
{
}

void Time::TakeTime()
{
	deltaTime = (clock.getElapsedTime() - time).asMicroseconds()*0.000001f;
	//printf("Time::Mult: %f\n", Time::Mult());
	time = clock.getElapsedTime();
}

float Time::Mult()
{
	return deltaTime * 60;
}
