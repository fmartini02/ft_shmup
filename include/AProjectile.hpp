#ifndef APROJECTILE_HPP
#define APROJECTILE_HPP

#include "AGameEntity.hpp"

class AProjectile : public AGameEntity {
protected:
	float	_dx;
	float	_dy;
	float	_speed;

public:
	AProjectile(float x, float y, char symbol, float dx, float dy, float speed);
	virtual ~AProjectile();

	virtual void	update(float dt) override;
	virtual void	render(WINDOW *win) override;
};

class PlayerBullet : public AProjectile {
public:
	PlayerBullet(float x, float y);
};

class EnemyBullet : public AProjectile {
public:
	EnemyBullet(float x, float y);
};

#endif
