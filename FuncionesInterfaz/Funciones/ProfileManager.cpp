#include "../Headers/ProfileManager.h"
using namespace std;


void ProfileManager(vector<Usuario>& listaUsuarios, vector<Perfil>& listaPerfiles){
  int opcion;
  //Perfil perfilActual;
  
  /*for(size_t i = 0; i<perfilActual.funciones.size(); i++){
    if(perfilActual.funciones[i] == 0) cout << "0) Salir " << endl;
    if(perfilActual.funciones[i] == 1) cout << "1) Crear Usuario" << endl;
    if(perfilActual.funciones[i] == 2) cout << "2) Listar Usuarios " << endl;
    if(perfilActual.funciones[i] == 3) cout << "3) Eliminar Usuario" << endl;
  }
  */
  
  
  while (true){
    cout << "0) Salir " << endl;
    cout << "1) Crear Perfil" << endl;
    cout << "2) Listar Perfiles " << endl;
    cout << "3) Eliminar Perfiles" << endl;
    
    cout << "Opcion: ";
    cin >>opcion;
    cout << endl;
    
    switch(opcion){
      case 0:
        return;
        break;
      case 1:
        crearPerfil(listaPerfiles);
        break;
      case 2:
        ListarPerfiles(listaPerfiles);
        break;
      case 3:
        eliminarPerfiles(listaPerfiles);
        break;
  }
  }

}
