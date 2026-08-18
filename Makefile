CXX = g++
CXXFLAGS = -Wall -g -std=c++17

SRC = main.cpp CrearUsuario.cpp Config.cpp ListarUsuario.cpp EliminarUsuario.cpp
OBJ = $(SRC:.cpp=.o)

all: programa

programa: $(OBJ)
	$(CXX) $(CXXFLAGS) -o programa $(OBJ)

clean:
	rm -f $(OBJ) programa