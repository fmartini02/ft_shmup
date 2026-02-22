#include "../include/Render.hpp"

Render::Render() {
	setlocale(LC_ALL, "");
	_win = initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	start_color();
	getmaxyx(stdscr, _height, _width);
	init_pair(1, COLOR_GREEN,  COLOR_BLACK);  // player
	init_pair(2, COLOR_RED,    COLOR_BLACK);  // nemici
	init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // proiettili nemici
	init_pair(4, COLOR_CYAN,   COLOR_BLACK);  // proiettile player
	init_pair(5, COLOR_MAGENTA,  COLOR_BLACK);  // sassi
}

Render::~Render() {
	endwin();
	// std::cout << "~RENDERING ENDED~" << std::endl;
}

int	Render::getHeight() const { return _height; }
int	Render::getWidth() const { return _width; }
