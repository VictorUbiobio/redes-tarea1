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

void displayBoard(const char* board) {
    std::cout << "TABLERO" << std::endl;
    for (int i = 0; i < BOARD_ROWS; ++i) {
        for (int j = 0; j < BOARD_COLS; ++j) {
            std::cout << board[i * BOARD_COLS + j] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "1 2 3 4 5 6 7" << std::endl;
}

bool isValidColumn(const char* board, int column) {
    return column >= 1 && column <= BOARD_COLS && board[column - 1] == '.';
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <direccion_IP_del_servidor> <puerto>" << std::endl;
        return EXIT_FAILURE;
    }

    int clientSocket;
    struct sockaddr_in serverAddr;

    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serverAddr.sin_addr) <= 0) {
        std::cerr << "Dirección IP inválida." << std::endl;
        return EXIT_FAILURE;
    }
    memset(&(serverAddr.sin_zero), '\0', 8);

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error al conectar con el servidor." << std::endl;
        return EXIT_FAILURE;
    }

    char buffer[MAX_BUFFER_SIZE];
    char board[BOARD_ROWS * BOARD_COLS];
    memset(board, '.', sizeof(board));

    while (true) {
        ssize_t bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error al recibir datos del servidor." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';
        std::memcpy(board, buffer, sizeof(board));

        displayBoard(board);

        int column;
        do {
            std::cout << "Tu turno. Ingresa el número de columna (1-7): ";
            std::cin >> column;
        } while (!isValidColumn(board, column));

        send(clientSocket, &column, sizeof(column), 0);

        bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesReceived <= 0) {
            std::cerr << "Error al recibir datos del servidor." << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';
        std::memcpy(board, buffer, sizeof(board));

        displayBoard(board);

    }

    close(clientSocket);
    return EXIT_SUCCESS;
}
