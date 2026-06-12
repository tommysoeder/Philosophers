NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror

SRCS = main.c parse.c time.c init.c cleanup.c simulation.c routine.c print.c \
	state.c monitor.c
OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
