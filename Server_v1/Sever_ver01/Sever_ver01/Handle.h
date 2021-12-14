#pragma once
#include"General.h"
#include"PlayerAccount.h"
class Handler
{
public:
	vector<PlayerAccount> getdata();
	bool Checkuser(vector<PlayerAccount>&, string format,string &s);
	bool Closeconnect(vector<PlayerAccount>& , string id);
	bool ListenLogin(vector<PlayerAccount>&, string format,string &s,string id1);
	bool ListenRegister(vector<PlayerAccount>&, string format);
	bool ListenChangePass(vector<PlayerAccount>&, string format);
	bool ListenChangeinfo(vector<PlayerAccount>&, string format);
	void Writeoutdatabase(vector<PlayerAccount>);
	string DatabaseToString(vector<PlayerAccount>);
	bool Finduserbyid(vector<PlayerAccount>&, string id, PlayerAccount& t);
};