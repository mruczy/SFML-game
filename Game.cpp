#include "Game.h"

//funckje prywatne
void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode(1920, 1080), "Space");
	this->window->setFramerateLimit(144);
}

void Game::initTextures()
{
	this->textures["BULLET"] = new sf::Texture();
	this->textures["BULLET"]->loadFromFile("Textures/bullet.png");

}

void Game::initGUI()
{
	//load font
	if (!this->font.loadFromFile("Font/font.ttf"))
		std::cout << "ERROR::GAME::Failed to load font" << std::endl;

	//init point text
	this->pointText.setFont(this->font);
	this->pointText.setCharacterSize(20);
	this->pointText.setFillColor(sf::Color::White);

	//gameover text
	this->GameOverText.setFont(this->font);
	this->GameOverText.setCharacterSize(60);
	this->GameOverText.setFillColor(sf::Color::Red);
	this->GameOverText.setString("Game Over");
	this->GameOverText.setPosition(
		this->window->getSize().x / 2.f - this->GameOverText.getGlobalBounds().width / 2.f,
		this->window->getSize().y / 2.f - this->GameOverText.getGlobalBounds().height / 2.f);

	//Init player gui
	this->playerHpBar.setSize(sf::Vector2f(300.f, 20.f));
	this->playerHpBar.setFillColor(sf::Color::Red);
	this->playerHpBar.setPosition(20.f, 20.f);

	this->playerHpBarBack = this->playerHpBar;
	this->playerHpBarBack.setFillColor(sf::Color::Black);
}

void Game::initWorld()
{
	if (!this->worldBackgraundTex.loadFromFile("Textures/background.png"))
	{
		std::cout << "ERROR::GAME::Failed to load background" << std::endl;
	}
	this->worldBackground.setTexture(this->worldBackgraundTex);
}

void Game::initSystems()
{
	this->points = 0;
}

void Game::initPlayer()
{
	this->player = new Player();
	this->player->setPosition(this->window->getSize().x / 2.f - this->player->getBounds().width / 2.f, 600.f);

}

void Game::initEnemies()
{
	this->spawnTimerMax = 10.f;
	this->spawnTimer = this->spawnTimerMax;
}

//con/des
Game::Game()
{
	this->initWindow();
	this->initTextures();
	this->initGUI();
	this->initWorld();
	this->initSystems();
	this->initPlayer();
	this->initEnemies();
}

Game::~Game()
{
	delete this->window;
	delete this->player;

	//usuwanie tekstur
	for (auto& i : this->textures)
	{
		delete i.second;
	}

	//usuwanie pociskow
	for (auto* i : this->bullets)
	{
		delete i;
	}

	//usuwanie przeciwnikow
	for (auto* i : this->enemies)
	{
		delete i;
	}
}


//funkcje
void Game::run()
{
	while (this->window->isOpen())
	{
		this->updatePollEvnets();

		if (this->player->getHp() > 0)
			this->update();

		this->render();
	}

}

void Game::updatePollEvnets()
{
	sf::Event e;
	while (this->window->pollEvent(e))
	{
		if (e.Event::type == sf::Event::Closed)
			this->window->close();

		if (e.Event::KeyPressed && e.Event::key.code == sf::Keyboard::Escape)
		{
			this->window->close();
		}
	}
}

void Game::updateInput()
{
	//move player
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		this->player->move(-1.f, 0.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		this->player->move(1.f, 0.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		this->player->move(0.f, -1.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		this->player->move(0.f, 1.f);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && this->player->canAttack())
	{
		this->bullets.push_back(
			new Bullet(
				this->textures["BULLET"],
				this->player->getPos().x + this->player->getBounds().width / 2.f,
				this->player->getPos().y,
				0.f,
				-1.f,
				5.f
			)
		);
	}
}

void Game::updateGUI()
{
	std::stringstream ss;

	ss << "Punkty: " << this->points;

	this->pointText.setString(ss.str());

	//update player gui
	float hpPercent = static_cast<float>(this->player->getHp()) / this->player->getHpMax();
	this->playerHpBar.setSize(sf::Vector2f(300.f * hpPercent, this->playerHpBar.getSize().y));

}

void Game::updateWorld()
{

}

void Game::updateCollision()
{
	//left world collision
	if (this->player->getBounds().left < 0.f)
	{
		this->player->setPosition(0.f, this->player->getBounds().top);
	}
	//right world collision
	else if (this->player->getBounds().left + this->player->getBounds().width >= this->window->getSize().x)
	{
		this->player->setPosition(this->window->getSize().x - this->player->getBounds().width, this->player->getBounds().top);
	}


	//top world collision
	if (this->player->getBounds().top < 0.f)
	{
		this->player->setPosition(this->player->getBounds().left, 0.f);
	}
	//bottom world collision
	else if (this->player->getBounds().top + this->player->getBounds().height >= this->window->getSize().y)
	{
		this->player->setPosition(this->player->getBounds().left, this->window->getSize().y - this->player->getBounds().height);
	}

}

void Game::updateBullets()
{
	unsigned counter = 0;
	for (auto* bullet : this->bullets)
	{
		bullet->update();

		//usuwanie pocisku kiedy wyjdzie poza ekran(z gory)
		if (bullet->getBounds().top + bullet->getBounds().height < 0.f)
		{
			//usuwanei pocisku
			delete this->bullets.at(counter);
			this->bullets.erase(this->bullets.begin() + counter);

			std::cout << this->bullets.size() << std::endl;
		}

		++counter;
	}
}

void Game::updateEnemies()
{
	//spawn
	this->spawnTimer += 0.5f;
	if (this->spawnTimer >= this->spawnTimerMax)
	{
		this->enemies.push_back(new Enemy(rand() % this->window->getSize().x - 20.f, -100.f));
		this->spawnTimer = 0.f;
	}

	//update
	unsigned counter = 0;
	for (auto* enemy : this->enemies)
	{
		enemy->update();

		//usuwanie pocisku kiedy wyjdzie poza ekran(z gory)
		if (enemy->getBounds().top > this->window->getSize().y)
		{
			//usuwanie przeciwnika
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);

			std::cout << this->enemies.size() << std::endl;
		}
		//usuwanie przeciwnika jestli dotknie gracza
		else if (enemy->getBounds().intersects(this->player->getBounds()))
		{
			this->player->loseHp(this->enemies.at(counter)->getDamage());
			delete this->enemies.at(counter);
			this->enemies.erase(this->enemies.begin() + counter);
		}

		++counter;
	}
}

void Game::updateCombat()
{
	for (size_t i = 0; i < this->enemies.size(); i++)
	{
		bool enemy_deleted = false;
		for (size_t k = 0; k < this->bullets.size() && enemy_deleted == false; k++)
		{
			if (this->enemies[i]->getBounds().intersects(this->bullets[k]->getBounds()))
			{
				this->points += this->enemies[i]->getPoints();

				delete this->enemies[i];
				this->enemies.erase(this->enemies.begin() + i);

				delete this->bullets[k];
				this->bullets.erase(this->bullets.begin() + k);

				enemy_deleted = true;
			}
		}
	}
}

void Game::update()
{
	this->updateInput();

	this->player->update();

	this->updateCollision();

	this->updateBullets();

	this->updateEnemies();

	this->updateCombat();

	this->updateGUI();

	this->updateWorld();
}

void Game::renderGUI()
{
	this->window->draw(this->pointText);
	this->window->draw(this->playerHpBarBack);
	this->window->draw(this->playerHpBar);
}

void Game::renderWordl()
{
	this->window->draw(this->worldBackground);
}

void Game::render()
{
	//background
	this->renderWordl();

	this->window->clear();

	//rysuje elementy
	this->player->render(*this->window);

	for (auto* bullet : this->bullets)
	{
		bullet->render(this->window);
	}

	for (auto* enemy : this->enemies)
	{
		enemy->render(this->window);
	}

	this->renderGUI();

	//game over screen
	if (this->player->getHp() <= 0)
		this->window->draw(this->GameOverText);

	this->window->display();
}