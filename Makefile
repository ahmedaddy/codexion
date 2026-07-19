NAME = codexion
SRC = src

CC = cc
CFLAGS = -pthread -g
SRCS = $(SRC)/main.c $(SRC)/parsing.c $(SRC)/debugger.c $(SRC)/init.c $(SRC)/coder.c $(SRC)/utils.c $(SRC)/heap.c $(SRC)/dongle.c $(SRC)/monitor.c
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
