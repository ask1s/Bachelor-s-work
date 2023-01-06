#include "ClientLib.h"
#pragma warning(disable:4996) 


using namespace std;


int main()
{
    char address[16] = "127.0.0.1"; // stable address and port
    int port = 2222;                // so it's faster to test it
    ClientLib client;
    SOCKET clientSocket = client.createClientSocket(address, port); // connection to server with client socket

    if (clientSocket == INVALID_SOCKET)
    {
        return -1;
    }

    sockaddr_in serverAddr;          // variable for server address 
    serverAddr.sin_family = AF_INET; // AF_INET - IPv4, AF_INET6 - IPv6 
    serverAddr.sin_addr.s_addr = inet_addr(address); // server address
    serverAddr.sin_port = htons(port); // server port
    //this is here, because i will use it in the future to send/receive data

    int serverAddrSize = sizeof(serverAddr);

    printf("Connected to server\n");

    int size;                                                       // define necessary values
    int old = 1024;
    int sec = 60;
    char* sbuf;
    char* rbuf;
    int packs = 0;
    char* sendSize = new char[old];

    printf("Enter the buffer size: ");

    while (scanf("%i", &size) != NULL) // infinite cycle, but it waits for an input
    {
        sendSize = new char[old];
        sprintf(sendSize, "%i", size);
        if (sendto(clientSocket, sendSize, strlen(sendSize)+1, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR)
        {
            printf("Send failed\n");
            closesocket(clientSocket);
            WSACleanup();
        } // send new buffer size to the server
        sbuf = new char[size]; // create new buffer with the size of "size" 
        rbuf = new char[size]; // create new buffer with the size of "size" 
        memset(rbuf, 0, size);
        memset(sbuf, 0, size); // fill it

        auto start = chrono::system_clock::now(); // start the timer
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        double delta = elapsed_seconds.count(); // making delta != NULL to start the cycle

        while (delta <= sec) // sec = casove okno, delta = now - start time (kolko casu trva)
        {
            if (sendto(clientSocket, sbuf, strlen(sbuf)+1, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR) // send this buffer to server
            {
                printf("Send failed\n");
                closesocket(clientSocket);
                WSACleanup();
                return -1;  // if send failed, stop the script
            }
            if (recvfrom(clientSocket, rbuf, size, 0, (sockaddr*)&serverAddr, &serverAddrSize) <= 0) // wait for an answer
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
        if (sendto(clientSocket, sbuf, strlen(sbuf) + 1, 0, (sockaddr*)&serverAddr, serverAddrSize) == SOCKET_ERROR) // send server command to restart
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
        free(sendSize);
        // cycle restarts
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}