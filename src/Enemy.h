#ifndef ENEMY_H
#define	ENEMY_H

#include <SFML/Graphics.hpp>

class Enemy
{
private:
	unsigned pointCount;
	//unsigned pointCount_
	sf::CircleShape shape;

	int type;
	float speed;
	int hp;
	int hpMAX;
	int damage;
	int points;
	int radiusMax;

	void initVariables();
	void initShape();

public:
	Enemy(float pos_x, float pos_y);
	virtual ~Enemy();

	//accessors
	const sf::FloatRect getBounds() const;
	const int& getPoints() const;
	const int& getDamage() const;

	//funkcje
	void update();
	void render(sf::RenderTarget* target);
};

#endif //!ENEMY_H