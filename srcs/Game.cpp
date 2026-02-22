#include "../include/Game.hpp"

Game::Game() : _running(false), _score(0), _lives(3),
			_deltaTime(0), _lastTime(std::chrono::steady_clock::now()),
			_input(), _render(){
	std::cout << "++GAME CLASS CREATED++" << std::endl;
}

Game::~Game() {
	std::cout << "~GAME CLASS DESTROYED~" << std::endl;
}

void Game::handleInput() {
	_input.read();
	if (_input.isQuit())
		_running = false;
	// _world.getPlayer().handleInput(_input);
}

void Game::run() {
	_running = true;
	_lastTime = std::chrono::steady_clock::now();
	while(_running) {
		auto now = std::chrono::steady_clock::now();
		_deltaTime = std::chrono::duration<float>(now - _lastTime).count();
		_lastTime = now;
//input → update → render
		handleInput();
		update();
		render();
	}
}
