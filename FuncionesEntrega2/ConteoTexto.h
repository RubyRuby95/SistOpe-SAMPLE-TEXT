#ifndef CONTEO_TEXTO_H
#define CONTEO_TEXTO_H

#include <string>

struct ResultadoConteo {
    long long vocales = 0;
    long long consonantes = 0;
    long long especiales = 0;
    long long palabras = 0;
};

// Cuenta vocales, consonantes, caracteres especiales y palabras de un archivo.
// Lee el archivo por bloques, asi funciona con libros grandes sin cargarlos
// completos en memoria. Acepta archivos en UTF-8 y en Latin-1 (ISO-8859-1).
// Devuelve false y deja un mensaje en "error" si el archivo no se puede leer.
bool contarArchivo(const std::string& ruta, ResultadoConteo& resultado, std::string& error);

// Opcion 6 del menu principal: muestra el conteo del archivo recibido con -f.
void ConteoSobreTexto(const std::string& rutaArchivo, const std::string& username, const std::string& perfil);

#endif
