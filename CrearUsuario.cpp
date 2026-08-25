#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <cstdlib>

#include "CrearUsuario.h"
#include "Config.h"

using namespace std;

int obtenerSiguienteID(const string& nombreArchivo) {

    ifstream archivo(nombreArchivo);

    // Si el archivo no existe o está vacío
    if (!archivo.is_open()) {
        return 1;
    }

    string linea;
    int mayorID = 0;

    while (getline(archivo, linea)) {

        // Ignorar líneas vacías
        if (linea.empty()) {
            continue;
        }

        // Obtener la primera parte de la línea
        // Ejemplo:
        // 5;Pedro Soto;pedro@gmail.com;1234;GENERAL

        size_t posicion = linea.find(';');

        if (posicion == string::npos) {
            continue;
        }

        string idTexto = linea.substr(0, posicion);

        try {
            int id = stoi(idTexto);

            if (id > mayorID) {
                mayorID = id;
            }

        } catch (...) {
            // Si el ID no es un número, ignorar la línea
        }
    }

    archivo.close();

    return mayorID + 1;
}


bool emailValido(const string& email) {
    regex formato(R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)");

    return regex_match(email, formato);
}


void crearUsuario(vector<Usuario>& listaUsuarios) {
    cout << "ejecutando la función crearUsuario..." << endl;

    Usuario nuevoUsuario;

    // Obtener archivo desde variable de entorno
    string nombreArchivo = obtenerVariableEnv("USER_FILE");

    if (nombreArchivo.empty()) {
        cout << "Error: no se encontro la variable USER_FILE.\n";
        return;
    }

    // Generar ID automáticamente
    nuevoUsuario.id = obtenerSiguienteID(nombreArchivo);

    cout << "\n===== CREAR USUARIO =====\n";

    // ID
    cout << "ID: " << nuevoUsuario.id << "\n";

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

        // Convertir el perfil ingresado a mayusculas
        transform(
            nuevoUsuario.perfil.begin(),
            nuevoUsuario.perfil.end(),
            nuevoUsuario.perfil.begin(),
            [](unsigned char c) {
                return toupper(c);
            }
        );

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
        // agregar usuario a la lista en memoria
        listaUsuarios.push_back(nuevoUsuario);

        cout << "\nUsuario guardado correctamente.\n";

    } else {

        cout << "\nOperacion cancelada.\n";
    }
}
