#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT "8080"
#define BUF_SIZE 512

int main(int argc, char **argv) {

    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints = {0};
    
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP; // TCP
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, PORT, &hints, &result) != 0) {
        printf("getaddrinfo failed\n");
        WSACleanup();
        return 2;
    }

    SOCKET listenSocket = INVALID_SOCKET;

    if((listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol)) 
        == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 3;
    }

    if((bind(listenSocket, result->ai_addr, (int)result->ai_addrlen)) 
        == SOCKET_ERROR){
            printf("bind failed: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(listenSocket);
            WSACleanup();
            return 4;
    }

    freeaddrinfo(result);

    if(listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        printf( "Listen failed with error: %d\n", WSAGetLastError() );
        closesocket(listenSocket);
        WSACleanup();
        return 5;
    }

    printf("Listening to port: %s\n", PORT);

    SOCKET clientSocket = INVALID_SOCKET;

    if((clientSocket = accept(listenSocket, NULL, NULL))
        == INVALID_SOCKET) {
        printf("accept failed: %d\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 6;
    }

    closesocket(listenSocket);

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
                return 7;
            }
            printf("Bytes sent: %d\n", iSendResult);
        } else if(iResult == 0)
            printf("Connection closing ...\n");
        else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 8;
        }
    }while(iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    if(shutdown(clientSocket, SD_SEND) == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 9;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}