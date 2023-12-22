CC = gcc            # C compiler
CFLAGS = -Wall -fsanitize=address -g  # Compiler flags
LDFLAGS = -fsanitize=address -lpthread  # Linker flags
SRC_FILES = $(wildcard *.c)   # Include all .c files in the directory
OBJ_FILES = $(SRC_FILES:.c=.o)  # List of object files
TARGET = test   # Final executable name

# Main rule
all: $(TARGET)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

# Clean compiled files
clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
