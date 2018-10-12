#pragma once
#include "Resources.h"
class Messages
{
public:
	Messages();
	~Messages();
	static void Add(std::string s);
	static void Notice(std::string s);
	static void FadeNotice(float f=1);
	static void Prompt(std::string s);
	static sf::Keyboard::Key WaitInput();
	static void SetInput(sf::Keyboard::Key k);
	static void Click(sf::Vector2f pos);
	static bool prompting;
	static sf::Text log;
	static sf::Text notice;
	static sf::Text prompt;
private:
	static sf::Keyboard::Key key;
	static sf::Vector2f promptClick;
	static std::vector<std::string> texts;
	static float fade;
};

