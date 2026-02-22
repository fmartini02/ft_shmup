#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Ship.hpp"
#include <vector>
#include <memory>

class Enemy : public Ship {
protected:
	float	_shootCooldown;
	float	_shootTimer;
	int		_points;        // punti che vale quando muore

public:
	Enemy(float x, float y, char symbol, int hp, float speed, int points);
	virtual ~Enemy();

	int				getPoints() const;
	virtual void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) override = 0;
	virtual void	update(float dt) override;
};

class BasicEnemy : public Enemy {
public:
	BasicEnemy(float x, float y);
	void	render(WINDOW *win) override;
	void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) override;
};

class ShooterEnemy : public Enemy {
public:
	ShooterEnemy(float x, float y);
	void	render(WINDOW *win) override;
	void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) override;
};

class ZigzagEnemy : public Enemy {
private:
	float	_zigzagTimer;
	float	_dx;
public:
	ZigzagEnemy(float x, float y);
	void	update(float dt) override;
	void	render(WINDOW *win) override;
	void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) override;
};

class TankEnemy : public Enemy {
private:
	float	_moveTimer;
public:
	TankEnemy(float x, float y);
	void	update(float dt) override;
	void	render(WINDOW *win) override;
	void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) override;
};

#endif
