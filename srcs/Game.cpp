#include "../include/Game.hpp"

Game::Game() : _running(false), _score(0), _lives(3),
			_deltaTime(0), _lastTime(std::chrono::steady_clock::now()),
			_input(), _render(){
	// std::cout << "++GAME CLASS CREATED++" << std::endl;
}

Game::~Game() {
	// std::cout << "~GAME CLASS DESTROYED~" << std::endl;
}

void Game::update() {
	_world.update(_deltaTime);
	_world.checkCollisions(_lives, _score);
}

void Game::render() {
	erase();
	_world.render(stdscr);
	_hud.render(stdscr, _score, _lives);
	doupdate();
}

void Game::handleInput() {
	_input.read();
	if (_input.isQuit())
		_running = false;
	if (_input.isUp())    _world.getPlayer().move(0, -1);
	if (_input.isDown())  _world.getPlayer().move(0,  1);
	if (_input.isLeft())  _world.getPlayer().move(-1, 0);
	if (_input.isRight()) _world.getPlayer().move(1,  0);
	// if (_input.isShoot()) _world.getPlayer().shoot(_world.getEntities());
	_world.getPlayer().shoot(_world.getEntities());
}
void Game::run() {
	_running = true;
	_lastTime = std::chrono::steady_clock::now();
	while(_running) {
		auto now = std::chrono::steady_clock::now();
		_deltaTime = std::chrono::duration<float>(now - _lastTime).count();
		_lastTime = now;
//input → update → render			exit(0);
		handleInput();
		update();
		render();
		if (!_lives){
			_running = false;// SE LE VITE NON SONO PIU VALIDE MI CONCLUDI IL RUNNING
		}
	}
}