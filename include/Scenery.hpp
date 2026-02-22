#ifndef SCENERY_HPP
#define SCENERY_HPP

#include "AGameEntity.hpp"
#include "World.hpp"

class Scenery : public AGameEntity {
private:
	float	_moveTimer;
public:
	Scenery(float x, float y);
	~Scenery();

	void	update(float dt) override;
	void	render(WINDOW *win) override;
};

#endif
