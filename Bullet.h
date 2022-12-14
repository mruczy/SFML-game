#ifndef BULLET_H
#define	BULLET_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Bullet
{
private:

	sf::Sprite shape;

	sf::Vector2f direction;
	float movementSpeed;

public:
	Bullet(sf::Texture* texture, float pos_x, float pos_y, float dir_X, float dir_Y, float movement_speed);
	virtual ~Bullet();

	//accesor
	const sf::FloatRect getBounds() const;

	void update();
	void render(sf::RenderTarget* target);
};

#endif // !BULLET_H