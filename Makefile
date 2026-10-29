CXX = g++
# Rutas de los headers
CXXFLAGS = -Wall -g -std=c++17 -I FuncionesAdministrador/Headers -I FuncionesUsuario/Headers -pthread

# Agregamos interpalindromo.cpp y funcion.cpp al final de la lista SRC
SRC = main.cpp \
      FuncionesAdministrador/Config.cpp \
      FuncionesAdministrador/CrearUsuario.cpp \
      FuncionesAdministrador/ListarUsuario.cpp \
      FuncionesAdministrador/EliminarUsuario.cpp \
      FuncionesAdministrador/CargarUsuarios.cpp \
      FuncionesInterfaz/ComprobarUsuario.cpp \
      funcionesGenerales.cpp \
      FuncionesAdministrador/CargarPerfiles.cpp \
      FuncionesAdministrador/crearPerfil.cpp \
      FuncionesInterfaz/ProfileManager.cpp \
      FuncionesInterfaz/UserManager.cpp \
      FuncionesAdministrador/ListarPerfiles.cpp \
      FuncionesAdministrador/EliminarPerfiles.cpp \
      FuncionesInterfaz/Mediario.cpp \
      FuncionesEntrega2/ConteoTexto.cpp \
      FuncionesEntrega2/ConteoArchivo.cpp \
      FuncionesEntrega2/ConteoUtils.cpp \
      FuncionesEntrega2/interpalindromo.cpp \
      FuncionesEntrega2/funcion.cpp
      

# --- Entrega 3: indice invertido (cada modulo = un item del enunciado) ---
# Mis modulos
SRC += FuncionesEntrega3/IndiceInvertido.cpp
SRC += FuncionesEntrega3/Semaforos.cpp
SRC += FuncionesEntrega3/ConteoPalabras.cpp
SRC += FuncionesEntrega3/IdentificacionLibros.cpp
SRC += FuncionesEntrega3/ConstruccionIndice.cpp
SRC += FuncionesEntrega3/Finalizacion.cpp

# Modulos del companero
SRC += FuncionesEntrega3/CarpetaLibros.cpp
SRC += FuncionesEntrega3/ColaTrabajo.cpp
SRC += FuncionesEntrega3/ThreadsTrabajadores.cpp
SRC += FuncionesEntrega3/NumeroThreads.cpp

# Modulos del companero (quitar el # cuando entregue sus archivos)
# SRC += FuncionesEntrega3/CarpetaLibros.cpp
# SRC += FuncionesEntrega3/ColaTrabajo.cpp
# SRC += FuncionesEntrega3/ThreadsTrabajadores.cpp
# SRC += FuncionesEntrega3/NumeroThreads.cpp



OBJ = $(SRC:.cpp=.o)

all: programa multi servidor_juego cliente_juego

programa: $(OBJ)
	$(CXX) $(CXXFLAGS) -o programa $(OBJ)

# Aqui actualizamos la ruta a la nueva carpeta
multi: ProgramaMultiMatriz/MultiMatrices.cpp
	$(CXX) $(CXXFLAGS) ProgramaMultiMatriz/MultiMatrices.cpp -o multi

# Entrega 3: juego multiplayer
servidor_juego: Juego/ServidorJuego.cpp Juego/Protocolo.h FuncionesAdministrador/Config.cpp
	$(CXX) $(CXXFLAGS) Juego/ServidorJuego.cpp FuncionesAdministrador/Config.cpp -o servidor_juego

cliente_juego: Juego/ClienteJuego.cpp Juego/Protocolo.h FuncionesAdministrador/Config.cpp
	$(CXX) $(CXXFLAGS) Juego/ClienteJuego.cpp FuncionesAdministrador/Config.cpp -o cliente_juego

clean:
	rm -f $(OBJ) programa multi servidor_juego cliente_juego
