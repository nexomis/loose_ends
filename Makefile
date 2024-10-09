# Makefile

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11 -O2

# Source directory
SRC_DIR = src

# Object directory
OBJ_DIR = obj

# Binary name
TARGET = loose_ends

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Header files
DEPS = $(wildcard $(SRC_DIR)/*.h)

# Default target
all: $(TARGET)

# Rule to make the target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to make object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(DEPS) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Create object directory if it doesn't exist
$(OBJ_DIR):
	mkdir -p $@

# Clean up
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean