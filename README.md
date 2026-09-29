# SISTEMAS-OPERATIVOS-PROYECTO-1---SAMPLE-TEXT
<span style="color:orange"> 1. Proposito de la aplicacion</span><br>

Creando un sistema que simula como seria la gestion de un sistema operativo implementando diferentes secciones con un menu que permite seleccionar cada parte, como gestion, calculos, lectura, etc <br>

<span style="color:orange"> 2. Para ejecutar desde bash </span><br>

Paso 1: Compilar el archivo usando <br>
```make``` <br>
Paso 2: Ya se puede ejecutar con <br>
```./programa -u <Usuario> -p <Contraseña> -f <Archivo>``` <br>
Siendo ```<Usuario>``` un nombre de usuario guardado en la base de datos, <br>
Siendo ```<Contraseña>``` su respectiva contraseña, <br>
y siendo ```<Archivo>``` la ruta absoluta de un archivo .txt disponible en el computador local: <br>
en este caso se proyecta usar los archivos disponibles en la carpeta LIBROS. <br>
La ruta absoluta se puede obtener con: <br>
```pwd``` <br>

Ejemplo: ```./programa -u lvc -p 1001 -f /home/lvc/archivo.txt``` <br>
Paso Extra: Al escribir cualquier archivo dentro del programa o ya sea en el paso anterior tiene que tener el siguente formato:
"home/wdadwa/dwadwa.txt" <br>
<span style="color:orange"> 3. descripcion de las variables de entorno </span><br>
<b>USER_FILE</b> -> almacena ruta del archivo USUARIOS.txt <br>
<b>PERFIL_FILE</b> -> almacena ruta del archivo PERFIL.txt

<span style="color:orange"> 4. Inicio de sesión y validación de usuarios</span><br>

Al iniciar sesión de la forma explicada anteriormente, el sistema buscará el usuario indicado y verificará su contraseña. En caso de no encontrar al usuario o si la contraseña ingresada no coincide, el programa mostrará un aviso y se cerrará automáticamente. <br>

En caso de no haber usuarios creados en el sistema, existe un Usuario con perfil de administrador y una clave fija diseñado específicamente para acceder y crear nuevos perfiles o usuarios.
El nombre de usuario de administrador es administrador y la clave es 1234. Este usuario permite crear nuevos perfiles si no se tiene alguno<br>

<span style="color:orange"> 5.1 Gestión de Usuarios/perfiles </span><br>
Este es el módulo principal encargado de la gestión de usuarios y perfiles. Para acceder a él, es requisito contar con un perfil de administrador. El usuario predeterminado permite ingresar a esta funcionalidad en caso de que aún no existan perfiles creados. <br>
Esta sección se divide en dos áreas: gestión de usuarios y gestión de perfiles, permitiendo listar, crear y eliminar registros en ambas categorías. La eliminación de usuarios se realiza mediante su ID, mientras que los perfiles se eliminan utilizando el nombre del perfil. Cada vez que se crea o elimina un registro, el sistema edita el archivo correspondiente para añadir o remover la información. <br>
Para registrar un usuario, el sistema solicita nombre, correo electrónico, contraseña y tipo de perfil. Por el momento, solo es posible asignar los roles "general" o "admin"; esto se modificará en el futuro cuando se requieran distintos perfiles con accesos específicos a diversas funciones. Al crear el usuario, este se almacena en el archivo USUARIOS.txt.
Para registrar un perfil, se solicita únicamente el nombre del mismo y los números de las funciones a las que tendrá acceso. Dado que actualmente no hay una lista restrictiva de funciones, el sistema acepta cualquier valor numérico y guarda la información en el archivo PERFILES.txt. <br>


<span style="color:orange"> 5.2 Multiplicacion de matrices</span><br>
Este es un programa aparte que se ejecuta en nuestro main y su proposito es multiplicar matrices. <br>
El programa le pide al usuario 2 rutas a 2 archivos distintos por ejemplo A (que representa la matriz A) y B (que representa la matriz B), estas rutas se piden de la forma "home/carpeta1/carpeta2/A.txt". Los dos archivos deben compartir formato y deben ser matrices cuadradas del mismo tamaño, si no se cumplen estas condiciones el programa fallará. Ademas de esto los numeros de las matrices deben tener un caracter separador que el programa pide al usuario, el programa considerará que el usuario le dio la informacion correcta y con añadirá los numeros a una matriz con vectores a estos vectores y sus valores internos se le aplicará la logica de multiplicación y devolverá el resultado.


<span style="color:orange"> 5.4 Palabra Palíndromo</span><br>
Es una serie de funciones que le indican al usuario si una palabra ingresada es palíndromo o no.
Al seleccionar la opción 4 "EsPalindromo", la interfaz le pide al usuario que ingrese una palabra, posteriormente la interfaz consulta al usuario si quiere verificar si la palabra es palíndromo o si quiere cancelar la operación. Si el usuario toma la primera opción, la interfaz llama a una función que hace la verificación a la palabra ingresada por el usuario y, dependiendo del resultado, la interfaz le indica si la palabra es o no palíndromo.

<span style="color:orange"> 5.4 Calcular función</span><br>
Primero el programa despliega dos opciones 1 para calcular x en función y 0 para salir, luego de ejecutar 1 el usuario tiene que se le entregar un parametro X, luego en f(x) = x*x + 2x + 8 se evalua x y se retorna el resultado implimiendolo en pantalla, x se permite todo tipo de números reales, por lo cual al ejecutar un caracter o símbolo no númerico envía mensaje de error y dejando volver a ingresar x 

