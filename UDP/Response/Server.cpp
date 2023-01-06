#include "ServerLib.h"
#pragma warning(disable:4996) 

using namespace std;

int main()
{
    ServerLib slib;
    int port = 2222; // stable port 2222

    SOCKET serverSocket = slib.createListenSocket(port);
    if (serverSocket == INVALID_SOCKET)
    {
        return -1;
    }

    printf("UDP socket created\n");

    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr); // if there is one connected - stop waiting

    int size = 0; // size of buffer
    int res;      // translated package from user

    char* r = new char[1024];
    memset(r, 0, 1024);
    int result = recvfrom(serverSocket, r, 1024, 0, (sockaddr*)&clientAddr, &clientAddrSize);
    if (result == SOCKET_ERROR)
    {
        printf("Receive failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    sscanf(r, "%i", &size);
    delete(r); // receiving information about new buffer size

    char* rbuf = new char[size]; // make new receive and send buffers with new size
    char* sbuf = new char[size];
    memset(rbuf, 0, size);       // fill them
    memset(sbuf, 0, size);

    do // endless cycle
    {
        res = 0;
        int result = recvfrom(serverSocket, rbuf, size, 0, (sockaddr*)&clientAddr, &clientAddrSize); // receive package from user
        sscanf(rbuf, "%i", &res);
        if (res == 666) // translate it's information. if it's "666", then restart server
        {
            int unno = size;
            char* r = new char[unno];
            memset(r, 0, unno);
            int result = recvfrom(serverSocket, r, unno, 0, (sockaddr*)&clientAddr, &clientAddrSize);
            if (result == SOCKET_ERROR)
            {
                printf("Receive failed\n");
                closesocket(serverSocket);
                WSACleanup();
                return -1;
            }
            sscanf(r, "%i", &size);
            delete(r);
            free(rbuf);
            free(sbuf);// deleting allocated memo
            rbuf = new char[size];
            sbuf = new char[size]; // create new buffer with new size
            memset(sbuf, 0, size);// create new buffer with new size
            memset(rbuf, 0, size); // fill it

            printf("Restarted\n");
        }

        else if (result == SOCKET_ERROR) // if it wasn't "666", but it was NULL, then destroy application
        {
            printf("Receive failed\n");
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        else // if everything is ok, just send an answer back
        {
            if (sendto(serverSocket, sbuf, strlen(sbuf) + 1, 0, (sockaddr*)&clientAddr, clientAddrSize) == SOCKET_ERROR)
            {
                printf("Send failed\n");
                closesocket(serverSocket);
                WSACleanup();
                return -1;
            }
        }
    } while (true); // infinite cycle

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}