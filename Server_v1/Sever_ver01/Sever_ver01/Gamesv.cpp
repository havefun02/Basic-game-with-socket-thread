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
		receive(client_id-1);
	}
}

void ServerGame::receive(int idnet)
{
	string idc1;
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
				string tmp = handler.DatabaseToString(network->database);
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "ConnectClient")
			{
				PlayerAccount t;
				if (handler.Finduserbyid(network->database, content, t))
				{
					//send to another client
					string tmp = "Join+"+idnet;
					send(network->sessions[stoi(content)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				}
			}
			else if (signal == "Yes") {
				int brecv = recv(network->sessions[stoi(content)].first, buf, 100, 0);
				string tmp = string(buf, 0, brecv);
				idc1 = network->sessions[stoi(content)].first;
				tmp = "Yes";
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "No") {
				tmp = "No";
				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "File1:")
			{
				BattleShip mapClient;
				if (mapClient.update(content)) {
					mapClient.FillShip();
				}
				network->sessions[stoi(idc1)].second = mapClient;
				string tmp = "StartGame";
				send(network->sessions[stoi(idc1)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "File2:")
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
				run(network->sessions,idnet,stoi(idc1));
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

void run(map<unsigned int, pair<SOCKET, BattleShip>> s,int id1,int id2)
{
	tuple<bool, bool, string> result;
	bool IsHit, IsFinish;
	string message;
	stringstream builder;
	
	bool checkturn = 0;	//0 -> Client 1, 1-> Client 2
	// GAMEPLAY
	while (1)
	{
		//
		int brecv;
		char buf[100];
		string tmp = "Your turn!";
		if (checkturn == 0) {
			send(s[id1].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			string tmp1 = "Wait!";
			send(s[id2].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
		}
		else {
			send(s[id2].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			string tmp1 = "Wait!";
			send(s[id1].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
		}

		int x, y;

		if (!checkturn) {
			brecv = recv(s[id1].first, buf, 100, 0);
		}
		else {
			brecv = recv(s[id2].first, buf, 100, 0);
		}

		string attackSignal = string(buf, 0, brecv);
		//Ex : attackSignal = "atk:0506" 
		x = BattleShip::convertToX(attackSignal);
		y = BattleShip::convertToY(attackSignal);
		//update map 2

		if (!checkturn) {
			result = s[id2].second.AttackShip(x, y);
		}
		else {
			result = s[id1].second.AttackShip(x, y);
		}

		//
		tie(IsHit, IsFinish, message) = result;

		builder << "result:";

		if (IsHit) {				// true mean the current Client who hit can continue playing
			builder << 01;
		}
		else						// false mean the current Client who miss stop playing and
		{
			checkturn = !checkturn;// the other Client can play now
			builder << 00;
		}


		if (IsFinish) {				//Is the game done yet?
			builder << 01;
		}
		else builder << 00;

		//Ex : message = "Hit the ship 2x4\nthe ship is destroyed\nthere is no ships in map"
		builder << message;

		// Ex : SignalGameplay = "result:0100Hit the ship 2x4\nthe ship i......"
		string signalGameplay = builder.str();


		send(s[id2].first, signalGameplay.c_str(), (int)strlen(signalGameplay.c_str()), 0);
		send(s[id1].first, signalGameplay.c_str(), (int)strlen(signalGameplay.c_str()), 0);


		// send map information to 2 client
		string resultMatrix1 = s[id1].second.convertMap();
		string resultMatrix2 = s[id2].second.convertMap();

		// Ex: resultMatrix1 = "010000100100...."
		send(s[id1].first, resultMatrix1.c_str(), (int)strlen(resultMatrix1.c_str()), 0);
		send(s[id2].first, resultMatrix2.c_str(), (int)strlen(resultMatrix2.c_str()), 0);

		if (IsFinish) {
			break;
		}
	}
}