#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

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

bool checkWinner(const char* board, char player) {
    //comprobacion de filas
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS - 3; ++col) {
            if (board[row * BOARD_COLS + col] == player &&
                board[row * BOARD_COLS + col + 1] == player &&
                board[row * BOARD_COLS + col + 2] == player &&
                board[row * BOARD_COLS + col + 3] == player) {
                return true;
            }
        }
    }
    //comprobacion de columnas
    for (int col = 0; col < BOARD_COLS; ++col) {
        for (int row = 0; row < BOARD_ROWS - 3; ++row) {
            if (board[row * BOARD_COLS + col] == player &&
                board[(row + 1) * BOARD_COLS + col] == player &&
                board[(row + 2) * BOARD_COLS + col] == player &&
                board[(row + 3) * BOARD_COLS + col] == player) {
                return true;
            }
        }
    }
    // Comprobación de diagonales ascendentes
    for (int row = 3; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS - 3; ++col) {
            if (board[row * BOARD_COLS + col] == player &&
                board[(row - 1) * BOARD_COLS + col + 1] == player &&
                board[(row - 2) * BOARD_COLS + col + 2] == player &&
                board[(row - 3) * BOARD_COLS + col + 3] == player) {
                return true;
            }
        }
    }
    //comprobacion de diagonales descendentes
    for (int row = 0; row < BOARD_ROWS - 3; ++row) {
        for (int col = 0; col < BOARD_COLS - 3; ++col) {
            if (board[row * BOARD_COLS + col] == player &&
                board[(row + 1) * BOARD_COLS + col + 1] == player &&
                board[(row + 2) * BOARD_COLS + col + 2] == player &&
                board[(row + 3) * BOARD_COLS + col + 3] == player) {
                return true;
            }
        }
    }
    return false;
}

struct ClientData {
    int clientSocket;
    struct sockaddr_in clientAddr;
};

void* handleClient(void* data) {
    ClientData* clientData = static_cast<ClientData*>(data);
    int clientSocket = clientData->clientSocket;
    struct sockaddr_in clientAddr = clientData->clientAddr;
    char board[BOARD_ROWS * BOARD_COLS];
    memset(board, '.', sizeof(board));

    bool isGameOver = false;
    char currentPlayer = 'S'; //comienza el server
    int column;

    while (!isGameOver) {
        send(clientSocket, board, sizeof(board), 0);
        displayBoard(board);

        if (currentPlayer == 'S') {
            std::cout << "Turno del servidor. Ingrese el número de columna (1-7): ";
            std::cin >> column;
            --column;
        } else {
            recv(clientSocket, &column, sizeof(column), 0);
            --column;
        }

        if (column < 0 || column >= BOARD_COLS || board[column] != '.') {
            std::cerr << "Columna inválida. Inténtelo de nuevo." << std::endl;
            continue;
        }

        int row;
        for (row = BOARD_ROWS - 1; row >= 0; --row) {
            if (board[row * BOARD_COLS + column] == '.') {
                board[row * BOARD_COLS + column] = currentPlayer;
                break;
            }
        }

        if (checkWinner(board, currentPlayer)) {
            send(clientSocket, board, sizeof(board), 0);
            displayBoard(board);
            std::cout << "¡El jugador " << currentPlayer << " ha ganado!" << std::endl;
            isGameOver = true;
        } else {
            bool isBoardFull = true;
            for (int i = 0; i < BOARD_ROWS * BOARD_COLS; ++i) {
                if (board[i] == '.') {
                    isBoardFull = false;
                    break;
                }
            }
            if (isBoardFull) {
                send(clientSocket, board, sizeof(board), 0);
                displayBoard(board);
                std::cout << "¡Empate!" << std::endl;
                isGameOver = true;
            } else {
                currentPlayer = (currentPlayer == 'S') ? 'C' : 'S';
            }
        }
    }

    close(clientSocket);
    delete clientData;
    return nullptr;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>" << std::endl;
        return EXIT_FAILURE;
    }

    int serverSocket;
    struct sockaddr_in serverAddr;

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        std::cerr << "Error al crear el socket." << std::endl;
        return EXIT_FAILURE;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(std::atoi(argv[1]));
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(&(serverAddr.sin_zero), '\0', 8);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(struct sockaddr)) == -1) {
        std::cerr << "Error al vincular el socket al puerto." << std::endl;
        return EXIT_FAILURE;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error al escuchar por conexiones entrantes." << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Esperando conexiones ..." << std::endl;

    while (true) {
        ClientData* clientData = new ClientData;
        socklen_t sinSize = sizeof(struct sockaddr_in);
        if ((clientData->clientSocket = accept(serverSocket, (struct sockaddr*)&clientData->clientAddr, &sinSize)) == -1) {
            std::cerr << "Error al aceptar la conexión entrante." << std::endl;
            delete clientData;
            continue;
        }

        std::cout << "Conexión aceptada desde " << inet_ntoa(clientData->clientAddr.sin_addr) << ":" << ntohs(clientData->clientAddr.sin_port) << std::endl;

        pthread_t thread;
        pthread_create(&thread, nullptr, handleClient, clientData);
        pthread_detach(thread);
    }

    close(serverSocket);
    return EXIT_SUCCESS;
}
