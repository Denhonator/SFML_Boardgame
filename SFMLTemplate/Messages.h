#pragma once
#include "Resources.h"
class Messages
{
public:
	Messages();
	~Messages();
	static void Add(std::string s);
	static void UpdatePos();
	static void Notice(std::string s);
	static void FadeNotice(float f=1);
	static char Prompt(std::string s);
	static char WaitInput();
	static void SetInput(char c);
	static void Click(sf::Vector2f pos);
	static void ScrollMessages(int off);
	static bool OnLog(sf::Vector2f mouse);
	static bool prompting;
	static sf::Text log;
	static sf::Text notice;
	static sf::Text prompt;
private:
	static void UpdateText();
	static char c;
	static int messageIndex;
	static sf::Vector2f promptClick;
	static std::vector<std::string> texts;
	static float fade;
};

