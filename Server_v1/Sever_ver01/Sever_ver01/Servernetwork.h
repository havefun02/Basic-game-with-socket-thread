#pragma once
#include"General.h"
#include"PlayerAccount.h"
#include"BattleShip.h"
class ServerNetwork {
public:
	ServerNetwork();
	bool acceptNewClient(unsigned int& id);
	void getInfosv(SOCKADDR_IN sv, int size);
	void getInfoClient(SOCKADDR_IN cl, int size);
	vector<PlayerAccount> database;
	SOCKADDR_IN addrsv;
	SOCKADDR_IN	addrcl;
	SOCKET ListenSocket;
	SOCKET ClientSocket;
	std::map<unsigned int, pair<SOCKET, BattleShip>> sessions;

};