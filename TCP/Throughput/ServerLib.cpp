#include "ServerLib.h"

#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

SOCKET ServerLib::createListenSocket(int port)                                              
{ 
    WSADATA wsaData;                                                                        // creating Winsock API
    WSAStartup(MAKEWORD(2, 2), &wsaData);                                                   // initialize Winsock and selecting version 2.2
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);                        // creating listen socket

    if (listenSocket == INVALID_SOCKET){
        return listenSocket;
    }

    sockaddr_in socketAddr;                                                                 // creating address variable
    socketAddr.sin_family = AF_INET;                                                        // address type (TCP/IP)
    socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");                                    // server address (local network)
    socketAddr.sin_port = htons(port);                                                      // server port

    if (bind(listenSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR){    // bind address variable to unconnected socket
        printf("Error binding socket\n");                                                   // if unsuccessful, close the program
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR){                                   // start listening this socket
        printf("Error listening on socket\n");                                              // if unsuccessful, close the program
        closesocket(listenSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    return listenSocket;

}

SOCKET ServerLib::acceptClient(SOCKET listenSocket)                                         // waiting and accepting users
{
    SOCKET clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET){
        printf("Error accepting client\n");
        return INVALID_SOCKET;
    }
    return clientSocket;
}

int ServerLib::receiveBuffSize(SOCKET clientSocket, int s)  // on input: clientSocket, old buffer size 
{
    int size = 0; // create variable for new buffer size
    char* r = new char[s]; // create buffer to reseive information
    memset(r, 0, s); 

    if (recv(clientSocket, r, s, 0) == SOCKET_ERROR) // receive data/package 
    {                                                //uspesne?
        printf("Receive failed\n");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    sscanf(r, "%i", &size); // translate received information and write it to the new size variable
    delete(r); // free used memory
    return size; // return this information to the main file
}

