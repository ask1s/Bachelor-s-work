#include "ClientLib.h"
#pragma warning(disable:4996) 


using namespace std;


int main()
{
    char address[16] = "127.0.0.1"; // stable address and port
    int port = 2222;                // less time to waste
    ClientLib client;
    SOCKET clientSocket = client.createClientSocket(address, port); // create client socket and connect to server

    if (clientSocket == INVALID_SOCKET)
    {
        return -1;
    }

    printf("Connected to server\n"); // if it's done, print connected. else - stop the program

    int size;          // size - size of buffer
    int old = 1024;    // old - previous size of buffer
    char* sbuf;        // sbuf - send buffer
    char* rbuf;        // rbuf - receive buffer

    int times = 1000000; // times - send the packages with chosen size this many times 

    float min = 1000;  // min - minimal delay
    float max = -1000; // max - maximum delay
    float avg = 0;     // avg - average delay
    double ping = 0;   // ping - time delay(time spent between sending and receiving)

    printf("Enter the buffer size: ");

    while (scanf("%i", &size) != NULL) // infinite cycle
    {
        client.sendBuffSize(clientSocket, size, old); // send new buffer size to server, (size - new size of the buffer, old - previous size of the buffer (1024 by default))

        sbuf = new char[size]; // create send and receive buffers
        rbuf = new char[size];
        memset(sbuf, 0, size); // fill them
        memset(rbuf, 0, size);

        for (int i = 0; i < times; i++) // a cycle, which will repeat "times" times
        {
            auto start = chrono::system_clock::now(); // starting timer
            if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR) // send package to server
            {
                printf("Send failed\n");
                closesocket(clientSocket);
                WSACleanup();
                return -1;
            }
            if (recv(clientSocket, rbuf, size, 0) == SOCKET_ERROR) // wait for an answer
            {
                printf("Error receiving data\n"); // if the answer was received, then proceed. else - stop the program
                closesocket(clientSocket);
                WSACleanup();
                return -1;
            }

            auto end = chrono::system_clock::now(); // after answer received, end the timer
            chrono::duration<double> elapsed_seconds = end - start; // calculate how much time was spent
            double ping = elapsed_seconds.count();  // write it to "ping"


            if (max == -1000 && min == 1000) // if it's first attempt, then "avg = delay"
            {
                avg = ping;
            }
            else // if not, then just a general formula 
            {
                avg = (avg + ping) / 2;
            }
            if (ping > max) // check if it is new maximum 
            {
                max = ping;
            }
            if (ping < min) // or minimum
            {
                min = ping;
            }
        }

        sprintf(sbuf, "%i", 666); // send server package, which makes it to restart
        if (send(clientSocket, sbuf, size, 0) == SOCKET_ERROR)
        {
            printf("Send failed\n");
            closesocket(clientSocket);
            WSACleanup();
            return -1;
        }
        client.show_info(min,max,avg); // show outputs
        printf("\nEnter the buffer size: "); // restarting client
        old = size;
        avg = 0;
        ping = 0;
        min = 1000;
        max = -1000;
    }
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}