#include "../include/AGameEntity.hpp"

AGameEntity::AGameEntity(float x, float y, char symbol) :
_x(x), _y(y), _symbol(symbol), _alive(true) {
	std::cout << "* AGAME_ENTITY HAS BEEN CREATED *" << std::endl;
}

AGameEntity::~AGameEntity() {
	std::cout << "~ AGAME_ENTITY HAS BEEN DESTROYED ~" << std::endl;
}

float	AGameEntity::getX()const { return _x; }
float	AGameEntity::getY()const { return _y; }
char	AGameEntity::getSymbol()const { return _symbol; }
bool	AGameEntity::isAlive() const { return _alive;}
void	AGameEntity::setX(float x){ _x = x; }
void	AGameEntity::setY(float y){ _y = y; }
void	AGameEntity::setAlive(bool alive){ _alive = alive;}
