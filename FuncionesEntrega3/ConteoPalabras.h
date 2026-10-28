// ============================================================================
// ITEM 1.8 - CONTEO DE PALABRAS
// Cuenta cuantas veces aparece cada palabra en UN libro. Lo llama cada thread
// (item 1.5, companero) con su libro, sobre su propio mapa (sin semaforos).
//
// Palabra = secuencia de letras (con tildes y n con tilde), pasada a minuscula.
// Numeros y signos de puntuacion separan palabras. Acepta UTF-8 y Latin-1.
// ============================================================================
#ifndef CONTEO_PALABRAS_H
#define CONTEO_PALABRAS_H

#include <string>
#include <unordered_map>

// Resultado de un libro: palabra -> cantidad de veces que aparece en ese libro
typedef std::unordered_map<std::string, long long> ConteoLibro;

// Lee el libro "ruta" y llena "conteo". Devuelve false y "error" si no se puede leer.
bool contarPalabras(const std::string& ruta, ConteoLibro& conteo, std::string& error);

#endif
