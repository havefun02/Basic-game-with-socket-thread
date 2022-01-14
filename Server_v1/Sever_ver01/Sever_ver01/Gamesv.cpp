#include"Gamesv.h"
#include"EncryptAndDecryp.h"
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
		//cout << "update()"<< client_id << endl;
		receive(client_id-1);
	}
}

void ServerGame::receive(int idnet)
{
	string idc1;
	bool checkturn = 0;
	//cout << "receive: " << idnet << endl;
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
			// Decrypt
			bool checken = 0;
			string tmp;
			string tmp_encrypt = string(buf, 0, brecv);
			if (tmp_encrypt.substr(0,1) == "1") checken = 1;
			if (checken) {
				tmp_encrypt = tmp_encrypt.substr(1);
				tmp = Encryption::Decrypt(tmp_encrypt);
			}
			else
				tmp = tmp_encrypt;

			int indexsig = tmp.find_first_of(":");
			string signal = tmp.substr(0, indexsig);
			string content = tmp.substr(indexsig + 1);
			//cout << "Wait for client!" << endl;
			cout << "Message from client: " << idnet <<" : " << signal << endl;
			//check 
			if (signal == "Find")
			{
				string s;
				if (handler.Checkuser(network->database, content, s))
				{
					string buffer = Encryption::Encrypt("Yes");

					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
					s = Encryption::Encrypt(s);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.Checkuser(network->database, content, s))
				{
					string buffer = Encryption::Encrypt("No");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
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
					string buffer = Encryption::Encrypt("Yes");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
					s = Encryption::Encrypt(s);
					send(curclient, s.c_str(), (int)strlen(s.c_str()), 0);
				}
				else if (!handler.ListenLogin(network->database, content, s, ind1))
				{
					string buffer = Encryption::Encrypt("No");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
			}
			else if (signal == "Register")
			{
				if (handler.ListenRegister(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					string buffer = Encryption::Encrypt("Yes");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
				else
				{
					string buffer = Encryption::Encrypt("No");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
			}
			else if (signal == "Changepass")
			{
				if (handler.ListenChangePass(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					string buffer = Encryption::Encrypt("Yes");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
				else
				{
					string buffer = Encryption::Encrypt("No");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
			}
			else if (signal == "Changeinfo")
			{
				if (handler.ListenChangeinfo(network->database, content))
				{
					handler.Writeoutdatabase(network->database);
					string buffer = Encryption::Encrypt("Yes");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
				else
				{
					string buffer = Encryption::Encrypt("No");
					send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				}
			}
			else if (signal == "ListOnline")
			{
				//return the list player online
				string tmp = handler.DatabaseToString(network->database,idnet);

				tmp = Encryption::Encrypt(tmp);

				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "ConnectClient")
			{
				PlayerAccount t;
				if (handler.Finduserbyid(network->database, content, t))
				{
					//send to another client

					idc1 = content;

					string tmp = "Join+" + to_string(idnet);

					tmp = Encryption::Encrypt(tmp);


					send(network->sessions[stoi(content)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				}
			}
			else if (signal == "Yes")
			{
				idc1 = content;
				tmp = "Yes";

				tmp = Encryption::Encrypt(tmp);

				send(network->sessions[stoi(idc1)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "No") {
				tmp = "No";
				idc1 = content;
				tmp = Encryption::Encrypt(tmp);
				send(network->sessions[stoi(idc1)].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
				tmp = Encryption::Encrypt("UiClient");
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

				tmp = Encryption::Encrypt(tmp);

				send(curclient, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "Start")
			{
				char buf[1024];
				string tmp = "Your turn!";

				tmp = Encryption::Encrypt(tmp);

				send(network->sessions[idnet].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "Start1")
			{
				char buf[1024];
				string tmp = "Wait!";

				tmp = Encryption::Encrypt(tmp);

				send(network->sessions[idnet].first, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
			}
			else if (signal == "atk")
			{
				tuple<bool, bool, string> result;
				int x, y;
				bool IsHit, IsFinish;
				string message;
				string attackSignal = content;

				cout << "content "<<content << endl;
				
				cout <<"Opp: " <<idc1 << endl;

				network->sessions[stoi(idc1)].second.ShowMap();



				x = BattleShip::convertToY(attackSignal);
				y = BattleShip::convertToX(attackSignal);
				result = network->sessions[stoi(idc1)].second.AttackShip(x-1, y-1);
				tie(IsHit, IsFinish, message) = result;
				
				cout << message << endl;
				
				string resultMatrix1 = network->sessions[stoi(idc1)].second.convertMap();
				string resultMatrix2 = network->sessions[idnet].second.convertMap();
				// Ex: resultMatrix1 = "010000100100...."

				/*cout << resultMatrix1 << endl;
				cout << resultMatrix2 << endl;*/

				resultMatrix1 = Encryption::Encrypt(resultMatrix1);
				resultMatrix2 = Encryption::Encrypt(resultMatrix2);

				


				send(network->sessions[stoi(idc1)].first, resultMatrix1.c_str(), (int)strlen(resultMatrix1.c_str()), 0);
				send(network->sessions[idnet].first, resultMatrix2.c_str(), (int)strlen(resultMatrix2.c_str()), 0);


				// send cai message dum con tu luc nay a thua cu
				//send(network->sessions[stoi(idc1)].first, message.c_str(), (int)strlen(message.c_str()), 0);
				//send(network->sessions[idnet].first, message.c_str(), (int)strlen(message.c_str()), 0);
				// roi gan cai message nay duoi your turn hoac wait cua client

				if (IsHit) {				// true mean the current Client who hit can continue playing
					string mes = "Your turn!";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = "Wait!";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
				}
				else						// false mean the current Client who miss stop playing and
				{
					string mes = "Your turn!";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = "Wait!";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);

					checkturn = !checkturn;
				}


				if (IsFinish) {				//Is the game done yet?
					string mes = "Win";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);

					mes = Encryption::Encrypt("UiClient");

					send(network->sessions[idnet].first, mes.c_str(), (int)strlen(mes.c_str()), 0);

					mes = "Lose";

					mes = Encryption::Encrypt(mes);

					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
					mes = Encryption::Encrypt("UiClient");

					send(network->sessions[stoi(idc1)].first, mes.c_str(), (int)strlen(mes.c_str()), 0);
				}
			}
			else if (signal == "Connect to sv")
			{
				string t1 = to_string(client_id - 1);
				string buffer = "Login:" + t1;

				buffer = Encryption::Encrypt(buffer);

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
				string buffer = Encryption::Encrypt("UiClient");

				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
				closesocket(curclient);
				ExitThread(0);
			}
			else if (signal == "BackUiClient")
			{
				string buffer = Encryption::Encrypt("UiClient");
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
			}
			else if (signal == "BackRegister")
			{
				string buffer = Encryption::Encrypt("Register");
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
			}
			else if (signal == "BackLogin")
			{
				string buffer = Encryption::Encrypt("Login");
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
			}
			else if (signal == "BackChangeinfo")
			{
				string buffer = Encryption::Encrypt("ChangeInfor");
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
			}
			else if (signal == "BackChangePass")
			{
				string buffer = Encryption::Encrypt("ChangePass");
				send(curclient, buffer.c_str(), (int)strlen(buffer.c_str()), 0);
			}

		}
	}
}
