#include "../include/World.hpp"
#include "../include/Scenery.hpp"
#include <cstdlib>
#include <algorithm>

World::World() :
	_player(COLS / 2, LINES - 2, 3, 1.0f, 3),
	_spawnTimer(0), /*_waveTimer(0),*/ _scrollOffset(0),
	_invincibleFrames(0), _startDelay(150) {}

World::~World() {}

Player& World::getPlayer() { return _player; }

std::vector<std::unique_ptr<AGameEntity>>& World::getEntities() { return _entities; }

// ============================================================================
// UPDATE PRINCIPALE
// ============================================================================
void World::update(float dt) {
	_player.update(dt);
	for (auto& e : _entities)
		e->update(dt);
	updateMap();
	spawnEnemies();
	enemyShoot();
	removeDeadEntities();
}

void World::render(WINDOW *win) {
	_player.render(win);
	for (auto& e : _entities)
		e->render(win);
}

// ============================================================================
// MAPPA - Scenario con rocce che scorrono
// ============================================================================
void World::updateMap() {
	_scrollOffset++;
	int sceneryCount = 0;
	for (auto& e : _entities) {
		if (dynamic_cast<Scenery*>(e.get()))
			sceneryCount++;
	}
	if (sceneryCount < 30 && rand() % 300 == 0)
		_entities.push_back(std::make_unique<Scenery>(COLS - 1, rand() % LINES));
}

// ============================================================================
// SPAWN - Genera nemici casuali + onde speciali ogni 250 frame
// ============================================================================
void World::spawnEnemies() {
	if (_startDelay > 0) {
		_startDelay--;
		return; // non spawnare nulla finché non finisce il delay
	}
	int enemyCount = 0;
    for (auto& e : _entities) {
        if (dynamic_cast<Enemy*>(e.get()))
            enemyCount++;
    }
	_spawnTimer++;
	// spawna solo se ce ne sono meno di 3
    if (enemyCount < 5 && rand() % 1000 < 1) {
        float x = rand() % COLS;
        _entities.push_back(std::make_unique<BasicEnemy>(x, 0));
    }
	// if (rand() % 100 < 1) {
	// 	float start_x = rand() % (COLS -10);
	// 	// int type = rand() % 4;
	// 	for (int i = 0; i < 2; i++) {
    //     _entities.push_back(std::make_unique<BasicEnemy>(start_x + i * 4, 0));
    // }

	// Onda speciale ogni 250 frame
	// _waveTimer++;
	// if (_waveTimer % 500 == 0) {
	// 	float start_x = rand() % (COLS - 10);
	// 	for (int i = 0; i < 5; i++) {
	// 		if (i % 2 == 0)
	// 			_entities.push_back(std::make_unique<BasicEnemy>(start_x + i * 2, 0));
	// 		// else
	// 			// _entities.push_back(std::make_unique<ShooterEnemy>(start_x + i * 2, 0));
	// 	}
	// }
}

// ============================================================================
// SPARO NEMICI
// ============================================================================
void World::enemyShoot() {
	std::vector<std::unique_ptr<AGameEntity>> toAdd;
	for (auto& e : _entities) {
		if (!e->isAlive()) continue;
		Enemy* enemy = dynamic_cast<Enemy*>(e.get());
		if (enemy)
			enemy->shoot(toAdd);
	}
	for (auto& p : toAdd)
		_entities.push_back(std::move(p));
}

// ============================================================================
// COLLISIONI con invincibility frames
// ============================================================================
void World::checkCollisions(int& lives, int& score) {
	bool player_hit_this_frame = false;

	for (auto& e : _entities) {
		if (!e->isAlive()) continue;

		// proiettile nemico colpisce player
		EnemyBullet* eb = dynamic_cast<EnemyBullet*>(e.get());
		if (eb && !player_hit_this_frame
			   && (int)eb->getX() == (int)_player.getX()
			   && (int)eb->getY() == (int)_player.getY()) {
			eb->setAlive(false);
			if (_invincibleFrames <= 0) {
				player_hit_this_frame = true;
				lives--;
				_invincibleFrames = 20;
			}
		}

		// nemico tocca player
		Enemy* enemy = dynamic_cast<Enemy*>(e.get());
		if (enemy && !player_hit_this_frame
				  && (int)enemy->getX() == (int)_player.getX()
				  && (int)enemy->getY() == (int)_player.getY()) {
			enemy->setAlive(false);
			if (_invincibleFrames <= 0) {
				player_hit_this_frame = true;
				lives--;
				_invincibleFrames = 20;
			}
		}

		// proiettile player colpisce nemico
		PlayerBullet* pb = dynamic_cast<PlayerBullet*>(e.get());
		if (pb && pb->isAlive()) {
			for (auto& e2 : _entities) {
				Enemy* target = dynamic_cast<Enemy*>(e2.get());
				if (target && target->isAlive()
					&& (int)pb->getX() == (int)target->getX()
					&& (int)pb->getY() == (int)target->getY()) {
					pb->setAlive(false);
					target->setAlive(false);
					score += target->getPoints();
				}
			}
		}
	}

	if (_invincibleFrames > 0)
		_invincibleFrames--;
}

// ============================================================================
// RIMOZIONE entità morte o fuori schermo
// ============================================================================
void World::removeDeadEntities() {
	_entities.erase(
		std::remove_if(_entities.begin(), _entities.end(),
			[](const std::unique_ptr<AGameEntity>& e){ return !e->isAlive(); }),
		_entities.end()
	);
}
