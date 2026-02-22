#ifndef SHIP_HPP
#define SHIP_HPP

#include <memory>
#include <vector>
#include "AGameEntity.hpp"

class Ship: public AGameEntity {
protected:
	int		_hp;
	float	_speed;
public:
	Ship(float x, float y, const char* symbol, int hp, float speed);
	virtual ~Ship();

	int		getHp() const;
	void	setHp(int hp);
	bool	isDead() const;

	virtual void	shoot(std::vector<std::unique_ptr<AGameEntity>>& entities) = 0;
};
#endif
