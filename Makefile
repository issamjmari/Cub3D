SRC = cub3d.c fixed_point.c
CFLAGS =  -lmlx -framework OpenGL -framework AppKit -fsanitize=address
NAME = cub3d

all : $(NAME)
$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean :
fclean :
	rm -f $(NAME)
re : fclean all