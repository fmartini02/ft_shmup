#include "../include/Enemy.hpp"
#include "../include/World.hpp"
#include <cmath>

Enemy::Enemy(float x, float y, const char* symbol, int hp, float speed, int points) :
	Ship(x, y, symbol, hp, speed), _shootCooldown(5.0f), _shootTimer(0.0f), _points(points) {
	// std::cout << "* ENEMY HAS BEEN CREATED *" << std::endl;
}

Enemy::~Enemy() {
	// std::cout << "~ ENEMY HAS BEEN DESTROYED ~" << std::endl;
}

int Enemy::getPoints() const { return _points; }

void Enemy::update(float dt) {
	if (_shootTimer > 0)
		_shootTimer -= dt;
	setY(getY() + _speed * dt * 10);
	if (getY() >= LINES)
		setAlive(false);
	if (_hp <= 0)
		setAlive(false);
}

// BasicEnemy
BasicEnemy::BasicEnemy(float x, float y) :
	Enemy(x, y, "⊙", 1, 1.0f, 10) {}

void BasicEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(2));
	if (getX() >= 0 && getX() < COLS && getY() >= 0 && getY() < LINES)
		mvwprintw(win, (int)getY(), (int)getX(), "%s", getSymbol());
	attroff(COLOR_PAIR(2));
}

void BasicEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX(), getY() + 1));
		_shootTimer = _shootCooldown;
	}
}

// ShooterEnemy
ShooterEnemy::ShooterEnemy(float x, float y) :
	Enemy(x, y, "Ω", 1, 1.0f, 20) {}

void ShooterEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(3));
	mvwprintw(win, (int)getY(), (int)getX(), "%s", getSymbol());
	attroff(COLOR_PAIR(3));
}

void ShooterEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX() - 1, getY() + 1));
		entities.push_back(std::make_unique<EnemyBullet>(getX(),     getY() + 1));
		entities.push_back(std::make_unique<EnemyBullet>(getX() + 1, getY() + 1));
		_shootTimer = _shootCooldown;
	}
}

// ZigzagEnemy
ZigzagEnemy::ZigzagEnemy(float x, float y) :
	Enemy(x, y, "Σ", 1, 1.0f, 15), _zigzagTimer(0), _dx(0) {}

void ZigzagEnemy::update(float dt) {
	Enemy::update(dt);
	_zigzagTimer += dt;
	if (_zigzagTimer > 0.5f) {
		_dx = (float)(rand() % 3 - 1);  // -1, 0, o 1
		setX(getX() + _dx);
		_zigzagTimer = 0;
	}
}

void ZigzagEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(2));
	mvwprintw(win, (int)getY(), (int)getX(), "%s", getSymbol());
	attroff(COLOR_PAIR(3));
}

void ZigzagEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX() + _dx, getY() + 1));
		_shootTimer = _shootCooldown;
	}
}

// TankEnemy
TankEnemy::TankEnemy(float x, float y) :
	Enemy(x, y, "ϕ", 3, 0.3f, 30), _moveTimer(0) {}

void TankEnemy::update(float dt) {
	_moveTimer += dt;
	if (_moveTimer > 0.3f) {   // si muove ogni 0.3 secondi
		Enemy::update(dt);
		_moveTimer = 0;
	}
}

void TankEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(2));
	mvwprintw(win, (int)getY(), (int)getX(), "%s", getSymbol());
	attroff(COLOR_PAIR(2));
}

void TankEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX(), getY() + 1));
		_shootTimer = _shootCooldown;
	}
}

Boss::Boss(float x, float y) :
	Enemy(x, y, "▄", 20, 1.0f, 100), _moveTimer(0) {
}

void Boss::update(float dt) {
	_moveTimer += dt;
	if (_moveTimer > 0.1f) {
		setX(getX() + (rand() % 3 - 1));  // movimento laterale casuale
		_moveTimer = 0;
	}
	if (getY() >= LINES)
		setAlive(false);
	if (_hp <= 0)
		setAlive(false);
}

bool Boss::containsPoint(float x, float y) const {
	return (x >= getX() && x <= getX() + 2 &&
			y >= getY() && y <= getY() + 1);
}

void Boss::updatePlayerPos(float x, float y) {
	_player_x = x;
	_player_y = y;
}

void Boss::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;

	int pattern = rand() % 3;  // 3 pattern diversi

	switch (pattern) {
		case 0:  // CERCHIO — spara in 8 direzioni
			entities.push_back(std::make_unique<EnemyBullet>(getX(), getY() + 1));
			entities.push_back(std::make_unique<EnemyBullet>(getX() + 1, getY() + 1));
			entities.push_back(std::make_unique<EnemyBullet>(getX() - 1, getY() + 1));
			entities.push_back(std::make_unique<EnemyBullet>(getX() + 1, getY()));
			entities.push_back(std::make_unique<EnemyBullet>(getX() - 1, getY()));
			entities.push_back(std::make_unique<EnemyBullet>(getX(), getY() - 1));
			entities.push_back(std::make_unique<EnemyBullet>(getX() + 1, getY() - 1));
			entities.push_back(std::make_unique<EnemyBullet>(getX() - 1, getY() - 1));
			_shootTimer = 0.3f;
			break;

		case 1:  // RAFFICA — 5 proiettili in colonna
			for (int i = -2; i <= 2; i++)
				entities.push_back(std::make_unique<EnemyBullet>(getX() + i, getY() + 1));
			_shootTimer = 0.5f;
			break;

		case 2:  // MIRINO — punta verso il player
		{
			float dx = _player_x - getX();
			float dy = _player_y - getY();
			float len = sqrt(dx * dx + dy * dy);
			if (len > 0) {
				dx /= len;
				dy /= len;
			}
			entities.push_back(std::make_unique<EnemyBullet>(
				getX() + dx, getY() + dy));
			_shootTimer = 0.2f;
			break;
		}
	}
}

void Boss::render(WINDOW *win) {
	attron(COLOR_PAIR(2) | A_BOLD);
	mvwprintw(win, (int)getY(),     (int)getX(),     "▄█▄");
	mvwprintw(win, (int)getY() + 1, (int)getX(),     "███");
	attroff(COLOR_PAIR(2) | A_BOLD);
}
