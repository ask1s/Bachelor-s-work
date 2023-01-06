#include "ServerLib.h"
#pragma warning(disable:4996) 

using namespace std;

int main()
{
    ServerLib slib; // class from my own library
    int port = 2222; // stable (faster to test)

    SOCKET listenSocket = slib.createListenSocket(port); // creating server
    if (listenSocket == INVALID_SOCKET)
    {
        return -1;
    }

    SOCKET clientSocket = slib.acceptClient(listenSocket); // accepting users
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    printf("Client connected\n");
    closesocket(listenSocket);  // i need just 1 user, so after one connects, stop listening

    int size = 0; // buffer size
    int res; // message from client

    size = slib.receiveBuffSize(clientSocket, 1024); // waiting for new buffer size from client
    char* rbuf = new char[size]; // creating receive buffer for that size
    char* sbuf = new char[size]; // creating send buffer for that size
    memset(rbuf, 0, size); // filling it with zerous
    memset(sbuf, 0, size);
    do // endless cycle
    {
        res = 0;
        int result = recv(clientSocket, rbuf, size, 0); // receiving package from user
        sscanf(rbuf, "%i", &res); // translating and writing it to res
        if (res == 666) // if the message is "666", then restart
        {
            int unno = size; // present size
            size = slib.receiveBuffSize(clientSocket, unno); // waiting for new size
            free(rbuf); // free buffers memo
            free(sbuf);
            rbuf = new char[size]; // create new buffers with new size
            sbuf = new char[size];
            memset(rbuf, 0, size); // fill them
            memset(sbuf, 0, size);// fill it

            printf("Restarted\n"); // to confirm that it's working
        }

        else if (result == SOCKET_ERROR) // if there is troubles with receiving information, then close application
        {
            printf("Receive failed\n");
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }
        if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR)
        {
            printf("Send failed\n");
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }

    } while (true); // endless cycle

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}