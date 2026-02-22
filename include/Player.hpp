#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "Ship.hpp"

class Player: public Ship {
private:
	int		_score;
	int		_lives;//numero di continua
	float	_shootCooldown;
	float	_shootTimer;
public:
	Player(float x, float y, int hp, float speed, int lives);
	~Player();

	int		getScore();
	int		getLives();
	void	update(float dt) override;
	void	render(WINDOW *win) override;
	void	shoot() override;
	void	move(float dx, float dy);
	void	addScore(int points);
	void	loseLife();

	// void	increseCooldown(int molt); //2x 3x ecc...
	// void	decreseCooldown(int molt); //2x 3x ecc...
	// void	increseShootTimer(int molt); //2x 3x ecc...
	// void	increseShootTimer(int molt); //2x 3x ecc..
};
#endif
