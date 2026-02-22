#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <memory>
#include <cstdlib>
#include "AGameEntity.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "AProjectile.hpp"

#define WIDTH   COLS
#define HEIGHT  LINES

class World {
private:
	std::vector<std::unique_ptr<AGameEntity>>	_entities;
	Player										_player;
	int											_spawnTimer;
	int											_waveTimer;
	int											_scrollOffset;
	int											_invincibleFrames;
	int											_startDelay;

public:
	World();
	~World();

	void	update(float dt);
	void	render(WINDOW *win);

	Player&	getPlayer();
	void	checkCollisions(int& lives, int& score);
	std::vector<std::unique_ptr<AGameEntity>>&	getEntities();

private:
	void	spawnEnemies();
	void	removeDeadEntities();
	void	enemyShoot();
	void	updateMap();
};

#endif
