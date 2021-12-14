#include"GameClient.h"
ClientGame* client;
int main()
{
	client = new ClientGame;
	client->ui.setDefault();

	while (1)
	{
		client->update();
	}
}
