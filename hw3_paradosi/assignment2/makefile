CXX = gcc
CXXFLAGS = -Wall -g
LDFLAGS = -lpthread
SRC_FILES = $(wildcard *.c) $(filter-out ../assignment1/test.c, $(wildcard ../monitor_lib/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = hw3_2

all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
