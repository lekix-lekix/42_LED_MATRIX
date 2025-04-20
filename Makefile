# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lekix <lekix@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 16:29:27 by kipouliq          #+#    #+#              #
#    Updated: 2025/04/20 03:04:54 by lekix            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = LED_MATRIX

CXX = gcc

CFLAGS = -Wall -Wextra -Werror -Wno-conversions -pthread

SRCS = ./src/mlx_utils.c \
		./src/main.c \
		./src/radial_gradient.c \
		./src/linear_gradient.c \
		./src/conway.c \
		./src/colors.c

PATH_MLX = ./minilibx-linux

MLX = ./minilibx-linux/libmlx.a

MLX_FLAGS = -I$(PATH_MLX) -lmlx -lXext -lX11 -lm 

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(MLX):
	make -C $(PATH_MLX)

$(NAME): $(MLX) $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -L ./minilibx-linux $(MLX_FLAGS) -o $@

$(OBJS_PATH):
	mkdir -p $(OBJS_PATH)

$(OBJS_PATH)%.o: $(SRCS)%.c
	$(CXX) $(CFLAGS) -c $< -o $@

clean:
	rm -rf ./src/*.o

fclean:	clean
	rm -f $(NAME)
	make clean -C $(PATH_MLX)

re: fclean
	make all

.PHONY: all clean fclean re