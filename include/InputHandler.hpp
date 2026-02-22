#ifndef INPUTHANDLER_HPP
#define INPUTHANDLER_HPP

class InputHandler {
private :
	int	_key;

public:
	InputHandler();
	~InputHandler();

	void	read();
	bool	isUp() const;
	bool	isDown() const;
	bool	isLeft() const;
	bool	isRight() const;
	bool	isShoot() const;
	bool	isQuit() const;
	int		getKey() const;
};

#endif
