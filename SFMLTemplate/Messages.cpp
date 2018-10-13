#include "Messages.h"

sf::Text Messages::log;
sf::Text Messages::notice;
sf::Text Messages::prompt;
std::vector<std::string> Messages::texts;
float Messages::fade = 0;
bool Messages::prompting = false;
sf::Keyboard::Key Messages::key = sf::Keyboard::Key::Unknown;
sf::Vector2f Messages::promptClick;

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
	if (texts.size() > 5) {
		texts.erase(texts.begin());
		texts.shrink_to_fit();
	}
	log.setString("");
	for (int i = 0; i < texts.size(); i++) {
		log.setString(log.getString() + texts.at(i)+"\n");
	}
	log.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.top + Constants::fixedView.height - log.getGlobalBounds().height);
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

void Messages::Prompt(std::string s)
{
	key = sf::Keyboard::Key::Unknown;
	prompt.setString(s!=""?s:"Prompt without text");
	prompt.setPosition(Constants::fixedView.left + Constants::fixedView.width / 3, Constants::fixedView.height / 3);
	prompting = true;
}

sf::Keyboard::Key Messages::WaitInput()
{
	if (key != sf::Keyboard::Key::Unknown) {
		sf::Keyboard::Key temp = key;
		key = sf::Keyboard::Key::Unknown;
		prompting = false;
		return temp;
	}
	return sf::Keyboard::Key::Unknown;
}

void Messages::SetInput(sf::Keyboard::Key k)
{
	key = k;
}

void Messages::Click(sf::Vector2f pos)
{
	promptClick = pos;
}
