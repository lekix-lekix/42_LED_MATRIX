# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/12/13 16:29:27 by kipouliq          #+#    #+#              #
#    Updated: 2025/03/27 19:04:41 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = LED_MATRIX

CXX = c++

FLAGS = -std=c++98 -Wall -Wextra -Werror -g

SRCS = ./src/main.cpp \
		./src/Mlx.cpp

PATH_MLX = ./minilibx-linux

MLX = ./minilibx-linux/libmlx.a

MLX_FLAGS = -lmlx -lXext -lX11 -lm 

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C $(PATH_MLX)
	$(CXX) $(FLAGS) $(OBJS) -L ./minilibx-linux $(MLX_FLAGS) -o $@

$(OBJS_PATH):
	mkdir -p $(OBJS_PATH)

$(OBJS_PATH)%.o: $(SCRS)%.cpp
	$(CXX) $(FLAGS) -c $< -o $@

clean:
	rm -rf ./src/*.o

fclean:	clean
	rm -f $(NAME)

re: fclean
	make all

.PHONY: all clean fclean re