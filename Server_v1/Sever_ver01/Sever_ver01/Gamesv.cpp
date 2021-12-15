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
		receive(client_id-1);
	}
}

void ServerGame::receive(int idnet)
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
				PlayerAccount t;
				if (handler.Finduserbyid(network->database, content, t))
				{
					//send to another client
					string tmp = "Join";
					send(network->sessions[stoi(content)], tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				}
				while (1)
				{
					vector<BattleShip> map;
					int brecv = recv(network->sessions[stoi(content)], buf, 100, 0);
					string tmp = string(buf, 0, brecv);
					if (tmp == "Yes")
					{
						tmp = "Yes";
						send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
					}
					else if (tmp == "No")
					{
						tmp = "No";
						send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
					}

					if (tmp == "Yes")
					{
						//nhan map	
						// network->map[0] : Client 1
						// network->map[1] : Client 2
						do
						{
							brecv = recv(network->sessions[stoi(content)], buf, 100, 0);
							string line_map = string(buf, 0, brecv);

							BattleShip mapClient;

							if (mapClient.update(line_map)) {
								mapClient.FillShip();
							}

							map.push_back(mapClient);

						} while (map.size()<1);

						do
						{
							brecv = recv(curclient, buf, 100, 0);
							string line_map = string(buf, 0, brecv);

							BattleShip mapClient;

							if (mapClient.update(line_map)) {
								mapClient.FillShip();
							}

							map.push_back(mapClient);

						} while (map.size() < 2);


						if (map.size() >= 2) {
							while (1) {
								string tmp = "StartGame";
								send(network->sessions[stoi(content)], tmp.c_str(), (int)strlen(tmp.c_str()), 0);
								send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
							}
							//send signal to show Gameplay UI to Client 1 <******************
							//send signal to show Gameplay UI to Client 2 <******************
						}
						//1 mean Client 1, 2 mean Client 2
						tuple<bool, bool, string> result;
						bool IsHit, IsFinish;
						string message;
						stringstream builder;
						string signalGameplay;
						
						
						bool checkturn = 0;
						// GAMEPLAY
						while (1) {
							//
							string tmp = "Your turn!";
							if (checkturn == 0) {
								send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
								string tmp1 = "Wait!";
								send(network->sessions[stoi(content)], tmp.c_str(), (int)strlen(tmp.c_str()), 0);
							}
							else {
								send(network->sessions[stoi(content)], tmp.c_str(), (int)strlen(tmp.c_str()), 0);
								string tmp1 = "Wait!";
								send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
							}

							int x, y;
							brecv = recv(curclient, buf, 100, 0);
							string attackSignal = string(buf, 0, brecv);
							string resultMatrix;
							//Ex : attackSignal = "atk:0506" 
							x = BattleShip::convertToX(attackSignal);
							y = BattleShip::convertToY(attackSignal);
							//update map 2
							result=map[1].AttackShip(x, y);
							//check 
							
							//
							tie(IsHit, IsFinish, message) = result;

							builder << "result:";

							if (IsHit) {		// true mean the current Client who hit can continue playing
								builder << 01;
							}
							else				// false mean the current Client who miss stop playing and
							{				
								checkturn = !checkturn;// the other Client can play now
								builder << 00;
							}

							
							if (IsFinish) {		//Is the game done yet?
								builder << 01;//
							}
							else builder << 00;//
							//Ex : message = "Hit the ship 2x4\nthe ship is destroyed\nthere is no ships in map"
							builder << message;

							// THIS CURRENT SIGNAL DOES NOT INCLUDE THE INFORMATION OF CLIENT'S MAP
							
							signalGameplay = builder.str();

							// Ex : SignalGameplay = "result:0100Hit the ship 2x4\nthe ship i......;0100011000...."

							// send signalGameplay to Client 1	<******************
							// send signalGameplay to Client 2	<******************

							if (IsFinish) {
								break;
							}

						}
					}

					break;
				}
				//id of player choosen -> socket
				//invite another to joint
				//return true if accept, false if refuse
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