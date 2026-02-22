#include "../include/Ship.hpp"

Ship::Ship(float x, float y, char symbol, int hp, float speed) :
	AGameEntity(x, y, symbol), _hp(hp), _speed(speed) {
	std::cout << "* A SHIP HAS BEEN CREATED *" << std::endl;
}

Ship::~Ship() {
	std::cout << "~ A SHIP HAS BEEN DESTROYED ~" << std::endl;
}

int		Ship::getHp() const { return _hp; }
void	Ship::setHp(int hp) { _hp = hp; }
bool	Ship::isDead() const { return _hp <= 0;}
