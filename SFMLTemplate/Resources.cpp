#include "Resources.h"

std::map<std::string, sf::Texture> Resources::tile = {};
std::map<std::string, sf::Font> Resources::font = {};
std::map<std::string, sf::SoundBuffer> Resources::sound;
std::vector<sf::Sound> Resources::player;
int Resources::roll = 0;

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
	int index = -1;
	for (unsigned int i = 0; i < player.size(); i++) {
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

	for (unsigned int k = 0; k < line.size(); k++) {
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

	for (unsigned int k = 0; k < line.size(); k++) {
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

int Resources::Roll()
{
	return roll;
}

std::vector<sf::Vector2i> Resources::Voffs(bool nond, int range)
{
	std::vector<sf::Vector2i> t;
	for (int i = 0; i <= range; i++) {
		for (int j = 0; j <= range; j++) {
			if (i + j <= range || (!nond && i <= range && j <= range)) {
				t.push_back(sf::Vector2i(i, j));
				if (i > 0)
					t.push_back(sf::Vector2i(-i, j));
				if (j > 0)
					t.push_back(sf::Vector2i(i, -j));
				if (i > 0 && j > 0)
					t.push_back(sf::Vector2i(-i, -j));
			}
		}
	}
	unsigned int x = rand()%t.size();	//Random starting pos for vector

	std::vector<sf::Vector2i> r;	//Put vector in new order for variance
	r.push_back(t.at(0));
	t.erase(t.begin());
	
	while (t.size() > 0) {			
		x = x >= t.size() ? 0 : x;
		r.push_back(t.at(x));
		t.erase(t.begin() + x);
	}
	return r;
}
