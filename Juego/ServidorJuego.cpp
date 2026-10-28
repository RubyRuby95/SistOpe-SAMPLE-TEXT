#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <atomic>
#include <random>
#include <ctime>
#include <cerrno>
#include <cstdio>
#include <csignal>
#include <stdexcept>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Protocolo.h"
#include "../FuncionesAdministrador/Config.h"

using namespace std;

// Configuracion (.env) 

struct Configuracion {
    int puerto = 0;
    int tablero = 0;        // posicion que hay que alcanzar o superar
    int caras = 0;          // caras del dado 
    int minEquipos = 0;
    int maxEquipos = 0;
    int minPorEquipo = 0;
    int maxPorEquipo = 0;
    string archivoLog;
};

static bool leerEntero(const string& nombre, int minimo, int maximo, int& destino) {
    string texto = obtenerVariableEnv(nombre);
    if (texto.empty()) {
        cerr << "Error: la variable " << nombre << " no esta definida en el archivo .env\n";
        return false;
    }
    try {
        size_t usados = 0;
        long valor = stol(texto, &usados);
        if (usados != texto.size() || valor < minimo || valor > maximo) {
            throw invalid_argument("fuera de rango");
        }
        destino = static_cast<int>(valor);
    } catch (...) {
        cerr << "Error: " << nombre << " debe ser un entero entre " << minimo
             << " y " << maximo << " (valor actual: '" << texto << "')\n";
        return false;
    }
    return true;
}

static bool cargarConfiguracion(Configuracion& c) {
    bool ok = true;
    ok = leerEntero("GAME_PORT", 1024, 65535, c.puerto) && ok;
    ok = leerEntero("GAME_BOARD_SIZE", 1, 100000, c.tablero) && ok;
    ok = leerEntero("GAME_DICE_FACES", 2, 1000, c.caras) && ok;
    ok = leerEntero("GAME_MIN_TEAMS", 2, 100, c.minEquipos) && ok;
    ok = leerEntero("GAME_MAX_TEAMS", 2, 100, c.maxEquipos) && ok;
    ok = leerEntero("GAME_MIN_PLAYERS_PER_TEAM", 1, 100, c.minPorEquipo) && ok;
    ok = leerEntero("GAME_MAX_PLAYERS_PER_TEAM", 1, 100, c.maxPorEquipo) && ok;

    c.archivoLog = obtenerVariableEnv("GAME_LOG_FILE");
    if (c.archivoLog.empty()) {
        cerr << "Error: la variable GAME_LOG_FILE no esta definida en el archivo .env\n";
        ok = false;
    }

    if (ok && c.minEquipos > c.maxEquipos) {
        cerr << "Error: GAME_MIN_TEAMS no puede ser mayor que GAME_MAX_TEAMS\n";
        ok = false;
    }
    if (ok && c.minPorEquipo > c.maxPorEquipo) {
        cerr << "Error: GAME_MIN_PLAYERS_PER_TEAM no puede ser mayor que GAME_MAX_PLAYERS_PER_TEAM\n";
        ok = false;
    }
    return ok;
}

// Estado compartido

enum Estado { ESPERA, JUGANDO, TERMINADO };

struct Jugador {
    int fd;
    string nombre;      // vacio hasta que hace JOIN
    string equipo;
    bool conectado;
};

static Configuracion cfg;
static mutex mtx;                       // protege TODO el estado del juego
static mutex mtxLog;                    // protege el archivo de log
static ofstream archivoLog;
static vector<Jugador> jugadores;       // se indexan por id (nunca se eliminan)
static vector<string> equipos;          // equipos en orden de creacion
static vector<string> equiposEnJuego;   // equipos que participan 
static map<string, int> posiciones;     // posicion en el tablero
static vector<int> ordenTurnos;         // ids de jugadores en orden de turno
static int turnoActual = 0;             // indice dentro de ordenTurnos
static Estado estado = ESPERA;
static int fdEscucha = -1;
static atomic<bool> terminado(false);
static mt19937 generador{random_device{}()};

// Utilidades
static string mayusculas(string texto) {
    for (char& c : texto) c = static_cast<char>(toupper(static_cast<unsigned char>(c)));
    return texto;
}

static string marcaTiempo() {
    time_t ahora = time(nullptr);
    tm info;
    localtime_r(&ahora, &info);
    char buffer[32];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &info);
    return buffer;
}

// Formato: fecha hora, jugador, equipo, ACCION
static void registrar(const string& jugador, const string& equipo,
                      const string& accion, const string& detalle = "") {
    lock_guard<mutex> guardia(mtxLog);
    archivoLog << marcaTiempo() << ";" << (jugador.empty() ? "SERVIDOR" : jugador)
               << ";" << (equipo.empty() ? "-" : equipo) << ";" << accion;
    if (!detalle.empty()) archivoLog << ";" << detalle;
    archivoLog << "\n";
    archivoLog.flush();
}

static bool estaUnido(const Jugador& j) { return !j.nombre.empty(); }

// Las funciones siguientes se llaman SIEMPRE con 'mtx' tomado.

static void enviarA(int id, const string& mensaje) {
    if (jugadores[id].conectado) enviarLinea(jugadores[id].fd, mensaje);
}

static void difundir(const string& mensaje) {
    for (const Jugador& j : jugadores) {
        if (j.conectado && estaUnido(j)) enviarLinea(j.fd, mensaje);
    }
}

static int conectadosEnEquipo(const string& equipo) {
    int n = 0;
    for (const Jugador& j : jugadores) {
        if (j.conectado && estaUnido(j) && j.equipo == equipo) n++;
    }
    return n;
}

static int equiposActivos() {
    int n = 0;
    for (const string& e : equipos) {
        if (conectadosEnEquipo(e) > 0) n++;
    }
    return n;
}

// Condicion de inicio: al menos minEquipos equipos y cada equipo con al menos minPorEquipo jugadores.
static bool condicionInicio() {
    if (equiposActivos() < cfg.minEquipos) return false;
    for (const string& e : equipos) {
        int n = conectadosEnEquipo(e);
        if (n > 0 && n < cfg.minPorEquipo) return false;
    }
    return true;
}

static string resumenEquipos() {
    ostringstream ss;
    bool primero = true;
    for (const string& e : equipos) {
        int n = conectadosEnEquipo(e);
        if (n == 0) continue;
        if (!primero) ss << ", ";
        ss << e << " (" << n << (n == 1 ? " jugador)" : " jugadores)");
        primero = false;
    }
    return primero ? "(ninguno todavia)" : ss.str();
}

static string resumenPosiciones() {
    ostringstream ss;
    bool primero = true;
    for (const string& e : equiposEnJuego) {
        if (!primero) ss << " | ";
        ss << e << ": " << posiciones[e] << "/" << cfg.tablero;
        primero = false;
    }
    return ss.str();
}

static string estadoActual() {
    switch (estado) {
        case ESPERA:
            return "Estado: en espera. Equipos: " + resumenEquipos();
        case JUGANDO: {
            const Jugador& j = jugadores[ordenTurnos[turnoActual]];
            return "Estado: en juego. Posiciones: " + resumenPosiciones() +
                   ". Turno de: " + j.nombre + " (Equipo " + j.equipo + ")";
        }
        default:
            return "Estado: la partida termino. Posiciones: " + resumenPosiciones();
    }
}

static void anunciarEspera() {
    ostringstream ss;
    ss << "Esperando jugadores. Equipos: " << resumenEquipos()
       << ". Para iniciar se necesitan al menos " << cfg.minEquipos << " equipos con "
       << cfg.minPorEquipo << " jugadores cada uno (maximo " << cfg.maxEquipos
       << " equipos de " << cfg.maxPorEquipo << " jugadores).";
    difundir(ss.str());
    if (condicionInicio()) {
        difundir("Condicion de inicio cumplida: cualquier jugador puede escribir 'iniciar o i' para comenzar.");
    }
}

// Logica del juego 

static void avanzarTurno() {
    int n = static_cast<int>(ordenTurnos.size());
    for (int k = 1; k <= n; k++) {
        int idx = (turnoActual + k) % n;
        if (jugadores[ordenTurnos[idx]].conectado) {
            turnoActual = idx;
            return;
        }
    }
}

static void anunciarTurno() {
    const Jugador& j = jugadores[ordenTurnos[turnoActual]];
    difundir("Turno de: " + j.nombre + " (Equipo " + j.equipo + "). Escribe 'tirar o t' para lanzar el dado.");
    registrar(j.nombre, j.equipo, "TURNO");
}

static void terminarPartida(const string& quien, const string& ganador, const string& motivo) {
    estado = TERMINADO;
    if (ganador.empty()) {
        difundir("=== PARTIDA TERMINADA === " + motivo + ". No hay ganador.");
        registrar("", "", "FIN_PARTIDA", motivo);
    } else {
        difundir("=== PARTIDA TERMINADA === Gana el equipo " + ganador + " (" + motivo + ").");
        registrar(quien, ganador, "GANA", motivo);
        registrar("", "", "FIN_PARTIDA", "ganador=" + ganador);
    }
    difundir("Posiciones finales: " + resumenPosiciones());
    difundir("FIN Gracias por jugar.");
    terminado = true;
    if (fdEscucha >= 0) shutdown(fdEscucha, SHUT_RDWR);   // despierta el accept() del hilo principal
}

static void iniciarPartida() {
    estado = JUGANDO;
    equiposEnJuego.clear();
    for (const string& e : equipos) {
        if (conectadosEnEquipo(e) > 0) equiposEnJuego.push_back(e);
    }
    for (const string& e : equiposEnJuego) posiciones[e] = 0;

    // Orden de turnos intercalando equipos: A1, B1, A2, B2, ...
    ordenTurnos.clear();
    for (int ronda = 0;; ronda++) {
        bool agrego = false;
        for (const string& e : equiposEnJuego) {
            int visto = 0;
            for (size_t i = 0; i < jugadores.size(); i++) {
                const Jugador& j = jugadores[i];
                if (j.conectado && estaUnido(j) && j.equipo == e) {
                    if (visto == ronda) {
                        ordenTurnos.push_back(static_cast<int>(i));
                        agrego = true;
                        break;
                    }
                    visto++;
                }
            }
        }
        if (!agrego) break;
    }

    ostringstream ss;
    ss << "=== COMIENZA LA PARTIDA === Objetivo: llegar o superar la posicion " << cfg.tablero
       << ". Dado de " << cfg.caras << " caras. Equipos: ";
    for (size_t i = 0; i < equiposEnJuego.size(); i++) {
        if (i > 0) ss << ", ";
        ss << equiposEnJuego[i];
    }
    difundir(ss.str());
    registrar("", "", "INICIO_PARTIDA", resumenEquipos());

    turnoActual = static_cast<int>(ordenTurnos.size()) - 1;   // el primer avanzarTurno() deja el 0
    avanzarTurno();
    anunciarTurno();
}

static void accionIniciar(int id) {
    const Jugador& j = jugadores[id];
    if (estado != ESPERA) {
        enviarA(id, "ERROR la partida ya esta en curso o ya termino");
    } else if (!condicionInicio()) {
        enviarA(id, "ERROR aun no se cumple la condicion de inicio. " + estadoActual());
        registrar(j.nombre, j.equipo, "INICIO_RECHAZADO", "condicion de inicio no cumplida");
    } else {
        registrar(j.nombre, j.equipo, "SOLICITA_INICIO");
        iniciarPartida();
    }
}

static void accionTirar(int id) {
    const Jugador& j = jugadores[id];
    if (estado != JUGANDO) {
        enviarA(id, "ERROR la partida no esta en curso");
        registrar(j.nombre, j.equipo, "ACCION_RECHAZADA", "TIRAR;partida no en curso");
        return;
    }
    if (ordenTurnos[turnoActual] != id) {
        const Jugador& dueno = jugadores[ordenTurnos[turnoActual]];
        enviarA(id, "ERROR no es tu turno, le toca a " + dueno.nombre);
        registrar(j.nombre, j.equipo, "ACCION_RECHAZADA", "TIRAR;fuera de turno");
        return;
    }

    uniform_int_distribution<int> dado(1, cfg.caras);
    int resultado = dado(generador);
    int antes = posiciones[j.equipo];
    int despues = antes + resultado;
    posiciones[j.equipo] = despues;

    registrar(j.nombre, j.equipo, "LANZA_DADO", to_string(resultado));
    registrar(j.nombre, j.equipo, "AVANZA", to_string(antes) + ";" + to_string(despues));

    difundir(j.nombre + " (Equipo " + j.equipo + ") obtuvo: " + to_string(resultado));
    difundir("Equipo " + j.equipo + " avanzo hasta la posicion " + to_string(despues) +
             " (meta: " + to_string(cfg.tablero) + ")");

    if (despues >= cfg.tablero) {
        terminarPartida(j.nombre, j.equipo, "alcanzo la posicion " + to_string(despues));
        return;
    }

    difundir("Posiciones: " + resumenPosiciones());
    avanzarTurno();
    anunciarTurno();
}

// ======================= Conexiones =======================

// Devuelve true si el JOIN fue aceptado.
static bool procesarJoin(int id, const string& linea) {
    istringstream entrada(linea);
    string comando, nombre, equipo, sobrante;
    entrada >> comando >> nombre >> equipo;

    if (mayusculas(comando) != "JOIN" || nombre.empty() || equipo.empty() || (entrada >> sobrante)) {
        enviarA(id, "ERROR formato invalido. Usa: JOIN <nombre> <equipo> (sin espacios dentro de cada uno)");
        return false;
    }
    if (nombre.size() > 20 || equipo.size() > 20 ||
        nombre.find(';') != string::npos || equipo.find(';') != string::npos) {
        enviarA(id, "ERROR nombre/equipo invalido (maximo 20 caracteres y sin ';')");
        return false;
    }
    equipo = mayusculas(equipo);

    if (estado != ESPERA) {
        enviarA(id, "FIN La partida ya comenzo o termino. Intenta conectarte mas tarde.");
        return false;
    }
    for (const Jugador& j : jugadores) {
        if (j.conectado && estaUnido(j) && j.nombre == nombre) {
            enviarA(id, "ERROR ya hay un jugador conectado con el nombre '" + nombre + "'");
            return false;
        }
    }

    bool existe = false;
    for (const string& e : equipos) {
        if (e == equipo) existe = true;
    }
    if (conectadosEnEquipo(equipo) >= cfg.maxPorEquipo) {
        enviarA(id, "ERROR el equipo " + equipo + " esta completo (maximo " + to_string(cfg.maxPorEquipo) + ")");
        return false;
    }
    if (conectadosEnEquipo(equipo) == 0 && equiposActivos() >= cfg.maxEquipos) {
        enviarA(id, "ERROR ya existen " + to_string(cfg.maxEquipos) + " equipos; unete a uno existente: " + resumenEquipos());
        return false;
    }
    if (!existe) {
        equipos.push_back(equipo);
        posiciones[equipo] = 0;
    }

    jugadores[id].nombre = nombre;
    jugadores[id].equipo = equipo;

    enviarA(id, "OK Bienvenido " + nombre + ". Perteneces al equipo " + equipo + ".");
    registrar(nombre, equipo, "CONECTA");
    registrar(nombre, equipo, "INGRESA_EQUIPO");
    difundir(nombre + " se unio al equipo " + equipo + ".");
    anunciarEspera();
    return true;
}

static void manejarDesconexion(int id) {
    Jugador& j = jugadores[id];
    if (!j.conectado) return;
    j.conectado = false;
    if (!estaUnido(j)) return;       // nunca llego a identificarse

    registrar(j.nombre, j.equipo, "DESCONECTA", estado == TERMINADO ? "cierre_servidor" : "cliente");
    if (estado == TERMINADO) return;

    difundir(j.nombre + " (Equipo " + j.equipo + ") se desconecto.");
    if (estado == ESPERA) {
        anunciarEspera();
        return;
    }

    // Partida en curso
    int activos = 0;
    string unico;
    for (const string& e : equiposEnJuego) {
        if (conectadosEnEquipo(e) > 0) {
            activos++;
            unico = e;
        }
    }
    if (activos <= 1) {
        if (activos == 1) terminarPartida("", unico, "los demas equipos abandonaron la partida");
        else terminarPartida("", "", "todos los jugadores abandonaron");
        return;
    }
    if (ordenTurnos[turnoActual] == id) {
        avanzarTurno();
        anunciarTurno();
    }
}

// false = el cliente pidió salir.
static bool procesarComando(int id, const string& linea) {
    istringstream entrada(linea);
    string comando;
    entrada >> comando;
    comando = mayusculas(comando);

    if (comando == "SALIR") {
        enviarA(id, "FIN Hasta luego.");
        return false;
    }
    if (comando == "TIRAR") accionTirar(id);
    else if (comando == "INICIAR") accionIniciar(id);
    else if (comando == "ESTADO") enviarA(id, estadoActual());
    else enviarA(id, "ERROR comando desconocido. Usa: tirar, iniciar, estado, salir");
    return true;
}

static void atenderCliente(int id, int fd) {
    LectorLineas lector(fd);
    string linea;

    // Fase 1: identificarse (se permiten reintentos si el JOIN es rechazado)
    bool unido = false;
    while (!unido && lector.leerLinea(linea)) {
        lock_guard<mutex> guardia(mtx);
        unido = procesarJoin(id, linea);
    }

    // Fase 2: jugar
    while (unido && lector.leerLinea(linea)) {
        bool seguir;
        {
            lock_guard<mutex> guardia(mtx);
            seguir = procesarComando(id, linea);
        }
        if (!seguir) break;
    }

    {
        lock_guard<mutex> guardia(mtx);
        manejarDesconexion(id);   // marca conectado=false ANTES de cerrar el descriptor
    }
    close(fd);
}

static void manejadorSenal(int) {
    terminado = true;
    if (fdEscucha >= 0) shutdown(fdEscucha, SHUT_RDWR);
}

// Programa principal

int main() {
    signal(SIGPIPE, SIG_IGN);

    if (!cargarConfiguracion(cfg)) {
        cerr << "No se pudo iniciar el servidor: corrige el archivo .env\n";
        return 1;
    }

    archivoLog.open(cfg.archivoLog, ios::app);
    if (!archivoLog.is_open()) {
        cerr << "Error: no se pudo abrir el archivo de log '" << cfg.archivoLog << "'\n";
        return 1;
    }

    fdEscucha = socket(AF_INET, SOCK_STREAM, 0);
    if (fdEscucha < 0) {
        perror("Error al crear el socket");
        return 1;
    }
    int opcion = 1;
    setsockopt(fdEscucha, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion));

    sockaddr_in direccion{};
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = INADDR_ANY;
    direccion.sin_port = htons(static_cast<uint16_t>(cfg.puerto));

    if (bind(fdEscucha, reinterpret_cast<sockaddr*>(&direccion), sizeof(direccion)) < 0) {
        perror("Error en bind (puerto ocupado?)");
        close(fdEscucha);
        return 1;
    }
    if (listen(fdEscucha, 16) < 0) {
        perror("Error en listen");
        close(fdEscucha);
        return 1;
    }

    struct sigaction accion{};
    accion.sa_handler = manejadorSenal;      // sin SA_RESTART: accept() sale con EINTR
    sigaction(SIGINT, &accion, nullptr);
    sigaction(SIGTERM, &accion, nullptr);

    registrar("", "", "SERVIDOR_INICIA", "puerto=" + to_string(cfg.puerto));
    cout << "Servidor escuchando en el puerto " << cfg.puerto << " (Ctrl+C para cerrar)\n";
    cout << "Log: " << cfg.archivoLog << "\n";

    vector<thread> hilos;
    while (!terminado) {
        sockaddr_in direccionCliente{};
        socklen_t largo = sizeof(direccionCliente);
        int fd = accept(fdEscucha, reinterpret_cast<sockaddr*>(&direccionCliente), &largo);
        if (fd < 0) {
            if (terminado) break;
            if (errno == EINTR) continue;
            perror("Error en accept");
            break;
        }

        int id;
        {
            lock_guard<mutex> guardia(mtx);
            jugadores.push_back(Jugador{fd, "", "", true});
            id = static_cast<int>(jugadores.size()) - 1;
        }
        enviarLinea(fd, "Bienvenido al juego");
        hilos.emplace_back(atenderCliente, id, fd);
    }

    // ---- Cierre controlado de recursos ----
    {
        lock_guard<mutex> guardia(mtx);
        if (estado != TERMINADO) {
            difundir("FIN El servidor se esta cerrando.");
            registrar("", "", "CIERRE_SERVIDOR", "interrumpido antes de terminar la partida");
            estado = TERMINADO;   // desde aqui las desconexiones ya no se evaluan como abandonos
        }
        for (const Jugador& j : jugadores) {
            if (j.conectado) shutdown(j.fd, SHUT_RDWR);   // desbloquea los hilos que esperan datos
        }
    }
    for (thread& h : hilos) h.join();

    close(fdEscucha);
    registrar("", "", "SERVIDOR_CIERRA", "recursos liberados");
    archivoLog.close();
    cout << "Servidor cerrado correctamente.\n";
    return 0;
}
