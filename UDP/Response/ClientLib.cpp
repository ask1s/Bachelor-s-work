#include "ClientLib.h"

SOCKET ClientLib::createClientSocket(const char* address, int port) // creating client socket 
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if (clientSocket == INVALID_SOCKET)
    {
        return clientSocket;
    }

    return clientSocket;
}
void ClientLib::sendBuffSize(SOCKET clientSocket, int size, int old, sockaddr_in serverAddr, int serverAddrSize) // send new buffer size to the server
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
void ClientLib::show_info(float min, float max, float avg) // show necessary output
{
    printf("Minimum: ");
    printf("%f", min);
    printf("\n");
    printf("Maximum: ");
    printf("%f", max);
    printf("\n");
    printf("Average: ");
    printf("%f", avg);
}
