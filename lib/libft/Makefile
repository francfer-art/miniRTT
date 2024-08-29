# Nombre de la biblioteca
NAME		= libft.a

# Directorio de origen de los archivos fuente (.c)
SRC_DIR		= src

# Directorio de destino de los archivos objeto (.o)
OBJ_DIR		= obj
INCLUDE = ./

# lsta de archivos fuente
SRC_FILES		= $(SRC_DIR)/ft_isalpha.c \
			$(SRC_DIR)/ft_isdigit.c \
			$(SRC_DIR)/ft_isalnum.c \
			$(SRC_DIR)/ft_isascii.c \
			$(SRC_DIR)/ft_isprint.c \
			$(SRC_DIR)/ft_isspace.c \
			$(SRC_DIR)/ft_strlen.c \
			$(SRC_DIR)/ft_memset.c \
			$(SRC_DIR)/ft_bzero.c \
			$(SRC_DIR)/ft_memcpy.c \
			$(SRC_DIR)/ft_memmove.c \
			$(SRC_DIR)/ft_strlcpy.c \
			$(SRC_DIR)/ft_strlcat.c \
			$(SRC_DIR)/ft_toupper.c \
			$(SRC_DIR)/ft_tolower.c \
			$(SRC_DIR)/ft_strchr.c \
			$(SRC_DIR)/ft_strrchr.c \
			$(SRC_DIR)/ft_strncmp.c \
			$(SRC_DIR)/ft_memchr.c \
			$(SRC_DIR)/ft_memcmp.c \
			$(SRC_DIR)/ft_strnstr.c \
			$(SRC_DIR)/ft_atoi.c \
			$(SRC_DIR)/ft_calloc.c \
			$(SRC_DIR)/ft_strdup.c \
			$(SRC_DIR)/ft_substr.c \
			$(SRC_DIR)/ft_strjoin.c \
			$(SRC_DIR)/ft_strtrim.c \
			$(SRC_DIR)/ft_split.c \
			$(SRC_DIR)/ft_split_space.c \
			$(SRC_DIR)/ft_itoa.c \
			$(SRC_DIR)/ft_strmapi.c \
			$(SRC_DIR)/ft_striteri.c \
			$(SRC_DIR)/ft_putchar_fd.c \
			$(SRC_DIR)/ft_putstr_fd.c \
			$(SRC_DIR)/ft_putendl_fd.c \
			$(SRC_DIR)/ft_putnbr_fd.c \
			$(SRC_DIR)/get_next_line.c \
			$(SRC_DIR)/ft_printf_utils.c \
			$(SRC_DIR)/ft_printf.c \
			$(SRC_DIR)/ft_error.c \
			$(SRC_DIR)/ft_atod.c \
			$(SRC_DIR)/ft_abs.c \
			$(SRC_DIR)/ft_strtok.c \
			$(SRC_DIR)/ft_strtok_r.c \
			$(SRC_DIR)/ft_strcmp.c \
			$(SRC_DIR)/ft_strcpy.c \
			$(SRC_DIR)/ft_strcat.c \
			$(SRC_DIR)/ft_strnlen.c \
			$(SRC_DIR)/ft_strndup.c \
			$(SRC_DIR)/ft_strncpy.c \
			$(SRC_DIR)/ft_strncat.c \
			$(SRC_DIR)/ft_strcspn.c \
			$(SRC_DIR)/ft_strstr.c \

SRC_BONUS_FILES	= $(SRC_DIR)/ft_lstnew_bonus.c \
			$(SRC_DIR)/ft_lstadd_front_bonus.c \
			$(SRC_DIR)/ft_lstlast_bonus.c \
			$(SRC_DIR)/ft_lstadd_back_bonus.c \
			$(SRC_DIR)/ft_lstdelone_bonus.c \
			$(SRC_DIR)/ft_lstsize_bonus.c \
			$(SRC_DIR)/ft_lstclear_bonus.c \
			$(SRC_DIR)/ft_lstiter_bonus.c \
			$(SRC_DIR)/ft_lstmap_bonus.c

# Genera la lsta de archivos objeto a partir de la lsta de archivos fuente
OBJ		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))
BONUS_OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_BONUS_FILES))
# DEP = $(addsuffix .d, $(basename $(SRC_FILES)))

# Comandos y variables adicionales
CC			= clang -Wall -Wextra -Werror -MMD -I$(INCLUDE)
RM			= rm -f

# Colours
BLACK				=	\033[0;30m
RED					=	\033[0;31m
GREEN				=	\033[0;32m
YELLOW				=	\033[0;33m
BLUE				=	\033[0;34m
PURPLE				=	\033[0;35m
CYAN				=	\033[0;36m
WHITE				=	\033[0;37m
RESET				=	\033[0m

# Regla para compilar un archivo fuente en un archivo objeto en el directorio OBJ_DIR
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@$(CC) -c $< -o $@


all: $(NAME)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $^
	@ranlib $(NAME)
	@printf "$(BLUE)==> $(CYAN)Libft compiled\n$(RESET)"

bonus: $(NAME) $(BONUS_OBJ)
	@ar rcs $^
	@printf "$(BLUE)==> $(CYAN)Libft bonus compiled\n$(RESET)"

clean:
	@$(RM) $(OBJ) $(BONUS_OBJ)
	@$(RM) -r ./$(OBJ_DIR)
	@printf "$(BLUE)==> $(RED)Removed Libft\n$(RESET)"

fclean: clean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus