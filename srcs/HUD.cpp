#include "../include/HUD.hpp"
#include "../include/World.hpp"

HUD::HUD() : _startTime(std::chrono::steady_clock::now()) {
	// std::cout << "* HUD CREATED *" << std::endl;
}

HUD::~HUD() {
	// std::cout << "~ HUD DESTROYED ~" << std::endl;
}

int HUD::getElapsedSeconds() const {
	auto now = std::chrono::steady_clock::now();
	return std::chrono::duration_cast<std::chrono::seconds>(now - _startTime).count();
}

void HUD::render(WINDOW *win, int score, int lives) {
	int seconds = getElapsedSeconds();
	mvwprintw(win, HEIGHT - 1, 0, "SCORE: %-6d | LIVES: %d | TIME: %02d:%02d",
			  score, lives, seconds / 60, seconds % 60);
}
