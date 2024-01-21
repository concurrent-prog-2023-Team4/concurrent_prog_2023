CXX = gcc
CXXFLAGS = -Wall -fsanitize=address -g
LDFLAGS = -fsanitize=address -lpthread
SRC_FILES = $(wildcard *.c) $(filter-out ../assignment1/test.c, $(wildcard ../monitor_lib/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = hw3_4

all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
