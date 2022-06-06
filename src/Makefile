#OBJS sepecifies wich files to compile as part of the project
# OBJS = main.cpp window.cpp map.cpp player.cpp ray.cpp mouse.cpp scene.cpp keyboard.cpp myMathLib.cpp collision.cpp
OBJS = $(wildcard **/*.cpp) main.cpp

#CC specifies compiler
CC = g++

#INCLUDE_PATHS specifies additional includes well need
INCLUDE_PATHS = -IC:\Users\cbent\dev\MinGW_Libs\include\SDL2

#LIBRARY_PATHS specifies additional liberary paths well need
LIBRARY_PATHS = -LC:\Users\cbent\dev\MinGW_Libs\lib

#COMPILER_FLAGS specifies the additional compilation options were using
# -Werror error descriptions
# -std=c++11 c++11 standard
# -g debug mode
COMPILER_FLAGS = -g -Werror -std=c++11

#LINKER_FLAGS specifies the libraries were linking againts
# -lm is to compile math lib
# -others are neccesary for SDL
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our executable
OBJ_NAME = mazeProject

# this is the target that compiles our executable
all: $(OBJS)
	$(CC) $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME).exe
