#include"Gamesv.h"
unsigned int ServerGame::client_id;
ServerGame::ServerGame()
{
    client_id = 0;
    network = new ServerNetwork();
	network->database = handler.getdata();
}
void ServerGame::update()
{
	if (network->acceptNewClient(client_id))
	{
		client_id++;
		receive();
	}
}

void ServerGame::receive()
{
	while (1)
	{
		char buf[100];
		bool check = false;
		ZeroMemory(buf, 100);
		SOCKET curclient = (SOCKET)network->ClientSocket;
		int brecv = recv(curclient, buf, 100, 0);
		if (brecv == SOCKET_ERROR)
		{
			cout << "error occur " << WSAGetLastError() << endl;
			ExitThread(0);
		}
		if (brecv != 0)
		{
			string tmp = string(buf, 0, brecv);
			int indexsig = tmp.find_first_of(":");
			string signal = tmp.substr(0, indexsig);
			string content = tmp.substr(indexsig + 1);
			cout << "Message from client: " << " " << signal << endl;
			//check 
			if (signal == "Find")
			{
				string s;
				if (handler.Checkuser(network->database, content,s))
				{
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.Checkuser(network->database, content,s))
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "Login")
			{
				//confirm
				int ttmp = content.find_first_of('.');
				string ind1 = content.substr(ttmp+1);
				content = content.substr(0, ttmp);
				string s;
				if (handler.ListenLogin(network->database, content, s,ind1))
				{
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.ListenLogin(network->database, content, s,ind1))
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "Register")
			{
				if (handler.ListenRegister(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
				else
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "Changepass")
			{
				if (handler.ListenChangePass(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
				else
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "Changeinfo")
			{
				if (handler.ListenChangeinfo(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
				else
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "ListOnline")
			{
				//return the list player online
				string tmp = handler.DatabaseToString(network->database);
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal=="ConnectClient")
			{
				while (1)
				{
					//
				}
				//id of player choosen -> socket
				//invite another to joint
				//return true if accept, false if refuse
			}
			else if (signal == "Attack")
			{
				

			}
			else if (signal == "Connect to sv")
			{
				string t1 = to_string(client_id-1);
				string buffer = "Login:" + t1;
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);

				//
			}
			else if (signal == "END")
			{
				closesocket(curclient);
				ExitThread(0);
				return;
			}
			else if (signal == "SHUTDOWN")
			{
				if (handler.Closeconnect(network->database, content))
				{
					closesocket(curclient);
					ExitThread(0);
				}
			}
			else if (signal == "CANCEL")
			{
				char buffer[] = { "UiClient" };
				send(curclient, buffer, (int)strlen(buffer), 0);
				closesocket(curclient);
				ExitThread(0);
			}

		}
	}
}