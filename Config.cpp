#include "Config.h"

#include <fstream>
#include <string>

std::string obtenerVariableEnv(const std::string& nombreVariable) {

    std::ifstream archivo(".env");

    // Si no se puede abrir el archivo
    if (!archivo.is_open()) {
        return "";
    }

    std::string linea;

    while (std::getline(archivo, linea)) {

        // Eliminar \r de archivos con formato Windows (CRLF)
        if (!linea.empty() && linea.back() == '\r') {
            linea.pop_back();
        }

        // Buscar el signo =
        size_t posicion = linea.find('=');

        // Si la línea no contiene =
        if (posicion == std::string::npos) {
            continue;
        }

        // Separar nombre y valor
        std::string nombre = linea.substr(0, posicion);
        std::string valor = linea.substr(posicion + 1);

        // Comparar con la variable buscada
        if (nombre == nombreVariable) {
            archivo.close();
            return valor;
        }
    }

    archivo.close();

    // Si no encuentra la variable
    return "";
}