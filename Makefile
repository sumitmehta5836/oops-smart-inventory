CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -MMD -MP
LDFLAGS  = -lsqlite3

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)

app: $(OBJ)
	$(CXX) $(OBJ) -o app $(LDFLAGS)

src/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o src/*.d app

-include $(OBJ:.o=.d)