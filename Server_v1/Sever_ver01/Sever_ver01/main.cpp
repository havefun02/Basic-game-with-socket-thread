#include"General.h"
#include"Gamesv.h"
#include"Servernetwork.h"
ServerGame *severgame;
int main()
{
	severgame = new ServerGame();
	while(1)
	_beginthread(Runsv, 0, (void*)12);
	cin.get();
}
void Runsv(void* arg)
{
	while (1)
	{
		severgame->update();
	}
}