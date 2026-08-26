CXX = g++
# Rutas de los headers
CXXFLAGS = -Wall -g -std=c++17 -I FuncionesAdministrador/Headers -I FuncionesUsuario/Headers

# Se actualiza la ruta de Config.cpp
SRC = main.cpp \
      FuncionesAdministrador/Funciones/Config.cpp \
      FuncionesAdministrador/Funciones/CrearUsuario.cpp \
      FuncionesAdministrador/Funciones/ListarUsuario.cpp \
      FuncionesAdministrador/Funciones/EliminarUsuario.cpp\
      FuncionesAdministrador/Funciones/CargarUsuarios.cpp\
      FuncionesInterfaz/Funciones/ComprobarUsuario.cpp\
      funcionesGenerales.cpp\
      FuncionesAdministrador/Funciones/CargarPerfiles.cpp\
      FuncionesAdministrador/Funciones/crearPerfil.cpp\
      FuncionesInterfaz/Funciones/ProfileManager.cpp\
      FuncionesInterfaz/Funciones/UserManager.cpp\
      FuncionesAdministrador/Funciones/ListarPerfiles.cpp\
      FuncionesAdministrador/Funciones/EliminarPerfiles.cpp\
      
OBJ = $(SRC:.cpp=.o)

all: programa

programa: $(OBJ)
	$(CXX) $(CXXFLAGS) -o programa $(OBJ)

clean:
	rm -f $(OBJ) programa
