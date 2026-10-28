#include "CarpetaLibros.h"
#include "Configuracion.h"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <system_error>

using namespace std;
namespace fs = std::filesystem;

bool listarArchivosTxt(vector<string>& rutas, string& error) {
  rutas.clear();
  string directorio = leerVariable("LIBROS_DIR");
  if (directorio.empty()) {
    error = "Falta LIBROS_DIR en el entorno o en .env.";
    return false;
  }
  error_code ec;
  fs::path carpeta(directorio);
  if (!fs::exists(carpeta, ec) || ec || !fs::is_directory(carpeta, ec) || ec) {
    error = "La carpeta de libros no existe o no es un directorio: \"" + directorio + "\".";
    return false;
  }
  fs::directory_iterator it(carpeta, ec), end;
  if (ec) {
    error = "No se puede leer la carpeta de libros: \"" + directorio + "\".";
    return false;
  }
  for (; it != end; it.increment(ec)) {
    if (ec) {
      error = "Ocurrio un error al recorrer la carpeta \"" + directorio + "\".";
      return false;
    }
    error_code itemEc;
    if (!it->is_regular_file(itemEc) || itemEc) continue;
    string ext = it->path().extension().string();
    transform(ext.begin(), ext.end(), ext.begin(),
              [](unsigned char c){ return static_cast<char>(tolower(c)); });
    if (ext == ".txt") rutas.push_back(it->path().string());
  }
  sort(rutas.begin(), rutas.end());
  if (rutas.empty()) {
    error = "No se encontraron archivos .txt en \"" + directorio + "\".";
    return false;
  }
  return true;
}
