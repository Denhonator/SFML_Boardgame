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
	Main game;
	std::thread loop(FixedLoop, &game);

	while (game.GetWindow()->isOpen())
	{
		game.Events();
		game.Update();
		game.Draw();
	}
	loop.join();
	return 0;
}