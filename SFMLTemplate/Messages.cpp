#include "Messages.h"

sf::Text Messages::log;
sf::Text Messages::notice;
std::vector<std::string> Messages::texts;
float Messages::fade = 0;

Messages::Messages()
{
	log = sf::Text("", *Resources::GetFont("default.ttf"));
	notice = sf::Text("", *Resources::GetFont("default.ttf"));
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
