#include"GameClient.h"
#include"FileSystem.h"
ClientGame* client;
int main()
{
	UI dr;
	dr.draw.DrawControler();
	string t = FileSystem::ReadFileCSV("TEST_MAP.csv");
	dr.Showmap(t);
	
	cin.get();
	/*client = new ClientGame;
	client->ui.setDefault();

	while (1)
	{
		client->update();
	}*/
}
