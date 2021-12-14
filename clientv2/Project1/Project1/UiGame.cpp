#include"UiGame.h"

void UI::ShowOnlinePlayer(vector<PlayerAccount*> player)
{
	clrscr();
	draw.DrawOut();
	for (int i = 0; i < player.size(); i++)
	{
		gotoXY(45, 22+i*2);
		cout << player[i]->id() << " " << player[i]->fullname() << " WIN " << player[i]->point()<< endl;
	}
	gotoXY(45, 44);
	cout << "BACK" << endl;
}

void UI::ShowforReadFile()
{
	clrscr();
	draw.DrawOut();
	gotoXY(45, 24);
	cout << "PLEASE INPUT YOUR FILE NAME " << endl;
}

void Draw::Draw10x10()
{
	for (int i = 61; i <= 99; i++)
	{
		gotoXY(i, 15);  putchar(196);
		gotoXY(i, 35);  putchar(196);
	}
	for (int i = 16; i <= 34; i++)
	{
		gotoXY(60, i); putchar(179);
		gotoXY(100, i); putchar(179);
	}
	for (int j = 0; j<10; j++)
		for (int i = 16; i <= 34; i++)
		{
			gotoXY(60+j*4, i); putchar(179);
		}
	for (int j = 0; j < 10; j++)
		for (int i = 61; i <= 99; i++)
		{
			gotoXY(i, 15 + j * 2); putchar(196);
		}
	for (int j = 0; j < 10; j++)
	{
		gotoXY(60 + j * 4, 15); putchar(194);
		gotoXY(60 + j * 4, 35); putchar(193);
	}
	for (int j = 0; j < 9; j++)
		for (int i = 0; i < 9; i++)
		{
			gotoXY(64 + 4 * i, 17 + 2 * j); putchar(197);
		}
	gotoXY(60, 35);//|_
	putchar(192);
	gotoXY(60, 15);//|-
	putchar(218);
	gotoXY(100, 15);//|_
	putchar(191);
	gotoXY(100, 35);//|_
	putchar(217);

}
void Draw::DrawControler()
{
	for (int i = 41; i <= 129; i++)
	{
		gotoXY(i, 37);  putchar(196);
		gotoXY(i, 47);  putchar(196);
	}
	for (int i = 38; i <= 46; i++)
	{
		gotoXY(40, i); putchar(179);
		gotoXY(130, i); putchar(179);
		gotoXY(85, i); putchar(179);

	}
	gotoXY(40, 47);//|_
	putchar(192);
	gotoXY(40, 37);//|-
	putchar(218);
	gotoXY(130, 37);//|_
	putchar(191);
	gotoXY(130, 47);//_|
	putchar(217);

	gotoXY(45, 40);
	cout << "Input X coordinate: " << endl;//x=45+20
	gotoXY(45, 42);
	cout << "Input Y coordinate: " << endl;
	gotoXY(45, 44);
	cout << "Attack" << endl;
}

void Draw::DrawShowWinner()
{
	gotoXY(90, 40);
	cout << "You Win " << endl;//x=45+20
}
void Draw::DrawShowLoser()
{
	gotoXY(90, 40);
	cout << "You lose " << endl;//x=45+20
}
void Draw::DrawMis()
{
	gotoXY(90, 42);
	cout << "MISS" << endl;
}

void UI::Map()
{
	clrscr();
	draw.DrawOut();
	draw.Draw10x10();
	draw.DrawControler();
}


void UI::setCheckuser()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 20);
	cout << "CHECK ONLINE           " << endl;
	gotoXY(45, 23);
	cout << "DATE     " << endl;
	gotoXY(45, 26);
	cout << "FULLNAME      " << endl;
	gotoXY(45, 29);
	cout << "NOTE        " << endl;
	gotoXY(45, 32);
	cout << "POINT        " << endl;
	gotoXY(45, 35);
	cout << "SHOW ALL        " << endl;
	gotoXY(45, 38);
	cout << "BACK        " << endl;
}

void UI::setUiClient()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 23);
	cout << "PLAY GAME       " << endl;
	gotoXY(45, 27);
	cout << "CHANGE PASS     " << endl;
	gotoXY(45, 31);
	cout << "CHANGE INFO     " << endl;
	gotoXY(45, 35);
	cout << "CHECK USER      " << endl;
	gotoXY(45, 39);
	cout << "LOG OUT         " << endl;

}

void UI::readme()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 25);
	cout << "As an online game" << endl;
	gotoXY(45, 28);
	cout << "Hope u enjoy it with the pleasures" << endl;
	gotoXY(45, 31);
	cout << "Press any key to back" << endl;
}
void UI::DrawFailChangeinfo()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 25);
	cout << "ChangeInfo fail!!" << endl;
}
void UI::DrawChangeinfoSuccess()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 25);
	cout << " success!!" << endl;
}

void UI::DrawChangePassSuccess()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 25);
	cout << "Successfully" << endl;
}
void UI::DrawFailChangepass()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	gotoXY(45, 25);
	cout << "Change pas Faill!" << endl;
}

void UI::DrawFailReg()
{
	clrscr();
	gotoXY(24, 44);
	cout << "Register faill!!" << endl;
	Sleep(500);
	gotoXY(40, 44);
	cout << "                " << endl;
}

void cur(int x, int y)
{
	gotoXY(x, y); putchar('_');
	Sleep(25);
	gotoXY(x, y); putchar(' ');
	Sleep(25);
}

void Draw::DrawChangeInfConfirm()
{
	gotoXY(60, 36);//top-right
	putchar(187);
	gotoXY(60, 38);//bottom-right
	putchar(188);
	gotoXY(60, 37);//between both above
	putchar(186);
	gotoXY(40, 36);//top-left
	putchar(201);
	gotoXY(40, 37);//middle
	putchar(186);
	gotoXY(40, 38);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 60; i++)
	{
		gotoXY(i, 36);
		putchar(205);
		gotoXY(i, 38);
		putchar(205);
	}
}
void Draw::DrawChangeInfCancel()
{
	gotoXY(120, 36);//top-right
	putchar(187);
	gotoXY(120, 38);//bottom-right
	putchar(188);
	gotoXY(120, 37);//between both above
	putchar(186);
	gotoXY(100, 36);//top-left
	putchar(201);
	gotoXY(100, 37);//middle
	putchar(186);
	gotoXY(100, 38);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 101; i < 120; i++)
	{
		gotoXY(i, 36);
		putchar(205);
		gotoXY(i, 38);
		putchar(205);
	}
}

void Draw::DrawChangeInfName()
{
	gotoXY(120, 22);//top-right
	putchar(187);
	gotoXY(120, 24);//bottom-right
	putchar(188);
	gotoXY(120, 23);//between both above
	putchar(186);
	gotoXY(40, 22);//top-left
	putchar(201);
	gotoXY(40, 23);//middle
	putchar(186);
	gotoXY(40, 24);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 22);
		putchar(205);
		gotoXY(i, 24);
		putchar(205);
	}
}
void Draw::DrawChangeInfDay()
{
	gotoXY(60, 29);//top-right
	putchar(187);
	gotoXY(60, 31);//bottom-right
	putchar(188);
	gotoXY(60, 30);//between both above
	putchar(186);
	gotoXY(40, 29);//top-left
	putchar(201);
	gotoXY(40, 30);//middle
	putchar(186);
	gotoXY(40, 31);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 60; i++)
	{
		gotoXY(i, 29);
		putchar(205);
		gotoXY(i, 31);
		putchar(205);
	}
}
void Draw::DrawChangeInfMonth()
{
	gotoXY(90, 29);//top-right
	putchar(187);
	gotoXY(90, 31);//bottom-right
	putchar(188);
	gotoXY(90, 30);//between both above
	putchar(186);
	gotoXY(70, 29);//top-left
	putchar(201);
	gotoXY(70, 30);//middle
	putchar(186);
	gotoXY(70, 31);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 71; i < 90; i++)
	{
		gotoXY(i, 29);
		putchar(205);
		gotoXY(i, 31);
		putchar(205);
	}
}
void Draw::DrawChangeInfYear()
{
	gotoXY(120, 29);//top-right
	putchar(187);
	gotoXY(120, 31);//bottom-right
	putchar(188);
	gotoXY(120, 30);//between both above
	putchar(186);
	gotoXY(100, 29);//top-left
	putchar(201);
	gotoXY(100, 30);//middle
	putchar(186);
	gotoXY(100, 31);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 101; i < 120; i++)
	{
		gotoXY(i, 29);
		putchar(205);
		gotoXY(i, 31);
		putchar(205);
	}
}

void Draw::DrawRegId()
{
	gotoXY(120, 17);//top-right
	putchar(187);
	gotoXY(120, 19);//bottom-right
	putchar(188);
	gotoXY(120, 18);//between both above
	putchar(186);
	gotoXY(40, 17);//top-left
	putchar(201);
	gotoXY(40, 18);//middle
	putchar(186);
	gotoXY(40, 19);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 17);
		putchar(205);
		gotoXY(i, 19);
		putchar(205);
	}
}
void Draw::DrawRegPass()
{
	gotoXY(120, 22);//top-right
	putchar(187);
	gotoXY(120, 24);//bottom-right
	putchar(188);
	gotoXY(120, 23);//between both above
	putchar(186);
	gotoXY(40, 22);//top-left
	putchar(201);
	gotoXY(40, 23);//middle
	putchar(186);
	gotoXY(40, 24);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 22);
		putchar(205);
		gotoXY(i, 24);
		putchar(205);
	}
}
void Draw::DrawRegConfirmPass()
{
	gotoXY(120, 27);//top-right
	putchar(187);
	gotoXY(120, 29);//bottom-right
	putchar(188);
	gotoXY(120, 28);//between both above
	putchar(186);
	gotoXY(40, 27);//top-left
	putchar(201);
	gotoXY(40, 28);//middle
	putchar(186);
	gotoXY(40, 29);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 27);
		putchar(205);
		gotoXY(i, 29);
		putchar(205);
	}
}
void Draw::DrawRegDay()
{
	gotoXY(60, 32);//top-right
	putchar(187);
	gotoXY(60, 34);//bottom-right
	putchar(188);
	gotoXY(60, 33);//between both above
	putchar(186);
	gotoXY(40, 32);//top-left
	putchar(201);
	gotoXY(40, 33);//middle
	putchar(186);
	gotoXY(40, 34);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 60; i++)
	{
		gotoXY(i, 32);
		putchar(205);
		gotoXY(i, 34);
		putchar(205);
	}
}
void Draw::DrawRegMonth()
{
	gotoXY(90, 32);//top-right
	putchar(187);
	gotoXY(90, 34);//bottom-right
	putchar(188);
	gotoXY(90, 33);//between both above
	putchar(186);
	gotoXY(70, 32);//top-left
	putchar(201);
	gotoXY(70, 33);//middle
	putchar(186);
	gotoXY(70, 34);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 71; i < 90; i++)
	{
		gotoXY(i, 32);
		putchar(205);
		gotoXY(i, 34);
		putchar(205);
	}
}
void Draw::DrawRegYear()
{
	gotoXY(120, 32);//top-right
	putchar(187);
	gotoXY(120, 34);//bottom-right
	putchar(188);
	gotoXY(120, 33);//between both above
	putchar(186);
	gotoXY(100, 32);//top-left
	putchar(201);
	gotoXY(100, 33);//middle
	putchar(186);
	gotoXY(100, 34);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 101; i < 120; i++)
	{
		gotoXY(i, 32);
		putchar(205);
		gotoXY(i, 34);
		putchar(205);
	}
}
void Draw::DrawRegFullName()
{
	gotoXY(120, 37);//top-right
	putchar(187);
	gotoXY(120, 39);//bottom-right
	putchar(188);
	gotoXY(120, 38);//between both above
	putchar(186);
	gotoXY(40, 37);//top-left
	putchar(201);
	gotoXY(40, 38);//middle
	putchar(186);
	gotoXY(40, 39);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 37);
		putchar(205);
		gotoXY(i, 39);
		putchar(205);
	}
}

void Draw::DrawRegCreate()
{
	gotoXY(60, 42);//top-right
	putchar(187);
	gotoXY(60, 44);//bottom-right
	putchar(188);
	gotoXY(60, 43);//between both above
	putchar(186);
	gotoXY(40, 42);//top-left
	putchar(201);
	gotoXY(40, 43);//middle
	putchar(186);
	gotoXY(40, 44);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 60; i++)
	{
		gotoXY(i, 42);
		putchar(205);
		gotoXY(i, 44);
		putchar(205);
	}
}

void Draw::DrawRegCancel()
{
	gotoXY(120, 42);//top-right
	putchar(187);
	gotoXY(120, 44);//bottom-right
	putchar(188);
	gotoXY(120, 43);//between both above
	putchar(186);
	gotoXY(100, 42);//top-left
	putchar(201);
	gotoXY(100, 43);//middle
	putchar(186);
	gotoXY(100, 44);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 101; i < 120; i++)
	{
		gotoXY(i, 42);
		putchar(205);
		gotoXY(i, 44);
		putchar(205);
	}
}

void Draw::DrawLogID()
{
	gotoXY(120, 22);//top-right
	putchar(187);
	gotoXY(120, 24);//bottom-right
	putchar(188);
	gotoXY(120, 23);//between both abo
	putchar(186);
	gotoXY(40, 22);//top-left
	putchar(201);
	gotoXY(40, 23);//middle
	putchar(186);
	gotoXY(40, 24);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 22);
		putchar(205);
		gotoXY(i, 24);
		putchar(205);
	}
}

void Draw::DrawLogPass()
{
	gotoXY(120, 28);//top-right
	putchar(187);
	gotoXY(120, 30);//bottom-right
	putchar(188);
	gotoXY(120, 29);//between both above
	putchar(186);
	gotoXY(40, 28);//top-left
	putchar(201);
	gotoXY(40, 29);//middle
	putchar(186);
	gotoXY(40, 30);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 28);
		putchar(205);
		gotoXY(i, 30);
		putchar(205);
	}
}

void Draw::DrawIn()
{
	for (int i = 20; i <= 140; i++)
	{
		gotoXY(i, 15);  putchar(176);
		gotoXY(i, 45);  putchar(176);
	}
	for (int i = 15; i <= 45; i++)
	{
		gotoXY(20, i); putchar(176);
		gotoXY(140, i); putchar(176);
	}
}

void Draw::DrawOut()
{
	for (int i = 10; i <= 150; i++)
	{
		gotoXY(i, 10);  putchar(219);
		gotoXY(i, 50);  putchar(219);
	}
	for (int i = 10; i <= 50; i++)
	{
		gotoXY(10, i); putchar(219);
		gotoXY(150, i); putchar(219);
	}
}
void Draw::DrawLogForgot()
{
	gotoXY(65, 33);//top-right
	putchar(187);
	gotoXY(65, 35);//bottom-right
	putchar(188);
	gotoXY(65, 34);//between both above
	putchar(186);
	gotoXY(40, 33);//top-left
	putchar(201);
	gotoXY(40, 34);//middle
	putchar(186);
	gotoXY(40, 35);//bottom-left
	putchar(200);
	for (int i = 41; i < 65; i++)
	{
		gotoXY(i, 33);
		putchar(205);
		gotoXY(i, 35);
		putchar(205);
	}
}
void Draw::DrawLogReg()
{
	gotoXY(120, 33);//top-right
	putchar(187);
	gotoXY(120, 35);//bottom-right
	putchar(188);
	gotoXY(120, 34);//between both above
	putchar(186);
	gotoXY(80, 33);//top-left
	putchar(201);
	gotoXY(80, 34);//middle
	putchar(186);
	gotoXY(80, 35);//bottom-left
	putchar(200);
	for (int i = 81; i < 120; i++)
	{
		gotoXY(i, 33);
		putchar(205);
		gotoXY(i, 35);
		putchar(205);
	}
}
void Draw::DrawLogEncrypt()
{
	gotoXY(40, 25);
	cout << "Do you want to encrypt message before sending? " << endl;
	gotoXY(40, 27);
	cout << "ENTER TO YES, ANY KEY TO ORGINAL" << endl;
}

void UI::DrawFailLogin()
{
	clrscr();
	gotoXY(40, 25);
	cout << "LOGIN FAIL!!" << endl;
	Sleep(100);
}

void Draw::DrawLogConfirm()
{
	gotoXY(65, 38);//top-right
	putchar(187);
	gotoXY(65, 40);//bottom-right
	putchar(188);
	gotoXY(65, 39);//between both above
	putchar(186);
	gotoXY(40, 38);//top-left
	putchar(201);
	gotoXY(40, 39);//middle
	putchar(186);
	gotoXY(40, 40);//bottom-left
	putchar(200);
	for (int i = 41; i < 65; i++)
	{
		gotoXY(i, 38);
		putchar(205);
		gotoXY(i, 40);
		putchar(205);
	}
}
void Draw::DrawLogCancel()
{
	gotoXY(120, 38);//top-right
	putchar(187);
	gotoXY(120, 40);//bottom-right
	putchar(188);
	gotoXY(120, 39);//between both above
	putchar(186);
	gotoXY(100, 38);//top-left
	putchar(201);
	gotoXY(100, 39);//middle
	putchar(186);
	gotoXY(100, 40);//bottom-left
	putchar(200);
	for (int i = 101; i < 120; i++)
	{
		gotoXY(i, 38);
		putchar(205);
		gotoXY(i, 40);
		putchar(205);
	}
}

void Draw::DrawChaNewPass()
{
	gotoXY(120, 33);//top-right
	putchar(187);
	gotoXY(120, 35);//bottom-right
	putchar(188);
	gotoXY(120, 34);//between both above
	putchar(186);
	gotoXY(40, 33);//top-left
	putchar(201);
	gotoXY(40, 34);//middle
	putchar(186);
	gotoXY(40, 35);//bottom-left
	putchar(200);
	//line top-bottom
	for (int i = 41; i < 120; i++)
	{
		gotoXY(i, 33);
		putchar(205);
		gotoXY(i, 35);
		putchar(205);
	}
}

void UI::setDefault()
{
	SetConsoleTitle(L"Final");
	ShowCur(0);
	SetWindowSize(150, 150);
	SetScreenBufferSize(200, 100);
}

void UI::setLoginUI()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	draw.DrawLogID();
	draw.DrawLogPass();
	draw.DrawLogConfirm();
	draw.DrawLogReg();
	draw.DrawLogForgot();
	draw.DrawLogCancel();
	//ID login
	gotoXY(45, 23);
	cout << "ID          :" << endl;
	//Pass login
	gotoXY(45, 29);
	cout << "PASSWORD    :" << endl;
	//enter
	gotoXY(45, 34);
	cout << "LOGIN" << endl;
	//register
	gotoXY(95, 34);
	cout << "REGISTER" << endl;
	//Forgot pass
	gotoXY(45, 39);
	cout << "README" << endl;
	gotoXY(105, 39);
	cout << "    QUIT" << endl;
}
void UI::setRegisterUI()
{
	clrscr();
	//ID
	gotoXY(45, 18);
	cout << "ID               :" << endl;
	//Pass 
	gotoXY(45, 23);
	cout << "PASSWORD         :" << endl;
	//PASS CONFIRM
	gotoXY(45, 28);
	cout << "PASSWORD CONFIRM :" << endl;
	//DATE
	gotoXY(45, 33);
	cout << "DAY :" << endl;
	gotoXY(75, 33);
	cout << "MONTH :" << endl;
	gotoXY(105, 33);
	cout << "YEAR :" << endl;
	//FULLNAME
	gotoXY(45, 38);
	cout << "FULL NAME        :" << endl;
	gotoXY(45, 43);
	//CREATE
	cout << "  CREATE  " << endl;
	//CANCEL
	gotoXY(105, 43);
	cout << "  CANCEL  " << endl;

	draw.DrawOut();
	draw.DrawRegId();
	draw.DrawRegPass();
	draw.DrawRegConfirmPass();
	draw.DrawRegDay();
	draw.DrawRegMonth();
	draw.DrawRegYear();
	draw.DrawRegFullName();
	draw.DrawRegCreate();
	draw.DrawRegCancel();
}
void UI::setChangeInforUI()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	draw.DrawChangeInfDay();
	draw.DrawChangeInfMonth();
	draw.DrawChangeInfYear();
	draw.DrawChangeInfName();
	draw.DrawChangeInfCancel();
	draw.DrawChangeInfConfirm();
	gotoXY(45, 23);
	cout << "FULL NAME       :" << endl;
	gotoXY(45, 30);
	cout << "DAY :" << endl;
	gotoXY(75, 30);
	cout << "MONTH :" << endl;
	gotoXY(105, 30);
	cout << "YEAR :" << endl;
	gotoXY(45, 37);
	cout << "  CONFIRM  " << endl;
	gotoXY(105, 37);
	cout << "  CANCEL  " << endl;
}
void UI::setChangePassUI()
{
	clrscr();
	draw.DrawIn();
	draw.DrawOut();
	draw.DrawLogPass();
	draw.DrawLogID();
	draw.DrawChaNewPass();
	draw.DrawLogConfirm();
	draw.DrawLogCancel();
	gotoXY(45, 23);
	cout << "ID                :" << endl;
	gotoXY(45, 29);
	cout << "CURRENT PASSWORD  :" << endl;
	gotoXY(45, 34);
	cout << "NEW PASSWORD      :" << endl;
	gotoXY(45, 39);
	cout << "   CONFIRM   " << endl;
	gotoXY(105, 39);
	cout << "   CANCEL   " << endl;
}

void UI::setDrawEncrypt()
{
	//clrscr();
	draw.DrawIn();
	draw.DrawOut();
	draw.DrawLogEncrypt();
}
