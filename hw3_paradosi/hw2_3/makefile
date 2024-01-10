CXX = gcc
CXXFLAGS = -Wall -fsanitize=address -g -DDEBUG
LDFLAGS = -fsanitize=address -lpthread 
SRC_FILES = $(wildcard *.c) $(filter-out ../assignment1/test.c, $(wildcard ../assignment1/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = hw2_3

all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
