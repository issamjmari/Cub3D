SRC = cub3d.c
CFLAGS =  -lmlx -framework OpenGL -framework AppKit
NAME = cub3d

all : $(NAME)
$(NAME) :
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)
clean :
fclean :
	rm -f $(NAME)
re : fclean all