#include "Config.h"
#include <fstream>
#include <iostream>
#include <cstdlib> 

std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

std::string get_env_var( std::string const & key ) {                                 
    char * val;                                                                    
    val = getenv( key.c_str() );                                                   
    std::string retval = "";                                                       
    if (val != NULL) {                                                             
        retval = val;                                                              
    }                                                                              
    return retval;                                                                 
}

std::string obtenerVariableEnv(const std::string& nombreVariable) {
    static bool inicializado = false;

    if (!inicializado) {
        std::ifstream archivo(".env");
        if (archivo.is_open()) {
            std::string linea;
            while (std::getline(archivo, linea)) {
                if (linea.empty() || linea[0] == '#') continue;

                size_t posicion = linea.find('=');
                if (posicion != std::string::npos) {
                    std::string nombre = trim(linea.substr(0, posicion));
                    std::string valor = trim(linea.substr(posicion + 1));
                    
                    setenv(nombre.c_str(), valor.c_str(), 1); 
                }
            }
            archivo.close();
        } else {
            std::cerr << "Advertencia: No se pudo abrir el archivo .env" << std::endl;
        }
        inicializado = true;
    }

    return get_env_var(nombreVariable);
}