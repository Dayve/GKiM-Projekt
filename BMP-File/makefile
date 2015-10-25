# Compiler:
CXX = g++

# File removing program & parameters (for "make clean"):
RM = rm -f -v

# Compiler and preprocessor flags:
CXXFLAGS = -Wall -Wextra
CPPFLAGS = -I include

# Libraries:
LDLIBS = -lsfml-graphics

# Folders: (not necessary)
vpath %.cpp src
vpath %.hpp %.h include

# Make checks if executable file needs to be recompiled, so we also give it this path:
vpath % bin

# Paths:
BIN_DIR = bin/
OBJ_DIR = obj/
SRC_DIR = src/

# Executable file name:
EXE_NAME = app

# ------------------------------------------------------- Files:

SRC_LIST = $(wildcard $(SRC_DIR)*.cpp)
OBJ_LIST = $(SRC_LIST:$(SRC_DIR)%.cpp=$(OBJ_DIR)%.o)

# Main target:
$(EXE_NAME): $(OBJ_LIST)
	$(CXX) $^ -o $(BIN_DIR)$(EXE_NAME) $(LDLIBS)

$(OBJ_DIR)%.o: $(SRC_DIR)%.cpp
	$(CXX) $(CPPFLAGS) -c $< -o $@ $(CXXFLAGS)

# ------------------------------------- Additional dependencies:

obj/main.o: include/test.hpp
obj/test.o: include/test.hpp

# --------------------------------------------------------------

# Cleaning:
clean:
	$(RM) bin/$(EXE_NAME) obj/*.o

# Running:
run:
	./$(BIN_DIR)$(EXE_NAME)

# Phony targets:
.PHONY:
	all run clean

