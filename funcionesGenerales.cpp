#include "funcionesGenerales.h"
using namespace std;

string cambiaraMayuscula(string texto){
  for(char& letra : texto){
    letra = toupper(letra);
  }
  return texto;
}

string convertiraMinuscula(string texto){
  for (char& letra : texto) {
        letra = tolower(letra);
  }
  return texto;
}

int verificarNumero(){
  int Id;
  while(true){
    if(cin >> Id){
      if(cin.peek() == '\n' || cin.peek() ==EOF){
        return Id;
      }
    }
    cout << "Entrada invalida, Intentar de nuevo..." << endl;
    
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }
}
