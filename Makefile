CXX = g++
CXXFLAGS = -Wall -g

# Regla principal (el objetivo final)
all: programa

# Cómo construir el ejecutable 'programa' a partir de 'main.cpp'
programa: main.cpp
	$(CXX) $(CXXFLAGS) -o programa main.cpp

# Regla para limpiar los archivos generados
clean:
	rm -f programa