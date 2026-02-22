#include "../include/InputHandler.hpp"
#include <iostream>
#include <ncurses.h>

InputHandler::InputHandler() : _key(0) {
	// std::cout << "*HANDLER HAS BEEN CREATED*" << std::endl;
}

InputHandler::~InputHandler() {
	// std::cout << "~HANDLER HAS BEEN DESTROYED~" << std::endl;
}

void	InputHandler::read() {
	_key = getch();
}

bool	InputHandler::isUp() const    { return _key == KEY_UP; }
bool	InputHandler::isDown() const  { return _key == KEY_DOWN; }
bool	InputHandler::isLeft() const  { return _key == KEY_LEFT; }
bool	InputHandler::isRight() const { return _key == KEY_RIGHT; }
bool	InputHandler::isShoot() const { return _key == ' '; }
bool	InputHandler::isQuit() const  { return _key == 'q'; }
int		InputHandler::getKey() const { return _key; }
