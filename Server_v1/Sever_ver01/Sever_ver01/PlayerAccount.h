#pragma once
#include<string>
#include<iostream>
#include<iomanip>
#include<vector>
#include<string>
#include<sstream>
using namespace std;
class PlayerAccount {
	static int stt;
private:
	string _username;
	string _password;
	string _socket;
private:
	string _title;
	string _date;
	bool _online;
	string _day;
	string _month;
	string _year;
	string _fullname;
	string _note;
	string _point;
	string _id;
	//bool IsPlaying;
public:
	PlayerAccount();
	PlayerAccount(string, string);
public:
	string username();
	string password();
	bool IsOnline();
	string date();
	string day();
	string month();
	string year();
	string fullname();
	string note();
	string id();
	string point();
	string socket();
public:
	bool set_username(string);
	bool set_password(string);
	bool set_fullname(string);
	bool set_day(string);
	bool set_month(string);
	bool set_year(string);
	bool set_date(int, int, int);
	bool set_note(string);
	bool set_point(string);
	bool set_id(string);
	bool set_socket(string);
	bool set_online(bool);
};

