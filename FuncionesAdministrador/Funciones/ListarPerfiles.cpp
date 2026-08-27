#include "../Headers/ListarPerfiles.h"
using namespace std;


void ListarPerfiles(vector<Perfil>& listaPerfiles, bool mostrarFunciones) {
    if (listaPerfiles.empty()) {
        CargarPerfiles(listaPerfiles);
    }
    cout << "\n===== LISTA DE PERFILES =====\n";
    cout << "Perfil\tFunciones\n";
    if (listaPerfiles.empty()) {
        cout << "(No hay perfiles registrados)\n";
    } else {
        for (const Perfil& p : listaPerfiles) {
            cout << p.perfil << "\t" << "[";
            for(size_t i = 0; i < p.funciones.size() - 1; i++){
              cout << p.funciones[i] << ",";
            }
            cout << p.funciones[p.funciones.size()-1] << "]" << "\n";
        }
    }
    while (mostrarFunciones) {
        cout << "\n1) para Volver : ";
        int opcion;
        cin >> opcion;
        if (opcion == 1) {
            break;
            cout << endl;
        }
    }
    
}
