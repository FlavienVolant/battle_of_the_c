#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")

#define ADDR "localhost"
#define PORT "8080"
#define BUF_SIZE 512

int main(int argc, char** argv) {

    WSADATA wsaData;

    if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("WSAStartup failed\n");
        return 1;
    }

    struct addrinfo *result = NULL, *ptr = NULL, hints = {0};

    hints.ai_family = AF_INET; // IPV_4
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    if(getaddrinfo(ADDR, PORT, &hints, &result) != 0) {
        printf("getaddrinfo failed\n");
        WSACleanup();
        return 2;
    }

    SOCKET connectSocket = INVALID_SOCKET;

    // Attempt to connect to the first address returned by
    // the call to getaddrinfo
    ptr=result;

    // Create a SOCKET for connecting to server
    if((connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol))
        == INVALID_SOCKET) {
        printf("Error at socket(): %d\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 3;
    }

    if(connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen)
        == SOCKET_ERROR) {
        closesocket(connectSocket);
        connectSocket = INVALID_SOCKET;
    }

    // Should really try the next address returned by getaddrinfo
    // if the connect call failed
    // But for this simple example we just free the resources
    // returned by getaddrinfo and print an error message

    freeaddrinfo(result);

    if(connectSocket == INVALID_SOCKET) {
        printf("Unable to connect to %s %s !\n", ADDR, PORT);
        WSACleanup();
        return 4;
    }

    char buf[BUF_SIZE];

    const char *msg = "Hello From Client";

    int iResult;

    iResult = send(connectSocket, msg, strlen(msg), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return 5;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // Receive data until the server closes the connection
    do {
        iResult = recv(connectSocket, buf, BUF_SIZE, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed: %d\n", WSAGetLastError());
    } while (iResult > 0);

    // shutdown the send half of the connection since no more data will be sent
    iResult = shutdown(connectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed: %d\n", WSAGetLastError());
        closesocket(connectSocket);
        WSACleanup();
        return 6;
    }

    // cleanup
    closesocket(connectSocket);
    WSACleanup();

    return 0;
} 