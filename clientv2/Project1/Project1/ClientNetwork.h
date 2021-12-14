#pragma once
#include <iostream>
#include <string>
#include<fstream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#pragma comment (lib, "Ws2_32.lib")
using namespace std;
class ClientNetwork
{
public:
	int iResult;
	ClientNetwork();
	void Connect();
	int Receive(char*);
	SOCKET ClientSocket;
};