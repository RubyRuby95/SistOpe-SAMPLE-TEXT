#include <iostream>
#include <string>
#include <csignal>
#include <cerrno>
#include <stdexcept>
#include <poll.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include "Protocolo.h"
#include "../FuncionesAdministrador/Config.h"
using namespace std;

static int conectar(const string& host, int puerto) {
    addrinfo pistas{};
    pistas.ai_family = AF_INET;
    pistas.ai_socktype = SOCK_STREAM;
    addrinfo* resultado = nullptr;
    if (getaddrinfo(host.c_str(), to_string(puerto).c_str(), &pistas, &resultado) != 0) {
        return -1;
    }
    int fd = -1;
    for (addrinfo* p = resultado; p != nullptr; p = p->ai_next) {
        fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (fd < 0) continue;
        if (connect(fd, p->ai_addr, p->ai_addrlen) == 0) break;
        close(fd);
        fd = -1;
    }
    freeaddrinfo(resultado);
    return fd;
}

static string minusculas(string texto) {
    for (char& c : texto) c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    return texto;
}

static string recortar(const string& texto) {
    size_t ini = texto.find_first_not_of(" \t");
    if (ini == string::npos) return "";
    size_t fin = texto.find_last_not_of(" \t");
    return texto.substr(ini, fin - ini + 1);
}

// Convierte lo que escribe el jugador en un comando del protocolo ("" = no valido).
static string traducirComando(const string& entrada) {
    string t = minusculas(recortar(entrada));
    if (t == "tirar" || t == "t") return "TIRAR";
    if (t == "iniciar" || t == "i") return "INICIAR";
    if (t == "estado" || t == "e") return "ESTADO";
    if (t == "salir" || t == "s" || t == "0") return "SALIR";
    return "";
}

// Deja el nombre apto para el protocolo sin espacios ni ';' y de maximo 20 caracteres.
static string limpiarNombre(string nombre) {
    for (char& c : nombre) {
        if (c == ' ' || c == ';' || c == '\t') c = '_';
    }
    if (nombre.size() > 20) nombre.resize(20);
    return nombre;
}

// Uso: ./cliente_juego [nombre]   (si no se indica nombre, se pregunta por teclado)
int main(int argc, char* argv[]) {
    signal(SIGPIPE, SIG_IGN);

    string nombrePredefinido = (argc > 1) ? limpiarNombre(argv[1]) : "";

    string host = obtenerVariableEnv("GAME_HOST");
    if (host.empty()) {
        cerr << "Error: la variable GAME_HOST no esta definida en el archivo .env\n";
        return 1;
    }
    string textoPuerto = obtenerVariableEnv("GAME_PORT");
    int puerto = 0;
    try {
        size_t usados = 0;
        long valor = stol(textoPuerto, &usados);
        if (textoPuerto.empty() || usados != textoPuerto.size() || valor < 1024 || valor > 65535) {
            throw invalid_argument("puerto");
        }
        puerto = static_cast<int>(valor);
    } catch (...) {
        cerr << "Error: GAME_PORT debe ser un entero entre 1024 y 65535 (valor actual: '"
             << textoPuerto << "')\n";
        return 1;
    }

    int fd = conectar(host, puerto);
    if (fd < 0) {
        cerr << "Error: no se pudo conectar con el servidor en " << host << ":" << puerto
             << ". Verifica que el servidor este en ejecucion.\n";
        return 1;
    }

    LectorLineas servidor(fd);
    LectorLineas teclado(STDIN_FILENO);
    string linea;

    if (!servidor.leerLinea(linea)) {
        cerr << "El servidor cerro la conexion.\n";
        close(fd);
        return 1;
    }
    cout << "\n===== JUEGO: CARRERA DE EQUIPOS =====\n" << linea << "\n";

    // Identificacion: se repite hasta que el servidor acepte el JOIN
    bool unido = false;
    while (!unido) {
        string nombre = nombrePredefinido;
        if (nombre.empty()) {
            cout << "Nombre de jugador: " << flush;
            if (!teclado.leerLinea(nombre)) { close(fd); return 0; }
        } else {
            cout << "Jugador: " << nombre << "\n";
        }
        cout << "Equipo: " << flush;
        string equipo;
        if (!teclado.leerLinea(equipo)) { close(fd); return 0; }

        nombre = recortar(nombre);
        equipo = recortar(equipo);
        if (nombre.empty() || equipo.empty() ||
            nombre.find(' ') != string::npos || equipo.find(' ') != string::npos) {
            cout << "El nombre y el equipo no pueden estar vacios ni contener espacios.\n";
            continue;
        }

        if (!enviarLinea(fd, "JOIN " + nombre + " " + equipo) || !servidor.leerLinea(linea)) {
            cerr << "Se perdio la conexion con el servidor.\n";
            close(fd);
            return 1;
        }
        if (linea.rfind("OK", 0) == 0) {
            cout << linea.substr(linea.size() > 3 ? 3 : 2) << "\n";
            unido = true;
        } else if (linea.rfind("FIN", 0) == 0) {
            cout << (linea.size() > 4 ? linea.substr(4) : "") << "\n";
            close(fd);
            return 0;
        } else {
            cout << linea << "\n";    // ERROR, se vuelve a pedir
            nombrePredefinido.clear(); // por si el problema era el nombre repetido, etc.
        }
    }

    cout << "\nComandos: tirar (t), iniciar (i), estado (e), salir (s o 0)\n\n";

    // Bucle principal: teclado + servidor
    bool activo = true;
    bool tecladoAbierto = true;
    pollfd fds[2];
    fds[0] = {STDIN_FILENO, POLLIN, 0};
    fds[1] = {fd, POLLIN, 0};

    while (activo) {
        fds[0].fd = tecladoAbierto ? STDIN_FILENO : -1;
        if (poll(fds, 2, -1) < 0) {
            if (errno == EINTR) continue;
            break;
        }

        if (fds[1].revents & (POLLIN | POLLHUP | POLLERR)) {
            if (!servidor.recibirDatos()) {
                cout << "Conexion con el servidor cerrada.\n";
                break;
            }
            while (servidor.extraerLinea(linea)) {
                if (linea.rfind("FIN", 0) == 0) {
                    cout << (linea.size() > 4 ? linea.substr(4) : "") << "\n";
                    activo = false;
                    break;
                }
                cout << linea << "\n";
            }
        }

        if (activo && tecladoAbierto && (fds[0].revents & (POLLIN | POLLHUP))) {
            if (!teclado.recibirDatos()) {
                tecladoAbierto = false;          // fin de la entrada
                enviarLinea(fd, "SALIR");
                continue;
            }
            while (teclado.extraerLinea(linea)) {
                if (recortar(linea).empty()) continue;
                string comando = traducirComando(linea);
                if (comando.empty()) {
                    cout << "Comando no reconocido. Usa: tirar (t), iniciar (i), estado (e), salir (s o 0)\n";
                } else if (!enviarLinea(fd, comando)) {
                    cout << "Se perdio la conexion con el servidor.\n";
                    activo = false;
                    break;
                }
            }
        }
    }

    close(fd);
    return 0;
}
