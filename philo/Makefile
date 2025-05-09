# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pamatya <pamatya@student.42heilbronn.de    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: Invalid date        by                   #+#    #+#              #
#    Updated: 2025/05/09 02:01:52 by pamatya          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ------------------------------ Compiler Flags ------------------------------ #
CC			= 	gcc -g
CFLAGS		= 	-Wall -Wextra -Werror -pthread

# --------------------------------- Commands --------------------------------- #
RM			= 	rm -rf

# --------------------------- Default Directories --------------------------- #
D_INC		=	include
D_SRC		=	src
D_OBJ		=	obj

# ------------------- Source, object and dependency files ------------------- #
SRC_MAIN	=	main.c errors.c init_df.c spawners.c string_utils.c parser.c \
				utils.c timers.c simulation.c events.c loggers.c getter_fns.c \
				setter_fns.c synchronizers.c manager.c forks.c

VPATH		+=	$(D_SRC)

SRC			=	$(SRC_MAIN)
OBJ 		+= 	$(addprefix $(D_OBJ)/, $(SRC:.c=.o))
DEP			= 	$(OBJ:.o=.d)

# ------------------------------ Target Binary ------------------------------ #
NAME		=	philo

# ------------------------------- Headers Flag ------------------------------- #
HEADS_FLG	=	-I$(D_INC)

# ------------------------------- Color Codes ------------------------------- #
RESET 		= 	\033[0m
RED 		= 	\033[31m
GREEN 		= 	\033[32m
YELLOW 		= 	\033[33m
BLUE 		= 	\033[34m

# ---------------------------------- Rules ---------------------------------- #
all: $(NAME)
	@echo "$(GREEN)Compilation finished$(RESET)"

$(NAME): $(OBJ)
	@echo "Compiling..."
	@$(CC) $(CFLAGS) $(HEADS_FLG) $^ -o $@ 

$(D_OBJ)/%.o: %.c
	@$(CC) $(CFLAGS) $(HEADS_FLG) -c $< -o $@

clean:
	@$(RM) $(OBJ)
	@echo "$(YELLOW)Cleaned Objects & Dependencies$(RESET)"

fclean:
	@$(RM) $(OBJ) $(NAME)
	@echo "$(YELLOW)Cleaned All$(RESET)"

re: fclean all

.PHONY: all clean fclean re
