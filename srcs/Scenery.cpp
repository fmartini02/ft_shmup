#include "../include/Scenery.hpp"

Scenery::Scenery(float x, float y) : AGameEntity(x, y, "█") {
	// std::cout << "* SCENERY CREATED *" << std::endl;
}

Scenery::~Scenery() {
	// std::cout << "~ SCENERY DESTROYED ~" << std::endl;
}

void Scenery::update(float dt) {
	_moveTimer += dt;
	if (_moveTimer > 0.8f) {    // si muove ogni 0.3 secondi — aumenta per rallentare
		setX(getX() - 1);
		_moveTimer = 0;
	}
	 if (getX() < 0)
		setAlive(false);
}

void Scenery::render(WINDOW *win) {
	attron(COLOR_PAIR(5));
	mvwprintw(win, (int)getY(), (int)getX(), "%s", getSymbol());
	attroff(COLOR_PAIR(5));
}
