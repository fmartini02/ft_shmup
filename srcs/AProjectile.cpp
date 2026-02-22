#include "../include/AProjectile.hpp"
#include "../include/World.hpp"

AProjectile::AProjectile(float x, float y, char symbol, float dx, float dy, float speed) :
	AGameEntity(x, y, symbol), _dx(dx), _dy(dy), _speed(speed) {}

AProjectile::~AProjectile() {}

void AProjectile::update(float dt) {
	setX(getX() + _dx * _speed * dt);
	setY(getY() + _dy * _speed * dt);
	if (getX() < 0 || getX() >= WIDTH || getY() < 0 || getY() >= HEIGHT)
		setAlive(false);
}

void AProjectile::render(WINDOW *win) {
	attron(COLOR_PAIR(3));
	mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
	attroff(COLOR_PAIR(3));
}

// PlayerBullet: va verso l'alto (dy = -1), simbolo '|'
PlayerBullet::PlayerBullet(float x, float y) :
	AProjectile(x, y, '|', 0, -1, 20.0f) {}

// EnemyBullet: va verso il basso (dy = 1), simbolo '*'
EnemyBullet::EnemyBullet(float x, float y) :
	AProjectile(x, y, '*', 0, 1, 8.0f) {}
