# Juego Cuatro en Línea - Cliente/Servidor

Este es un juego de Cuatro en Línea implementado utilizando el modelo Cliente/Servidor en C++.

## Descripción del Juego

El juego consiste en alinear cuatro fichas del mismo color en horizontal, vertical o diagonal en un tablero de 6 filas por 7 columnas.

## Instrucciones de Uso

### Servidor

1. Compila el archivo "servidor.cpp" utilizando el siguiente comando:
    ```
    $ g++ -o servidor servidor.cpp
    ```

2. Ejecuta el servidor proporcionando el número de puerto como argumento:
    ```
    $ ./servidor <puerto>
    ```

### Cliente

1. Compila el archivo "cliente.cpp" utilizando el siguiente comando:
    ```
    $ g++ -o cliente cliente.cpp
    ```

2. Ejecuta el cliente proporcionando la dirección IP del servidor y el puerto como argumentos:
    ```
    $ ./cliente <dirección_IP_del_servidor> <puerto>
    ```

## Dependencias

- Se requiere un compilador de C++ que sea compatible con el estándar C++11.
- Este proyecto utiliza sockets de red, por lo que el sistema operativo debe admitir operaciones de red.

## Contribuciones

Siéntete libre de contribuir con mejoras, correcciones de errores o nuevas características al proyecto. ¡Toda ayuda es bienvenida!

## Licencia

Este proyecto está bajo la Licencia MIT. Consulta el archivo LICENSE para obtener más detalles.
