#pragma once
#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include"UiGame.h"
#include"Player.h"
class ClientGame
{
private:
	string _signal;
	bool _Access;
public:
	ClientGame();
	UI ui;
	ClientNetwork* network;
	PlayerAccount *player;
	char network_data[1024];
	vector<vector<int>> map;
	vector<vector<int>> updatemap(string);

	vector<PlayerAccount*> Onlinelist;
	vector<PlayerAccount*> getOnlinelist(string);
	void Sendpacket(char*);
	void Playgame();
	void Register();
	void Changepass();
	void Changeinfo();
	void Login();
	void update();
	void UiClient();
	void CheckUser();
	string getsignal() { return _signal; }
	bool getaccess() { return _Access; }
	void setsignal(string s) { _signal = s; }
	void setaccess(bool b) { _Access = b; }
};
