#include "ConteoTexto.h"
#include "../funcionesGenerales.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

namespace {

enum TipoCaracter { ESPACIO, VOCAL, CONSONANTE, DIGITO, ESPECIAL, IGNORAR };

// Clasifica un caracter segun su codigo Unicode. Cubre ASCII y los acentos
// de Latin-1 (a, e, i, o, u con tilde, dieresis, n con tilde, etc).
TipoCaracter clasificar(unsigned int c){
  if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f' || c == 0xA0){
    return ESPACIO;
  }
  //Marca BOM que algunos editores dejan al inicio del archivo, no es texto
  if (c == 0xFEFF) return IGNORAR;

  if (c >= '0' && c <= '9') return DIGITO;

  if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
    unsigned int minuscula = c | 0x20;
    if (minuscula == 'a' || minuscula == 'e' || minuscula == 'i' || minuscula == 'o' || minuscula == 'u'){
      return VOCAL;
    }
    return CONSONANTE;
  }

  //Letras con acento de Latin-1 (0xD7 y 0xF7 son los signos de multiplicar y dividir)
  if (c >= 0xC0 && c <= 0xFF && c != 0xD7 && c != 0xF7){
    bool esConsonante = c == 0xC7 || c == 0xE7   // C con cedilla
                     || c == 0xD1 || c == 0xF1   // N con tilde
                     || c == 0xD0 || c == 0xF0   // eth
                     || c == 0xDE || c == 0xFE   // thorn
                     || c == 0xDD || c == 0xFD   // y con acento
                     || c == 0xDF || c == 0xFF;  // eszett, y con dieresis
    return esConsonante ? CONSONANTE : VOCAL;
  }

  return ESPECIAL;
}

// Recibe el archivo byte a byte, arma los caracteres UTF-8 y lleva la cuenta.
// Si una secuencia no es UTF-8 valido, sus bytes se interpretan como Latin-1,
// que es la otra codificacion comun en libros en espanol.
class Contador {
public:
  explicit Contador(ResultadoConteo& resultado) : resultado(resultado) {}

  void procesarByte(unsigned char b){
    if (esperados > 0){
      if ((b & 0xC0) == 0x80){
        secuencia[largo++] = b;
        if (largo == esperados){
          procesarCaracter(decodificarSecuencia());
          largo = 0;
          esperados = 0;
        }
        return;
      }
      vaciarComoLatin1();
    }

    if (b < 0x80){
      procesarCaracter(b);
      return;
    }

    int bytesSecuencia = 0;
    if (b >= 0xC2 && b <= 0xDF) bytesSecuencia = 2;
    else if (b >= 0xE0 && b <= 0xEF) bytesSecuencia = 3;
    else if (b >= 0xF0 && b <= 0xF4) bytesSecuencia = 4;

    if (bytesSecuencia == 0){
      procesarCaracter(b);
      return;
    }

    secuencia[0] = b;
    largo = 1;
    esperados = bytesSecuencia;
  }

  //Se llama al terminar el archivo, por si quedo algo pendiente
  void finalizar(){
    if (esperados > 0) vaciarComoLatin1();
    cerrarPalabra();
  }

private:
  ResultadoConteo& resultado;
  unsigned char secuencia[4] = {};
  int largo = 0;
  int esperados = 0;
  bool palabraConLetra = false;

  unsigned int decodificarSecuencia() const {
    if (esperados == 2){
      return ((secuencia[0] & 0x1F) << 6) | (secuencia[1] & 0x3F);
    }
    if (esperados == 3){
      return ((secuencia[0] & 0x0F) << 12) | ((secuencia[1] & 0x3F) << 6) | (secuencia[2] & 0x3F);
    }
    return ((secuencia[0] & 0x07) << 18) | ((secuencia[1] & 0x3F) << 12)
         | ((secuencia[2] & 0x3F) << 6) | (secuencia[3] & 0x3F);
  }

  void vaciarComoLatin1(){
    for (int i = 0; i < largo; i++){
      procesarCaracter(secuencia[i]);
    }
    largo = 0;
    esperados = 0;
  }

  // Una palabra es un grupo de caracteres separado por espacios que tiene al
  // menos una letra o numero. Asi "hola," cuenta como 1 y un "--" suelto no cuenta.
  void cerrarPalabra(){
    if (palabraConLetra) resultado.palabras++;
    palabraConLetra = false;
  }

  void procesarCaracter(unsigned int c){
    switch (clasificar(c)){
      case ESPACIO:
        cerrarPalabra();
        break;
      case VOCAL:
        resultado.vocales++;
        palabraConLetra = true;
        break;
      case CONSONANTE:
        resultado.consonantes++;
        palabraConLetra = true;
        break;
      case DIGITO:
        palabraConLetra = true;
        break;
      case ESPECIAL:
        resultado.especiales++;
        break;
      case IGNORAR:
        break;
    }
  }
};

}

bool contarArchivo(const string& ruta, ResultadoConteo& resultado, string& error){
  resultado = ResultadoConteo();
  error.clear();

  if (ruta.empty()){
    error = "No se indico ningun archivo. Debe ejecutar el programa con -f \"ruta del archivo\".";
    return false;
  }

  error_code ec;
  if (!filesystem::exists(ruta, ec)){
    error = "El archivo \"" + ruta + "\" no existe.";
    return false;
  }
  if (!filesystem::is_regular_file(ruta, ec)){
    error = "\"" + ruta + "\" no es un archivo (puede ser una carpeta).";
    return false;
  }

  ifstream archivo(ruta, ios::binary);
  if (!archivo.is_open()){
    error = "No se pudo abrir \"" + ruta + "\". Revise que tenga permisos de lectura.";
    return false;
  }

  //Se lee por bloques para no cargar libros grandes completos en memoria
  Contador contador(resultado);
  vector<char> bloque(64 * 1024);
  while (archivo.read(bloque.data(), bloque.size()) || archivo.gcount() > 0){
    streamsize leidos = archivo.gcount();
    for (streamsize i = 0; i < leidos; i++){
      contador.procesarByte(static_cast<unsigned char>(bloque[i]));
    }
  }

  if (archivo.bad()){
    resultado = ResultadoConteo();
    error = "Ocurrio un error mientras se leia \"" + ruta + "\".";
    return false;
  }

  contador.finalizar();
  return true;
}

void ConteoSobreTexto(const string& rutaArchivo, const string& username, const string& perfil){
  cout << "\n===== CONTEO SOBRE TEXTO =====\n";
  cout << "Usuario: " << (username.empty() ? "(no indicado)" : username)
       << " | Perfil: " << (perfil.empty() ? "(no indicado)" : perfil) << endl;
  cout << "Archivo: " << (rutaArchivo.empty() ? "(no indicado)" : rutaArchivo) << endl << endl;

  cout << "Contando..." << endl;
  ResultadoConteo resultado;
  string error;

  if (contarArchivo(rutaArchivo, resultado, error)){
    cout << "\n----- Resumen de conteo -----" << endl;
    cout << "Vocales:               " << resultado.vocales << endl;
    cout << "Consonantes:           " << resultado.consonantes << endl;
    cout << "Caracteres especiales: " << resultado.especiales << endl;
    cout << "Palabras:              " << resultado.palabras << endl;
  }
  else {
    cout << "No se pudo realizar el conteo: " << error << endl;
  }

  cout << "\n0) Volver" << endl;
  cout << "Opcion: ";
  while (verificarNumero() != 0){
    cout << "Opcion invalida, ingrese 0 para volver." << endl;
    cout << "Opcion: ";
  }
}
