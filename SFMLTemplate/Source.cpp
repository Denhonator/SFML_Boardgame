#include "Main.h"
#include <thread>

void FixedLoop(Main* game) {
	sf::Clock clock;

	while (game->GetWindow()->isOpen()) {
		if (clock.getElapsedTime().asMilliseconds() > 16) {
			clock.restart();
			game->FixedUpdate();
		}
		else {
			sf::sleep(sf::milliseconds(1));
		}
	}
}

int main()
{
	Resources res;
	Messages mes;
	Main game;
	std::thread loop(FixedLoop, &game);

	while (game.GetWindow()->isOpen())
	{
		Time::TakeTime();
		game.Events();
		//printf("Events: %f\n", Time::GetTime());
		game.Update();
		//printf("Update: %f\n", Time::GetTime());
		game.Draw();
		//printf("Draw: %f\n", Time::GetTime());
	}
	loop.join();
	return 0;
}