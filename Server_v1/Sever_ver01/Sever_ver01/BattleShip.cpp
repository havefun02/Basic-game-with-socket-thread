#include "BattleShip.h"

#pragma region BattleShip
BattleShip::BattleShip() {
	n = 10;
	totalShip = 0;
	_map.resize(n, vector<int>(n, 0));
	_Ship.resize(5);
	_Ship[0].setLimit(2);
	_Ship[1].setLimit(2);
	_Ship[2].setLimit(2);
	_Ship[3].setLimit(2);
	_Ship[4].setLimit(1);
}

BattleShip::BattleShip(int dimension) {
	n = dimension;
	totalShip = 0;
	_map.resize(n, vector<int>(n, 0));
	_Ship.resize(5);
	_Ship[0].setLimit(2);
	_Ship[1].setLimit(2);
	_Ship[2].setLimit(2);
	_Ship[3].setLimit(2);
	_Ship[4].setLimit(1);
}

bool BattleShip::UpdateMap(string filename) {
	fstream file;
	file.open(filename);
	if (!file.is_open()) {
		cout << "can't open file" << endl;
		return false;
	}
	regex ValidRow("\\b\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\,\\d{1}\\b");
	int i = 0;
	string line, block;

	while (i < n) {
		getline(file, line);
		if (!regex_match(line, ValidRow)) {
			cout << "the map file is invalid format (error at line " << i + 1 << ")" << endl;
			file.close();
			return false;
		}
		stringstream s(line);
		int j = 0;
		while (getline(s, block, ',')) {
			int temp = stoi(block);
			_map[i][j] = temp;
			j++;
		}
		i++;
	}
	file.close();
	return true;
}

bool BattleShip::update(string matrix) {
	int pos = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			string temp = matrix.substr(pos, 1);
			int num = -1;
			try
			{
				num = stoi(temp);
			}

			catch (const std::exception&)
			{
				return false;
			}

			_map[i][j] = num;
			pos++;
		}
	}
}

string BattleShip::ShipType(int type) {
	switch (type)
	{
	case 1:
		return "1x1";
		break;
	case 2:
		return "1x2";
		break;
	case 3:
		return "1x4";
		break;
	case 4:
		return "2x5";
		break;
	case 5:
		return "2x7";
		break;
	default:
		return "Invalid ship type";
		break;
	}
}

int BattleShip::FillShip() {
	vector < vector<bool>> visited(n, vector<bool>(n, false));
	int countShips = 0;
	tuple<Point, Point, bool, string> Check;
	bool succeed = true;
	string error_message;
	Point starttemp;
	Point endtemp;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (_map[i][j] && !visited[i][j]) {
				Check = FindShip(i, j, _map[i][j], visited);
				tie(starttemp, endtemp, succeed, error_message) = Check;
				if (succeed) {
					if (!_Ship[_map[i][j] - 1].AddShip(starttemp, endtemp, _map[i][j])) {
						cout << "Too many ships of this type " << ShipType(_map[i][j]) << endl;
						return -2;
					}
					Visit(starttemp, endtemp, visited);
					countShips++;
				}
				else {
					cout << "error : " << error_message << endl;
					return -1;
				}
			}
		}
	}

	totalShip = countShips;
	if (!CheckNumofShip()) {
		cout << "The number of ships is not enough";
		return -1;
	}

	return countShips;
}

tuple<Point, Point, bool, string> BattleShip::FindShip(int x, int y, int type, vector < vector<bool>> visited) {
	tuple<Point, Point, bool, string> result;
	stringstream error;
	Point start;
	Point end;
	start.setX(x); start.setY(y);

	if (type == 1) {
		end.setX(x); end.setY(y);
		result = make_tuple(start, end, true, "no error");
		return result;
	}
	else if (type == 2) {
		if (!OutOfBoder(y + 1)) {
			if (_map[x][y + 1] && !visited[x][y + 1]) {
				end.setX(x); end.setY(y + 1);
			}
			else if (_map[x + 1][y] && !visited[x + 1][y]) {
				end.setX(x + 1); end.setY(y);
			}
		}
		else {
			end.setX(x + 1); end.setY(y);
		}


		if (!CanShipPlaceHere(start, end, 2, visited)) {
			error << "Can not place ship here # error at (" << start.x() << "," << start.y() << ")";
			result = make_tuple(start, end, false, error.str());
		}
		else result = make_tuple(start, end, true, "no error");
		return result;

	}
	else if (type == 3) {
		if (!OutOfBoder(y + 1)) {
			if (_map[x][y + 1] && !visited[x][y + 1]) {
				end.setX(x); end.setY(y + 3);
			}
			else if (_map[x + 1][y] && !visited[x + 1][y]) {
				end.setX(x + 3); end.setY(y);
			}
		}
		else {
			end.setX(x + 3); end.setY(y);
		}

		//error

		if (!CanShipPlaceHere(start, end, 3, visited)) {
			error << "Can not place ship here # error at (" << start.x() << "," << start.y() << ")";
			result = make_tuple(start, end, false, error.str());
		}
		else result = make_tuple(start, end, true, "no error");
		return result;


	}
	else if (type == 4) {
		if (!OutOfBoder(y + 2)) {
			if (_map[x][y + 2] && !visited[x][y + 2]) { //horizonatlly
				end.setX(x + 1); end.setY(y + 4);
			}
			else if (_map[x + 2][y] && !visited[x + 2][y]) {
				end.setX(x + 4); end.setY(y + 1);
			}
		}
		else {
			end.setX(x + 4); end.setY(y + 1);
		}


		if (!CanShipPlaceHere(start, end, 4, visited)) {
			error << "Can not place ship here # error at (" << start.x() << "," << start.y() << ")";
			result = make_tuple(start, end, false, error.str());
		}
		else result = make_tuple(start, end, true, "no error");
		return result;

	}
	else if (type == 5) {
		if (!OutOfBoder(y + 2)) {
			if (_map[x][y + 2] && !visited[x][y + 2]) { //horizonatlly
				end.setX(x + 1); end.setY(y + 6);
			}
			else if (_map[x + 2][y] && !visited[x + 2][y]) {
				end.setX(x + 6); end.setY(y + 1);
			}
		}
		else {
			end.setX(x + 6); end.setY(y + 1);
		}


		if (!CanShipPlaceHere(start, end, 5, visited)) {
			error << "Can not place ship here # error at (" << start.x() << "," << start.y() << ")";
			result = make_tuple(start, end, false, error.str());
		}
		else result = make_tuple(start, end, true, "no error");
		return result;

	}

	error << "Invalid type # error type is " << type + 1;
	result = make_tuple(start, end, false, error.str());
	return result;
}

bool BattleShip::CanShipPlaceHere(Point start, Point end, int type, vector < vector<bool>> visited) {
	if (start.x() >= n || start.y() >= n || end.x() >= n || end.y() >= n) {
		return false;
	}


	if (type == 1) {
		return !visited[start.x()][start.y()];
	}
	else if (type == 2) {
		return !visited[start.x()][start.y()] && !visited[end.x()][end.y()];
	}
	else if (type == 3) {
		if (end.y() > start.y()) { //place horizontally
			for (int i = start.y(); i < start.y() + 4; i++) {
				if (visited[start.x()][i]) {
					return false;
				}
			}
		}
		else {
			for (int i = start.x(); i < start.x() + 4; i++) {
				if (visited[i][start.y()]) {
					return false;
				}
			}
		}
		return true;
	}
	else if (type == 4) {
		if (end.y() > start.y()) { //place horizontally
			for (int row = start.x(); row < start.x() + 2; row++) {
				for (int i = start.y(); i < start.y() + 5; i++) {
					if (visited[row][i]) {
						return false;
					}
				}
			}
		}
		else {
			for (int row = start.x(); row < start.x() + 5; row++) {
				for (int i = start.y(); i < start.y() + 2; i++) {
					if (visited[row][i]) {
						return false;
					}
				}
			}
		}
		return true;
	}
	else if (type == 5) {
		if (end.y() > start.y()) { //place horizontally
			for (int row = start.x(); row < start.x() + 2; row++) {
				for (int i = start.y(); i < start.y() + 7; i++) {
					if (visited[row][i]) {
						return false;
					}
				}
			}
		}
		else {
			for (int row = start.x(); row < start.x() + 7; row++) {
				for (int i = start.y(); i < start.y() + 2; i++) {
					if (visited[row][i]) {
						return false;
					}
				}
			}
		}
		return true;
	}
	cout << "Invalid type" << endl;
	return false;
}

void BattleShip::Visit(Point start, Point end, vector < vector<bool>>& visited) {
	int x = start.x();
	while (x <= end.x()) {
		int y = start.y();
		while (y <= end.y()) {
			visited[x][y] = true;
			y++;
		}
		x++;
	}
}

bool BattleShip::OutOfBoder(int x) {
	return x >= n;
}
#pragma endregion Set up

#pragma region Playing
tuple<bool, bool, string> BattleShip::AttackShip(int x, int y) {
	tuple<bool, bool, string> result;
	bool Finish = false;
	if (_map[x][y] <= 0) {
		result = make_tuple(false, Finish, "Missed");
		return result;
	}

	//hit
	for (int i = 0; i < _Ship[_map[x][y] - 1].size(); i++) {
		Ship temp = _Ship[_map[x][y] - 1].GetShip(i);
		if (PointInShip(x, y, temp.startPoint(), temp.endPoint())) {
			stringstream resultAfter;
			resultAfter << "Hit the ship " << ShipType(_map[x][y]);
			totalShip--;
			if (temp.Destroy()) {
				_Ship[_map[x][y] - 1].RemoveShip(i);
				resultAfter << "\nthe ship is destroyed";
			}

			if (totalShip <= 0) {
				resultAfter << "\nthere is no ships in map";
				Finish = true;
			}
			result = make_tuple(true, Finish, resultAfter.str());
			_map[x][y] = -1;
			//DestroyShip(temp.startPoint(), temp.endPoint());
			return result;
		}
	}

	result = make_tuple(false, Finish, "error between ship and map");
	return result;

}

bool BattleShip::PointInShip(int x, int y, Point start, Point end) {
	return (x >= start.x() && y >= start.y()) && (x <= end.x() && y <= end.y());
}

void BattleShip::DestroyShip(Point start, Point end) {
	int x = start.x();
	while (x <= end.x()) {
		int y = start.y();
		while (y <= end.y()) {
			_map[x][y] = 0;
			y++;
		}
		x++;
	}
}

bool BattleShip::CheckNumofShip() {
	for (int i = 0; i < _Ship.size(); i++) {
		if (!_Ship[i].isFull())return false;
	}

	return true;
}
#pragma endregion

int BattleShip::TotalShip()const { return totalShip; }

void BattleShip::ShowPureMap() {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (_map[i][j]) {
				cout << "|[]|";
			}
			else {
				cout << "~~~~";
			}
		}
		cout << endl;
	}
}

void BattleShip::ShowMap() {
	cout << "    ";
	for (int i = 0; i < n; i++) {
		if ((i + 1) < 10) {
			cout << "|0" << i + 1;
		}
		else cout << "|" << i + 1;
	}
	cout << endl;
	for (int i = 0; i < n; i++) {
		if ((i + 1) < 10) {
			cout << "|0" << i + 1 << "|";
		}
		else cout << "|" << i + 1 << "|";

		for (int j = 0; j < n; j++) {
			cout << "|";
			if (_map[i][j] > 0) {
				cout << "[]";
			}
			else if (_map[i][j] < 0)
			{
				cout << "><";
			}
			else
			{
				cout << "~~";
			}
		}
		cout << endl;
	}
}

string BattleShip::convertMap() {
	stringstream result;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			result << _map[i][j];
		}
	}

	return result.str();
}


#pragma region Point
Point::Point() {
	_x = 0;
	_y = 0;
}

Point::Point(int x, int y) :_x(x), _y(y) {}

void Point::setX(int x) {
	_x = x;
}

void Point::setY(int y) {
	_y = y;
}

int Point::x()const { return _x; }
int Point::y()const { return _y; }
#pragma endregion

#pragma region Ship
Ship::Ship() {
	_hp = 0;
	start = { 0,0 };
	end = { 0,0 };
}

Ship::Ship(int x1, int y1, int x2, int y2, int HP) {
	start = { x1,y1 };
	end = { x2,y2 };
	_hp = HP;
}

Ship::Ship(Point _start, Point _end, int HP) {
	this->start.setX(_start.x()); this->start.setY(_start.y());
	this->end.setX(_end.x()); this->end.setY(_end.y());
	_hp = HP;
}

void Ship::Replace(int x1, int y1, int x2, int y2) {
	start.setX(x1); start.setY(y1);
	end.setX(x2); end.setY(y2);
}

void Ship::Replace(Point _start, Point _end) {
	this->start.setX(_start.x()); this->start.setY(_start.y());
	this->end.setX(_end.x()); this->end.setY(_end.y());
}

Point Ship::startPoint() { return start; }
Point Ship::endPoint() { return end; }
int Ship::HP() { return _hp; }

bool Ship::Destroy() {
	_hp--;
	if (_hp <= 0) {
		return true;
	}
	else return false;
}
#pragma endregion

#pragma region ShipManager
ShipManager::ShipManager(int MAX) : limit(MAX) {
}
ShipManager::ShipManager() {
	limit = 0;
}

void ShipManager::setLimit(int MAX) {
	limit = MAX;
	//type = 0;
}



bool ShipManager::AddShip(Point start, Point end, int type) {
	if (_ship.size() > limit) {
		return false;
	}
	int HP = typeToHp(type);
	if (HP < 0) return false;

	Ship temp(start, end, HP);
	_ship.push_back(temp);
	if (_ship.size() == limit) _ship.resize(limit);
	return true;
}

Ship ShipManager::GetShip(int index) {
	Ship result;
	if (index < 0 && index >= _ship.size()) {
		cout << "Out of range" << endl;
		return result;
	}

	return _ship[index];
}

bool ShipManager::RemoveShip(int index) {
	if (index < 0 && index >= _ship.size()) {
		cout << "Out of range" << endl;
		return false;
	}

	_ship.erase(_ship.begin() + index);
	return true;
}

int ShipManager::size() { return _ship.size(); }

bool ShipManager::isFull() {
	return _ship.size() == limit;
}

int ShipManager::typeToHp(int type) {
	switch (type)
	{
	case 1:
		return 1;
	case 2:
		return 2;
	case 3:
		return 4;
	case 4:
		return 10;
	case 5:
		return 14;
	default:
		return -1;
		break;
	}
}
#pragma endregion




// convert signal

// Example :  signal = "atk:0506"  --> x = 5, y = 6
int BattleShip::convertToX(string signal) {
	int x = 0;
	int twoDot = signal.find_first_of(":");

	string posX = signal.substr(twoDot + 1, 2);
	x = stoi(posX);

	return x;
}
int BattleShip::convertToY(string signal) {
	int y = 0;
	int twoDot = signal.find_first_of(":");

	string posX = signal.substr(twoDot + 3, 2);
	y = stoi(posX);

	return y;
}