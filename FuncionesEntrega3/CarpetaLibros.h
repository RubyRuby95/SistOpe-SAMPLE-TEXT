#ifndef CARPETA_LIBROS_H
#define CARPETA_LIBROS_H
#include <string>
#include <vector>

// Verifica la carpeta configurada en LIBROS_DIR y lista archivos regulares .txt.
bool listarArchivosTxt(std::vector<std::string>& rutas, std::string& error);
#endif
