#include "ServerLib.h"

#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

SOCKET ServerLib::createListenSocket(int port)  // create server
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (serverSocket == INVALID_SOCKET)
    {
        return serverSocket;
    }

    sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    socketAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR)
    {
        printf("Error binding socket\n");
        closesocket(serverSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return serverSocket;

}


/*int ServerLib::receiveBuffSize(SOCKET clientSocket, int s) // receive new buffer size
{
    int size = 0;
    char* r = new char[s];
    memset(r, 0, s);

    if (recv(clientSocket, r, s, 0) == SOCKET_ERROR)
    {
        printf("Receive failed\n");
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }
    sscanf(r, "%i", &size);
    delete(r);
    return size;
}*/

