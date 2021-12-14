#include "PlayerAccount.h"

PlayerAccount::PlayerAccount() {
	_username = "";
	_password = "";
	_online = false;
	_date = "";
	_fullname = "";
	_note = "";
	_point = "0";
}

PlayerAccount::PlayerAccount(string username, string password) {
	_username = username;
	_password = password;
	_online = false;
	_date = "";
	_fullname = "";
	_note = "";
	_point = '0';
}

string PlayerAccount::username() { return _username; }
string PlayerAccount::password() { return _password; }
string PlayerAccount::date() { return _date; }
string PlayerAccount::day() { return _day; }
string PlayerAccount::month() { return _month; }
string PlayerAccount::year() { return _year; }
string PlayerAccount::fullname() { return _fullname; }
string PlayerAccount::note() { return _note; }
string PlayerAccount::id() { return _id; }
string PlayerAccount::socket() { return _socket; }
bool PlayerAccount::IsOnline() { return _online; }
string PlayerAccount::point() { return _point; }

bool PlayerAccount::set_password(string password) {
	_password = password;
	return true;
}
bool PlayerAccount::set_username(string username) {
	_username = username;
	return true;
}

bool PlayerAccount::set_day(string day) {
	_day = day;
	return true;
}
bool PlayerAccount::set_month(string month) {
	_month = month;
	return true;
}
bool PlayerAccount::set_year(string year) {
	_year = year;
	return true;
}

bool PlayerAccount::set_date(int day, int month, int year) {
	stringstream newDate;
	newDate << setw(2) << setfill('0') << day << "/"
		<< setw(2) << setfill('0') << month << "/" << year;
	_date = newDate.str();
	return true;
}

bool PlayerAccount::set_fullname(string fullname) {
	_fullname = fullname;
	return true;
}

bool PlayerAccount::set_note(string note) {
	_note = note;
	return true;
}
bool PlayerAccount::set_id(string id) {
	_id = id;
	return true;
}
bool PlayerAccount::set_socket(string socket) {
	_socket = socket;
	return true;
}
bool PlayerAccount::set_point(string point) {
	_point = point;
	return true;
}
bool PlayerAccount::set_online(bool online) {
	_online = online;
	return true;
}