#include "Main.h"
#include <thread>

void FixedLoop() {
	sf::Clock clock;

	while (Main::GetWindow()->isOpen()) {
		if (clock.getElapsedTime().asMilliseconds() > 16) {
			clock.restart();
			Main::FixedUpdate();
		}
		else {
			sf::sleep(sf::milliseconds(1));
		}
	}
}

int main()
{
	Main game;
	std::thread loop(FixedLoop);

	while (Main::GetWindow()->isOpen())
	{
		Main::Events();
		Main::Update();
		Main::Draw();
	}
	loop.join();
	return 0;
}