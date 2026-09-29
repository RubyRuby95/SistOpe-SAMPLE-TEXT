#include "../Headers/Config.h"


std::string obtenerVariableEnv(const std::string& nombreVariable) {
    // La palabra 'static' hace que este mapa y este booleano 
    // se mantengan vivos en memoria durante todo el programa.
    static std::map<std::string, std::string> entorno;
    static bool inicializado = false;

    // Solo entramos a leer el archivo de texto la PRIMERA vez que se llama a la función
    if (!inicializado) {
        std::ifstream archivo(".env");
        if (archivo.is_open()) {
            std::string linea;
            while (std::getline(archivo, linea)) {
                if (!linea.empty() && linea.back() == '\r') {
                    linea.pop_back();
                }
                size_t posicion = linea.find('=');
                if (posicion != std::string::npos) {
                    std::string nombre = linea.substr(0, posicion);
                    std::string valor = linea.substr(posicion + 1);
                    entorno[nombre] = valor; // Guardamos en el mapa estático
                }
            }
            archivo.close();
        }
        inicializado = true; // Marcamos que ya se leyó el archivo
    }

    // Buscamos la variable solicitada directamente en el mapa (muy rápido)
    auto it = entorno.find(nombreVariable);
    if (it != entorno.end()) {
        return it->second;
    }

    return "";
}
