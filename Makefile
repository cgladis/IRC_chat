NAME = ircserv
PORT = 6000
PASSWORD = 123

#BOTS
BOT_PATH = ./numbers_bot_folder/
BOT_NAME = numbers_bot

#COLORS
RED = \033[0;31m
GREEN = \033[0;32m
GOLD = \033[0;33m
BLUE = \033[0;34m
RESET = \033[0m

#FOLDERS
SRC_FOLDER = ./src/
OBJ_FOLDER = ./obj/
HPP_FOLDER = ./includes/

#FILES
SRC_NAME = ircserv.cpp Server.cpp Database.cpp Connection.cpp User.cpp Сhannel.cpp Message.cpp

OBJ_NAME = $(SRC_NAME:.cpp=.o)
OBJ = $(addprefix $(OBJ_FOLDER), $(OBJ_NAME))
SRC = $(addprefix $(SRC_FOLDER), $(SRC_NAME))

CC = clang++
FLAGS = -I $(HPP_FOLDER) -Wall -Wextra -Werror -std=c++98 -g

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.cpp

	@mkdir -p $(OBJ_FOLDER)
	@printf  "\r%-25s$(BLUE)[$<]$(RESET)\n" "$(NAME): $@"
	@$(CC) $(FLAGS) -o $@ -c $<
	@printf "\033[A\033[2K"

all: $(NAME)

$(NAME): $(OBJ)

	@$(CC) $(FLAGS) $(OBJ) -o $(NAME)
	@printf  "%-25s$(GREEN)[✔]$(RESET)\n" "$(NAME): $@"

clean:

	@$(MAKE) -C $(BOT_PATH) clean
	@rm -rf $(OBJ_FOLDER)
	@printf  "%-25s$(GREEN)[✔]$(RESET)\n" "$(NAME): $@"

fclean:

	@$(MAKE) -C $(BOT_PATH) fclean
	@rm -rf $(OBJ_FOLDER)
	@rm -f $(NAME)
	@rm -f $(BOT_NAME)
	@printf  "%-25s$(GREEN)[✔]$(RESET)\n" "$(NAME): $@"

re: fclean all

bot:
	@$(MAKE) -C $(BOT_PATH)
	@cp $(BOT_PATH)$(BOT_NAME) .

test: all bot

	@./$(NAME) $(PORT) $(PASSWORD) > /dev/null 2>&1 &
	@./$(BOT_NAME) $(PORT) $(PASSWORD) > /dev/null 2>&1 &

leaks: all
	@leaks --atExit -- ./$(NAME)

.PHONY:				all clean fclean re test leaks
