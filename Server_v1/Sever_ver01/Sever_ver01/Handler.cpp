#include"Handle.h"


bool Handler:: Finduserbyid(vector<PlayerAccount>& database, string id,PlayerAccount &t)
{
	for (int i = 0; i < database.size(); i++)
	{
		if (id == database[i].id()) {
			t = database[i];
			return 1;
		}
	}
	return 0;
}

bool Handler::Closeconnect(vector<PlayerAccount>& database,string id)
{
	for (int i=0;i<database.size();i++)
		if (id == database[i].id())
		{
			database[i].set_id("");
			database[i].set_online(0);
			return 1;
		}
	return 0;
	
}
bool Handler::ListenLogin(vector<PlayerAccount> &database,string format,string &s,string id1)
{
	//id,pass
	int ind = format.find_first_of(",");
	string id = format.substr(0, ind);
	string pass = format.substr(ind + 1);
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username() == id && database[i].password() == pass)
		{
			s = database[i].username() +"," + database[i].password() +"." + database[i].fullname() +"/" + database[i].day() +"+" + database[i].month() +"-" + database[i].year();
			database[i].set_id(id1);
			database[i].set_online(1);
			return 1;
		}
	}
	return 0;
}
bool Handler::ListenRegister(vector<PlayerAccount>& database, string format)
{
	//id,pass.fullname/day+month-year
	int ind = format.find_first_of(",");
	int ind1 = format.find_first_of(".");
	int ind2 = format.find_first_of("/");
	int ind3 = format.find_first_of("+");
	int ind4 = format.find_first_of("-");
	string id = format.substr(0, ind);
	string pass = format.substr(ind + 1, ind1 - ind - 1);
	string fullname = format.substr(ind1 + 1, ind2 - ind1 - 1);
	string day = format.substr(ind2 + 1, ind3- ind2 - 1);
	string month = format.substr(ind3 + 1, ind4 - ind3 - 1);
	string year = format.substr(ind4 + 1, format.size() - ind4 - 1);
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username()==id)
		{
			return 0;
		}
	}
	PlayerAccount player;
	player.set_username(id);
	player.set_password(pass);
	player.set_fullname(fullname);
	player.set_day(day);
	player.set_month(month);
	player.set_year(year);
	player.set_point("0");
	database.push_back(player);
	return 1;
}
bool Handler::ListenChangePass(vector<PlayerAccount>& database, string format)
{
	//id,pass.newpass
	int ind = format.find_first_of(",");
	int ind1 = format.find_first_of(".");
	string id = format.substr(0, ind);
	string curpass = format.substr(ind + 1,ind1-ind-1);
	string newpass = format.substr(ind1 + 1, format.size() - ind1 - 1);
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username() == "") break;
		if (database[i].username() == id && database[i].password() == curpass)
		{
			database[i].set_password(newpass);
			return 1;
		}
	}
	return 0;
}

bool Handler::Checkuser(vector<PlayerAccount>& database, string format,string &s)
{
	//name
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username() == "") break;
		if (database[i].username() == format) {
			s = database[i].fullname() + "," + database[i].day() + "." + database[i].month() + "/" + database[i].year() + "+" + database[i].point()+"-"+ to_string(database[i].IsOnline());
			return 1;
		}
	}
	return 0;
}

bool Handler::ListenChangeinfo(vector<PlayerAccount>& database, string format)
{
	//id-pass+fullname/day,month.year
	int ind3 = format.find_first_of("-");
	int ind4 = format.find_first_of("+");
	int ind = format.find_first_of("/");
	int ind1 = format.find_first_of(",");
	int ind2 = format.find_first_of(".");
	string id= format.substr(0, ind3);
	string pass= format.substr(ind3+1, ind4-ind3-1);
	string fullname = format.substr(ind4+1, ind-ind4-1);
	string day = format.substr(ind + 1, ind1 - ind - 1);
	string month = format.substr(ind1 + 1, ind2 - ind1 - 1);
	string year = format.substr(ind2 + 1, format.size() - ind2 - 1);
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username() == id && database[i].password() == pass) {\
			database[i].set_fullname(fullname);
			database[i].set_day(day);
			database[i].set_month(month);
			database[i].set_year(year);
			return 1;
		}
	}
	return 0;
}

vector<PlayerAccount>Handler:: getdata()
{
	fstream f;
	vector<PlayerAccount> list;
	PlayerAccount tmp;
	f.open("database.csv", ios::in);
	if (!f.is_open())
	{
		cout << "cant open file" << endl;
		exit(0);
	}
	string title;
	getline(f, title);
	while (!f.eof())
	{
		string  username, password, fullname, day, month, year,point;
		getline(f, username, ',');
		getline(f, password, ',');
		getline(f,fullname,',');
		getline(f, day, ',');
		getline(f, month, ',');
		getline(f, year, ',');
		getline(f, point);
		if (username == "") break;
		tmp.set_username(username);
		tmp.set_password(password);
		tmp.set_fullname(fullname);
		tmp.set_day(day);
		tmp.set_month(month);
		tmp.set_year(year);
		tmp.set_point(point);
		list.push_back(tmp);
	}
	f.close();
	return list;
}

void Handler::Writeoutdatabase(vector<PlayerAccount> database)
{
	fstream f;
	vector<PlayerAccount> list;
	PlayerAccount tmp;
	f.open("database.csv", ios::out);
	if (!f.is_open())
	{
		cout << "cant open file" << endl;
		return;
	}
	f << "id,pass,fullname,day,month,year,point" << endl;
	for (int i = 0; i < database.size(); i++)
	{
		if (database[i].username() == "") break;
		f << database[i].username() << "," << database[i].password() << ","
			<< database[i].fullname() << "," << database[i].day() << "," << database[i].month() << ","
			<< database[i].year() <<","<< database[i].point() << endl;
	}
	f.close();
}


string Handler::DatabaseToString(vector<PlayerAccount> database,int id) {
	stringstream result;
	for (int i = 0; i < database.size(); i++){
		if (database[i].IsOnline() && database[i].id()!=to_string(id)) {
			result << "id:" << database[i].id() << ","
				<< "name:" << database[i].fullname() << ","
				<< "point:" << database[i].point() << ";";
		}
	}

	return result.str();
}