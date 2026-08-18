#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstdlib>

#include "CrearUsuario.h"
#include "Config.h"

using namespace std;


bool emailValido(const string& email) {
    regex formato(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    return regex_match(email, formato);
}


void crearUsuario(vector<Usuario>& listaUsuarios) {
    cout << "ejecutando la función crearUsuario..." << endl;

    Usuario nuevoUsuario;

    cout << "\n===== CREAR USUARIO =====\n";

    // ID
    cout << "Ingrese ID: ";
    cin >> nuevoUsuario.id;

    cin.ignore();

    // Nombre
    cout << "Ingrese nombre: ";
    getline(cin, nuevoUsuario.nombre);

    // Username / Email
    do {
        cout << "Ingrese email: ";
        getline(cin, nuevoUsuario.username);

        if (!emailValido(nuevoUsuario.username)) {
            cout << "Error: el formato del email no es valido.\n";
        }

    } while (!emailValido(nuevoUsuario.username));

    // Password
    cout << "Ingrese password: ";
    getline(cin, nuevoUsuario.password);

    // Perfil
    do {
        cout << "Ingrese perfil (GENERAL / ADMIN): ";
        getline(cin, nuevoUsuario.perfil);

        if (nuevoUsuario.perfil != "GENERAL" &&
            nuevoUsuario.perfil != "ADMIN") {

            cout << "Error: perfil no valido.\n";
        }

    } while (nuevoUsuario.perfil != "GENERAL" &&
             nuevoUsuario.perfil != "ADMIN");


    // Confirmar guardado
    int opcion;

    cout << "\n1. Guardar usuario\n";
    cout << "0. Cancelar\n";
    cout << "Seleccione una opcion: ";

    cin >> opcion;


    if (opcion == 1) {

        // Agregar a la lista en memoria
        listaUsuarios.push_back(nuevoUsuario);

        // Obtener archivo desde variable de entorno
        string nombreArchivo = obtenerVariableEnv("USER_FILE");

        if (nombreArchivo.empty()) {
            cout << "Error: no se encontro la variable USER_FILE.\n";
            return;
        }

        // Abrir archivo en modo append
        ofstream archivo(nombreArchivo, std::ios::app);

        if (!archivo.is_open()) {
            cout << "Error al abrir el archivo de usuarios.\n";
            return;
        }

        // Guardar usuario
        archivo << nuevoUsuario.id << ";"
                << nuevoUsuario.nombre << ";"
                << nuevoUsuario.username << ";"
                << nuevoUsuario.password << ";"
                << nuevoUsuario.perfil
                << "\n";

        archivo.close();

        cout << "\nUsuario guardado correctamente.\n";

    } else {

        cout << "\nOperacion cancelada.\n";
    }
}
