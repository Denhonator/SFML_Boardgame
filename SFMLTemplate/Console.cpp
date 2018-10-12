#include "Console.h"

sf::RectangleShape Console::bg;
sf::Text Console::input;
std::string Console::previous = "unit,swordguy,2";
bool Console::enable = false;

Console::Console()
{
}

Console::~Console()
{
}

void Console::init()
{
	input = sf::Text("", *Resources::GetFont("default.ttf"));
	bg.setFillColor(sf::Color(0, 0, 0, 128));
}

void Console::Write(char c)
{
	//printf("%d\n", c);
	if (c == 8)
		input.setString(input.getString().substring(0, input.getString().getSize() - 1));
	else if(c!=13)
		input.setString(input.getString() + c);
}

std::vector<sf::String> Arguments(sf::String s) {
	int index = 0;
	bool space = false;
	std::vector<sf::String> result;
	for (int i = 0; i < s.getSize(); i++) {
		if (s[i] == ',' && !space) {
			result.push_back(s.substring(index, i - index));
			index = i + 1;
			space = true;
		}
		else {
			space = false;
		}
	}
	result.push_back(s.substring(index, s.getSize() - index));
	return result;
}

void Console::Command(Scene* scene)
{
	if (input.getString().getSize() > 0) {
		previous = input.getString();
		std::vector<sf::String> args = Arguments(input.getString());

		if (args.at(0) == "unit") {
			try {
				int player = 0;
				std::string nick = "";
				if (args.size() > 2)
					player = std::stoi(std::string(args.at(2)));
				if (args.size() > 3)
					nick = args.at(3);
				scene->AddUnit(&Unit(std::string(args.at(1)), player, nick));
				Messages::Notice("Unit added");
			}
			catch(const std::exception& e){
				Messages::Notice("Could not parse unit");
			}
		}
		input.setString("");
	}
}

void Console::GetPrevious()
{
	if (previous != "") {
		input.setString(previous);
	}
}
