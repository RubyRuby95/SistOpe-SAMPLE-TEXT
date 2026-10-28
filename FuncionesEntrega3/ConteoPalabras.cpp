#include "ConteoPalabras.h"

#include <fstream>
#include <vector>

using namespace std;

namespace {   // funciones y clases privadas de este archivo

// Si "c" (codigo Unicode) es una letra devuelve su minuscula; si no es letra devuelve 0.
unsigned int letraMinuscula(unsigned int c){
  if (c >= 'a' && c <= 'z') return c;                          // minuscula ASCII: queda igual
  if (c >= 'A' && c <= 'Z') return c | 0x20;                   // mayuscula ASCII: se pasa a minuscula
  if (c >= 0xC0 && c <= 0xDE && c != 0xD7) return c + 0x20;    // mayuscula con tilde (Latin-1) -> minuscula
  if (c >= 0xDF && c <= 0xFF && c != 0xF7) return c;           // minuscula con tilde (Latin-1) (0xD7 y 0xF7 no son letras)
  if (c >= 0x100 && c <= 0x17F) return c | 1;                  // letras europeas extendidas (aproximado)
  return 0;                                                    // no es letra
}

// Agrega el caracter "c" al final del texto "destino", codificado en UTF-8.
void agregarUtf8(string& destino, unsigned int c){
  if (c < 0x80) destino += static_cast<char>(c);               // 1 byte (ASCII)
  else if (c < 0x800){                                         // 2 bytes
    destino += static_cast<char>(0xC0 | (c >> 6));
    destino += static_cast<char>(0x80 | (c & 0x3F));
  } else {                                                     // 3 bytes
    destino += static_cast<char>(0xE0 | (c >> 12));
    destino += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
    destino += static_cast<char>(0x80 | (c & 0x3F));
  }
}

// Recibe el archivo byte a byte, arma los caracteres y va contando palabras.
class Tokenizador {
public:
  explicit Tokenizador(ConteoLibro& conteo) : conteo(conteo) {}

  void procesarByte(unsigned char b){
    if (esperados > 0){                          // estamos a mitad de un caracter UTF-8 de varios bytes
      if ((b & 0xC0) == 0x80){                   // byte de continuacion valido (10xxxxxx)
        secuencia[largo++] = b;
        if (largo == esperados){                 // ya llegaron todos los bytes del caracter
          procesarCaracter(decodificar());
          largo = 0;
          esperados = 0;
        }
        return;
      }
      vaciarComoLatin1();                        // no era UTF-8 valido: se tratan los bytes como Latin-1
    }
    if (b < 0x80){ procesarCaracter(b); return; }   // caracter ASCII normal

    int n = 0;                                   // cuantos bytes tiene este caracter UTF-8
    if (b >= 0xC2 && b <= 0xDF) n = 2;
    else if (b >= 0xE0 && b <= 0xEF) n = 3;
    else if (b >= 0xF0 && b <= 0xF4) n = 4;
    if (n == 0){ procesarCaracter(b); return; }  // byte suelto: se interpreta como Latin-1

    secuencia[0] = b;                            // empieza un caracter de varios bytes
    largo = 1;
    esperados = n;
  }

  // Se llama al terminar el archivo: cierra lo que haya quedado pendiente.
  void finalizar(){
    if (esperados > 0) vaciarComoLatin1();
    cerrarPalabra();
  }

private:
  ConteoLibro& conteo;            // donde se acumulan los resultados
  string palabra;                 // palabra que se esta armando
  unsigned char secuencia[4] = {};// bytes del caracter UTF-8 en construccion
  int largo = 0;                  // bytes recibidos del caracter actual
  int esperados = 0;              // bytes que tiene el caracter actual

  // Convierte los bytes recibidos en el codigo Unicode del caracter.
  unsigned int decodificar() const {
    if (esperados == 2) return ((secuencia[0] & 0x1F) << 6) | (secuencia[1] & 0x3F);
    if (esperados == 3) return ((secuencia[0] & 0x0F) << 12) | ((secuencia[1] & 0x3F) << 6) | (secuencia[2] & 0x3F);
    return 0xFFFD;                // 4 bytes (emojis, etc.): nunca es letra
  }

  // Trata cada byte pendiente como un caracter Latin-1 independiente.
  void vaciarComoLatin1(){
    for (int i = 0; i < largo; i++) procesarCaracter(secuencia[i]);
    largo = 0;
    esperados = 0;
  }

  // Si es letra se agrega a la palabra actual; si no, la palabra termino.
  void procesarCaracter(unsigned int c){
    unsigned int letra = letraMinuscula(c);
    if (letra != 0) agregarUtf8(palabra, letra);
    else cerrarPalabra();
  }

  // Si hay una palabra armada, suma 1 a su contador y la reinicia.
  void cerrarPalabra(){
    if (!palabra.empty()){
      conteo[palabra]++;
      palabra.clear();
    }
  }
};

}  // namespace

bool contarPalabras(const string& ruta, ConteoLibro& conteo, string& error){
  ifstream archivo(ruta, ios::binary);           // abre el libro en modo binario (lee bytes tal cual)
  if (!archivo.is_open()){
    error = "No se pudo abrir \"" + ruta + "\" (revise los permisos de lectura).";
    return false;
  }
  Tokenizador tok(conteo);
  vector<char> bloque(64 * 1024);                // buffer de 64 KB: no se carga el libro entero en memoria
  while (archivo.read(bloque.data(), bloque.size()) || archivo.gcount() > 0){
    streamsize leidos = archivo.gcount();        // bytes realmente leidos en este bloque
    for (streamsize i = 0; i < leidos; i++){
      tok.procesarByte(static_cast<unsigned char>(bloque[i]));
    }
  }
  if (archivo.bad()){                            // error de lectura (no confundir con fin de archivo)
    error = "Error de lectura en \"" + ruta + "\".";
    return false;
  }
  tok.finalizar();                               // cierra la ultima palabra
  return true;
}
