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
	static sf::Text log;
	static sf::Text notice;
private:
	static std::vector<std::string> texts;
	static float fade;
};

