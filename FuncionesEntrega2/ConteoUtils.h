#ifndef CONTEO_UTILS_H
#define CONTEO_UTILS_H

#include <iostream>

struct ResultadoConteo {
    int vocales = 0;
    int consonantes = 0;
    int especiales = 0;
    int palabras = 0;
};

// Recorre el stream (puede ser un ifstream ya abierto) y cuenta todo de una pasada.
ResultadoConteo realizarConteo(std::istream& entrada);

// Imprime el resumen con el mismo formato para las opciones 6 y 7.
void mostrarConteo(const ResultadoConteo& resultado);

#endif
