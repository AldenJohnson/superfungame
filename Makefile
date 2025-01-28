# Compiler and flags
CXX = g++
CXXFLAGS = -Isrc/include -std=c++11 -Wall
LDFLAGS = -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

# Source files and object files
SRC_FILES = main.cpp Items.cpp NPC.cpp World.cpp Player.cpp
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Output executable
EXEC = main

# Default target: compile, link, and clean
all: compile link clean

# Compile source files into object files
compile: $(OBJ_FILES)

# Link object files to create the executable
link: 
	$(CXX) $(OBJ_FILES) -o $(EXEC) $(LDFLAGS)

# Pattern rule for compiling individual .cpp files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable (Windows compatible)
clean:
	del /q $(OBJ_FILES) $(EXEC)
