CXX = gcc
CXXFLAGS = -Wall -g -DDEBUG
LDFLAGS = -lpthread 
SRC_FILES = $(filter-out maria.c, $(wildcard *.c) $(wildcard ../monitor_lib/*.c))
OBJ_FILES = $(SRC_FILES:.c=.o)
TARGET = hw3_3

all: $(TARGET)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJ_FILES)
	$(CXX) $^ -o $@ $(LDFLAGS)

clean:
	rm -f $(OBJ_FILES) $(TARGET)

.PHONY: all clean
