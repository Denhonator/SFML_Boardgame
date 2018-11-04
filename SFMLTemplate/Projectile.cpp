#include "Projectile.h"

Projectile::Projectile(sf::Vector2i from, sf::Vector2i to, std::string attackName, int effect)
{
	sf::Texture * tex = Resources::GetTexture("projectiles/" + attackName);
	sprite.setTexture(tex != nullptr ? *tex : *Resources::GetTexture("projectiles/default"));	//Set texture to attack-specific or default if missing
	sprite.scale(0.5f, 0.5f);
	sf::Vector2f offset = sf::Vector2f(sprite.getGlobalBounds().width / 2, sprite.getGlobalBounds().height / 2);
	sprite.setPosition(sf::Vector2f(from)*Constants::tileSize + offset);
	speed = sf::Vector2f(to.x*Constants::tileSize, to.y*Constants::tileSize) + offset - sprite.getPosition();
	if (effect == -1) {	//miss effect
		speed += sf::Vector2f((Resources::Roll() % 3 - 1) * 2 * offset.x, (Resources::Roll() % 5 - 2) * offset.y);
	}
	speed = sf::Vector2f(speed.x / 30, speed.y / 30);
	life = 30;
	this->effect = effect;
}

Projectile::~Projectile()
{
}

void Projectile::Update()
{
	sprite.move(speed);
	life -= Time::Mult();
	if (life <= 0 && effect == 1) {
		speed = sf::Vector2f(-speed.x, -speed.y);
		life = 30;
		effect = 0;
	}
}
