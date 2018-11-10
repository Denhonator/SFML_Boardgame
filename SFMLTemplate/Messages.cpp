#include "Messages.h"

sf::Text Messages::log;
sf::Text Messages::notice;
sf::Text Messages::prompt;
std::vector<std::string> Messages::texts;
float Messages::fade = 0;
bool Messages::prompting = false;
char Messages::c = 0;
sf::Vector2f Messages::promptClick;
int Messages::messageIndex;

Messages::Messages()
{
	log = sf::Text("", *Resources::GetFont("default.ttf"));
	notice = sf::Text("", *Resources::GetFont("default.ttf"));
	prompt = sf::Text("", *Resources::GetFont("default.ttf"));
	prompt.setScale(sf::Vector2f(1.75f, 1.75f));
	prompt.setOutlineThickness(2);
}

Messages::~Messages()
{
}

void Messages::Add(std::string s)
{
	texts.push_back(s);
	messageIndex = texts.size() - 5;
	UpdateText();
}

void Messages::UpdatePos()
{
	log.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.top + Constants::fixedView.height*0.82f);
	notice.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.top);
	prompt.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.height / 3);
}

void Messages::Notice(std::string s)
{
	notice.setString(s);
	notice.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.top);
	fade = 255;
}

void Messages::FadeNotice(float f)
{
	if (fade > 0) {
		fade -= f;
		notice.setFillColor(sf::Color(255, 255, 255, fade));
	}
}

char Messages::Prompt(std::string s)
{
	c = 0;
	prompt.setString(s!=""?s:"Prompt without text");
	prompt.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.height / 3);
	prompting = true;
	while (1) {
		sf::sleep(sf::milliseconds(2));
		char key = Messages::WaitInput();
		if (key != 0)
			return key;
	}
}

char Messages::WaitInput()
{
	if (c != 0) {
		char temp = c;
		c = 0;
		prompting = false;
		return temp;
	}
	return 0;
}

void Messages::SetInput(char c)
{
	Messages::c = c;
}

void Messages::Click(sf::Vector2f pos)
{
	promptClick = pos;
}

void Messages::ScrollMessages(int off)
{
	messageIndex = std::max(std::min((int)texts.size() - 5, messageIndex + off), 0);
	UpdateText();
}

bool Messages::OnLog(sf::Vector2f mouse)
{
	return log.getGlobalBounds().contains(mouse.x,mouse.y);
}

void Messages::UpdateText()
{
	std::string buffer = "";
	messageIndex = std::max(std::min((int)texts.size() - 5, messageIndex), 0);
	for (unsigned int i = messageIndex; i < texts.size() && i < messageIndex+5; i++) {
		buffer += texts.at(i);
		if (i < texts.size() - 1)
			buffer += "\n";
	}
	log.setString(buffer);
	log.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.top + Constants::fixedView.height*0.82f);
}
