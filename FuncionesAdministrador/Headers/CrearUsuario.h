#ifndef CREAR_USUARIO_H
#define CREAR_USUARIO_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <cstdlib>

#include "../Headers/CrearUsuario.h"
#include "../Headers/Config.h"
#include "../../funcionesGenerales.h"
#include "../../Usuario.h"
#include "../../perfil.h"

void crearUsuario(std::vector<Usuario>& listaUsuarios, std::vector<Perfil>& listaPerfiles);

#endif
