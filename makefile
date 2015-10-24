# Files:
OBJS = main.cpp

# Compiler:
CC = g++

# Compiler flags:
C_FLAGS = -Wall -Wextra

# Libraries:
L_FLAGS = -lsfml-graphics

# Name and directory of the executable file:
OBJ_NAME = app
OBJ_DIR = bin/

# Target:
all : $(OBJS)
	$(CC) $(OBJS) $(C_FLAGS) $(L_FLAGS) -o $(OBJ_DIR)$(OBJ_NAME)

run:
	./$(OBJ_DIR)$(OBJ_NAME)

clean:
	$(RM) -v *.o *~ $(OBJ_DIR)$(OBJ_NAME)

.PHONY: all clean

