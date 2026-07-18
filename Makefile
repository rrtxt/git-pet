# Compiler
CXX = g++

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = bin

# Output executable
TARGET = $(BIN_DIR)/app
LDFLAGS = -lgit2

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++20 -I$(INC_DIR) -Isrc 

# Find all source files
# SRCS := $(wildcard $(SRC_DIR)/*.cpp)
SRCS := $(shell find $(SRC_DIR) -name "*.cpp")

# Convert src/main.cpp -> build/main.o
OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(TARGET)

# Link
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run program
run: all
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild run
