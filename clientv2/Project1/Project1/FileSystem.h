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
	static string ReadFileCSV(string filename); // read csv file and return matrix line Ex: "010000100..."
	static vector<vector<int>> LineToMatrix(string matrix);	// convert "010001000..." to matrix
};

