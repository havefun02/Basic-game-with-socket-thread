#include "EncryptAndDecryp.h"


string Encryption::Encrypt(const string& source) {
	const int EncryptNumber = 95;
	stringstream result;
	vector<char> encrypt;
	for (int i = 0; i < source.length(); i++) {
		int num = int(source[i]);
		num += EncryptNumber;
		char temp = char(num);
		encrypt.push_back(temp);
	}

	while (!encrypt.empty()) {
		result << encrypt.back();
		encrypt.pop_back();
	}

	return result.str();
}

string Encryption::Decrypt(const string& source) {
	const int EncryptNumber = 95;
	stringstream result;
	vector<char> decrypt;

	for (int i = 0; i < source.length(); i++) {
		int num = int(source[i]);
		num -= EncryptNumber;
		char temp = char(num);
		decrypt.push_back(temp);
	}

	while (!decrypt.empty()) {
		result << decrypt.back();
		decrypt.pop_back();
	}

	return result.str();
}