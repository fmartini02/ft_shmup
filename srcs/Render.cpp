#include "../include/Render.hpp"

Render::Render() {
	_win = initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	nodelay(stdscr, TRUE);
	curs_set(0);
	start_color();
	getmaxyx(stdscr, _height, _width);
	std::cout << "*RENDERING STARTED*" << std::endl;
}

Render::~Render() {
	endwin();
	std::cout << "~RENDERING ENDED~" << std::endl;
}
