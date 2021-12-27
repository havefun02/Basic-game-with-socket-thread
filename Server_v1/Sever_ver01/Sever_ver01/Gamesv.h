#include"General.h"
#include"Servernetwork.h"
#include"Handle.h"
class ServerGame
{

public:
    Handler handler;
    ServerGame();
    void update();
    void receive(int idnet);
    ServerNetwork* network;
private:
    static unsigned int client_id; 
    
};