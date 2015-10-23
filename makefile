# Files:
OBJS = main.c

# Compiler:
CC = gcc

# Compiler flags:
C_FLAGS = -Wall

# Libraries:
L_FLAGS = -lSDL2

# Name of the executable file:
OBJ_NAME = app

# Targets:
all : $(OBJS)
	$(CC) $(OBJS) $(C_FLAGS) $(L_FLAGS) -o $(OBJ_NAME)

clean:
	$(RM) -v *.o *~ $(OBJ_NAME)
