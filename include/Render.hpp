#ifndef RENDER_HPP
#define RENDER_HPP

#include <ncurses.h>
#include <iostream>

class Render {
private:
	WINDOW	*_win;
	int		_width;
	int		_height;
public:
	Render();
	~Render();
};

#endif
