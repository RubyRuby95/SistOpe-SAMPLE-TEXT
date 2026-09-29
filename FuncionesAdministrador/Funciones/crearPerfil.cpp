#include "../Headers/crearPerfil.h"


using namespace std;

void crearPerfil(vector<Perfil>& listaPerfiles){
    string nombrePerfil;
    cout << "Entrando a la seccion para crear perfil..." << endl;
    cout << "Ingrese nombre del perfil: ";
    cin >> nombrePerfil;
    cout << endl;
    
    Perfil nuevoPerfil;
    
    nuevoPerfil.perfil = cambiaraMayuscula(nombrePerfil);
    nuevoPerfil.funciones.push_back(0);
    int permiso;
    while (true){
        cout << "Ingrese numero de permiso (0 para terminar): ";
        cin >> permiso;

        // 1. Interceptar si el usuario ingresó algo que no es un entero (letras, símbolos)
        if (cin.fail()) {
            cin.clear(); // Desbloquea cin
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Elimina la basura del buffer
            cout << "...Error, debe ingresar un numero entero valido..." << endl;
            continue;
        }

        // 2. Lógica normal
        if (permiso == 0) break;

        if (permiso < 0) {
            cout << "...Error, el numero debe ser positivo... " << endl;
            continue;
        } else {
            nuevoPerfil.funciones.push_back(permiso);
            cout << "Permiso guardado con exito!!!" << endl;
        }
    }
    
    int opcion;

    cout << "\n1. Guardar perfil\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opcion: ";

    cin >> opcion;
    
    if (opcion == 1) {

        listaPerfiles.push_back(nuevoPerfil);

        string nombreArchivo = obtenerVariableEnv("PERFIL_FILE");

        if (nombreArchivo.empty()) {
            cout << "Error: no se encontro la variable PERFIL_FILE.\n";
            return;
        }

        ofstream archivo(nombreArchivo, std::ios::app);

        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de perfiles.\n";
            return;
        }

        archivo << nuevoPerfil.perfil << ";";

        for (size_t i = 0; i < nuevoPerfil.funciones.size(); i++) {
            archivo << nuevoPerfil.funciones[i];
            
            if (i < nuevoPerfil.funciones.size() - 1) {
                archivo << ","; 
            }
        }
        
        archivo << "\n";
        
        archivo.close();

    } else {

        cout << "\nOperacion cancelada.\n";
    }
}
