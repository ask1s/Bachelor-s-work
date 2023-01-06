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
	SOCKET createClientSocket(const char* address, int port);    // creating client socket 
	void   sendBuffSize(SOCKET clientSocket, int size, int old); // send new buffer size to the server
	void   show_info(float min, float max, float avg);           // show necessary output
};
