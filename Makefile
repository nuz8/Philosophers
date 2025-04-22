# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2025/04/22 16:53:38 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# ----------------- Compiler Flags ----------------- #

CC			= 	gcc -g
# CC			= 	gcc -g -fsanitize=address
CFLAGS		= 	-Wall -Wextra -Werror -pthread
DEPFLAGS	= 	-MMD

# ----------------- Commands ----------------- #

RM			= 	rm -rf

# ----------------- Default Directories ----------------- #

# D_BIN		=	bin
D_INC		=	include
D_LIB		=	lib
D_OBJ		=	obj

D_SRC		=	src
D_TEST		=	test_src

# ----------------- Docker files ----------------- #

DK_FILE		=	Dockerfile
DK_COMP		=	docker-compose.yml

# Define the script name
DOCK_BUILD_SCRIPT = dock_build.sh

# Define the Docker image name
IMAGE_NAME = valgrind

# ----------------- Source, Object and Dependency files ----------------- #

SRC_MAIN	=	main.c errors.c init_df.c spawners.c string_utils.c parser.c utils.c timers.c
SRC_TEST	=	test_prints.c

# VPATH		+=	$(D_PTHREADS) $(D_SRC)
VPATH		+=	$(D_SRC) $(D_TEST)

SRC			=	$(SRC_MAIN) $(SRC_TEST)
OBJ 		+= 	$(addprefix $(D_OBJ)/, $(SRC:.c=.o))
DEP			= 	$(OBJ:.o=.d)

# ----------------- Target Binary ----------------- #
NAME		=	philo
ARGS		=	5 200 200 200

# ----------------- Headers Flag ----------------- #
HEADS_FLG	=	-I$(D_INC)

# ----------------- Libraries ----------------- #

LIBS_FLG		=	$(LIBFT_FLG) $(READLINE_FLG)

# LIBFT
LIBFT			=	libft.a
LIBFT_DIR		=	$(D_LIB)
LIBFT_HED		=	$(D_LIB)/includes
LIBFT_FLG		=	-L$(LIBFT_DIR) -l$(basename $(subst lib,,$(LIBFT)))

# ----------------- Valgrind ----------------- #
VALGRIND_OPTS	= 	--leak-check=full \
					--show-leak-kinds=all \
					--trace-children=yes \
					--track-fds=yes \
					-s
# --child-silent-after-fork=yes

# ----------------- Helgrind ----------------- #
HELGRIND_OPTS =		--tool=helgrind \
					--track-lockorders=yes \
					--history-level=full \
					-s

# Leak Sanitizer for leak check on Mac (run: make LEAK=1)
LEAKSAN			=	liblsan.dylib
LEAKSAN_DIR		=	/Users/dstinghe/LeakSanitizer
LEAKSAN_HED		= 	/Users/dstinghe/LeakSanitizer/include
LEAKSAN_FLG		=	-L$(LEAKSAN_DIR) -l$(basename $(subst lib,,$(LEAKSAN)))
		
ifeq ($(LEAK), 1)
	LIBS_FLG	+=	$(LEAKSAN_FLG)
endif

# ----------------- Color Codes ----------------- #

RESET 		= 	\033[0m
RED 		= 	\033[31m
GREEN 		= 	\033[32m
YELLOW 		= 	\033[33m
BLUE 		= 	\033[34m

# ----------------- Rules ----------------- #

all: $(NAME)
	@echo "$(GREEN)Compilation finished$(RESET)"

$(NAME): $(OBJ)
	@echo "Compiling..."
	@make -sC $(D_LIB)
	@$(CC) $(CFLAGS) $(HEADS_FLG) $^ $(LIBS_FLG) -o $@ 

$(D_OBJ)/%.o: %.c
	@$(CC) $(CFLAGS) $(HEADS_FLG) $(DEPFLAGS) -c $< -o $@

-include $(DEP)

clean:
	@$(RM) $(OBJ) $(DEP)
	@make clean -sC $(D_LIB)
	@echo "$(YELLOW)Cleaned Objects & Dependencies$(RESET)"

fclean:
	@$(RM) $(OBJ) $(DEP) $(NAME)
	@make fclean -sC $(D_LIB)
	@echo "$(YELLOW)Cleaned All$(RESET)"

re: fclean all

valgrind: $(NAME)
	@valgrind $(VALGRIND_OPTS) ./$(NAME) $(ARGS)

mutex: $(NAME)
	@valgrind $(HELGRIND_OPTS) ./$(NAME) $(ARGS)

format:
	@c_formatter_42 main.c $(D_PTHREADS)/*.c $(D_SRC)/*.c $(D_INC)/*.h

dock:
	@docker build -t $(IMAGE_NAME) .
	@docker-compose run --rm $(IMAGE_NAME)

.PHONY: all clean fclean re valgrind valgrind_n norm format dock

