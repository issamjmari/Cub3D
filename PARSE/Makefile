# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ael-hiou <ael-hiou@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/09/05 14:43:07 by ael-hiou          #+#    #+#              #
#    Updated: 2022/09/09 14:39:57 by ael-hiou         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


SRC = map_parsing/parsing.c library/ft_split.c library/ft_strncmp.c includes/get_next_line.c \
includes/get_next_line_utils.c library/ft_lstclear.c library/ft_isalpha.c library/ft_atoi.c\
library/ft_isalnum.c library/ft_isdigit.c library/ft_substr.c map_parsing/map_validation.c \
library/ft_strtrim.c map_parsing/parsing_utils.c library/ft_strchr.c
CC = cc
NAME = cub3d
# CFLAGS = -Wall -Wextra -Werror
SRC_O = $(SRC:.c=.o)
all :$(NAME)
#
$(NAME) : $(SRC)
	@$(CC)  -fsanitize=address -g $(SANITIZE) $(CFLAGS) $(SRC) -o $(NAME) 
clean:
	@rm -f $(SRC_O)
fclean:clean
	@rm -f $(NAME)
re: fclean all