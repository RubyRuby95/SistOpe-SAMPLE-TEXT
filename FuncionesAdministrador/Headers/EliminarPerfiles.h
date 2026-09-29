#ifndef ELIMINAR_PERFIL_H
#define ELIMINAR_PERFIL_H

#include <vector>
#include <limits>
#include <fstream>
#include <sstream>
#include <string>
#include "../../perfil.h"


void eliminarPerfiles(std::vector<Perfil>& listaPerfiles);
int verificarNumero();
int buscarPerfil(std::string perfil, std::vector<Perfil>& listaPerfiles);


#endif


