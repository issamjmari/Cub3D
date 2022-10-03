SRC = c_files/cub3d.c map_parsing/parsing.c library/ft_split.c library/ft_strncmp.c includes/get_next_line.c \
includes/get_next_line_utils.c library/ft_lstclear.c library/ft_isalpha.c library/ft_atoi.c \
library/ft_isalnum.c library/ft_isdigit.c library/ft_substr.c map_parsing/map_validation.c \
library/ft_strtrim.c map_parsing/parsing_utils.c library/ft_strchr.c c_files/init_variables.c c_files/draw.c \
c_files/3d_handling.c c_files/map_image.c c_files/handle_events.c c_files/rays1.c c_files/rays2.c
CC = cc
CFLAGS =  -lmlx -framework OpenGL -framework AppKit -Wall -Wextra -Werror -fsanitize=address
NAME = cub3d

all : $(NAME)
$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean :
fclean :
	rm -f $(NAME)
re : fclean all