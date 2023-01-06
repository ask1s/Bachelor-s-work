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
    //int     receiveBuffSize(SOCKET clientSocket, int s); // receive new buffer size
};
