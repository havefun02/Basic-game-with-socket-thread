#pragma once
#include"console.h"
#include <iostream>
#include<Windows.h>
#include <stdio.h>
#include <conio.h>
#include<string>
#include<fstream>
#include <ctime>
#include <vector>
#include"Player.h"
using namespace std;
class Draw
{
public:
	
	void DrawMis();
	void DrawShowWinner();
	void DrawShowLoser();
	void DrawControler();
	void Draw10x10();
	void DrawOut();
	void DrawIn();
	void DrawLogEncrypt();
	void DrawLogID();
	void DrawLogPass();
	void DrawLogConfirm();
	void DrawLogReg();
	void DrawLogForgot();
	void DrawLogCancel();
	void DrawChaNewPass();
	void DrawRegId();
	void DrawRegPass();
	void DrawRegDay();
	void DrawRegMonth();
	void DrawRegYear();
	void DrawRegFullName();
	void DrawRegConfirmPass();
	void DrawRegCreate();
	void DrawRegCancel();
	void DrawChangeInfName();
	void DrawChangeInfDay();
	void DrawChangeInfMonth();
	void DrawChangeInfYear();
	void DrawChangeInfConfirm();
	void DrawChangeInfCancel();
};




class UI
{
public:
	Draw draw;//interfac
	void Showmap(string);
	void setCheckuser();
	void ShowforReadFile();
	void ShowOnlinePlayer(vector<PlayerAccount*>);
	void Map();
	void readme();
	void setDefault();
	void setLoginUI();
	void setRegisterUI();
	void setChangePassUI();
	void setChangeInforUI();
	void setDrawEncrypt();
	void setUiClient();
	void DrawFailLogin();
	void DrawFailReg();
	void DrawFailChangepass();
	void DrawChangePassSuccess();
	void DrawChangeinfoSuccess();
	void DrawFailChangeinfo();
	void cur(int x, int y)
	{
		gotoXY(x, y); putchar('_');
		Sleep(25);
		gotoXY(x, y); putchar(' ');
		Sleep(25);
	}
};