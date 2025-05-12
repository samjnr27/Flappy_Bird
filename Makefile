#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mmoumini <mmoumini@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/05/12 13:41:19 by mmoumini          #+#    #+#              #
#    Updated: 2025/04/27 17:46:43 by mmoumini         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

.PHONY = all, clean, fclean, re

ifeq ($(OS),Windows_NT)
    NAME = FlappyBird.exe
	CC	 = g++
    LIB_PATH = -L./lib/windows \
        -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio \
        -lopengl32 -lwinmm -lgdi32
else
    NAME = FlappyBird
	CC	 = clang++
    LIB_PATH = -L./lib/macos -lFLAC 						\
		-lsfml-graphics -lvorbis 					\
		-lfreetype -lsfml-network 					\
		-lvorbisenc -logg -lsfml-system 			\
		-lvorbisfile -lsfml-audio -lsfml-window 	\
		-framework OpenGL -framework AppKit 		\
		-framework IOKit -framework CoreServices 	\
		-framework Carbon -framework OpenAL

endif

CFLAGS = -Wall -Wextra -Werror -std=c++17

INC = -I./include

SRC	=	src/main.cpp		

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		$(CC) -o $(NAME) $(OBJ) $(LIB_PATH) $(CFLAGS) $(INC)

%.o: %.cpp
	$(CC) -o $@ -c $< $(INC) $(CFLAGS)

clean:
		rm -rf $(OBJ)

fclean: clean
		rm -rf $(NAME)

re: fclean all

.PHONY: all clean re fclean