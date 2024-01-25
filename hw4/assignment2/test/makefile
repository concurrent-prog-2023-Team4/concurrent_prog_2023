CXX = gcc             # C compiler
CXXFLAGS = -Wall -g # Compiler flags
LDFLAGS = -lpthread # Linker flags
SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)  # List of object files
TARGET = test   # Final executable name

# Main rule
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
