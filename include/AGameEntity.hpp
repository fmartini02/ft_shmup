#ifndef AGAMEENTITY_HPP
#define AGAMEENTITY_HPP

#include <ncurses.h>
#include <iostream>

class AGameEntity{
private:
	float	_x;
	float	_y;
	char	_symbol;
	bool	_alive;
public:
	AGameEntity(float x, float y, char symbol);
	virtual ~AGameEntity();

	virtual void	update(float dt) = 0;
	virtual void	render(WINDOW *win) = 0;

	float			getX() const;
	float			getY() const;
	char			getSymbol() const;
	bool			isAlive() const;
	void			setX(float x);
	void			setY(float y);
	void			setAlive(bool alive);
};

#endif
