# Simple Makefile for QueueBee
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -Wpedantic -pthread
CXXFLAGS_DEBUG = -g -O0
CXXFLAGS_RELEASE = -O3

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin

# Source files
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/server.cpp
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET = $(BIN_DIR)/queuebee

# Default target
all: debug

# Debug build
debug: CXXFLAGS += $(CXXFLAGS_DEBUG)
debug: $(TARGET)

# Release build
release: CXXFLAGS += $(CXXFLAGS_RELEASE)
release: $(TARGET)

# Create directories
$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

# Build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Link executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Run the program
run: $(TARGET)
	$(TARGET) --help

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

# Rebuild
rebuild: clean all

.PHONY: all debug release clean rebuild run