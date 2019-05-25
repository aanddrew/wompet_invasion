#the files to compile as part of project
OBJS = main.cpp stage3/graphics/src/*.cpp stage3/graphics/stbi/*.c stage3/*.cpp game/src/*.cpp utils/src/*.cpp

#the compiler we will use
CC = g++

# the flags for the compiler 
#-w surpresses all warnings
COMPILER_FLAGS = -std=c++11 -w

#the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lGL -lglut -lGLEW -lm

#the name of our executable
OBJ_NAME = a.out

all:
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
