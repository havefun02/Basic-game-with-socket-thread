#include"General.h"
#include"Servernetwork.h"
ServerNetwork::ServerNetwork()
{
	WSADATA wsdata;
	WORD ver = MAKEWORD(2, 2);
	int res = WSAStartup(ver, &wsdata);
	//wsastartup allows an application or DLL to specify the version of Windows Sockets 
	//required and to retrieve details of the specific Windows Sockets implementation
	if (res != 0)
	{
		cout << "error occur " << WSAGetLastError() << endl;
		exit(0);
	}
	//create socket
	ListenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "error occur " << WSAGetLastError() << endl;
		WSACleanup();
		exit(0);
	}
	// bind the ip addr and port to sk
	addrsv.sin_family = AF_INET;
	addrsv.sin_port = htons(8080);
	addrsv.sin_addr.S_un.S_addr = INADDR_ANY;
	int r = bind(ListenSocket, (sockaddr*)&addrsv, sizeof(addrsv)); //associates a local address with a socket.
	if (r == SOCKET_ERROR)
	{
		cout << "error occur " << WSAGetLastError() << endl;
		closesocket(ListenSocket);
		WSACleanup();
		exit(0);
	}
	//listen

	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)//allow more than one connection request at a time
	{
		cout << "closesocket function failed with error" << WSAGetLastError() << endl;
		WSACleanup();
		exit(0);
	}
	getInfosv(addrsv, sizeof(addrsv));
}

bool ServerNetwork::acceptNewClient(unsigned int& id)
{
	int size = sizeof(addrcl);
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket != INVALID_SOCKET) 
    {
		cout << "A client connected" << endl;
		cout << "Id: " << id << endl;
		getInfoClient(addrcl,size);
        //disable nagle on the client's socket
        char value = 1;
        setsockopt( ClientSocket, IPPROTO_TCP, TCP_NODELAY, &value, sizeof( value ) );

        // insert new client into session id table
		BattleShip t;
		sessions.insert({ id, { ClientSocket, t } });
        return true;
    }
    return false;
}
void ServerNetwork::getInfosv(SOCKADDR_IN sv, int size)
{
		char host[NI_MAXHOST];
		char service[NI_MAXSERV];
		ZeroMemory(host, NI_MAXHOST);
		ZeroMemory(host, NI_MAXSERV);
		cout << "------------------------------------------------------------" << endl;
		if (getnameinfo((sockaddr*)&sv, sizeof(sv), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
		{
			cout << "hostname = " << host << endl;
		}
		else
		{
			inet_ntop(AF_INET, &sv.sin_addr, host, NI_MAXHOST);
			cout << "hostname sever = " << host << endl;
		}
		struct sockaddr_in* ip_client = (struct sockaddr_in*)&sv;
		char ipclient[INET_ADDRSTRLEN];
		//convert Internet network address into a string 
		inet_ntop(AF_INET, &ip_client->sin_addr, ipclient, INET_ADDRSTRLEN);
		cout << "ip SV: " << ipclient << endl;
		cout << "port : " << ntohs(sv.sin_port) << endl;
		cout << "------------------------------------------------------------" << endl;
}
void ServerNetwork::getInfoClient(SOCKADDR_IN cl, int size)
{
	char host[NI_MAXHOST];
	char service[NI_MAXSERV];
	ZeroMemory(host, NI_MAXHOST);
	ZeroMemory(host, NI_MAXSERV);
	cout << "------------------------------------------------------------" << endl;
	if (getnameinfo((sockaddr*)&cl, sizeof(cl), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
	{
		cout << "hostname = " << host << endl;
	}
	else
	{
		inet_ntop(AF_INET, &cl.sin_addr, host, NI_MAXHOST);
		cout << "hostname sever = " << host << endl;
	}
	struct sockaddr_in* ip_client = (struct sockaddr_in*)&cl;
	char ipclient[INET_ADDRSTRLEN];
	//convert Internet network address into a string 
	inet_ntop(AF_INET, &ip_client->sin_addr, ipclient, INET_ADDRSTRLEN);
	cout << "From ip: " << ipclient << endl;
	cout << "port : " << ntohs(cl.sin_port) << endl;
	cout << "------------------------------------------------------------" << endl;
}