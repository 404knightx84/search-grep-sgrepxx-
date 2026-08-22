CXX := g++
CXXFLAGS := -Wall -Wextra -std=c++17 -Iinclude -g
SRC := src/main.cpp src/Config.cpp src/Walker.cpp src/Matcher.cpp
OBJ := $(SRC:.cpp=.o)
BIN := sgrepxx

.PHONY: all clean test

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) -lstdc++fs

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(BIN)

test: all
	./$(BIN) -n "class" src
