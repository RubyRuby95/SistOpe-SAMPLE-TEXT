#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>
using namespace std;

vector<vector<double>> leerMatriz(const string& ruta, char separador) {
    ifstream archivo(ruta);
    if (!archivo.is_open()) {
        throw runtime_error("No se pudo abrir el archivo: " + ruta);
    }

    vector<vector<double>> matriz;
    string linea;
    int columnasEsperadas = -1;

    while (getline(archivo, linea)) {
        if (linea.empty()) continue; 

        vector<double> fila;
        stringstream ss(linea);
        string valorStr;

        while (getline(ss, valorStr, separador)) {
            try {
                double valor = stod(valorStr);
                fila.push_back(valor);
            } catch (...) {
                throw runtime_error("Contenido invalido en archivo " + ruta);
            }
        }

        if (columnasEsperadas == -1) {
            columnasEsperadas = fila.size();
        } else if ((int)fila.size() != columnasEsperadas) {
            throw runtime_error("Formato invalido: Filas no uniformes en " + ruta);
        }

        matriz.push_back(fila);
    }
    return matriz;
}

void imprimirMatriz(const vector<vector<double>>& matriz) {
    for (const auto& fila : matriz) {
        for (double val : fila) {
            cout << val << "\t";
        }
        cout << endl;
    }
}

int main(int argc, char* argv[]) {
    // Validar cantidad de argumentos
    if (argc < 4) {
        cerr << "Uso: " << argv[0] << " <ruta_A> <ruta_B> <separador>" << endl;
        return 1;
    }
    string rutaA = argv[1];
    string rutaB = argv[2];
    char separador = argv[3][0];

    cout << "\n--- RESULTADO DE MULTIPLICACION ---" << endl;
    cout << "-----------------------------------" << endl;

    try {
        vector<vector<double>> matrizA = leerMatriz(rutaA, separador);
        vector<vector<double>> matrizB = leerMatriz(rutaB, separador);

        if (matrizA.empty() || matrizB.empty()) {
             throw runtime_error("Una o ambas matrices estan vacias.");
        }

        int filasA = matrizA.size();
        int colsA = matrizA[0].size();
        int filasB = matrizB.size();
        int colsB = matrizB[0].size();

        if (colsA != filasB) {
            throw runtime_error("Imposible multiplicar: Columnas de A (" + to_string(colsA) + 
                                ") no coinciden con Filas de B (" + to_string(filasB) + ").");
        }

        vector<vector<double>> matrizC(filasA, vector<double>(colsB, 0.0));

        for (int i = 0; i < filasA; ++i) {
            for (int j = 0; j < colsB; ++j) {
                for (int k = 0; k < colsA; ++k) {
                    matrizC[i][j] += matrizA[i][k] * matrizB[k][j];
                }
            }
        }

        cout << "Matriz Resultado (A x B):" << endl;
        imprimirMatriz(matrizC);

    } catch (const exception& e) {
        cerr << "[ERROR] " << e.what() << endl;
        return 1;
    }

    return 0;
}
