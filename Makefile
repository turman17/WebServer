CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98 -g -I./include
NAME = webserv

SRC = $(shell find ./src -name '*.cpp')

OBJ = $(patsubst %.cpp, %.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

v:
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes ./$(NAME)

re: fclean all
