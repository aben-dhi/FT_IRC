SRCS =	main.cpp \
		srcs/Server.cpp \

HDRS = includes/Server.hpp \

NAME = ircserv

BLUE = \033[1;34m
RED = \033[1;31m

OBJS := $(SRCS:%.cpp=%.o)

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

all: credit $(NAME)

$(NAME): $(OBJS)
	c++ $(CPPFLAGS) $(OBJS) -o $@

%.o: %.cpp %.hpp
	c++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	@echo "All cleaned up!"

re: fclean all

credit:
	@echo " ┏━━━┓┏━━━━┓    ┏━━┓┏━━━┓┏━━━┓"
	@echo " ┃┏━━┛┃┏┓┏┓┃    ┗┫┣┛┃┏━┓┃┃┏━┓┃"
	@echo " ┃┗━━┓┗┛┃┃┗┛     ┃┃ ┃┗━┛┃┃┃ ┗┛"
	@echo " ┃┏━━┛  ┃┃       ┃┃ ┃┏┓┏┛┃┃ ┏┓"
	@echo " ┃┃    ┏┛┗┓┏━━━┓┏┫┣┓┃┃┃┗┓┃┗━┛┃"
	@echo " ┗┛    ┗━━┛┗━━━┛┗━━┛┗┛┗━┛┗━━━┛"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
	@echo "━━━━━━━━━┃ Made by : aben-dhi - rrakman ┃━━━━━━━━━"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

.PHONY: all clean fclean re