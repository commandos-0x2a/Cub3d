# Sprite Reader Test Makefile
# This builds the sprite reader test separately from the main cub3d project

NAME = test_sprite_reader

CC = cc
CFLAGS = -Wall -Wextra -Werror -g
CPPFLAGS = -Iinclude

# Core sprite reader sources
SPRITE_SRC = src/image/sprite_reader_test.c \
			 scripts/test_sprite_reader_new.c

# Object files
OBJ = $(SPRITE_SRC:%.c=build_test/%.o)

# Build the test program
all: $(NAME)

$(NAME): $(OBJ)
	$(CC) -o $@ $^ -lm

build_test/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf build_test $(NAME)

fclean: clean

re: fclean all

.PHONY: all clean fclean re