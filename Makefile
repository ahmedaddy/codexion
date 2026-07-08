NAME = codexion

CC = cc
CFLAGS = -Wall -Wextra -Werror -pthread -g
SRCS = src/main.c src/parsing.c src/debugger.c src/data.c src/coder.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) && make clean

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
