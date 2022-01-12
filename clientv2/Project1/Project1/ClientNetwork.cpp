#include"ClientNetwork.h"
#include"NetworkService.h"
void ClientNetwork::Connect()
{
	string ipAddress = "192.168.1.13";			// IP Address of the server
	int port = 8080;						// Listening port # on the server

	// Init
	WSADATA data;
	WORD ver = MAKEWORD(2, 2);
	int res = WSAStartup(ver, &data);
	if (res != 0)
	{
		cout << "Can't start Winsock, Err" << res << endl;
		return;
	}

	// Create socket
	ClientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ClientSocket == INVALID_SOCKET)
	{
		cerr << "Can't create socket, Err" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Tell the ip and port to connect to
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	//converts 16bit number in host byte order and 
	//return 16-bit number in network byte order used in TCP/IP networks(which is big - endian).
	hint.sin_port = htons(port);
	//convert string ipaddr to binary ipaddr which socket can implement
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server
	int connResult = connect(ClientSocket, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)//0 is success, -1 is fail
	{
		cout << "Can't connect to server, Err" << WSAGetLastError() << endl;
		closesocket(ClientSocket);
		WSACleanup();
		return;
	}
}

ClientNetwork::ClientNetwork()
{
	Connect();
}

int ClientNetwork::Receive(char* recvbuf)
{
	iResult = NetworkService::receiveMessage(ClientSocket, recvbuf, strlen(recvbuf));
	//catch the packet
	if (iResult == 0)
	{
		closesocket(ClientSocket);
		WSACleanup();
		exit(1);
	}

	return iResult;
}