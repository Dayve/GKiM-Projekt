# Files:
OBJS = main.cpp

# Compiler:
CC = g++

# Compiler flags:
C_FLAGS = -Wall -Wextra

# Libraries:
L_FLAGS = -lsfml-graphics

# Name of the executable file:
OBJ_NAME = app

# Target:
all : $(OBJS)
	$(CC) $(OBJS) $(C_FLAGS) $(L_FLAGS) -o $(OBJ_NAME)

clean:
	$(RM) -v *.o *~ $(OBJ_NAME)

.PHONY: all clean

