# SISTEMAS-OPERATIVOS-PROYECTO-1---SAMPLE-TEXT
<span style="color:orange"> 1. Proposito de la aplicacion</span><br>

Creando un sistema que simula como seria la gestion de un sistema operativo implementando diferentes secciones con un menu que permite seleccionar cada parte, como gestion, calculos, lectura, etc <br>

<span style="color:orange"> 2. Para ejecutar desde bash </span><br>

Paso 1: Compilar el archivo usando <br>
```make``` <br>
Paso 2: Ya se puede ejecutar con <br>
```./programa -u <Usuario> -p <Contraseña> -f <Archivo>``` <br>
Siendo <Usuario> un nombre de usuario guardado en la base de datos, <br>
Siendo <Contraseña> su respectiva contraseña, <br>
y siendo <Archivo> la ruta absoluta de un archivo .txt disponible en el computador local: en este caso se proyecta usar los archivos disponibles en la carpeta LIBROS. <br>
La ruta absoluta se puede obtener con: <br>
```pwd``` <br>

Ejemplo: ```./programa -u lvc -p 1001 -f /home/lvc/archivo.txt``` <br>
Paso Extra: Al escribir cualquier archivo dentro del programa o ya sea en el paso anterior tiene que tener el siguente formato:
"home/wdadwa/dwadwa.txt" <br>
<span style="color:orange"> 3. descripcion de las variables de entorno </span><br>
<b>USER_FILE</b> -> almacena ruta del archivo USUARIOS.txt <br>
<b>PERFIL_FILE</b> -> almacena ruta del archivo PERFIL.txt
