#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define MAX_BUFFER_SIZE 1024

//funcion para mostrar el tablero
void displayBoard(const char* board) {
    std::cout << "TABLERO" << std::endl;
    std::cout << board << std::endl;
}

//funcion para verificar si hay un ganador
bool checkWinner(const char* board, char player) {
    //comprobacin de filas
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row * 7 + col] == player &&
                board[row * 7 + col + 1] == player &&
                board[row * 7 + col + 2] == player &&
                board[row * 7 + col + 3] == player) {
                return true;
            }
        }
    }
    //comprobacin de columnas
    for (int col = 0; col < 7; ++col) {
        for (int row = 0; row < 3; ++row) {
            if (board[row * 7 + col] == player &&
                board[(row + 1) * 7 + col] == player &&
                board[(row + 2) * 7 + col] == player &&
                board[(row + 3) * 7 + col] == player) {
                return true;
            }
        }
    }
    //comprobacin de diagonales ascendentes
    for (int row = 3; row < 6; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row * 7 + col] == player &&
                board[(row - 1) * 7 + col + 1] == player &&
                board[(row - 2) * 7 + col + 2] == player &&
                board[(row - 3) * 7 + col + 3] == player) {
                return true;
            }
        }
    }
    //comprobacin de diagonales descendentes
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 4; ++col) {
            if (board[row * 7 + col] == player &&
                board[(row + 1) * 7 + col + 1] == player &&
                board[(row + 2) * 7 + col + 2] == player &&
                board[(row + 3) * 7 + col + 3] == player) {
                return true;
            }
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>" << std::endl;
        return EXIT_FAILURE;
    }

    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t sinSize = sizeof(struct sockaddr_in);

    //crear el socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return EXIT_FAILURE;
    }

    //configurar la direccion del servidor
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::atoi(argv[1]));
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(serverAddr.sin_zero), '\0', 8);

    //vincular el socket al puerto
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == -1) {
        std::cerr << "Error al vincular el socket al puerto." << std::endl;
        return EXIT_FAILURE;
    }

    //escuchar por conexiones entrantes
    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error al escuchar por conexiones entrantes." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Esperando conexiones ..." << std::endl;

    //aceptar conexiones entrantes
    while (true) {
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &sinSize)) == -1) {
            std::cerr << "Error al aceptar la conexi�n entrante." << std::endl;
            continue;
        }

        std::cout << "Juego nuevo[" << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "]" << std::endl;

        char board[6 * 7];
        memset(board, '.', sizeof(board));

        bool isGameOver = false;
        char currentPlayer = 'S'; //comienza el servidor
        int column;

        //loop del juego
        while (!isGameOver) {
            //mostrar el tablero
            displayBoard(board);

            //solicitar al jugador actual que seleccione una columna
            std::cout << "Turno de " << currentPlayer << ". Ingrese el n�mero de columna (1-7): ";
            std::cin >> column;
            --column; //convertir de base 1 a base 0

            //validar la columna ingresada
            if (column < 0 || column >= 7 || board[column] != '.') {
                std::cerr << "Columna inv�lida. Int�ntelo de nuevo." << std::endl;
                continue;
            }

            //colocar la ficha en la columna seleccionada
            int row;
            for (row = 5; row >= 0; --row) {
                if (board[row * 7 + column] == '.') {
                    board[row * 7 + column] = currentPlayer;
                    break;
                }
            }

            //verificar si hay un ganador
            if (checkWinner(board, currentPlayer)) {
                displayBoard(board);
                std::cout << "�El jugador " << currentPlayer << " ha ganado!" << std::endl;
                isGameOver = true;
            } else {
                //verificar si hay un empate
                bool isBoardFull = true;
                for (int i = 0; i < 6 * 7; ++i) {
                    if (board[i] == '.') {
                        isBoardFull = false;
                        break;
                    }
                }
                if (isBoardFull) {
                    displayBoard(board);
                    std::cout << "�Empate!" << std::endl;
                    isGameOver = true;
                } else {
                    //cambiar al siguiente jugador
                    currentPlayer = (currentPlayer == 'S') ? 'C' : 'S';
                }
            }
        }

        close(clientSocket);
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}

