CXX = gcc
CXXFLAGS = -Wall -fsanitize=address -g
LDFLAGS = -fsanitize=address -lpthread
SRC_FILES = $(filter_out ../../hw2/assignment1/test.c) $(wildcard *.c) $(filter-out ../assignment1/test.c, $(wildcard ../../hw2/assignment1/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = hw2_2

all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
