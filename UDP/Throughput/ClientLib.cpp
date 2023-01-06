#include "ClientLib.h"

SOCKET ClientLib::createClientSocket(const char* address, int port) // create client socket (with address 127.0.0.1 and port 2222)
{
    WSADATA wsaData; // creating Winsock API
    WSAStartup(MAKEWORD(2, 2), &wsaData); // initialize Winsock and selecting version 2.2

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); // creating client socket

    if (clientSocket == INVALID_SOCKET)
    {
        return clientSocket;
    }

    return clientSocket;
}
void ClientLib::sendBuffSize(SOCKET clientSocket, int size, int old, sockaddr_in serverAddr, int serverAddrSize) // send new buffer size to server
{
    char* sendSize = new char[old];
    sprintf(sendSize, "%i", size);
    if (sendto(clientSocket, sendSize, old, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR)
    {
        printf("Send failed\n");
        closesocket(clientSocket);
        WSACleanup();
    }
}
void ClientLib::show_info(int pack, int size, double time) // display outputs
{
    float bytes = ((pack * size)/1000000) / time;
    printf("%2.2f", bytes);
    printf(" MB per second");
}