#include "server.h"
#include <stdio.h>

int init_winsock()
{
    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    return 0;
}

SOCKET create_listen_socket() {
    struct addrinfo *result = NULL, hints = {0};
    SOCKET listenSocket = INVALID_SOCKET;

    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, PORT, &hints, &result) != 0) {
        printf("getaddrinfo failed\n");
        return INVALID_SOCKET;
    }

    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }

    if (bind(listenSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
        printf("bind failed: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        return INVALID_SOCKET;
    }

    printf("Listening on port %s\n", PORT);
    return listenSocket;
}

SOCKET accept_client(SOCKET listenSocket) {
    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
    }
    return clientSocket;
}

void cleanup_winsock() {
    WSACleanup();
}

void handle_client(SOCKET clientSocket)
{
    char buf[BUF_SIZE];
    int iResult, iSendResult; // will take the number of bytes read/write
    
    do {
        iResult = recv(clientSocket, buf, BUF_SIZE, 0);
        
        if(iResult > 0) {
            printf("Bytes received: %d\n", iResult);

            // Echo the buffer back to the sender 
            iSendResult = send(clientSocket, buf, iResult, 0);
            if(iSendResult == SOCKET_ERROR) {
                printf("send failed: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return;
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if(iResult == 0)
            printf("Connection closing ...\n");
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
    }while(iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    if(shutdown(clientSocket, SD_SEND) == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
    }

    closesocket(clientSocket);
}
