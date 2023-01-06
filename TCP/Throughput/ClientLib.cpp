#include "ClientLib.h"

SOCKET ClientLib::createClientSocket(const char* address, int port) // create client socket (with address 127.0.0.1 and port 2222)
{
    WSADATA wsaData;                                                  // creating Winsock API
    WSAStartup(MAKEWORD(2, 2), &wsaData);                             // initialize Winsock and selecting version 2.2

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);  // creating client socket

    if (clientSocket == INVALID_SOCKET)
    {
        return clientSocket;
    }

    sockaddr_in socketAddr;                                           // creating sever address variable
    socketAddr.sin_family = AF_INET;                                  // address type (TCP/IP)
    socketAddr.sin_addr.s_addr = inet_addr(address);                  // server address 
    socketAddr.sin_port = htons(port);                                // server port

    if (connect(clientSocket, (sockaddr*)&socketAddr, sizeof(socketAddr)) == SOCKET_ERROR) //connect to server
    {
        printf("Error connecting to server\n");
        closesocket(clientSocket);
        WSACleanup();
        return INVALID_SOCKET;
    }
    return clientSocket;
}
void ClientLib::sendBuffSize(SOCKET clientSocket, int size, int old) // on input: clientSocket, new size of buffers, old size of buffers
{
    char* sendSize = new char[old]; // create new buffer with old size (for right communication)
    sprintf(sendSize, "%i", size); // fill it with information about new size 
    if (send(clientSocket, sendSize, old, 0) == SOCKET_ERROR) // send it to server 
    {
        printf("Send failed\n");
        closesocket(clientSocket);
        WSACleanup();
    }
}
void ClientLib::show_info(int pack, int size, double time) // on input: numbers of succeed packages, size of buffers, casove okno
{
    float bytes = ((pack * size))/1000000 / time; // calculate priestupnost v MB/s (using formula: (packages * size))/1000000 / time(uplynuty cas) 
    printf("%2.2f", bytes); // show it to the console
    printf(" MB per second");
}