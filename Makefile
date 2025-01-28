# Compiler and flags
CXX = g++
CXXFLAGS = -Isrc/include
LDFLAGS = -Lsrc/lib -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SOURCES = main.cpp bullet.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Output executable
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(OBJECTS) $(TARGET)