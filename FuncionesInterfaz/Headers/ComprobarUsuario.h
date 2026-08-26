#include <iostream>
#include <string> 
#include <cctype>
#include <vector>
#include "../../Usuario.h"

std::string convertiraMinuscula(std::string texto);
int comprobarUsuario(std::string user, std::string password, std::vector<Usuario>& listaUsuario);
int buscarUsuario(std::string user, std::string password, std::vector<Usuario>& listaUsuario);

