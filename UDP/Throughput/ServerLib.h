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
    //int     receiveBuffSize(SOCKET serverSocket, int s, sockaddr_in clientAddr, int clientAddrSize);    // receiving new buffer size of packages
};
