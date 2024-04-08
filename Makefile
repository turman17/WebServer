CC = c++
CFLAGS = -std=c++11 -g -I./include # -Wall -Wextra -Werror
NAME = webserv

SRC = $(shell find ./srcs -name '*.cpp')

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

re: fclean all
