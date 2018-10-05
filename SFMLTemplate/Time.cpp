#include "Time.h"

sf::Clock Time::clock = sf::Clock();
float Time::deltaTime = 0;

Time::Time()
{

}

Time::~Time()
{
}

void Time::TakeTime()
{
	deltaTime = clock.getElapsedTime().asMicroseconds()*0.000001f;
	//printf("Time::Mult: %f\n", Time::Mult());
	clock.restart();
}

float Time::GetTime()
{
	return clock.getElapsedTime().asMicroseconds()*0.000001f;
}

float Time::Mult()
{
	return deltaTime * 60;
}
