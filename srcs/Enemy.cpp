#include "../include/Enemy.hpp"
#include "../include/World.hpp"

Enemy::Enemy(float x, float y, char symbol, int hp, float speed, int points) :
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
	if (getY() >= HEIGHT)
		setAlive(false);
	if (_hp <= 0)
		setAlive(false);
}

// BasicEnemy
BasicEnemy::BasicEnemy(float x, float y) :
	Enemy(x, y, 'V', 1, 1.0f, 10) {}

void BasicEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(2));
	if (getX() >= 0 && getX() < WIDTH && getY() >= 0 && getY() < HEIGHT)
		mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
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
	Enemy(x, y, 'W', 1, 1.0f, 20) {}

void ShooterEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(3));
	mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
	attroff(COLOR_PAIR(3));
}

void ShooterEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX() - 1, getY() + 1));
		// entities.push_back(std::make_unique<EnemyBullet>(getX(),     getY() + 1));
		// entities.push_back(std::make_unique<EnemyBullet>(getX() + 1, getY() + 1));
		_shootTimer = _shootCooldown;
	}
}

// ZigzagEnemy
ZigzagEnemy::ZigzagEnemy(float x, float y) :
	Enemy(x, y, 'Z', 1, 1.0f, 15), _zigzagTimer(0), _dx(0) {}

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
	mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
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
	Enemy(x, y, 'T', 3, 0.3f, 30), _moveTimer(0) {}

void TankEnemy::update(float dt) {
	_moveTimer += dt;
	if (_moveTimer > 0.3f) {   // si muove ogni 0.3 secondi
		Enemy::update(dt);
		_moveTimer = 0;
	}
}

void TankEnemy::render(WINDOW *win) {
	attron(COLOR_PAIR(2));
	mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
	attroff(COLOR_PAIR(2));
}

void TankEnemy::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0) return;
	if (rand() % 100 < 1) {
		entities.push_back(std::make_unique<EnemyBullet>(getX(), getY() + 1));
		_shootTimer = _shootCooldown;
	}
}
