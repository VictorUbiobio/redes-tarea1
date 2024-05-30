#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024
#define BOARD_ROWS 6
#define BOARD_COLS 7

//funcion para mostrar el tablero
void displayBoard(const char* board) {
    std::cout << "TABLERO" << std::endl;
    std::cout << "1 2 3 4 5 6 7" << std::endl;
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            std::cout << board[i * BOARD_COLS + j] << " ";
        }
        std::cout << std::endl;
    }
}

//Funcin para validar la entrada del jugador
bool isValidColumn(const char* board, int column) {
    return column >= 1 && column <= BOARD_COLS && board[(BOARD_ROWS - 1) * BOARD_COLS + (column - 1)] == '.';
}

//funcion para verificar si hay un empate
bool isBoardFull(const char* board) {
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            if (board[i * BOARD_COLS + j] == '.') {
                return false;
            }
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <direcci�n_IP_del_servidor> <puerto>" << std::endl;
        return EXIT_FAILURE;
    }

    int clientSocket;
    struct sockaddr_in serverAddr;

    //crear el socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return EXIT_FAILURE;
    }

    //configurar la direccion del server
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0) {
        std::cerr << "Direcci�n IP inv�lida." << std::endl;
        return EXIT_FAILURE;
    }
    memset(&(serverAddr.sin_zero), '\0', 8);

    //conectar al servidor
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error al conectar con el servidor." << std::endl;
        return EXIT_FAILURE;
    }

    char buffer[MAX_BUFFER_SIZE];
    char board[BOARD_ROWS * BOARD_COLS];

    //ini el tablero
    memset(board, '.', sizeof(board));

    //loop para interactuar con el servidor
    while (true) {
        //recibir el estado actual del tablero desde el servidor
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error al recibir datos del servidor." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';

        //mostrar el tablero
        displayBoard(buffer);

        //turno del jugador
        int column;
        do {
            std::cout << "Tu turno. Ingresa el n�mero de columna (1-7): ";
            std::cin >> column;
        } while (!isValidColumn(board, column));

        //enviar la columna seleccionada al server
        send(clientSocket, &column, sizeof(column), 0);

        //actualizar el tablero con el movimiento del jugador
        for (int i = BOARD_ROWS - 1; i >= 0; --i) {
            if (board[i * BOARD_COLS + (column - 1)] == '.') {
                board[i * BOARD_COLS + (column - 1)] = 'C';
                break;
            }
        }

        //mostrar el tablero actualizado
        displayBoard(board);

        //verificar si el jugador gano
        //aqui implementarias la logica para verificar si el jugador gano

        //verificar si hay un empate
        if (isBoardFull(board)) {
            std::cout << "El juego ha terminado en empate." << std::endl;
            break;
        }

        //bro aqui falta implementar el algoritmo que reciba el movimiento del servidor y actualize el tablero
    }

    close(clientSocket);
    return EXIT_SUCCESS;
}

