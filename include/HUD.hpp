#ifndef HUD_HPP
#define HUD_HPP

#include <ncursesw/ncurses.h>
#include <chrono>
#include <iostream>

class HUD {
private:
	std::chrono::steady_clock::time_point   _startTime;

public:
	HUD();
	~HUD();

	void    render(WINDOW *win, int score, int lives);
	int     getElapsedSeconds() const;
};

#endif
