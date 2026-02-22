#ifndef RENDER_HPP
#define RENDER_HPP

#include <ncursesw/ncurses.h>
#include <iostream>

class Render {
private:
	WINDOW	*_win;
	int		_width;
	int		_height;
public:
	Render();
	~Render();
	int	getWidth() const;
	int	getHeight() const;
};

#endif
