#ifndef USUARIO_H
#define USUARIO_H

#include <string>

struct Usuario {
    int id;
    std::string nombre;
    std::string username;
    std::string password;
    std::string perfil;
};

#endif