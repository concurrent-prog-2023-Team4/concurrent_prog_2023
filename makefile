CXX = gcc
CXXFLAGS = -Wall -fsanitize=address
LINK = -lpthread

SRC = hw1_1.c
HEADER = hw1_1.h
OUT = hw1_1


all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC) $(LINK)

clean:
	rm -f $(OUT)

.PHONY: all clean