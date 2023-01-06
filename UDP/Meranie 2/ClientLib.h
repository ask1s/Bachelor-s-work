#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib,"Ws2_32.lib")

#include <chrono>
#include <ctime>   
#include <stdio.h>
#include <WinSock2.h>
#include <memory.h>
#include <string>
#include <iostream>

class ClientLib {
private:

public:
	SOCKET createClientSocket(const char* address, int port); // create client socket (with address 127.0.0.1 and port 2222)
	void   sendBuffSize(SOCKET clientSocket, int size, int old, sockaddr_in serverAddr, int serverAddrSize); // send new buffer size to server
	void   show_info(float min, float max, float avg);           // show necessary output
};
