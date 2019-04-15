NAME = doom-nukem

SRC_PATH =		./src/
PATH_1 =	/data/
PATH_2 =	/draw/
SRC =	$(wildcard $(SRC_PATH)*.c)\
		$(wildcard $(SRC_PATH)$(PATH_1)*.c)\
		$(wildcard $(SRC_PATH)$(PATH_2)*.c)

BIN_PATH =		./bin
BIN = $(patsubst $(SRC_PATH)%.c,./bin/%.o,$(SRC))

INCLUDE_PATH = include
FW_PATH = ./frameworks
CC = gcc
CFLAGS += -Wall -Wextra -Werror -g -fsanitize=address -I./$(INCLUDE_PATH)\
														  -I./libft/includes/\
														  -I$(FW_PATH)/SDL2_image.framework/Headers/\
														  -I$(FW_PATH)/SDL2_ttf.framework/Headers/\
														  -I$(FW_PATH)/SDL2.framework/Headers/

LIBSDL2 = -framework SDL2 -F $(FW_PATH) -framework SDL2_image -framework SDL2_ttf -rpath $(FW_PATH)
LIBFT = libft
LIBRARIES = $(LIBSDL2) $(LIBFT)/$(LIBFT).a

all:	directory $(NAME)

$(NAME): $(BIN)
		make -C $(LIBFT)
		$(CC) $(CFLAGS) $(BIN) -o $(NAME) $(LIBRARIES)

$(BIN_PATH)/%.o : $(SRC_PATH)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

$(BIN_PATH)/$(SRC_PATH_1)/%.o : $(SRC_PATH)/$(SRC_PATH_1)/%.c
		$(CC) $(CFLAGS) -c $< -o $@

directory:
		@mkdir $(BIN_PATH) 2> /dev/null || true
		@mkdir $(BIN_PATH)/$(PATH_1) 2> /dev/null || true
		@mkdir $(BIN_PATH)/$(PATH_2) 2> /dev/null || true

clean:
		rm -rf $(BIN_PATH)
		make clean -C $(LIBFT)

fclean:	clean
		rm -rf $(NAME)
		make fclean -C $(LIBFT)

re:		fclean all

norme:
		@norminette $(SRC_PATH)
		@norminette $(INCLUDE_PATH)

.PHONY: all, clean, fclean, re, directory, norme
