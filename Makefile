NAME = cub3d

LIBMLX	:= ./lib/MLX42
LIBFT	:= ./lib/libft

CC = cc
CFLAGS = -Wall -Wextra -Werror
CFLAGS += -g #-fsanitize=address
CPPFLAGS = -I${LIBMLX}/include -I$(LIBFT)/include -Iinclude
LDFLAGS = -L$(LIBFT) -lft -lmlx42 -lglfw -lm -Lbuild #-fsanitize=address


UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S), Linux)
	LDFLAGS += -ldl -pthread
else ifeq ($(UNAME_S), Darwin)
	LDFLAGS += -L"/opt/homebrew/Cellar/glfw/3.4/lib"
	CPPFLAGS += -I"/opt/homebrew/Cellar/glfw/3.4/include"
endif

SRC += 	src/main.c							\
		src/utils.c							\
		src/render.c 						\
		src/controls.c						\
		src/game.c							\
		src/end_game.c						\
		src/hooks.c							\
		src/player.c						\
		src/raycasting.c					\
		src/vector.c						\
		\
		src/map/read_map.c					\
		src/map/validate_map.c				\
		src/map/validate_surrounded.c		\
		src/map/get_color.c					\
		src/map/print_validate_error.c		\
		src/map/read_map_utils.c			\
		src/map/surrounded_utils.c			\
		\
		src/image/read_image.c				\
		src/image/sprite.c					\
		src/image/sprite_reader.c			\
		src/image/sprite_to_animate.c		\
		src/image/read_pixels.c				\
		src/image/sprites_utils.c			\
		\
		src/minimap/minimap.c				\
		src/minimap/minimap_utils.c			\
		src/minimap/minimap_render.c		\

OBJ = $(SRC:%.c=build/%.o)

all: MLX42 $(NAME)

MLX42:
	cmake -S ${LIBMLX} -B build -Wno-dev
	cmake --build build -j4

libft:
	$(MAKE) -C $(LIBFT)

$(NAME): $(OBJ) | libft
	$(CC) -o $@ $^ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS)  -c $< -o $@

clean:
	rm -f $(OBJ)
	$(MAKE) -C $(LIBFT) clean
	rm -rf build

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re MLX42 test libft
