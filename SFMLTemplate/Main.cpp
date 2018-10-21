#include "Main.h"

Main::Main()
{
	window.create(sf::VideoMode(1920, 1080), "Title");
	sprites = new std::vector<sf::Sprite>;
	mousePos = sf::Mouse::getPosition();
	view = window.getView();
	fixedView = sf::View(sf::FloatRect(0, 0, 2400, 2400 / ((float)window.getSize().x / (float)window.getSize().y)));
	Constants::UpdateView(fixedView);
	Messages::UpdatePos();

	window.setFramerateLimit(55);
	mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
	Console::init();
}

Main::~Main()
{
	if (keyThread.joinable())
		keyThread.join();
	if (mouseThread.joinable())
		mouseThread.join();
}

float clamp(float val, float min, float max) {
	return std::max(std::min(val, max), min);
}

void Main::Events()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (event.type == sf::Event::Resized) {
			view.setSize(sf::Vector2f(window.getSize()));
			fixedView = sf::View(sf::FloatRect(0, 0, 2400, 2400 / ((float)window.getSize().x / (float)window.getSize().y)));
			Constants::UpdateView(fixedView);
			window.setView(view);
			mainScene.SetTileSize(window.getSize().y / mainScene.GetBoard()->boardSize.y);
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::F1) {
				Console::enable = !Console::enable;
			}
			else if (event.key.code == sf::Keyboard::Return && Console::enable) {
				Console::Command(&mainScene);
			}
			else if (event.key.code == sf::Keyboard::Up && Console::enable) {
				Console::GetPrevious();
			}
			else if(!Console::enable) {
				if (keyThread.joinable() && !Messages::prompting)
					keyThread.join();
				if (keyThread.joinable()&&Messages::prompting) {
					;
				}
				else
					keyThread = std::thread(&Scene::KeyPress, &mainScene, event.key.code);
			}
		}

		if (event.type == sf::Event::TextEntered) {
			if (Console::enable) {
				Console::Write(event.text.unicode);
			}
			if (keyThread.joinable() && Messages::prompting) {
				Messages::SetInput(static_cast<char>(event.text.unicode));
			}
		}

		if (event.type == sf::Event::MouseWheelScrolled) {
			view.zoom(1-event.mouseWheelScroll.delta/30);
			window.setView(view);
		}

		if (event.type == sf::Event::MouseMoved) {
			sf::Vector2i mouseDelta = sf::Vector2i(event.mouseMove.x, event.mouseMove.y) - mousePos;
			mousePos = sf::Vector2i(event.mouseMove.x, event.mouseMove.y);
			mainScene.SetMousePos(window.mapPixelToCoords(mousePos, fixedView));
			if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
				view.move(sf::Vector2f(-mouseDelta));
				window.setView(view);
			}
			else {
				mainScene.MouseHover(sf::Vector2i(window.mapPixelToCoords(mousePos)));
			}
		}

		if (event.type == sf::Event::MouseButtonPressed) {
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (mouseThread.joinable() && !Messages::prompting)
					mouseThread.join();
				if (mouseThread.joinable() && Messages::prompting) {
					Messages::Click(window.mapPixelToCoords(mousePos, fixedView));
				}
				else
					mouseThread = std::thread(&Scene::Click, &mainScene);
			}
		}
	}
}

void Main::Update()
{
	mainScene.Update();
}

void Main::FixedUpdate()
{
	Messages::FadeNotice();
}

void Main::Draw()
{
	window.clear();
	for (unsigned int i = 0; i < mainScene.drawTiles.size(); i++) {
		window.draw(mainScene.tiles.at(mainScene.drawTiles.at(i)));
	}
	for (unsigned int i = 0; i < mainScene.units.size(); i++) {
		if (!mainScene.units.at(i).Dead()) {
			window.draw(mainScene.units.at(i).sprite);
			window.draw(mainScene.units.at(i).bars);
		}
	}
	for (unsigned int i = 0; i < mainScene.boardUi.size(); i++) {
		window.draw(mainScene.boardUi.at(i));
	}
	window.setView(fixedView);
	if (mainScene.menu.draw) {
		window.draw(*mainScene.menu.Draw());
	}
	else {
		for (unsigned int i = 0; i < mainScene.texts.size(); i++) {
			window.draw(mainScene.texts.at(i));
		}
	}
	window.draw(Messages::log);
	if(Messages::prompting)
		window.draw(Messages::prompt);
	window.draw(Messages::notice);
	if (Console::enable) {
		Console::bg.setSize(sf::Vector2f(fixedView.getSize().x, fixedView.getSize().y/4));
		window.draw(Console::bg);
		window.draw(Console::input);
	}
	window.display();
	sprites->clear();
	window.setView(view);
}

sf::RenderWindow * Main::GetWindow()
{
	return &window;
}
