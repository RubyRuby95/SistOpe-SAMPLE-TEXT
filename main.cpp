#include <iostream>
#include <vector>
#include "FuncionesAdministrador/Headers/CrearUsuario.h"
#include "FuncionesAdministrador/Headers/ListarUsuario.h"
#include "FuncionesAdministrador/Headers/EliminarUsuario.h"
#include "FuncionesAdministrador/Headers/CargarUsuarios.h"
#include "FuncionesInterfaz/Headers/ComprobarUsuario.h"
#include "FuncionesInterfaz/Headers/UserManager.h"
#include "FuncionesAdministrador/Headers/CargarPerfiles.h"
#include "FuncionesInterfaz/Headers/ProfileManager.h"
using namespace std;

int main() {
    vector<Usuario> listaUsuarios;
    vector<Perfil> listaPerfiles;
    CargarUsuarios(listaUsuarios);
    CargarPerfiles(listaPerfiles);
    int idUsuario;
    int opcion;
    
    
    /*string user = ""
    ;
    string password = "";
    
    int salida;
    cout << "Entrando al menu de ingreso, desea continuar (0 para no): ";
    cin >> salida;
    cout << endl;
      
    while (true){
      
      if (salida == 0) return 0;
      cout << "Ingrese usuario y contraseña (usuario: admin, contraseña: 1234):" << endl;
      cout << "user: ";
      cin >> user;
      cout << "password: ";
      cin >> password;
      
      int estado = comprobarUsuario(user, password, listaUsuarios);
      
      if (estado == 1) break;
      if (estado == 2){
        cout << "Contrasena incorrecta... " << endl;
      }
      else{
        cout << "No se ha encontrado el usuario, desea reintentar (0 para no): ";
        cin >> salida;
        cout << endl;
        if (salida == 0) return 0;
      }
      
    }
    
    idUsuario = buscarUsuario(user, password, listaUsuarios);  
    */
    
    while (true) {
        
        cout << "\n===== MENÚ =====\n";
        
        cout << "0. Salir" << endl;        
        cout << "1. Gestionar Usuarios" << endl;
        cout << "2. Gestionar Perfiles" << endl;
        cout << "Opcion: ";
        cin >> opcion;
        cout << endl;
        
        switch (opcion) {
            case 0:
                cout << "Saliendo del programa..." << endl;
                return 0;
            case 1:
                UserManager(listaUsuarios, listaPerfiles);
                break;
            case 2:
                ProfileManager(listaUsuarios, listaPerfiles);
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while (opcion != 0);
    return 0;
}
