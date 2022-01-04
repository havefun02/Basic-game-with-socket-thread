#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<sstream>
#include<regex>
#include<string>;
#include<tuple>
using namespace std;

#pragma region CAC CLASS PHUC VU CHO GAME 
class Point {
private:
	int _x;
	int _y;
public:
	Point();
	Point(int, int);
public:
	void setX(int);
	void setY(int);
	int x()const;
	int y()const;
};

class Ship {
private:
	Point start;
	Point end;
	int _hp;
public:
	Ship();
	Ship(int, int, int, int, int);
	Ship(Point, Point, int);
public:
	void Replace(int, int, int, int);
	void Replace(Point, Point);
	Point startPoint();
	Point endPoint();
	int HP();
public:
	bool Destroy();
};

class ShipManager {
private:
	vector<Ship> _ship;
	int limit;
public:
	ShipManager();
	ShipManager(int);
public:
	void setLimit(int);
	bool AddShip(Point, Point, int);
	Ship GetShip(int);
	bool RemoveShip(int);
	int size();
	bool isFull();
private:
	int typeToHp(int);
};
#pragma endregion 

class BattleShip {
private:
	vector < vector<int>> _map;	// n*n
	int n;
	vector<ShipManager> _Ship;
	int totalShip;
	//bool IsSetUp;
public:
	BattleShip();
	BattleShip(int);
public:	// nhung ham duoc su dung
	tuple<bool, bool, string> AttackShip(int, int); //<hit or miss ,is game finished, message> AttackShip at (x,y)
	bool UpdateMap(string);						// up map tu file .csv
	bool update(string);						// up map tu dong ma tran trong file .csv
	int FillShip();								// doc du lieu tu ma tran map de quan ly tau
	int TotalShip()const;
	void ShowPureMap();							// xem map 
	void ShowMap();								// xem map		
	string convertMap();						// matrix -> line
private:										// phuc vu cho cac ham public (k can dung vao)
	string ShipType(int);
	tuple<Point, Point, bool, string> FindShip(int, int, int, vector < vector<bool>>);
	bool CanShipPlaceHere(Point, Point, int, vector < vector<bool>>);
	void Visit(Point, Point, vector < vector<bool>>&);
	bool PointInShip(int, int, Point, Point);
	void DestroyShip(Point, Point);
	bool CheckNumofShip();
	bool OutOfBoder(int);
public:
	static int convertToX(string);
	static int convertToY(string);
};




