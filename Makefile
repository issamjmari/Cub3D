SRC = c_files/cub3d.c map_parsing/parsing.c library/ft_split.c library/ft_strncmp.c includes/get_next_line.c \
includes/get_next_line_utils.c library/ft_lstclear.c library/ft_isalpha.c library/ft_atoi.c \
library/ft_isalnum.c library/ft_isdigit.c library/ft_substr.c map_parsing/map_validation.c \
library/ft_strtrim.c map_parsing/parsing_utils.c library/ft_strchr.c
CC = cc
CFLAGS =  -lmlx -framework OpenGL -framework AppKit -fsanitize=address -g
NAME = cub3d

all : $(NAME)
$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean :
fclean :
	rm -f $(NAME)
re : fclean all