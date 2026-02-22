NAME    = ft_shmup

CXX     = c++
CXXFLAGS = -Wall -Wextra -Werror -g

INCLUDES = -I include
LIBS     = -lncursesw
VALGRIND_SUPP = file.supp

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

val: $(NAME)
	valgrind --leak-check=full \
	         --show-leak-kinds=all \
	         --track-origins=yes \
	         --error-exitcode=1 \
	         --log-file=valgrind.log \
	         --suppressions=$(VALGRIND_SUPP) \
	         ./$(NAME)
	@echo "Report salvato in valgrind.log"
	@cat valgrind.log

$(VALGRIND_SUPP):
	valgrind --leak-check=full \
	         --gen-suppressions=all \
	         ./$(NAME) 2>&1 | grep -A 10 "^{" > $(VALGRIND_SUPP)
clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
