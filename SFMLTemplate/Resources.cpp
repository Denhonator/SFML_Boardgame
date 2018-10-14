#include "Resources.h"

std::map<std::string, sf::Texture> Resources::tile = {};
std::map<std::string, sf::Font> Resources::font = {};
std::map<std::string, sf::SoundBuffer> Resources::sound;
std::vector<sf::Sound> Resources::player;
short Resources::roll = 0;
std::vector <sf::Vector2i> Resources::voffs = {sf::Vector2i(0,0),sf::Vector2i(-1,0) ,sf::Vector2i(0,-1) ,sf::Vector2i(1,0) ,sf::Vector2i(0,1) ,sf::Vector2i(1,1) ,sf::Vector2i(-1,-1) ,sf::Vector2i(1,-1) ,sf::Vector2i(-1,1) };

void Resources::RollerFunction() {
	srand(time(NULL));
	while (running) {
		sf::sleep(sf::milliseconds(5));
		roll = rand() % 100 + 1;
	}
}

Resources::Resources()
{
	running = true;
	roller = std::thread(&Resources::RollerFunction, this);
}

Resources::~Resources()
{
	running = false;
	roller.join();
}

sf::Texture * Resources::GetTexture(std::string name)
{
	if(tile.count(name)>0)
		return &tile.at(name);
	sf::Texture buffer;
	if (buffer.loadFromFile(name + ".png")) {
		tile[name] = buffer;
		return &tile[name];
	}
	return &tile.at("0");
}

sf::Font * Resources::GetFont(std::string name)
{
	if (font.count(name) > 0)
		return &font.at(name);
	sf::Font buffer;
	if (buffer.loadFromFile(name)) {
		font[name] = buffer;
		return &font[name];
	}
	return nullptr;
}

std::vector<std::string> Resources::GetText(std::string path)
{
	std::vector<std::string> list;
	std::string line;
	std::ifstream file(path+".txt");
	if (file.is_open())
	{
		while (getline(file, line))
		{
			list.push_back(line);
		}
		file.close();
	}
	return list;
}

void Resources::PlayWav(std::string s)
{
	short index = -1;
	for (int i = 0; i < player.size(); i++) {
		if (player.at(i).getStatus() != sf::Sound::Status::Playing) {
			index = i;
			break;
		}
	}
	if (index < 0) {
		player.push_back(sf::Sound());
		index = player.size() - 1;
	}
	if (!sound.count(s)) {
		sf::SoundBuffer buffer;
		buffer.loadFromFile("sounds/" + s + ".wav");
		sound[s] = buffer;
	}
	player.at(index).setBuffer(sound[s]);
	player.at(index).play();
}

std::pair<std::string, std::vector<int>> Resources::KeyWithInts(std::string line)
{
	std::string buffer = "";
	std::pair<std::string, std::vector<int>> temp;
	temp.first = "";

	for (int k = 0; k < line.size(); k++) {
		if (line[k] == '=') {
			temp.first = buffer;
			buffer = "";
		}
		else if (line[k] == ',') {
			temp.second.push_back(std::stoi(buffer));
			buffer = "";
		}
		else {
			buffer += line[k];
		}
	}
	temp.second.push_back(std::stoi(buffer));
	return temp;
}

std::pair<std::string, std::vector<std::string>> Resources::KeyWithStrings(std::string line)
{
	std::string buffer = "";
	std::pair<std::string, std::vector<std::string>> temp;
	temp.first = "";

	for (int k = 0; k < line.size(); k++) {
		if (line[k] == '=' && temp.first=="") {
			temp.first = buffer;
			buffer = "";
		}
		else if (line[k] == ',') {
			temp.second.push_back(buffer);
			buffer = "";
		}
		else {
			buffer += line[k];
		}
	}
	temp.second.push_back(buffer);
	return temp;
}

bool Resources::StrInVector(std::string s, std::vector<std::string> v)
{
	return std::find(v.begin(), v.end(), s) != v.end();
}

short Resources::Roll()
{
	return roll;
}

std::vector<sf::Vector2i> Resources::Voffs()
{
	std::vector<sf::Vector2i> t = voffs;
	std::vector<sf::Vector2i> r;
	r.push_back(t.at(0));
	t.erase(t.begin());
	while (t.size() > 0) {
		short x = roll % t.size();
		r.push_back(t.at(x));
		t.erase(t.begin() + x);
	}
	return r;
}
