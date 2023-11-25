#include "Bullet.h"


Bullet::Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_X, float dir_Y, float movement_speed)
{
	this->shape.setTexture(*texture);
	this->shape.scale(5.f, 8.f);
	this->shape.setPosition(pos_x, pos_y);
	this->direction.x = dir_X;
	this->direction.y = dir_Y;
	this->movementSpeed = movement_speed;
}

Bullet::~Bullet()
{

}

const sf::FloatRect Bullet::getBounds() const
{
	return this->shape.getGlobalBounds();
}

void Bullet::update()
{
	//ruch
	this->shape.move(this->movementSpeed * this->direction);
}

void Bullet::render(sf::RenderTarget* target)
{
	target->draw(this->shape);
}