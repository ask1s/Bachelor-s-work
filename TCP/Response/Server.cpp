#include "ServerLib.h"
#pragma warning(disable:4996) 

using namespace std;

int main()
{
    ServerLib slib;
    int port = 2222; // stable port 2222

    SOCKET listenSocket = slib.createListenSocket(port); // create server
    if (listenSocket == INVALID_SOCKET)
    {
        return -1;
    }

    SOCKET clientSocket = slib.acceptClient(listenSocket); // wait for users
    if (clientSocket == INVALID_SOCKET)
    {
        closesocket(listenSocket);
        WSACleanup();
        return -1;
    }

    printf("Client connected\n"); // if there is one connected - stop waiting
    closesocket(listenSocket);

    int size = 0; // size of buffer
    int res;      // translated package from user

    size = slib.receiveBuffSize(clientSocket, 1024); // receiving information about new buffer size

    char* rbuf = new char[size]; // make new receive and send buffers with new size
    char* sbuf = new char[size];
    memset(rbuf, 0, size);       // fill them
    memset(sbuf, 0, size);

    do // endless cycle
    {
        res = 0;
        int result = recv(clientSocket, rbuf, size, 0); // receive package from user
        sscanf(rbuf, "%i", &res);
        if (res == 666) // translate it's information. if it's "666", then restart server
        {
            int unno = size;
            size = slib.receiveBuffSize(clientSocket, unno); // waiting for new package size
            free(rbuf); // free buffers memo
            free(sbuf);
            rbuf = new char[size]; // create new buffers with new size
            sbuf = new char[size];
            memset(rbuf, 0, size); // fill them
            memset(sbuf, 0, size);

            printf("Restarted\n");
        }

        else if (result == SOCKET_ERROR) // if it wasn't "666", but it was NULL, then destroy application
        {
            printf("Receive failed\n");
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }
        else // if everything is ok, just send an answer back
        {
            if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR)
            {
                printf("Send failed\n");
                closesocket(clientSocket);
                WSACleanup();
                return -1;
            }
        }
    } while (true); // infinite cycle

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}