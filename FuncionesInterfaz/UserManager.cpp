#include "UserManager.h"


using namespace std;

void UserManager(vector<Usuario>& listaUsuarios, vector<Perfil>& listaPerfiles){
  int opcion;
  
  while (true){
    cout << "0) Salir " << endl;
    cout << "1) Crear Usuario" << endl;
    cout << "2) Listar Usuarios " << endl;
    cout << "3) Eliminar Usuario" << endl;
    
    cout << "Opcion: ";
    cin >>opcion;
    cout << endl;
    
    switch(opcion){
      case 0:
        return;
        break;
      case 1:
        crearUsuario(listaUsuarios, listaPerfiles);
        break;
      case 2:
        listarUsuario(listaUsuarios);
        break;
      case 3:
        eliminarUsuario(listaUsuarios);
        break;
  }
  }
}

