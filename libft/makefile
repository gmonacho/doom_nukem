# **************************************************************************** #
#                                                           LE - /             #
#                                                               /              #
#    Makefile                                         .::    .:/ .      .::    #
#                                                  +:+:+   +:    +:  +:+:+     #
#    By: gmonacho <marvin@le-101.fr>                +:+   +:    +:    +:+      #
#                                                  #+#   #+    #+    #+#       #
#    Created: 2018/10/02 16:01:28 by gmonacho     #+#   ##    ##    #+#        #
#    Updated: 2019/02/07 17:11:26 by gmonacho    ###    #+. /#+    ###.fr      #
#                                                          /                   #
#                                                         /                    #
# **************************************************************************** #

NAME = libft.a

CC = gcc

SRC = ./ft_atoi.c\
	./ft_abs.c\
	./ft_bzero.c\
	./ft_isalnum.c\
	./ft_isalpha.c\
	./ft_isascii.c\
	./ft_isdigit.c\
	./ft_isprint.c\
	./ft_itoa.c\
	./ft_memalloc.c\
	./ft_memccpy.c\
	./ft_memchr.c\
	./ft_memcmp.c\
	./ft_memcpy.c\
	./ft_memdel.c\
	./ft_memmove.c\
	./ft_memset.c\
	./ft_putchar.c\
	./ft_putchar_fd.c\
	./ft_putendl.c\
	./ft_putendl_fd.c\
	./ft_putnbr.c\
	./ft_putnbr_fd.c\
	./ft_putstr.c\
	./ft_putstr_fd.c\
	./ft_strcat.c\
	./ft_strchr.c\
	./ft_strclr.c\
	./ft_strcmp.c\
	./ft_strcpy.c\
	./ft_strdel.c\
	./ft_strdup.c\
	./ft_strequ.c\
	./ft_strjoin.c\
	./ft_strnequ.c\
	./ft_strlcat.c\
	./ft_strlen.c\
	./ft_strncat.c\
	./ft_strncmp.c\
	./ft_strncpy.c\
	./ft_strnew.c\
	./ft_strnstr.c\
	./ft_strrchr.c\
	./ft_strsplit.c\
	./ft_strstr.c\
	./ft_strsub.c\
	./ft_strtrim.c\
	./ft_tolower.c\
	./ft_striter.c\
	./ft_striteri.c\
	./ft_strmap.c\
	./ft_strmapi.c\
	./ft_toupper.c\
	./ft_lstadd.c\
	./ft_lstaddend.c\
	./ft_lstdel.c\
	./ft_lstdelone.c\
	./ft_lstiter.c\
	./ft_lstmap.c\
	./ft_lstnew.c\
	./ft_swap.c\
	./ft_2dintdel.c\
	./ft_count_char.c\
	./ft_2dintpushback.c\
	./get_next_line.c\
	./ft_index.c\
	./ft_2dputint.c\
	./ft_tabputint.c\
	./ft_addtofloat.c \
	./ft_countnumbers.c \
	./ft_round.c \
	./ft_intdel.c \
	./ft_2dstrdel.c

HEADER = ./includes/libft.h

OBJ = $(SRC:.c=.o)

CFLAGS += -Wall -Wextra  -Werror

all: $(NAME)

%.o: %.c 
		@$(CC) $(CFLAGS) -c $< -o $@
		@printf "#"

$(NAME): $(OBJ)
		@printf "#"
		@ar rcs $(NAME) $(OBJ)
		@echo "\nlib compiled"

clean:
	@rm -rf $(OBJ)
	@echo "objects deleted"

fclean: clean
	rm -rf $(NAME)
	@echo "$(NAME) is deleted"

re: fclean all
