#include "../include/World.hpp"
#include "../include/Scenery.hpp"
#include "../include/Enemy.hpp"
#include <cstdlib>
#include <algorithm>

World::World() :
	_player(COLS / 2, HEIGHT - 2, 3, 1.0f, 3),
	_scrollOffset(0), _invincibleFrames(0),
	_startDelay(150), _level(1), _levelTimer(0) {}

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
	renderBackground(win);
	_player.render(win);
	for (auto& e : _entities)
		e->render(win);
}

void World::renderBackground(WINDOW *win) {
	attron(COLOR_PAIR(6) | A_DIM);  // colore dedicato al background — aggiungilo in Render
	for (int col = 0; col < COLS; col += 4) {  // colonnine ogni 3 celle
		for (int row = 0; row < LINES - 1; row++) {
			mvwprintw(win, row, col, "│");
		}
	}
	attroff(COLOR_PAIR(6) | A_DIM);
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
		_entities.push_back(std::make_unique<Scenery>(COLS - 1, rand() % HEIGHT));
}

// ============================================================================
// SPAWN - Genera nemici casuali + onde speciali ogni 250 frame
// ============================================================================
void World::spawnEnemies() {
	if (_startDelay > 0) { _startDelay--; return; }

	// aggiorna timer livello
	_levelTimer++;
	if (_levelTimer > 1800) {  // ogni 30 secondi
		_level++;
		_levelTimer = 0;
	}

	// maxEnemies parte da 1 e aumenta col livello, si resetta ogni 3 livelli
	int levelInCycle = (_level - 1) % 20;  // 0, 1, 2, poi ricomincia
	int maxEnemies = 1 + levelInCycle;     // 1, 2, 3, poi torna a 1

	int enemyCount = 0;
	for (auto& e : _entities) {
		if (dynamic_cast<Enemy*>(e.get()))
			enemyCount++;
	}
	if (enemyCount >= maxEnemies) return;

	// tipo di nemico aumenta ogni ciclo completo
	int cycle = (_level - 1) / 5;  // 0=basic, 1=shooter, 2=zigzag, 3=tank
	float x = rand() % COLS;

	if (rand() % 500 < 1) {
		switch (cycle) {
			case 0:  // livelli 1-5: solo basic
				_entities.push_back(std::make_unique<BasicEnemy>(x, 0));
				break;
			case 1:  // livelli 6-10: basic + shooter
				if (rand() % 2 == 0)
					_entities.push_back(std::make_unique<BasicEnemy>(x, 0));
				else
					_entities.push_back(std::make_unique<ShooterEnemy>(x, 0));
				break;
			case 2:  // livelli 11-15: shooter + zigzag
				if (rand() % 2 == 0)
					_entities.push_back(std::make_unique<ShooterEnemy>(x, 0));
				else
					_entities.push_back(std::make_unique<ZigzagEnemy>(x, 0));
				break;
			case 3:  // livelli 16-20: tutti e 3
				switch (rand() % 3) {
					case 0: _entities.push_back(std::make_unique<BasicEnemy>(x, 0));   break;
					case 1: _entities.push_back(std::make_unique<ShooterEnemy>(x, 0)); break;
					case 2: _entities.push_back(std::make_unique<ZigzagEnemy>(x, 0));  break;
				}
				break;
			case 4:
				switch (rand() % 4) {
					case 0: _entities.push_back(std::make_unique<BasicEnemy>(x, 0));   break;
					case 1: _entities.push_back(std::make_unique<ShooterEnemy>(x, 0)); break;
					case 2: _entities.push_back(std::make_unique<ZigzagEnemy>(x, 0));  break;
					// case 3: _entities.push_back(std::make_unique<Boss>(x, 0)); break;
				}
			default:  // livelli 21+: tutti incluso tank
				switch (rand() % 5) {
					case 0: _entities.push_back(std::make_unique<BasicEnemy>(x, 0));   break;
					case 1: _entities.push_back(std::make_unique<ShooterEnemy>(x, 0)); break;
					case 2: _entities.push_back(std::make_unique<ZigzagEnemy>(x, 0));  break;
					case 3: _entities.push_back(std::make_unique<TankEnemy>(x, 0));    break;
					// case 4: _entities.push_back(std::make_unique<Boss>(x, 0)); break;
				}
				break;
		}
	}
}

// ============================================================================
// SPARO NEMICI
// ============================================================================
void World::enemyShoot() {
	std::vector<std::unique_ptr<AGameEntity>> toAdd;
	for (auto& e : _entities) {
		if (!e->isAlive()) continue;
		// Boss* boss = dynamic_cast<Boss*>(e.get());
		// if (boss) {
		// 	boss->updatePlayerPos(_player.getX(), _player.getY());
		// 	boss->shoot(toAdd);
		// 	continue;
		// }
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
	(void)score;
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
		// PlayerBullet* pb = dynamic_cast<PlayerBullet*>(e.get());
		// if (pb && pb->isAlive()) {
		// 	for (auto& e2 : _entities) {
		// 		// controlla nemici normali
		// 		Enemy* target = dynamic_cast<Enemy*>(e2.get());
		// 		if (target && !dynamic_cast<Boss*>(e2.get()) && target->isAlive()
		// 			&& (int)pb->getX() == (int)target->getX()
		// 			&& (int)pb->getY() == (int)target->getY()) {
		// 			pb->setAlive(false);
		// 			target->setAlive(false);
		// 			score += target->getPoints();
		// 		}
		// 		// controlla boss
		// 		Boss* boss = dynamic_cast<Boss*>(e2.get());
		// 		if (boss && boss->isAlive()
		// 			&& boss->containsPoint(pb->getX(), pb->getY())) {
		// 			pb->setAlive(false);
		// 			boss->setHp(boss->getHp() - 1);
		// 			if (boss->isDead()) {
		// 				boss->setAlive(false);
		// 				score += boss->getPoints();
		// 			}
		// 		}
		// 	}
		// }
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
