#pragma once
#include<iostream>
#include<string>
#include<sstream>
#include<vector>

using namespace std;

class Encryption {
public:
	static string Encrypt(const string&);
	static string Decrypt(const string&);
};

