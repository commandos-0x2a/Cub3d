NAME		= cub3d

CC			= cc
# LDFLAGS		= -L./libft -lft -lreadline -lncurses
LDFLAGS		= -lbsd
CFLAGS		= -Wall -Wextra -Werror -g
# CPPFLAGS	= -I./libft/include -I./include

SRCDIR = src
OBJDIR = build

ifeq ($(DEBUG), 1)
CFLAGS += -DDEBUG
endif

FILES = 	main					\
			read_map			\
			get_next_line/get_next_line		\
			get_next_line/get_next_line_utils		\
			
#

OBJECTS = $(FILES:%=$(OBJDIR)/%.o)

all: $(NAME)


$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all bonus libft clean fclean re test