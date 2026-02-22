#include "../include/Player.hpp"
#include "../include/AProjectile.hpp"

Player::Player(float x, float y, int hp, float speed, int lives):
	Ship(x, y, 'P', hp, speed),  _score(0), _lives(lives),
	_shootCooldown(0.5f), _shootTimer(0.0f) {
	// std::cout << "* PLAYER HAS BEEN CREATED *" << std::endl;
}

Player::~Player() {
	// std::cout << "~ PLAYER IS DESTROYED ~" << std::endl;
}

int		Player::getScore() { return _score; }
int		Player::getLives() { return _lives; }
void	Player::update(float dt) {
	if (_shootTimer > 0)
		_shootTimer -= dt;

	// boundary check (dipenderà dalle dimensioni dello schermo)
	if (getX() < 0)
		setX(0);
	if (getY() < 0)
		setY(0);
	// if (getX() > MAX_X) setX(MAX_X);
	// if (getY() > MAX_Y) setY(MAX_Y);
	if (_hp <= 0)
		setAlive(false);
}

void Player::render(WINDOW *win) {
	attron(COLOR_PAIR(1));
	mvwaddch(win, (int)getY(), (int)getX(), getSymbol());
	attroff(COLOR_PAIR(1));
}

void Player::shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) {
	if (_shootTimer > 0)
		return;
	entities.push_back(std::make_unique<PlayerBullet>(getX(), getY() - 1));
	_shootTimer = _shootCooldown;
}

void Player::move(float dx, float dy) {
	setX(getX() + dx * _speed);
	setY(getY() + dy * _speed);
}

void Player::addScore(int points) {
	_score += points;
}

void Player::loseLife() {
	_lives--;
	if (_lives <= 0)
		setAlive(false);
}
