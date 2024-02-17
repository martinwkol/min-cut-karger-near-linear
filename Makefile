# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++17 -Wall

# Source files directory
SRC_DIR = src

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=%.o)

# Executable name
TARGET = mincut-karger-simple.exe

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Clean up the generated files
clean:
	del /Q $(OBJS) $(TARGET)
