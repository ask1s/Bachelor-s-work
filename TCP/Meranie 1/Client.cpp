#include "ClientLib.h"
#pragma warning(disable:4996) 


using namespace std;


int main()
{
    char address[16] = "127.0.0.1"; // local network
    int port = 2222;                // just easier and faster to test it
    ClientLib client;               // my own class, which contains a few functions
    SOCKET clientSocket = client.createClientSocket(address, port); // connection to server with client socket

    if (clientSocket == INVALID_SOCKET)
    {
        return -1;
    }

    printf("Connected to server\n");

    // define necessary values
    int size; // size of send/receive packages
    int old = 1024; // old size of packages (useful for restarting cycle)
    int sec = 60; // time window
    char* sbuf; // send buffer
    char* rbuf; // receive buffer
    int packs = 0; // ammount of sent packages

    printf("Enter the buffer size: ");
    while (scanf("%i", &size) != NULL) // infinite cycle, but it waits for an input
    {
        client.sendBuffSize(clientSocket, size, old); // send new buffer size to the server
        sbuf = new char[size]; // create new send buffer with the size of "size" 
        rbuf = new char[size]; // create new receive buffer with the size of "size" 
        memset(rbuf, 0, size);
        memset(sbuf, 0, size); // fill them

        auto start = chrono::system_clock::now(); // start the timer
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start; // elapsed time
        double delta = elapsed_seconds.count(); // making delta != NULL to start the cycle

        while (delta <= sec) // sec = casove okno, delta = now - start  is time (kolko casu trva)
        {
            if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR) // send this buffer to server
            {
                printf("Send failed\n");
                closesocket(clientSocket);
                WSACleanup();
                return -1;  // if send failed, stop the script
            }
            if (recv(clientSocket, rbuf, size, 0) == SOCKET_ERROR) // wait for an answer
            {
                printf("Error receiving data\n"); // if the answer was received, then proceed. else - stop the program
                closesocket(clientSocket);
                WSACleanup();
                return -1;
            }

            auto end = chrono::system_clock::now(); // check now time
            chrono::duration<double> elapsed_seconds = end - start;
            delta = elapsed_seconds.count(); // make a new delta
            packs++; // succeeded sending a package. All sended packages + 1
        }

        sprintf(sbuf, "%i", 666); // server is always checking received buffer if it's "666" then he starts waiting for new buffer size
        if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR) // send server command to restart
        {
            printf("Send failed\n");
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }
        client.show_info(packs, size, sec); // show all necessary outputs
        printf("\nEnter the buffer size: "); // print this message for a user
        old = size; // server is still working with packages of, for example, 1024. To communicate with him, client needs to now this size 
        packs = 0;
        // cycle restarts
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}