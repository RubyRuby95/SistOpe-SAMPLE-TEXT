#ifndef PROTOCOLO_H
#define PROTOCOLO_H
#include <string>
#include <cerrno>
#include <sys/socket.h>
#include <unistd.h>

// Envia una linea completa (maneja envios parciales y evita SIGPIPE).
inline bool enviarLinea(int fd, const std::string& mensaje) {
    std::string datos = mensaje + "\n";
    size_t total = 0;
    while (total < datos.size()) {
        ssize_t n = send(fd, datos.data() + total, datos.size() - total, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) continue;
            return false;
        }
        total += static_cast<size_t>(n);
    }
    return true;
}

// Acumula los bytes recibidos y los entrega de a una linea.
struct LectorLineas {
    int fd;
    std::string buffer;

    explicit LectorLineas(int descriptor) : fd(descriptor) {}

    // Lee lo disponible y lo agrega al buffer. false = conexion cerrada o error.
    bool recibirDatos() {
        char temporal[1024];
        while (true) {
            ssize_t n = read(fd, temporal, sizeof(temporal));
            if (n < 0 && errno == EINTR) continue;
            if (n <= 0) return false;
            buffer.append(temporal, static_cast<size_t>(n));
            // protección: un cliente no puede mandar líneas gigantes
            if (buffer.size() > 8192 && buffer.find('\n') == std::string::npos) return false;
            return true;
        }
    }

    // Saca una linea del buffer (sin bloquear). false = no hay linea completa.
    bool extraerLinea(std::string& linea) {
        size_t pos = buffer.find('\n');
        if (pos == std::string::npos) return false;
        linea = buffer.substr(0, pos);
        buffer.erase(0, pos + 1);
        if (!linea.empty() && linea.back() == '\r') linea.pop_back();
        return true;
    }

    // Bloquea hasta tener una linea completa. false = conexion cerrada.
    bool leerLinea(std::string& linea) {
        while (!extraerLinea(linea)) {
            if (!recibirDatos()) return false;
        }
        return true;
    }
};

#endif
