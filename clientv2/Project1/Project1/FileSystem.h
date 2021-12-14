#pragma once
#include<iostream>
#include<fstream>
#include<vector>
#include<regex>
#include<string>
#include<sstream>
using namespace std;


class FileSystem {
public:
	static bool ReadFileCSV(string filename, vector<vector<int>>_map); // read csv file and store them to _map, return true if it's successful or false if it fails
};

