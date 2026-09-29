#include "../Headers/ComprobarUsuario.h"
#include "../../funcionesGenerales.h"

using namespace std;


int comprobarUsuario(string user, string password, vector<Usuario>& listaUsuarios){
    user = convertiraMinuscula(user);
    for(size_t i = 0; i < listaUsuarios.size(); i++){
            string usuarioMinuscula = convertiraMinuscula(listaUsuarios[i].nombre);
            if (usuarioMinuscula == user && listaUsuarios[i].password == password) return 1;
            if (usuarioMinuscula == user && listaUsuarios[i].password != password) return 2;
    }
    return 0;      
}

int buscarUsuario(string user, string password, vector<Usuario>& listaUsuarios){
    user = convertiraMinuscula(user);
    for(size_t i = 0; i < listaUsuarios.size(); i++){
            string usuarioMinuscula = convertiraMinuscula(listaUsuarios[i].nombre);
            if (usuarioMinuscula == user && listaUsuarios[i].password == password) return i;
    }
    return 0;      
}
