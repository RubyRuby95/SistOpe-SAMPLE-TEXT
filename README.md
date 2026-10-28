# Sistemas Operativos – Proyecto 1 (Sample Text)

Aplicación en C++ que simula la gestión básica de un sistema operativo mediante un **menú interactivo**. Cada opción del menú corresponde a un módulo: gestión de usuarios y perfiles, cálculos, lectura y análisis de texto, entre otros.

## Tabla de contenidos

1. [Pre-requisitos](#1-pre-requisitos)
2. [Compilación y ejecución](#2-compilación-y-ejecución)
3. [Variables de entorno](#3-variables-de-entorno)
4. [Inicio de sesión y validación](#4-inicio-de-sesión-y-validación)
5. [Módulos del menú](#5-módulos-del-menú)

---

## 1. Pre-requisitos

- Compilador de C++ (por ejemplo `g++`)
- `make`

## 2. Compilación y ejecución

**Paso 1 – Compilar:**

```bash
make
```

**Paso 2 – Ejecutar:**

```bash
./programa -u <Usuario> -p <Contraseña> -f <Archivo>
```

| Parámetro | Descripción |
|:----------|:------------|
| `-u <Usuario>` | Nombre de un usuario guardado en la base de datos |
| `-p <Contraseña>` | Contraseña del usuario |
| `-f <Archivo>` | Ruta **absoluta** de un archivo `.txt` del computador local. Se proyecta usar los libros de la carpeta `LIBROS` |

Para obtener la ruta absoluta de un archivo, ubícate en su carpeta y ejecuta:

```bash
pwd
```

**Ejemplo:**

```bash
./programa -u lvc -p 1001 -f /home/lvc/archivo.txt
```

> **Formato de rutas:** cualquier ruta de archivo ingresada dentro del programa (o en el comando anterior) debe seguir el formato `home/carpeta/archivo.txt`.

## 3. Variables de entorno

| Variable | Descripción |
|:---------|:------------|
| `USER_FILE` | Ruta del archivo `Database/USUARIOS.txt` |
| `PERFIL_FILE` | Ruta del archivo `Database/PERFILES.txt` |

## 4. Inicio de sesión y validación

Al iniciar, el sistema busca el usuario indicado y verifica su contraseña. Si el usuario no existe o la contraseña no coincide, el programa muestra un aviso y **se cierra automáticamente**.

### Usuario administrador por defecto

Si todavía no hay usuarios creados, existe un administrador predefinido que permite crear los primeros perfiles y usuarios:

| Usuario | Contraseña |
|:--------|:-----------|
| `administrador` | `1234` |

> ⚠️ Estas credenciales son fijas y solo deben usarse para la configuración inicial.

## 5. Módulos del menú

| Sección | Módulo | Resumen |
|:--------|:-------|:--------|
| 5.1 | Gestión de usuarios/perfiles | Crear, listar y eliminar usuarios y perfiles |
| 5.2 | Multiplicación de matrices | Multiplica dos matrices cuadradas leídas desde archivos |
| 5.3 | Juego | En construcción |
| 5.4 | Palabra palíndromo | Verifica si una palabra es palíndromo |
| 5.5 | Calcular función | Evalúa `f(x) = x*x + 2x + 8` |
| 5.6 | Conteo sobre texto | Analiza el archivo indicado con `-f` |
| 5.7 | Conteo de archivos | Analiza un archivo cuya ruta se ingresa en el menú |

### 5.1 Gestión de usuarios/perfiles

Módulo principal para administrar usuarios y perfiles. **Requiere un perfil de administrador**; el usuario predeterminado permite acceder cuando aún no existen perfiles.

Se divide en dos áreas, ambas con las operaciones **listar, crear y eliminar**. Cada vez que se crea o elimina un registro, el sistema actualiza el archivo correspondiente.

| Área | Datos solicitados al crear | Eliminación por | Archivo |
|:-----|:---------------------------|:----------------|:--------|
| Usuarios | Nombre, correo electrónico, contraseña y tipo de perfil | ID | `USUARIOS.txt` |
| Perfiles | Nombre del perfil y números de las funciones a las que tendrá acceso | Nombre del perfil | `PERFILES.txt` |

**Notas:**

- Por ahora solo se pueden asignar los roles `general` o `admin`. Esto cambiará cuando existan perfiles con acceso a funciones específicas.
- Como todavía no hay una lista restrictiva de funciones, el sistema acepta cualquier valor numérico al crear un perfil.

### 5.2 Multiplicación de matrices

Programa independiente ejecutado desde el `main` que multiplica dos matrices.

1. El programa pide las rutas de dos archivos (matriz **A** y matriz **B**), con el formato `home/carpeta1/carpeta2/A.txt`.
2. Pide el **carácter separador** usado entre los números de las matrices.
3. Lee ambos archivos, carga los valores en matrices y calcula el producto.

**Condiciones (si no se cumplen, el programa falla):**

- Ambas matrices deben ser **cuadradas y del mismo tamaño**.
- Ambos archivos deben compartir el mismo formato.
- El separador indicado debe ser el realmente usado en los archivos.

### 5.3 Juego

Muestra el mensaje "en construcción".

### 5.4 Palabra palíndromo

Indica si una palabra ingresada es palíndromo.

1. Selecciona la opción 4, **"EsPalindromo"**.
2. Ingresa una palabra.
3. Elige entre verificar la palabra o cancelar la operación.
4. Si verificas, el programa llama a la función de comprobación y muestra si la palabra es o no palíndromo.

### 5.5 Calcular función

Evalúa `f(x) = x*x + 2x + 8`.

1. Elige `1` para calcular o `0` para salir.
2. Ingresa el valor de `x` (se aceptan todos los números reales).
3. El programa imprime el resultado.

Si se ingresa un carácter o símbolo no numérico, se muestra un mensaje de error y se permite ingresar `x` nuevamente.

### 5.6 Conteo sobre texto

Cuenta **vocales, consonantes, caracteres especiales y palabras** del archivo indicado con `-f` al ejecutar el programa.

- Verifica que el archivo exista y pueda abrirse; de lo contrario muestra un error.
- Las vocales y consonantes incluyen letras con tilde y la `ñ`.
- Los números **no** se consideran caracteres especiales.
- Una palabra es cualquier grupo de caracteres separado por espacios que contenga al menos una letra o número.
- El archivo se lee por bloques, por lo que funciona con libros grandes.
- Acepta textos en **UTF-8** y **Latin-1**.
- Ingresa `0` para volver al menú.

### 5.7 Conteo de archivos

Pide la ruta de un archivo de texto y entrega un resumen con la cantidad de vocales, consonantes, caracteres especiales y palabras.

- Valida que el archivo exista antes de procesarlo.
- Ingresa `0` para cancelar la operación.
