#pragma once

#include <map>
#include <string>
#include <sstream>
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"

class Game
{
private:

	//okno
	sf::RenderWindow* window;

	//zasoby
	std::map<std::string, sf::Texture*> textures;
	std::vector<Bullet*> bullets;

	//gui
	sf::Font font;
	sf::Text pointText;

	sf::Text GameOverText;

	//world
	sf::Texture worldBackgraundTex;
	sf::Sprite worldBackground;

	//systems
	unsigned points;

	//Player
	Player* player;

	//Player GUI
	sf::RectangleShape playerHpBar;
	sf::RectangleShape playerHpBarBack;

	//przeciwnicy
	float spawnTimer;
	float spawnTimerMax;
	std::vector<Enemy*> enemies;

	//funkcje prywaten
	void initWindow();
	void initTextures();
	void initGUI();
	void initWorld();
	void initSystems();

	void initPlayer();
	void initEnemies();

public:
	Game();
	virtual ~Game();

	//funkcje
	void run();

	void updatePollEvnets();
	void updateInput();
	void updateGUI();
	void updateWorld();
	void updateCollision();
	void updateBullets();
	void updateEnemies();
	void updateCombat();
	void update();
	void renderGUI();
	void renderWordl();
	void render();
};

