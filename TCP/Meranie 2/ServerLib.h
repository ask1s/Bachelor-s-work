#include <stdio.h>
#include <WinSock2.h>
#include <memory.h>
#include <string>
#include <chrono>
#include <ctime>   
#include <iostream>

class ServerLib {
public:
    SOCKET  createListenSocket(int port);                // create server
    SOCKET  acceptClient(SOCKET listenSocket);           // wait for users
    int     receiveBuffSize(SOCKET clientSocket, int s); // receive new buffer size
};
