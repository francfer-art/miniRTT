NAME = miniRT
OBJ_DIR = obj
SRC_DIR = src

# Libft
LIBFT = libft.a
LIBFT_SRC = ./lib/libft/

# Compiler
RM					=	rm -f
INCLUDE 			= 	-I ./includes -I $(LIBFT_SRC) -I ./lib/minilibx-linux
# CFLAGS				=	-Wall -Werror -Wextra 
# CC					=	gcc -g $(CFLAGS) $(INCLUDE)
CC					=	clang -O0 -g -MMD $(CFLAGS) $(INCLUDE)
# Colours
RED					=	\033[0;31m
GREEN				=	\033[0;32m
YELLOW				=	\033[0;33m
BLUE				=	\033[0;34m
PURPLE				=	\033[0;35m
CYAN				=	\033[0;36m
WHITE				=	\033[0;37m
RESET				=	\033[0m


SRC_FILES = $(SRC_DIR)/camera.c\
			$(SRC_DIR)/color.c\
			$(SRC_DIR)/cylinder.c\
			$(SRC_DIR)/elements.c\
			$(SRC_DIR)/error.c\
			$(SRC_DIR)/events.c\
			$(SRC_DIR)/free.c\
			$(SRC_DIR)/main.c\
			$(SRC_DIR)/parser.c\
			$(SRC_DIR)/plane.c\
			$(SRC_DIR)/render.c\
			$(SRC_DIR)/scene.c\
			$(SRC_DIR)/server.c\
			$(SRC_DIR)/sphere.c\
			$(SRC_DIR)/square.c\
			$(SRC_DIR)/triangle.c\
			$(SRC_DIR)/utils.c\
			$(SRC_DIR)/utils1.c\
			$(SRC_DIR)/utils2.c\
			$(SRC_DIR)/vector.c\

OBJ_FILES = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_FILES))
DEP = $(addsuffix .d, $(basename $(SRC_FILES)))

# Rules
all: $(NAME)

$(LIBFT_SRC)/libft.a:
	make -C $(LIBFT_SRC)
	make bonus -C $(LIBFT_SRC)

$(OBJ_DIR)/%.o: %.c Makefile
	@mkdir -p $(dir $@)
	$(CC) -c $< -o $@

$(NAME): $(LIBFT_SRC)/libft.a $(OBJ_FILES)
	make -C ./lib/minilibx-linux
	$(CC) $(OBJ_FILES) -L$(LIBFT_SRC) -L./lib/minilibx-linux -lXext -lX11 -lmlx -lm -lglfw -lft -o $(NAME)

-include $(DEP)

clean:
	$(RM) $(OBJ_FILES) $(DEP)
	rm -rf ./$(OBJ_DIR)
	make -C $(LIBFT_SRC) clean
	make -C ./lib/minilibx-linux clean
	@printf "$(BLUE)==> $(RED)Removed\n$(RESET)"

fclean: clean
	rm -rf $(NAME) 
	@make -C $(LIBFT_SRC) fclean
	make -C ./lib/minilibx-linux clean

re: fclean all
	@printf "$(BLUE)==> $(CYAN)Recompiled\n$(RESET)"

.PHONY: all clean fclean re
