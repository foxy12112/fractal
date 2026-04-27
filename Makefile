# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ldick <ldick@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/18 19:28:14 by ldick             #+#    #+#              #
#    Updated: 2025/06/03 14:50:41 by ldick            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = fractal

#################################################################################################
#											Colors												#
#################################################################################################

CLR_RMV		:= \033[0m
RED			:= \033[1;31m
GREEN		:= \033[1;32m
YELLOW		:= \033[1;33m
BLUE		:= \033[1;34m
CYAN 		:= \033[1;36m
BOLD_BLUE	:= \033[0;34m
NC			:= \033[0m

#################################################################################################
#											Flags												#
#################################################################################################

COMPILER	=	gcc
INCLUDES	=	-I includes
CFLAGS		=	-g -fsanitize=address
EXTRA_FLAGS	=	-O3 -march=native -flto -ffast-math -funroll-loops -ftree-vectorize -pthread -lglfw -lGL -ldl -lm
ERROR_FILE	=	error.log

#################################################################################################
#											Sources												#
#################################################################################################

_SRCS			=	main.c glad.c text.c utils.c
SRCS			=	$(addprefix srcs/, $(_SRCS))

OBJS			=	$(SRCS:srcs/%.c=bin/%.o)
LIBRARY			=	main-libs/libs.a

#################################################################################################
#											MLX													#
#################################################################################################

USER = $(shell whoami)
OS = $(shell uname)
# OS = ("MINISHELL");

	OPENGL = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

#################################################################################################
#											Rules												#
#################################################################################################

all:			$(NAME)

bin:
				@echo "\t\t\t$(BLUE) Making bin directory"
				@mkdir -p bin

bin/%.o:		srcs/%.c | bin
				@echo "$(GREEN) Compiling $(Compiler) $(CLR_RMV) -c -o $(YELLOW) $@ $(CYAN) $^ $(GREEN) $(EXTRA_FLAGS) $(CFLAGS) $(GREEN) $(INCLUDES) $(NC)"
				@$(COMPILER) -c -o $@ $^ $(EXTRA_FLAGS) $(CFLAGS) $(INCLUDES) 2> $(ERROR_FILE) || (cat $(ERROR_FILE) && echo "$(RED)Compilation failed :0\nfailed file: \t\t$(YELLOW)$<$(NC)\n\n" && exit 1)

$(NAME): $(OBJS)
				@$(COMPILER) -o $(NAME) $(OBJS) $(OPENGL) $(EXTRA_FLAGS) $(CFLAGS)
				@echo "\t\t\t\t$(RED) compilation success :3$(NC)"

clean:
				@rm -rf bin
				@rm -f $(ERROR_FILE)

fclean:			clean
				@rm -f $(NAME)

soft_clean:
				@rm -f $(NAME)
				@rm -rf bin

re:				fclean all

res:			soft_clean all

.PHONY:			all clean fclean re