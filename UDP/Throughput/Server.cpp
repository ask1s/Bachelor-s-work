#include "ServerLib.h"
#pragma warning(disable:4996) 

using namespace std;

int main()
{
    ServerLib slib;  // my own class
    int port = 2222; // stable (faster to test)

    SOCKET serverSocket = slib.createListenSocket(port); // creating listen socket
    if (serverSocket == INVALID_SOCKET)
    {
        return -1;
    }

    printf("UDP socket created\n");

    sockaddr_in clientAddr; // variable for client address
    int clientAddrSize = sizeof(clientAddr);

    int size = 0; // buffer size
    int res; // message from client

    char* r = new char[1024]; // just for first communication
    char* rbuf; // buffers
    char* sbuf;
    memset(r, 0, 1024);
    int result = recvfrom(serverSocket, r, 1024, 0, (sockaddr*)&clientAddr, &clientAddrSize); // receive buff size
    if (result == SOCKET_ERROR)
    {
        printf("Receive failed\n");
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }
    sscanf(r, "%i", &size); // write it to size
    delete(r); // delete(free) allocated memory


    rbuf = new char[size]; // create buffers for size of "size"
    sbuf = new char[size];
    memset(sbuf, 0, size); // fill them 
    memset(rbuf, 0, size);

    do // endless cycle
    {
        res = 0;
        int result = recvfrom(serverSocket, rbuf, size, 0, (sockaddr*)&clientAddr, &clientAddrSize); // receiving package from user
        sscanf(rbuf, "%i", &res); // translating and writing it to res
        if (res == 666) // if the message is "666", then restart
        {
            int unno = size; // present size
            char* r = new char[unno]; // create buffer for old communication size
            memset(r, 0, unno);
            int result = recvfrom(serverSocket, r, unno, 0, (sockaddr*)&clientAddr, &clientAddrSize); // receive new buffer size
            if (result == SOCKET_ERROR)
            {
                printf("Receive failed\n");
                closesocket(serverSocket);
                WSACleanup();
                return -1;
            }
            sscanf(r, "%i", &size); // write new size to "size"
            delete(r); // free memo
            free(rbuf);
            free(sbuf);// deleting allocated memo
            rbuf = new char[size];
            sbuf = new char[size]; // create new buffer with new size
            memset(sbuf, 0, size);// create new buffer with new size
            memset(rbuf, 0, size); // fill it

            printf("Restarted\n"); // to confirm that it's working
        }

        else if (result == SOCKET_ERROR) // if there is troubles with receiving information, then close application
        {
            printf("Receive failed\n");
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
        if (sendto(serverSocket, sbuf, strlen(sbuf) + 1, 0, (sockaddr*)&clientAddr, clientAddrSize) == SOCKET_ERROR) // send answer
        {
            printf("Send failed\n");
            closesocket(serverSocket);
            WSACleanup();
            return -1;
        }
    } while (true); // endless cycle

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}