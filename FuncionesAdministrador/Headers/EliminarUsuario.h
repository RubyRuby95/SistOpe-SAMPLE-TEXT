#ifndef ELIMINAR_USUARIO_H
#define ELIMINAR_USUARIO_H

#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include "../../Usuario.h"

void eliminarUsuario(std::vector<Usuario>& listaUsuarios);
int verificarNumero();
int buscarUsuario(int Id, std::vector<Usuario>& listaUsuarios);


#endif
