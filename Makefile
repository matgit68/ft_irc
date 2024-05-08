RED		:= \033[31;1m
YELLOW	:= \033[33;1m
RESET	:= \033[0m

NAME = ircserv

CXX = c++
CPPFLAGS = -std=c++98 -Wall -Wextra -Werror -g3# -fsanitize=address

SRC =	Bot.cpp\
		Channel.cpp\
		Client.cpp\
		invite.cpp\
		join.cpp\
		kick.cpp\
		main.cpp\
		mode.cpp\
		nick.cpp\
		pass.cpp\
		ping.cpp\
		privmsg.cpp\
		quit.cpp\
		part.cpp \
		Server.cpp\
		ServerRun.cpp\
		time.cpp\
		topic.cpp\
		user.cpp\
		utils.cpp\
		who.cpp\

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	@$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJ)
	@printf '$(RED)- $(NAME) done -$(RESET)\n'

clean:
	@printf '$(YELLOW)Cleaning $(NAME) $(RESET)\n'
	@rm -f $(OBJ)

fclean: clean
	@rm -f $(NAME)

re: fclean $(NAME)
