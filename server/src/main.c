#include <stdio.h>
#include "server.h"
#include "battleship.h"

int main(int argc, char **argv) {

    struct Board board;

    init_board(&board);
    display(&board);

    return 0;

    if (init_winsock() != 0)
        return 1;

    SOCKET listenSocket = create_listen_socket();
    if (listenSocket == INVALID_SOCKET) {
        cleanup_winsock();
        return 2;
    }

    SOCKET clientSocket = accept_client(listenSocket);
    closesocket(listenSocket);

    if (clientSocket == INVALID_SOCKET) {
        cleanup_winsock();
        return 3;
    }

    handle_client(clientSocket);

    cleanup_winsock();

    return 0;
}