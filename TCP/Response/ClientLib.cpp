#include "ClientLib.h"

SOCKET ClientLib::createClientSocket(const char* address, int port) // creating client socket 
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET)
    {
        return clientSocket;
    }

    sockaddr_in socketAddr;
    socketAddr.sin_family = AF_INET;
    socketAddr.sin_addr.s_addr = inet_addr(address);
    socketAddr.sin_port = htons(port);

    if (connect(clientSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR)
    {
        printf("Error connecting to server\n");
        closesocket(clientSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    return clientSocket;
}
void ClientLib::sendBuffSize(SOCKET clientSocket, int size, int old) // send new buffer size to the server
{
    char* sendSize = new char[old];
    sprintf(sendSize, "%i", size);
    if (send(clientSocket, sendSize, old, 0) == SOCKET_ERROR)
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