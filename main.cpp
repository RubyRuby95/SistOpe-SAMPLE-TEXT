#include <iostream>
#include <cstdlib> 
#include <string>
#include <cstring>
#include "funcionesGenerales.h"
#include "FuncionesInterfaz/Headers/Mediario.h"
#include  "FuncionesEntrega2/ConteoTexto.h"
#include  "FuncionesEntrega2/ConteoArchivo.h"
#include "FuncionesEntrega2/interpalindromo.h"
#include "FuncionesEntrega2/funcion.h"

using namespace std;

int main(int argc, char* argv[]) {
    string usuario = "";
    string password = "";
    string archivoFile = "";
    string perfil = "ADMIN";

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "-u") == 0 && i + 1 < argc) {
            usuario = argv[i + 1];
            i++;
        } 
        else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            password = argv[i + 1];
            i++;
        } 
        else if (strcmp(argv[i], "-f") == 0 && i + 1 < argc) {
            archivoFile = argv[i + 1];
            i++;
        }
    }

    if (usuario.empty() || password.empty() || archivoFile.empty()) {
        cerr << "Error: Faltan argumentos obligatorios." << endl;
        cerr << "Uso: " << argv[0] << " -u <usuario> -p <password> -f <archivo>" << endl;
        return 1;
    }

    while (true) {
        cout << "\n--- MENU DE OPCIONES ---" << endl;
        cout << "0. Salir" << endl;
        cout << "1. Gestionar Usuarios/Perfiles" << endl;
        cout << "2. Multiplicar Matrices" << endl;
        cout << "3. Juego" << endl;
        cout << "4. EsPalindromo" << endl;
        cout << "5. Funcion" << endl;
        cout << "6. Conteo sobre texto" << endl;
        cout << "7. Conteo sobre archivo" << endl;
        
        cout << "Ingrese opcion: ";
        int opcion = verificarNumero();
        
        switch (opcion) {
            case 0:
                cout << "Saliendo del programa..." << endl;
                return 0;
                
            case 1:
                mediario();
                break;
                
            case 2: {
                string rutaA, rutaB, separador;
                
                cout << "Ingrese la ruta absoluta del archivo A con el formato /home/archivo.txt : ";
                cin >> rutaA;
                cout << "Ingrese la ruta absoluta del archivo B: ";
                cin >> rutaB;
                cout << "Ingrese el caracter separador: ";
                cin >> separador;
                
                string comando = "./multi '" + rutaA + "' '" + rutaB + "' '" + separador + "'";
                
                cout << "\nEjecutando: " << comando << endl;
                int resultado = std::system(comando.c_str());
                
                if (resultado != 0) {
                    cout << "\n[!] Fallo la ejecucion del multiplicador." << endl;
                }
                break;
            }
            case 3:
                cout << "En construccion..." <<endl;
                break;
                
            case 4:
                interpalindromo();
                break;
                
            case 5:
                mainfuncion();
                break;
                
            case 6:
                ConteoSobreTexto(archivoFile, usuario, perfil);
                break;
                
            case 7:
               ConteoArchivo();
               break;
              
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }
    }
    return 0;
}
