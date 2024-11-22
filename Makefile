SRCS =	main.cpp \
		srcs/Server.cpp \
		srcs/Client.cpp \
		srcs/Channel.cpp \
		srcs/Request.cpp \
		srcs/File.cpp \
		srcs/Join.cpp\
		srcs/Kick.cpp\
		srcs/Part.cpp\
		srcs/privMsg.cpp\
		srcs/RoboServ.cpp\
		srcs/commands.cpp\
		srcs/getSocket.cpp\
		srcs/message.cpp\
		srcs/pollHandling.cpp\
		srcs/requestHandling.cpp\
		srcs/transferFile.cpp\
		srcs/utils.cpp\

HDRS = includes/Server.hpp \
		includes/Client.hpp \
		includes/Channel.hpp \
		includes/Request.hpp \
		includes/File.hpp \

NAME = ircserv


OBJS := $(SRCS:%.cpp=%.o)

CPPFLAGS = -Wall -Wextra -Werror #-g -fsanitize=address #-pthread #-std=c++98

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
	@echo "━━━━━━━━━━━┃ Made by : aben-dhi - rrakman - mrital- ┃━━━━━━━━━━━"
	@echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"

.PHONY: all clean fclean re