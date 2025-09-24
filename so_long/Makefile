NAME = libso_long.a

CC = cc
CFLAGS += -Wall -Wextra -Werror -O2
CPPFLAGS += -I./include -I../libft/include -I ../MLX42/include

OBJECT_DIR = obj
SOURCE_DIR = src
BINARY_DIR = .

FILES =	so_long							\
		utils							\
		camera							\
		game_schema						\
		game_schema_utils				\
		hooks							\
		core/object						\
		core/object_utils				\
		core/put_image_to_image			\
		core/schema						\
		core/sprites					\
		player/player_hook				\
		player/player_movement			\
		player/player					\
		player/valid_move				\
		player/update_player			\
		components/banner				\
		components/fire					\
		components/components			\
		components/boat					\
		components/tree					\
		map/map_parser					\
		map/validate/map_validation		\
		map/validate/map_validation2	\
		map/validate/map_validation_boat_path	\
		map/validate/print_map_error	\
		map/grid_utils					\
		map/tile_utils					\
		map/map							\
		map/map_normalization			\
		map/tile_based					\

OBJECTS = $(FILES:%=$(OBJECT_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJECTS)
	ar rcs $@ $(OBJECTS)

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJECTS)

fclean: clean
	rm -rf $(NAME)

re: fclean all 

.PHONY: all clean fclean re libft library
