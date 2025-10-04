#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>
#include <ws2tcpip.h>

#define PORT "8080"
#define BUF_SIZE 512

int init_winsock();
void cleanup_winsock();
SOCKET create_listen_socket();
SOCKET accept_client(SOCKET listenSocket);
void handle_client(SOCKET clientSocket);

#endif