#OBJS sepecifies wich files to compile as part of the project
OBJS = main.c sdl_window.c loadImg.c

#CC specifies compiler
CC = gcc

#INCLUDE_PATHS specifies additional includes well need
INCLUDE_PATHS = -IC:\Users\cbent\dev\MinGW_Libs\include\SDL2

#LIBRARY_PATHS specifies additional liberary paths well need
LIBRARY_PATHS = -LC:\Users\cbent\dev\MinGW_Libs\lib

#COMPILER_FLAGS specifies the additional compilation options were using
# -w surpresses all warnings
# -Werror error descriptions
COMPILER_FLAGS = -Werror

#LINKER_FLAGS specifies the libraries were linking againts
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2 -lSDL2_image

#OBJ_NAME specifies the name of our executable
OBJ_NAME = mazeProject


# this is the target that compiles our executable
all: $(OBJS)
	gcc -v $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME).exe
