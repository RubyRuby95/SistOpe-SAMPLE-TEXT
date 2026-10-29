Sistemas Operativos – Proyecto (Sample Text)
Aplicación en C++ que simula la gestión básica de un sistema operativo mediante un menú interactivo. Cada opción del menú corresponde a un módulo: gestión de usuarios y perfiles, cálculos, lectura y análisis de texto, creación paralela de un índice invertido y un juego multiplayer por sockets.
Tabla de contenidos
1.	Pre-requisitos
2.	Compilación y ejecución
3.	Variables de entorno
4.	Inicio de sesión y validación
5.	Módulos del menú
6.	Entrega 3 – Problema 1: índice invertido paralelo
7.	Entrega 3 – Problema 2: juego multiplayer por sockets
8.	Estructura del repositorio

1. Pre-requisitos
·	Linux o WSL (se usan semáforos POSIX y sockets POSIX)
·	Compilador de C++ con soporte de C++17 (por ejemplo g++)
·	make
2. Compilación y ejecución
Paso 1 – Crear el archivo de configuración (ver sección 3):
cp .env.example .env

Todos los programas se ejecutan desde la carpeta raíz del proyecto, porque leen el archivo .env del directorio actual.
Paso 2 – Compilar (genera programa, multi, servidor_juego y cliente_juego):
make

Paso 3 – Ejecutar el programa principal:
./programa -u <Usuario> -p <Contraseña> -f <Archivo>

Parámetro	Descripción
-u <Usuario>	Nombre de un usuario guardado en la base de datos
-p <Contraseña>	Contraseña del usuario
-f <Archivo>	Ruta absoluta de un archivo .txt del computador local. Se proyecta usar los libros de la carpeta Database/LIBROS

Para obtener la ruta absoluta de un archivo, ubícate en su carpeta y ejecuta:
pwd

Ejemplo:
./programa -u administrador -p 1234 -f $(pwd)/Database/LIBROS/Heidi.txt

Formato de rutas: cualquier ruta de archivo ingresada dentro del programa (o en el comando anterior) debe seguir el formato /home/carpeta/archivo.txt.
Para limpiar los archivos compilados: make clean.
3. Variables de entorno
Las configuraciones no están escritas en el código fuente: se leen de variables de entorno. El programa busca cada variable primero en el entorno de la terminal (export VARIABLE=valor) y, si no existe, en el archivo .env de la carpeta actual. Así, una variable exportada en la terminal tiene prioridad sobre el .env.
El repositorio incluye una plantilla .env.example con todos los valores; basta copiarla a .env (el .env está en .gitignore). Antes de usar cada valor, el programa valida que exista y que sea válido, y si no lo es muestra un mensaje de error comprensible.
Usuarios y perfiles
Variable	Descripción	Ejemplo
USER_FILE	Ruta del archivo de usuarios	Database/USUARIOS.txt
PERFIL_FILE	Ruta del archivo de perfiles	Database/PERFILES.txt

Problema 1 – Índice invertido
Variable	Descripción	Validación	Ejemplo
LIBROS_DIR	Carpeta con los libros .txt a indexar	Debe existir, ser carpeta y poder leerse; debe contener al menos un .txt	Database/LIBROS
N_THREADS	Cantidad de threads trabajadores	Entero positivo	4
INDEX_FILE	Archivo de salida del índice invertido	Debe estar definida	indice_invertido.idx
BOOKS_MAP_FILE	Archivo donde se guarda la asociación ID;nombre del libro	Debe estar definida	mapa_libros.txt

Problema 2 – Juego multiplayer
Variable	Usada por	Descripción	Rango válido	Ejemplo
GAME_HOST	cliente	Dirección del servidor	texto	127.0.0.1
GAME_PORT	servidor y cliente	Puerto TCP	1024 – 65535	5050
GAME_LOG_FILE	servidor	Archivo de log del juego	texto	juego.log
GAME_BOARD_SIZE	servidor	Posición (X) que hay que alcanzar o superar para ganar	1 – 100000	30
GAME_DICE_FACES	servidor	Caras (R) del dado	2 – 1000	6
GAME_MIN_TEAMS	servidor	Mínimo de equipos para iniciar	2 – 100	2
GAME_MAX_TEAMS	servidor	Máximo de equipos	2 – 100	4
GAME_MIN_PLAYERS_PER_TEAM	servidor	Mínimo de jugadores por equipo para iniciar	1 – 100	2
GAME_MAX_PLAYERS_PER_TEAM	servidor	Máximo de jugadores por equipo	1 – 100	4

Además se exige GAME_MIN_TEAMS ≤ GAME_MAX_TEAMS y GAME_MIN_PLAYERS_PER_TEAM ≤ GAME_MAX_PLAYERS_PER_TEAM.
Ejemplo de uso con variables exportadas
N_THREADS=8 ./programa -u administrador -p 1234 -f $(pwd)/Database/LIBROS/Heidi.txt
GAME_PORT=6000 ./servidor_juego

4. Inicio de sesión y validación
Al iniciar, el sistema busca el usuario indicado y verifica su contraseña. Si el usuario no existe o la contraseña no coincide, el programa muestra un aviso y se cierra automáticamente.
Usuario administrador por defecto
Si todavía no hay usuarios creados, existe un administrador predefinido que permite crear los primeros perfiles y usuarios:
Usuario	Contraseña
administrador	1234

⚠️ Estas credenciales son fijas y solo deben usarse para la configuración inicial.
5. Módulos del menú
Opción	Módulo	Resumen
1	Gestión de usuarios/perfiles	Crear, listar y eliminar usuarios y perfiles (requiere perfil ADMIN)
2	Multiplicación de matrices	Multiplica dos matrices cuadradas leídas desde archivos
3	Juego	Abre el cliente del juego multiplayer (Problema 2)
4	Palabra palíndromo	Verifica si una palabra es palíndromo
5	Calcular función	Evalúa f(x) = x*x + 2x + 8
6	Conteo sobre texto	Analiza el archivo indicado con -f
7	Conteo de archivos	Analiza un archivo cuya ruta se ingresa en el menú
8	Crear índice invertido	Construye el índice invertido con threads y semáforos (Problema 1)

5.1 Gestión de usuarios/perfiles
Módulo principal para administrar usuarios y perfiles. Requiere un perfil de administrador; el usuario predeterminado permite acceder cuando aún no existen perfiles.
Se divide en dos áreas, ambas con las operaciones listar, crear y eliminar. Cada vez que se crea o elimina un registro, el sistema actualiza el archivo correspondiente.
Área	Datos solicitados al crear	Eliminación por	Archivo
Usuarios	Nombre, correo electrónico, contraseña y tipo de perfil	ID	USER_FILE
Perfiles	Nombre del perfil y números de las funciones a las que tendrá acceso	Nombre del perfil	PERFIL_FILE

Notas:
·	Por ahora solo se pueden asignar los roles general o admin. Esto cambiará cuando existan perfiles con acceso a funciones específicas.
·	Como todavía no hay una lista restrictiva de funciones, el sistema acepta cualquier valor numérico al crear un perfil.
5.2 Multiplicación de matrices
Programa independiente (multi) ejecutado desde el main que multiplica dos matrices.
1.	El programa pide las rutas de dos archivos (matriz A y matriz B), con el formato /home/carpeta1/carpeta2/A.txt.
2.	Pide el carácter separador usado entre los números de las matrices.
3.	Lee ambos archivos, carga los valores en matrices y calcula el producto.
Condiciones (si no se cumplen, el programa falla):
·	Ambas matrices deben ser cuadradas y del mismo tamaño.
·	Ambos archivos deben compartir el mismo formato.
·	El separador indicado debe ser el realmente usado en los archivos.
5.3 Juego
Lanza el cliente del juego (./cliente_juego) usando el nombre del usuario con el que se inició sesión como nombre de jugador; el equipo se pregunta por teclado. El servidor debe estar ejecutándose antes (en otra terminal: ./servidor_juego). Si no hay servidor, se muestra un mensaje de error y se vuelve al menú. Ver sección 7 para las reglas y la arquitectura.
5.4 Palabra palíndromo
Indica si una palabra ingresada es palíndromo.
1.	Selecciona la opción 4, "EsPalindromo".
2.	Ingresa una palabra.
3.	Elige entre verificar la palabra o cancelar la operación.
4.	Si verificas, el programa llama a la función de comprobación y muestra si la palabra es o no palíndromo.
5.5 Calcular función
Evalúa f(x) = x*x + 2x + 8.
1.	Elige 1 para calcular o 0 para salir.
2.	Ingresa el valor de x (se aceptan todos los números reales).
3.	El programa imprime el resultado.
Si se ingresa un carácter o símbolo no numérico, se muestra un mensaje de error y se permite ingresar x nuevamente.
5.6 Conteo sobre texto
Cuenta vocales, consonantes, caracteres especiales y palabras del archivo indicado con -f al ejecutar el programa.
·	Verifica que el archivo exista y pueda abrirse; de lo contrario muestra un error.
·	Las vocales y consonantes incluyen letras con tilde y la ñ.
·	Los números no se consideran caracteres especiales.
·	Una palabra es cualquier grupo de caracteres separado por espacios que contenga al menos una letra o número.
·	El archivo se lee por bloques, por lo que funciona con libros grandes.
·	Acepta textos en UTF-8 y Latin-1.
·	Ingresa 0 para volver al menú.
5.7 Conteo de archivos
Pide la ruta de un archivo de texto y entrega un resumen con la cantidad de vocales, consonantes, caracteres especiales y palabras.
·	Valida que el archivo exista antes de procesarlo.
·	Ingresa 0 para cancelar la operación.
5.8 Crear índice invertido
Construye el índice invertido de todos los libros de LIBROS_DIR usando varios threads. El detalle del funcionamiento está en la sección 6. Al terminar muestra un resumen (libros procesados, palabras distintas, tiempo y archivo generado) y pide ingresar 0 para volver al menú.

6. Entrega 3 – Problema 1: índice invertido paralelo
6.1 Cómo ejecutarlo
1.	Verificar que el .env tenga definidas LIBROS_DIR, N_THREADS, INDEX_FILE y BOOKS_MAP_FILE (ver sección 3).
2.	Ejecutar ./programa -u <usuario> -p <contraseña> -f <archivo>.
3.	Elegir la opción 8. Crear índice invertido.
Antes de procesar, el programa valida: que INDEX_FILE y BOOKS_MAP_FILE estén definidas, que N_THREADS sea un entero positivo, y que la carpeta de libros exista, pueda leerse y contenga archivos .txt. Si algo falla, informa el motivo y vuelve al menú sin crear nada.
6.2 Flujo de procesamiento: cola → threads → semáforos → resultados → índice invertido
  carpeta LIBROS_DIR
        │  (1) se listan los .txt y se asigna un ID a cada libro (1, 2, 3...)
        ▼
  ┌──────────────┐   (2) el hilo principal encola la posición de cada libro
  │ COLA DE      │       y luego una marca FIN por cada thread
  │ TRABAJO      │
  └──────┬───────┘
         │  (3) cada thread saca UN libro de la cola
   ┌─────┼─────┬─────┐
   ▼     ▼     ▼     ▼          N_THREADS threads trabajadores
  T1    T2    T3 …  TN          (4) cada uno cuenta las palabras de su libro
   │     │     │     │              en su PROPIO mapa local (sin bloqueos)
   └─────┴──┬──┴─────┘
            │  (5) al terminar un libro, el thread mezcla su resultado parcial
            ▼       en el índice global (SECCIÓN CRÍTICA protegida con semáforo)
   ┌────────────────┐
   │ ÍNDICE GLOBAL  │──► (6) cuando todos los threads terminaron (join),
   └────────────────┘        se ordena y se escribe en INDEX_FILE

1.	Cola de trabajo (ColaTrabajo). Cola FIFO compartida. Cada elemento es la posición de un libro en la lista de libros. Como todos los threads la usan, está protegida: un semáforo binario (mutex_, valor 1) protege la estructura y un semáforo contador (disponibles_, valor inicial 0) cuenta cuántos elementos hay, de modo que un thread se duerme (sin gastar CPU) cuando la cola está vacía. Un libro sale de la cola una sola vez, por lo que nunca es procesado por más de un thread.
2.	Threads trabajadores (ThreadsTrabajadores). Se crean N_THREADS threads antes de cargar la cola. Cada uno repite: sacar un libro → contar sus palabras → integrar el resultado → anotar el progreso → pedir otro libro, hasta recibir la marca FIN.
3.	Semáforos (Semaforos). Clase Semaforo sobre sem_t POSIX (sem_init, sem_wait, sem_post, sem_destroy) y la clase Guardia, que hace wait al crearse y post al destruirse (así el semáforo siempre se libera, incluso con un return anticipado). Secciones críticas protegidas:

| Recurso compartido | Dónde | Semáforo |

|:-------------------|:------|:---------|

| La cola (estructura FIFO) | ColaTrabajo::encolar/desencolar/vacia | mutex_ (binario) |

| Cantidad de elementos disponibles | ColaTrabajo | disponibles_ (contador) |

| El índice global | IndiceGlobal::integrar | mutex (binario) |

| Contadores de progreso y salida por pantalla | Progreso::registrarExito/registrarFallo | mutex (binario) |
4.	Resultados parciales (ConteoPalabras). Cada thread cuenta las palabras de su libro en un mapa propio (palabra → veces); como nadie más lo toca, no necesita semáforos. Una palabra es una secuencia de letras (con tildes y ñ) pasada a minúscula; los números y signos de puntuación separan palabras. Se lee por bloques de 64 KB y acepta UTF-8 y Latin-1.
5.	Construcción del índice (ConstruccionIndice). IndiceGlobal::integrar mezcla el resultado de un libro en el índice único bajo el semáforo. Cada palabra aparece una sola vez con todos los libros donde se encontró. Al final, guardar ordena las palabras alfabéticamente y los libros por ID, de modo que el archivo es idéntico con 1 o con 20 threads.
6.	Finalización (Finalizacion). El hilo principal espera con join a todos los threads y verifica que la cola quedó vacía y que procesados + fallidos = total de libros. Solo entonces escribe el índice. Si un libro no puede leerse, se informa en el resumen y no detiene a los demás.
6.3 Archivos de salida
Índice invertido (INDEX_FILE). Una línea por palabra, con el formato palabra;(idLibro,veces);(idLibro,veces)...:
aa;(8,2);(50,1);(52,1);(55,1);(59,30);(63,1)
aaaa;(61,2)
computador;(1,15);(3,8);(7,21)

Mapa de libros (BOOKS_MAP_FILE). Asocia el ID numérico con el nombre del archivo, de modo que se pueda saber a qué libro corresponde cada ID del índice:
1;A_Book_of_Myths.txt
2;A_Doll's_House_a_play.txt
3;A_Life_of_Walt_Whitman.txt

Los IDs se asignan siguiendo el orden de los nombres de archivo de la carpeta (1, 2, 3…), por lo que son los mismos en cada ejecución mientras no cambie el contenido de la carpeta.

7. Entrega 3 – Problema 2: juego multiplayer por sockets
El juego es una carrera de equipos por turnos sobre un tablero. El servidor y los clientes son programas separados que se comunican por sockets TCP.
7.1 Cómo ejecutarlo
Se necesitan al menos 4 terminales (con la configuración por defecto: 2 equipos × 2 jugadores), todas en la carpeta raíz del proyecto:
# Terminal 1: servidor
./servidor_juego

# Terminales 2, 3, 4, 5: un cliente por jugador (cada uno pregunta su equipo)
./cliente_juego            # pregunta nombre y equipo
./cliente_juego Pedro      # o recibe el nombre como argumento y pregunta el equipo

También se puede entrar al juego desde el programa principal con la opción 3 del menú (usa el nombre del usuario conectado como nombre de jugador).
El servidor termina solo, cerrando todo ordenadamente, cuando acaba la partida; también se puede cerrar con Ctrl+C.
Comandos del jugador (se escriben en el cliente):
Comando	Atajo	Acción
tirar	t	Lanza el dado (solo en tu turno)
iniciar	i	Pide iniciar la partida (solo si se cumple la condición de inicio)
estado	e	Muestra el estado actual de la partida
salir	s o 0	Se desconecta

7.2 Arquitectura: clientes → sockets → servidor → estado compartido del juego
 ┌───────────┐                     ┌────────────────────────────────────────┐
 │ cliente 1 │──┐                  │            servidor_juego              │
 └───────────┘  │   sockets TCP    │                                        │
 ┌───────────┐  ├───(líneas de ───►│  hilo principal: accept()              │
 │ cliente 2 │──┤    texto)        │     └─► 1 thread por cliente conectado │
 └───────────┘  │                  │            │                           │
 ┌───────────┐  │                  │            ▼   (mutex)                 │
 │ cliente N │──┘                  │   ESTADO COMPARTIDO: jugadores,        │
 └───────────┘ ◄── difusión a ─────│   equipos, posiciones, turno, estado   │
                  todos            │            │   (mutex del log)         │
                                   │            ▼                           │
                                   │       archivo de log                   │
                                   └────────────────────────────────────────┘

·	Servidor (Juego/ServidorJuego.cpp): crea el socket de escucha y por cada conexión lanza un thread. El estado de la partida (jugadores, equipos, posiciones, turno actual, estado de la partida) vive solo en el servidor y está protegido con un mutex; un segundo mutex protege el archivo de log. Cada acción de un cliente se valida contra ese estado y luego el servidor difunde el resultado a todos los jugadores.
·	Cliente (Juego/ClienteJuego.cpp): se conecta al servidor, se identifica y usa poll() para atender a la vez el teclado y los mensajes que llegan del servidor.
·	Protocolo (Juego/Protocolo.h): mensajes de una línea de texto terminada en \n. Contiene enviarLinea (maneja envíos parciales y evita SIGPIPE) y LectorLineas (reconstruye líneas completas a partir de los bytes recibidos y limita su tamaño).
Mensaje cliente → servidor	Significado
JOIN <nombre> <equipo>	Identificarse y unirse a un equipo (si el equipo no existe, se crea)
TIRAR	Lanzar el dado
INICIAR	Pedir el inicio de la partida
ESTADO	Consultar el estado
SALIR	Desconectarse

Los mensajes del servidor al cliente son OK ..., ERROR ... (acción rechazada), FIN ... (el cliente termina) o texto informativo para mostrar.
7.3 Reglas del juego
Aspecto	Regla
Jugadores	Cada jugador tiene un nombre único (máx. 20 caracteres, sin espacios ni ;). Con la configuración por defecto: mínimo 4 y máximo 16 jugadores.
Equipos	Entre GAME_MIN_TEAMS y GAME_MAX_TEAMS equipos (por defecto 2 a 4), con entre GAME_MIN_PLAYERS_PER_TEAM y GAME_MAX_PLAYERS_PER_TEAM jugadores cada uno (por defecto 2 a 4). El nombre del equipo se guarda en mayúsculas.
Condición de inicio	La partida comienza cuando existen al menos GAME_MIN_TEAMS equipos y cada equipo tiene al menos GAME_MIN_PLAYERS_PER_TEAM jugadores conectados, y algún jugador escribe iniciar. Mientras no se cumpla, el servidor informa a todos el estado de espera. Una vez iniciada, no se aceptan nuevos jugadores.
Turnos	Los turnos se intercalan entre equipos (A1, B1, A2, B2, …). Solo el jugador al que le toca puede lanzar; si otro lo intenta, el servidor lo rechaza con un error y lo registra en el log.
Acción permitida	tirar: lanza un dado de GAME_DICE_FACES caras (resultado aleatorio de 1 a R, generado por el servidor). El equipo del jugador avanza ese número de posiciones. Luego pasa el turno al siguiente jugador conectado.
Estado compartido	El servidor informa a todos: quién lanzó, qué obtuvo, hasta qué posición avanzó su equipo, las posiciones de todos los equipos y de quién es el siguiente turno.
Condición de término	La partida termina cuando un equipo alcanza o supera la posición GAME_BOARD_SIZE: ese equipo gana. También termina si solo queda un equipo con jugadores conectados (gana por abandono de los demás) o si no queda nadie. El servidor informa el resultado a todos.
Desconexiones	Un cliente que se desconecta no afecta al servidor ni a los demás jugadores: se registra en el log, y si era su turno, este pasa al siguiente.
Cierre	Al terminar la partida el servidor avisa a todos, cierra los sockets de los clientes, espera (join) a todos sus threads, cierra el socket de escucha y el archivo de log.

7.4 Archivo de log
El servidor escribe en GAME_LOG_FILE (en modo append, sin borrar ejecuciones anteriores) una línea por evento, con el formato:
fecha hora;jugador;equipo;ACCION[;detalle]

Para eventos del servidor el jugador es SERVIDOR y el equipo -. Ejemplo real:
2026-10-28 19:13:18;Pedro;ROJO;CONECTA
2026-10-28 19:13:18;Pedro;ROJO;INGRESA_EQUIPO
2026-10-28 19:13:19;Pedro;ROJO;SOLICITA_INICIO
2026-10-28 19:13:19;SERVIDOR;-;INICIO_PARTIDA;ROJO (2 jugadores), AZUL (2 jugadores)
2026-10-28 19:13:19;Pedro;ROJO;TURNO
2026-10-28 19:13:21;Pedro;ROJO;LANZA_DADO;2
2026-10-28 19:13:21;Pedro;ROJO;AVANZA;0;2
2026-10-28 19:13:21;Juan;ROJO;DESCONECTA;cliente
2026-10-28 19:13:25;Zeta;AZUL;GANA;alcanzo la posicion 32
2026-10-28 19:13:25;SERVIDOR;-;FIN_PARTIDA;ganador=AZUL
2026-10-28 19:13:25;SERVIDOR;-;SERVIDOR_CIERRA;recursos liberados

Evento	Cuándo se registra
SERVIDOR_INICIA	El servidor arranca (detalle: puerto)
CONECTA, INGRESA_EQUIPO	Un jugador se conecta y se une a un equipo
SOLICITA_INICIO, INICIO_PARTIDA	Un jugador pide iniciar y la partida comienza
INICIO_RECHAZADO	Se pidió iniciar sin cumplir la condición de inicio
TURNO	Inicia o cambia el turno
LANZA_DADO	Resultado del dado (detalle: valor)
AVANZA	Cambio de posición del equipo (detalle: posición anterior;nueva)
ACCION_RECHAZADA	Acción fuera de turno o con la partida no en curso
GANA, FIN_PARTIDA	Equipo ganador y término de la partida
DESCONECTA	Un jugador se desconecta
CIERRE_SERVIDOR, SERVIDOR_CIERRA	Cierre del servidor (con o sin partida terminada)


8. Estructura del repositorio
.
├── main.cpp                     Menú principal
├── Makefile                     Compila programa, multi, servidor_juego y cliente_juego
├── .env.example                 Plantilla de variables de entorno (copiar a .env)
├── Database/                    USUARIOS.txt, PERFILES.txt y LIBROS/
├── FuncionesAdministrador/      Gestión de usuarios/perfiles y lectura del .env (Config.cpp)
├── FuncionesInterfaz/           Menús de gestión
├── FuncionesEntrega2/           Palíndromo, función y conteos de texto
├── FuncionesEntrega3/           Problema 1: índice invertido
│   ├── IndiceInvertido.*          Opción de menú: coordina todos los pasos        (1.1)
│   ├── Configuracion.*            Variables de entorno INDEX_FILE / BOOKS_MAP_FILE (1.2, 3.4)
│   ├── CarpetaLibros.*            Verificación y listado de la carpeta de libros   (1.3)
│   ├── ColaTrabajo.*              Cola de trabajo compartida                       (1.4)
│   ├── ThreadsTrabajadores.*      Creación y ciclo de vida de los threads          (1.5)
│   ├── NumeroThreads.*            Lectura y validación de N_THREADS                (1.6)
│   ├── Semaforos.*                Clase Semaforo y Guardia                         (1.7)
│   ├── ConteoPalabras.*           Conteo de palabras por libro                     (1.8)
│   ├── IdentificacionLibros.*     ID de libros y archivo ID;nombre                 (1.9)
│   ├── ConstruccionIndice.*       Índice global y escritura del .idx               (1.10)
│   └── Finalizacion.*             Progreso, join y resumen final                   (1.11)
├── Juego/                       Problema 2: juego multiplayer
│   ├── ServidorJuego.cpp          Servidor
│   ├── ClienteJuego.cpp           Cliente
│   └── Protocolo.h                Envío y lectura de líneas por socket
└── ProgramaMultiMatriz/         Multiplicación de matrices

