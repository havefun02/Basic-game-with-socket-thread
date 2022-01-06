#include"Gamesv.h"
unsigned int ServerGame::client_id;
void run(map<unsigned int, pair<SOCKET, BattleShip>> s, int id1, int id2);
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
		cout << "update()"<< client_id << endl;
		receive(client_id-1);
	}
}

void ServerGame::receive(int idnet)
{
	string idc1;
	bool checkturn = 0;
	cout << "receive: " << idnet << endl;
	while (1)
	{
		char buf[1024];
		bool check = false;
		ZeroMemory(buf, 1024);
		SOCKET curclient = (SOCKET)network->sessions[idnet].first;
		int brecv = recv(curclient, buf, 1024, 0);
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
			cout << "Wait for client!" << endl;
			cout << "Message from client: " << idnet <<" : " << signal << endl;
			//check 
			if (signal == "Find")
			{
				string s;
				if (handler.Checkuser(network->database, content, s))
				{
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.Checkuser(network->database, content, s))
				{
					char buffer[] = { "No" };
					send(curclient, buffer, (int)strlen(buffer), 0);
				}
			}
			else if (signal == "Login")
			{
				//confirm
				int ttmp = content.find_first_of('.');
				string ind1 = content.substr(ttmp + 1);
				content = content.substr(0, ttmp);
				string s;
				if (handler.ListenLogin(network->database, content, s, ind1))
				{
					char buffer[] = { "Yes" };
					send(curclient, buffer, (int)strlen(buffer), 0);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.ListenLogin(network->database, content, s, ind1))
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
				string tmp = handler.DatabaseToString(network->database,idnet);
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "ConnectClient")
			{
				PlayerAccount t;
				if (handler.Finduserbyid(network->database, content, t))
				{
					//send to another client

					string tmp = "Join+" + to_string(idnet);

					send(network->sessions[stoi(content)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				}
			}
			else if (signal == "Yes")
			{
				idc1 = content;
				tmp = "Yes";
				send(network->sessions[stoi(idc1)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "No") {
				tmp = "No";
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			/*	else if (signal == "File1:")
				{
					BattleShip mapClient;
					if (mapClient.update(content)) {
						mapClient.FillShip();
					}
					network->sessions[stoi(idc1)].second = mapClient;
					string tmp = "StartGame";
					send(network->sessions[stoi(idc1)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				}*/
			else if (signal == "File")
			{
				BattleShip mapClient;
				if (mapClient.update(content)) {
					mapClient.FillShip();
				}
				network->sessions[idnet].second = mapClient;
				string tmp = "StartGame";
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "Start")
			{
				char buf[1024];
				string tmp = "Your turn!";
				send(network->sessions[idnet].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "Start1")
			{
				char buf[1024];
				string tmp = "Wait!";
				send(network->sessions[idnet].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "atk")
			{

				tuple<bool, bool, string> result;
				int x, y;
				bool IsHit, IsFinish;
				string message;
				string attackSignal = content;
				x = BattleShip::convertToX(attackSignal);
				y = BattleShip::convertToY(attackSignal);
				result = network->sessions[stoi(idc1)].second.AttackShip(x, y);
				tie(IsHit, IsFinish, message) = result;
				string resultMatrix1 = network->sessions[stoi(idc1)].second.convertMap();
				string resultMatrix2 = network->sessions[idnet].second.convertMap();
				// Ex: resultMatrix1 = "010000100100...."
				send(network->sessions[stoi(idc1)].first, resultMatrix1.c_str(), (int)strlen(resultMatrix1.c_str()), 0);
				send(network->sessions[idnet].first, resultMatrix2.c_str(), (int)strlen(resultMatrix2.c_str()), 0);
				if (IsHit) {				// true mean the current Client who hit can continue playing
					string mes = "Your turn!";
					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = "Wait!";
					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
				}
				else						// false mean the current Client who miss stop playing and
				{
					string mes = "Your turn!";
					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = "Wait!";
					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
				}


				if (IsFinish) {				//Is the game done yet?
					string mes = "Win";
					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = "Lose";
					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
				}
			}
			else if (signal == "Connect to sv")
			{
				string t1 = to_string(client_id - 1);
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
