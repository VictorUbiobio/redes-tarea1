# Juego Cuatro en Línea - Cliente/Servidor

## Descripción del Juego

Cuatro en Línea es un juego clásico de mesa donde dos jugadores se turnan para colocar fichas en un tablero de 6 filas por 7 columnas. El objetivo es ser el primer jugador en alinear cuatro fichas del mismo color en horizontal, vertical o diagonal.

## Requisitos

- Compilador de C++ compatible con el estándar C++11.
- Sistema operativo con soporte para operaciones de red (sockets).
- WSL (Windows Subsystem for Linux) si estás utilizando Windows.

## Estructura del Proyecto

- Cliente.cpp
- Servidor.cpp
- Makefile
- Comandos.txt
- README.md

## Instrucciones de Uso

### Servidor

#### Compilación:

1. Abre una terminal y navega al directorio del proyecto.
2. Ejecuta el siguiente comando para compilar el servidor: `make servidor`

#### Ejecución:

Una vez compilado, inicia el servidor proporcionando el número de puerto como argumento: `./servidor <puerto>`
Ejemplo: `./servidor 7777`

### Cliente

#### Compilación:

1. Abre una nueva terminal y navega al directorio del proyecto.
2. Ejecuta el siguiente comando para compilar el cliente: `make cliente`

#### Ejecución:

Una vez compilado, inicia el cliente proporcionando la dirección IP del servidor y el puerto como argumentos: `./cliente <dirección_IP_del_servidor> <puerto>`
Ejemplo: `./cliente 127.0.0.1 7777`

## Detalles de Implementación

### Cliente

- `Cliente.cpp` se encarga de conectarse al servidor, recibir el estado del tablero, mostrarlo al usuario y permitirle hacer su jugada.
- Funciones principales:
  - `displayBoard(const char* board)`: Muestra el tablero.
  - `isValidColumn(const char* board, int column)`: Valida la entrada del jugador.
  - `isBoardFull(const char* board)`: Verifica si el tablero está lleno.

### Servidor

- `Servidor.cpp` se encarga de aceptar conexiones de clientes, gestionar el estado del juego y determinar el ganador.
- Funciones principales:
  - `displayBoard(const char* board)`: Muestra el tablero.
  - `checkWinner(const char* board, char player)`: Verifica si hay un ganador.

## Comandos de Compilación y Ejecución

### Compilación

- make servidor
- make cliente

### Ejecucion

- ./servidor <puerto>
- ./cliente <dirección_IP_del_servidor> <puerto>

### Ejemplo de Uso

### Compilar el servidor y el cliente:

- make servidor
- make cliente

### Ejecutar el servidor (en una terminal):

- ./servidor 7777

### Ejecutar el cliente (en otra terminal):

- ./cliente 127.0.0.1 7777

## Contribuciones

- Siéntete libre de contribuir con mejoras, correcciones de errores o nuevas características al proyecto. ¡Toda ayuda es bienvenida!

## Licencia

- Este proyecto está bajo la Licencia MIT.