#include <stdio.h>
#include <WinSock2.h>
#include <memory.h>
#include <string>
#include <chrono>
#include <ctime>   
#include <iostream>

class ServerLib {
public:
    SOCKET  createListenSocket(int port);                   // creating server (with port 2222)
    SOCKET  acceptClient(SOCKET listenSocket);              // waiting and accepting users
    int     receiveBuffSize(SOCKET clientSocket, int s);    // receiving new buffer size of packages
};
	