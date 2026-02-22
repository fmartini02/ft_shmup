#ifndef SHIP_HPP
#define SHIP_HPP

#include "AGameEntity.hpp"

class Ship: public AGameEntity {
protected:
	int		_hp;
	float	_speed;
public:
	Ship(float x, float y, char symbol, int hp, float speed);
	virtual ~Ship();

	int		getHp() const;
	void	setHp(int hp);
	bool	isDead() const;

	virtual void	shoot() = 0;
};
#endif
