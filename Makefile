NAME    = ft_shmup

CXX     = c++
CXXFLAGS = -Wall -Wextra -Werror

INCLUDES = -I include
LIBS     = -lncurses

SRCS    = srcs/main.cpp \
		  srcs/AGameEntity.cpp \
		  srcs/AProjectile.cpp \
		  srcs/Ship.cpp \
		  srcs/Player.cpp \
		  srcs/Enemy.cpp \
		  srcs/Scenery.cpp \
		  srcs/ScrollingMap.cpp \
		  srcs/World.cpp \
		  srcs/Game.cpp \
		  srcs/InputHandler.cpp \
		  srcs/Render.cpp \
		  srcs/HUD.cpp

OBJS    = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
