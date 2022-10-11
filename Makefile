SRC = 3d_handling.c             ft_atoi.c                 get_next_line.c           parsing_utils1.c \
Cub3d.c                   ft_isalnum.c  \
ft_isalpha.c              get_next_line_utils.c      \
ft_isdigit.c              handle_events.c            \
angle_handling.c               init_variables.c        \
          rays1.c \
ft_split.c                          rays2.c \
ft_strchr.c               map_image.c             \
              map_validation.c           \
data_init.c               ft_strncmp.c              map_validation_utils.c \
draw.c                    ft_strtrim.c              parsing.c \
        ft_substr.c               parsing_utils.c

OBJ = $(SRC:.c=.o)
CC = cc
GFLAGS = -Wall -Wextra -Werror
MLX = -lmlx -framework OpenGL -framework AppKit
RM = rm -rf
NAME = Cub3d

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) $(MLX) -o $(NAME)
%.o: %.c
	$(CC) $(GFLAGS) -c $?
clean:
	$(RM) $(OBJ)
fclean : clean
	$(RM) $(NAME)
re : fclean all