#include "ServerLib.h"

#pragma warning(disable:4996) 
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

SOCKET ServerLib::createListenSocket(int port) 
{
    WSADATA wsaData;  // creating Winsock API
    WSAStartup(MAKEWORD(2, 2), &wsaData); // initialize Winsock and selecting version 2.2

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // creating server socket

    if (serverSocket == INVALID_SOCKET)
    {
        return serverSocket;
    }

    sockaddr_in socketAddr;  // creating sever address variable
    socketAddr.sin_family = AF_INET; // address type (UDP/IP same as TCP/IP)
    socketAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // server address 
    socketAddr.sin_port = htons(port); // server port

    if (bind(serverSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR) // bind address variable to unconnected socket
    {
        printf("Error binding socket\n");
        closesocket(serverSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }

    return serverSocket;

}


/*int ServerLib::receiveBuffSize(SOCKET serverSocket, int s, sockaddr_in clientAddr, int clientAddrSize)  // receiving new buffer size of packages
{
    char* r = new char[s];
    memset(r, 0, s);
    int size;
    int result = recvfrom(serverSocket, r, s, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (result == SOCKET_ERROR)
    {
        printf("Receive failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    sscanf(r, "%i", &size);
    delete(r);
    return size;
}*/

