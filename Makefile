#OBJS specifies which files to compile as part of the project
OBJS = ./sources/graphics/* ./sources/main/* ./sources/command_input/* ./sources/file_io/* ./sources/interface/* ./sources/logic/*

#CC specifies which compiler we're using
CC = clang++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./bin/light_game

#INCLUDE_DIRECTORIES specifies the directories where .h files are located
INCLUDE_DIRECTORIES = -I./headers -I/usr/include/SDL2

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) $(INCLUDE_DIRECTORIES) -o $(OBJ_NAME) -g
