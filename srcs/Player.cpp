#include "../include/Player.hpp"

Player::Player(float x, float y, int hp, float speed, int lives):
	Ship(x, y, 'P', hp, speed), _lives(lives),  _score(0),
	_shootCooldown(0.5f), _shootTimer(0.0f) {
	std::cout << "* PLAYER HAS BEEN CREATED *" << std::endl;
}

Player::~Player() {
	std::cout << "~ PLAYER IS DESTROYED ~" << std::endl;
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
