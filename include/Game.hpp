#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <chrono>
#include "InputHandler.hpp"
#include "Render.hpp"
#include "World.hpp"
#include "HUD.hpp"

class Game {
private:
	bool	_running;
	int		_score;
	int		_lives;
	float	_deltaTime;
//game time
	std::chrono::steady_clock::time_point _lastTime;
// //class needed by game;
	InputHandler	_input;
	Render			_render;
	HUD				_hud;
	World			_world;
public:
	Game();
	~Game();
	void	run();
private:
	void	handleInput();
	void	update();
	void	render();
	void	computeDelta();
};

#endif
