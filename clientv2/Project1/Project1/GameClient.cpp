#include"ClientNetwork.h"
#include"GameClient.h"
#include"NetworkService.h"
#include"UiGame.h"
#include"FileSystem.h"
#include"EncryptAndDecryp.h"

ClientGame::ClientGame() {
	network = new ClientNetwork();
    player = new PlayerAccount();
    string packet_data = Encryption::Encrypt("Connect to sv:");

    packet_data = "1" + packet_data;
    send(network->ClientSocket, packet_data.c_str(), (int)strlen(packet_data.c_str()), 0);
	//NetworkService::sendMessage(network->ClientSocket, packet_data.c_str(), strlen(packet_data.c_str()));//init message
}

void ClientGame::Sendpacket(char*buffer)
{
	//send to sv
	if (strlen(buffer) <= 0) return;
	int res = NetworkService::sendMessage(network->ClientSocket, buffer, sizeof(buffer));

	//
}
void ClientGame::update()
{
    // tin hieu tu sever auto ma hoa
    // su dung Encrytion::Decrypt(string); de tin hieu duoc giai ma

    int data_length = network->Receive(network_data);
    if (data_length > 0)
    {
        string sig = string(network_data, 0, data_length);
        sig = Encryption::Decrypt(sig);
        if (sig.substr(0, 6) == "Login:") 
        {
            setsignal("Login");
            player->set_id(sig.substr(6));
        }
    }
    if (_signal == "Login")
        Login();
    if (_signal == "Register")
        Register();
    if (_signal == "ChangePass")
        Changepass();
    if (_signal == "ChangeInfor")
        Changeinfo();
    if (_signal == "UiClient")
        UiClient();
}


vector<PlayerAccount*> ClientGame::getOnlinelist(string format)
{           
    vector<PlayerAccount*> list;
    int beginLine = 0;
    int semicolon = format.find_first_of(";");
    // "id:1,name:abc,point:13;id:2,name:abd,point:15;" 2 client
    string line;

    if (semicolon < 0) {
        cout << "error at ClientGame::getOnlinelist(string format)" << endl;
        return list;
    }

    while (beginLine != format.size() - 1) {
        PlayerAccount* newClient = new PlayerAccount();
        line = format.substr(beginLine, semicolon - beginLine); //split format into a line
        int comma = line.find_first_of(',' );
        int twoDots = line.find_first_of(':');
        string temp = line.substr(twoDots + 1, comma - twoDots - 1); // id

        newClient->set_id(temp);
        twoDots = line.find_first_of(':', comma);
        comma = line.find_first_of(',', twoDots);
        temp = line.substr(twoDots + 1, comma - twoDots - 1); // name
        newClient->set_fullname(temp);

        twoDots = line.find_first_of(":", comma);
        temp = line.substr(twoDots + 1, line.size() - twoDots - 1); // point
        newClient->set_point(temp);

        list.push_back(newClient);

        beginLine = semicolon;
        if (beginLine == format.size() - 1) {
            break;
        }
        else beginLine++;
        semicolon = format.find_first_of(";", beginLine);
    }


    //cout << "Finish getonline" << endl;
    return list;
}

void ClientGame::UiClient()
{
    ui.setUiClient();
    int y = 23;
    int x = 40;
    int data_length = 0;
    string sig = "";
    gotoXY(40, 23); cout << ">>" << endl;
    while (1)
    {

        data_length = network->Receive(network_data);
        sig = string(network_data, 0, data_length);

        sig = Encryption::Decrypt(sig);

        string idplayer=sig.substr(5);
        sig = sig.substr(0, 4);
        if (data_length > 0 && sig == "Join")
        {
            clrscr();
            ui.draw.DrawOut();
            gotoXY(40, 25);
            cout << "Someone invite you to play";
            gotoXY(40, 27);
            cout << "Enter to play";
            while (1)
            {
                if (_kbhit()) {
                    char t = _getch();
                    if (t == 13)
                    {
                        //send to sv
                        string tmp = "Yes:"+ idplayer;
                        if (getaccess()) {
                            tmp = Encryption::Encrypt(tmp);
                            tmp = "1" + tmp;
                        }

                        send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                        break;
                    }
                    else
                    {
                        string tmp = "No:";

                        if (getaccess()) {
                            tmp = Encryption::Encrypt(tmp);
                            tmp = "1" + tmp;
                        }
                        send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                        setsignal("UiClient");
                        return;
                    }
                }
            }
            //first update map
            ui.ShowforReadFile();
            string smap;//map
            string map_send;
            string namef;
            int x1 = 75, ix1 = 75;
            while (1)
            {
                if (_kbhit())
                {
                    char t = _getch();
                    if (t == 13)
                    {
                        smap = FileSystem::ReadFileCSV(namef);
                        map_send = smap;
                        break;
                    }
                    else if (t != 8 && x1 >= 75 && x1 < 117)
                    {
                        //type id
                        gotoXY(x1, 24);
                        putchar(t);
                        namef.push_back(t);
                        x1++;
                        ix1 = x1;
                    }
                    else if (t == 8 && x1 > 75 && x1 <= 117)
                    {
                        //clear
                        gotoXY(x1, 24);
                        putchar(' ');
                        namef.erase(namef.length() - 1);
                        x1--;
                        ix1 = x1;
                    }
                }
            }
            map_send = "File:" + map_send;
            if (getaccess()) {
                map_send = Encryption::Encrypt(map_send);
                map_send = "1" + map_send;
            }


            send(network->ClientSocket, map_send.c_str(), (int)strlen(map_send.c_str()), 0);
           

            //wait
            int data_length1 = 0;
            string sig1;
            while (sig1!="StartGame")
            {
                data_length1 = network->Receive(network_data);
                sig1 = string(network_data, 0, data_length1);
                sig1 = Encryption::Decrypt(sig1);
            } 
            //send to play game

            if (sig1 == "StartGame")
            {
                string tm = "Start1:";
                if (getaccess()) {
                    tm = Encryption::Encrypt(tm);
                    tm = "1" + tm;
                }

                send(network->ClientSocket, tm.c_str(), (int)strlen(tm.c_str()), 0);
                while (1)
                {
                    ui.draw.DrawControler();
                    ui.Showmap(smap);
                    int dt=0;
                    string si;
                    while (1)
                    {

                        dt = network->Receive(network_data);
                        si = string(network_data, 0, dt);

                        si = Encryption::Decrypt(si);
                        if (si == "Wait!") {
                            gotoXY(105, 40);
                            cout << "                  ";
                            //Sleep(100);
                            gotoXY(105, 40);
                            cout << "Wait";
                            continue;
                        }

                        else if (si == "Your turn!")
                        {
                            gotoXY(105, 40);
                            cout << "Your turn";
                            string Id = "", CurPass = "";
                            int x = 65, y = 40, ix = x, pcx = x;
                            gotoXY(37, 40); cout << ">>" << endl;
                            while (1)
                            {
                                if (y != 44) ui.cur(x, y);
                                if (_kbhit())
                                {
                                    char t = _getch();
                                    if (t == 9)
                                    {
                                        if (y == 40)
                                        {
                                            x = pcx; y = 42;
                                            gotoXY(37, 42);
                                            cout << ">>" << endl;
                                            gotoXY(37, 40);
                                            cout << "  " << endl;
                                        }
                                        else if (y == 42)
                                        {
                                            y = 44;
                                            gotoXY(37, 44);
                                            cout << ">>" << endl;
                                            gotoXY(37, 42);
                                            cout << "  " << endl;
                                        }
                                        else if (y == 44)
                                        {
                                            x = ix; y = 40;
                                            gotoXY(37, 40);
                                            cout << ">>" << endl;
                                            gotoXY(37, 44);
                                            cout << "  " << endl;
                                        }
                                        continue;
                                    }
                                    else if (t != 8 && x >= 65 && x <= 117 && y == 40)
                                    {
                                        //type password
                                        gotoXY(x, y);
                                        CurPass.push_back(t);
                                        putchar(t);
                                        x++;
                                        pcx = x;
                                    }
                                    else if (CurPass != "" && Id != "" && t == 13 && y == 44)
                                    {
                                        //send to sv
                                        if (stoi(Id) < 10)
                                            Id = "0" + Id;//x
                                        if (stoi(CurPass) < 10)
                                            CurPass = "0" + CurPass;//y
                                        string packet = "atk:" + Id + CurPass;
                                        if (getaccess()) {
                                            packet = Encryption::Encrypt(packet);
                                            packet = "1" + packet;
                                        }
                                        send(network->ClientSocket, packet.c_str(), (int)strlen(packet.c_str()), 0);
                                        break;
                                       // dt = network->Receive(network_data);
                                       // si = string(network_data, 0, dt);
                                       //
                                       // si = Encryption::Decrypt(si);
                                       //
                                       // smap = si;
                                       // break;
                                    }
                                    else if (t != 8 && x >= 65 && x <= 117 && y == 42)
                                    {
                                        //type id
                                        gotoXY(x, y);
                                        putchar(t);
                                        Id.push_back(t);
                                        x++;
                                        ix = x;
                                    }
                                    else if (t == 8 && x > 65 && x <= 117)
                                    {
                                        //clear
                                        gotoXY(x, y);
                                        putchar(' ');
                                        CurPass.erase(CurPass.length() - 1);
                                        x--;
                                        pcx = x;
                                    }
                                    else if (t == 8 && x > 65 && x <= 117)
                                    {
                                        gotoXY(x, y);
                                        putchar(' ');
                                        Id.erase(Id.length() - 1);
                                        x--;
                                        ix = x;
                                    }
                                }
                            }
                        }
                        else if (si == "Win") {
                            gotoXY(105, 40);
                            cout << "You Win";
                            Sleep(500);
                            setsignal("UiClient");
                            return;
                        }
                        else if (si == "Lose") {
                            gotoXY(105, 40);
                            cout << "You lose";
                            Sleep(500);
                            setsignal("UiClient");
                            return;
                        }
                        else if (si != "Your turn!") {
                            smap = si;
                            break;
                        }
                    }
                }
            }
            else
            {
                setsignal("UiClient");
                return;
            }


        }
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y == 23)
                {
                    y = 27;
                    gotoXY(40, 27);
                    cout << ">>" << endl;
                    gotoXY(40, 23);
                    cout << "  " << endl;

                }
                else if (y == 27)
                {
                    y = 31;
                    gotoXY(40, 31);
                    cout << ">>" << endl;
                    gotoXY(40, 27);
                    cout << "  " << endl;
                }
                else if (y == 31)
                {
                    y = 35;
                    gotoXY(40, 35);
                    cout << ">>" << endl;
                    gotoXY(40, 31);
                    cout << "  " << endl;
                }
                else if (y == 35)
                {
                    y = 39;
                    gotoXY(40, 39);
                    cout << ">>" << endl;
                    gotoXY(40, 35);
                    cout << "  " << endl;
                }
                else if (y == 39)
                {
                    y = 23;
                    gotoXY(40, 23);
                    cout << ">>" << endl;
                    gotoXY(40, 39);
                    cout << "  " << endl;
                }

                continue;
            }
            if (t == 13)
            {
                switch (y) {
                case 39:
                    setsignal("Login");
                    return;
                case 23:
                    Playgame();
                    //
                    break;
                case 27:
                    Changepass();
                    return;
                    //
                    break;
                case 31:
                    Changeinfo();
                    return;
                    //
                    break;
                case 35:
                    CheckUser();
                    return;
                    //
                    break;
                default:
                    break;
                }
            }
        }
    }
}

void ClientGame::CheckUser()
{
    clrscr();
    ui.draw.DrawOut();
    ui.draw.DrawIn();
    gotoXY(45, 21);
    int x = 80, y = 21;
    PlayerAccount* user1 = new PlayerAccount;
    string name = "";
    cout << "FIND ACCOUNT BY ID     :" << endl;
    while (1)
    {
        ui.cur(x, y);
        if (_kbhit)
        {
            char t = _getch();
            if (t != 8 && x < 100 && t!=13) {
                gotoXY(x, y);
                putchar(t);
                name.push_back(t);
                x++;
            }
            else if (t == 8 && x > 80 && x <= 100)
            {
                //clear
                gotoXY(x, y);
                putchar(' ');
                name.erase(name.length() - 1);
                x--;
            }
            else if (t == 13 && y==21)
            {
                //send to sv to check
                int data;
                string sig;
                string tmp = "Find:" + name;
                if (getaccess())
                {
                    tmp = Encryption::Encrypt(tmp);
                    tmp = "1" + tmp;
                }

                send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                break;
                gotoXY(40, 22);
                cout << "                                                         ";
                while (1)
                {
                    //wait for receive

                    data = network->Receive(network_data);
                    sig = string(network_data, 0, data);

                    sig = Encryption::Decrypt(sig);

                    if (data > 0)
                        break;
                }
                if (sig == "Yes")
                {
                    while (1)
                    {
                        //wait for receive
                        //receive
                        data = network->Receive(network_data);
                        sig = string(network_data, 0, data);

                        sig = Encryption::Decrypt(sig);

                        if (data > 0)
                            break;
                    }
                    cout << sig;
                    int ind = sig.find_first_of(",");
                    int ind1 = sig.find_first_of(".");
                    int ind2 = sig.find_first_of("/");
                    int ind3 = sig.find_first_of("+");
                    int ind4 = sig.find_first_of("-");
                    string fullname = sig.substr(0, ind);
                    string day = sig.substr(ind + 1, ind1 - ind - 1);
                    string month = sig.substr(ind1 + 1, ind2 - ind1 - 1);
                    string year = sig.substr(ind2 + 1, ind3 - ind2 - 1);
                    string point = sig.substr(ind3 + 1, ind4 - ind3 - 1);
                    string isonl = sig.substr(ind4 + 1);
                    user1->set_fullname(fullname);
                    user1->set_day(day);
                    user1->set_month(month);
                    user1->set_year(year);
                    user1->set_onl(isonl);
                    user1->set_point(point);
                    ui.setCheckuser();
                    int y1 = 20;
                    gotoXY(40, 20); cout << ">>" << endl;
                    while (1)
                    {
                        if (_kbhit())
                        {
                            char t = _getch();
                            if (t == 9)
                            {
                                if (y1 == 20)
                                {
                                    y1 = 23;
                                    gotoXY(40, 23);
                                    cout << ">>" << endl;
                                    gotoXY(40, 20);
                                    cout << "  " << endl;

                                }
                                else if (y1 == 23)
                                {
                                    y1 = 26;
                                    gotoXY(40, 26);
                                    cout << ">>" << endl;
                                    gotoXY(40, 23);
                                    cout << "  " << endl;
                                }
                                else if (y1 == 26)
                                {
                                    y1 = 29;
                                    gotoXY(40, 29);
                                    cout << ">>" << endl;
                                    gotoXY(40, 26);
                                    cout << "  " << endl;
                                }
                                else if (y1 == 29)
                                {
                                    y1 = 32;
                                    gotoXY(40, 32);
                                    cout << ">>" << endl;
                                    gotoXY(40, 29);
                                    cout << "  " << endl;
                                }
                                else if (y1 == 32)
                                {
                                    y1 = 35;
                                    gotoXY(40, 35);
                                    cout << ">>" << endl;
                                    gotoXY(40, 32);
                                    cout << "  " << endl;
                                }
                                else if (y1 == 35)
                                {
                                    y1 = 38;
                                    gotoXY(40, 38);
                                    cout << ">>" << endl;
                                    gotoXY(40, 35);
                                    cout << "  " << endl;
                                }
                                else if (y1 == 38)
                                {
                                    y1 = 20;
                                    gotoXY(40, 20);
                                    cout << ">>" << endl;
                                    gotoXY(40, 38);
                                    cout << "  " << endl;
                                }

                                continue;
                            }
                            if (t == 13)
                            {
                                switch (y1) {
                                case 38:
                                    setsignal("UiClient");
                                    return;
                                case 20:
                                    //check online
                                    if (user1->IsOnline())
                                    {
                                        gotoXY(60, 20);
                                        cout << "Online";
                                        Sleep(500);
                                        cout << "             ";
                                    }
                                    else
                                    {
                                        gotoXY(60, 20);
                                        cout << "Offline";
                                        Sleep(500);
                                        cout << "             ";
                                    }
                                    break;
                                case 23:
                                    //check date 
                                    gotoXY(60, 23);
                                    user1->set_date(day, month, year);
                                    cout << user1->date();
                                    Sleep(500);
                                    cout << "               ";
                                    break;
                                case 26:
                                    //check name
                                    gotoXY(60, 26);
                                    user1->fullname();
                                    Sleep(500);
                                    cout << "                 ";
                                    break;
                                case 29:
                                    //note
                                    gotoXY(60, 29);
                                    cout << "empty";
                                    Sleep(500);
                                    cout << "                 ";
                                    break;
                                case 32:
                                    //point
                                    gotoXY(60, 32);
                                    user1->point();
                                    Sleep(500);
                                    cout << "                 ";
                                    break;
                                case 35:
                                    //show all
                                    if (user1->IsOnline())
                                    {
                                        gotoXY(60, 20);
                                        cout << "Online";
                                    }
                                    else
                                    {
                                        gotoXY(60, 20);
                                        cout << "Offline";
                                    } 

                                    gotoXY(60, 23);
                                    user1->set_date(day, month, year);
                                    cout << user1->date();

                                    gotoXY(60, 26);
                                    cout<<user1->fullname();

                                    gotoXY(60, 29);
                                    cout << "empty";

                                    gotoXY(60, 32);
                                    cout<<user1->point();

                                    break;
                                default:
                                    break;
                                }
                            }
                        }

                    }
                }
                else if (sig == "No")
                {
                    gotoXY(40, 22);
                    cout << "NO EXIST" << endl;
                    Sleep(500);
                    gotoXY(40, 22);
                    cout << "        " << endl;
                }
            }
        }
    }
}

void ClientGame::Login()
{
    ui.setLoginUI();
    bool Checkid = true, Checkpass = false, Create = false, Create1 = false;
    bool quit = false;
    string Id = "", Pass = "";
    int x = 61, y = 23, px = x, ix = x;
    gotoXY(37, 23); cout << ">>" << endl;
    while (1)
    {
        if (y != 39 && y != 34) ui.cur(x, y);
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y == 23)
                {
                    Checkid = false;
                    Checkpass = true;
                    x = px; y = 29;
                    gotoXY(37, 29);
                    cout << ">>" << endl;
                    gotoXY(37, 23);
                    cout << "  " << endl;

                }
                else if (y == 29 && !Create)
                {
                    Create = true;
                    Checkpass = false;
                    y = 34;
                    gotoXY(37, 34);
                    cout << ">>" << endl;
                    gotoXY(37, 29);
                    cout << "  " << endl;
                }
                else if (y == 34 && Create)
                {
                    Create = false;
                    gotoXY(70, 34);
                    cout << ">>" << endl;
                    gotoXY(37, 34);
                    cout << "  " << endl;
                }
                else if (y == 34 && !Create1)
                {
                    Create1 = true;
                    gotoXY(37, 39);
                    y = 39;
                    cout << ">>" << endl;
                    gotoXY(70, 34);
                    cout << "  " << endl;
                }
                else if (y == 39 && Create1)
                {
                    Create1 = false;
                    quit = true;
                    gotoXY(97, 39);
                    cout << ">>" << endl;
                    gotoXY(37, 39);
                    cout << "  " << endl;
                }
                else if (y == 39 && !Create1)
                {
                    Checkid = true;
                    quit = false;
                    x = ix; y = 23;
                    gotoXY(37, 23);
                    cout << ">>" << endl;
                    gotoXY(97, 39);
                    cout << "  " << endl;
                }
                continue;
            }
            if (Checkid == false && Checkpass == false && !Create && !Create1 && !quit && t == 13 && y==34)
            {
                //register
                setsignal("Register");
                return;
            }
            else
                //send to sv to check
                if (Checkid == false && Checkpass == false && Create && !Create1 && !quit && t == 13 && y==34)
                {
                    int data;
                    string sig;
                    string tmp = "Login:" + Id + "," + Pass+"."+player->id();
                    clrscr();
                    while (1)
                    {
                        ui.setDrawEncrypt();
                        if (_kbhit()) {
                            char t = _getch();
                            if (t == 13)
                            {
                                //encrypt

                                tmp = Encryption::Encrypt(tmp);
                                tmp = "1" + tmp;
                                setaccess(1);
                                send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                                break;
                            }
                            else
                            {
                                setaccess(0);
                                send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                                break;
                            }
                        }
                    }

                    while (1)
                    {
                        //wait for receive

                        data = network->Receive(network_data);
                        sig = string(network_data, 0, data);

                        sig = Encryption::Decrypt(sig);

                        if (data > 0)
                            break;
                    }
                    

                    if (sig == "Yes")
                    {
                        while (1)
                        {
                            //wait for receive
                            data = network->Receive(network_data);
                            sig = string(network_data, 0, data);
                            sig = Encryption::Decrypt(sig);
                            if (data > 0)
                                break;
                        }
                        setsignal("UiClient");
                        int ind = sig.find_first_of(",");
                        int ind1 = sig.find_first_of(".");
                        int ind2 = sig.find_first_of("/");
                        int ind3 = sig.find_first_of("+");
                        int ind4 = sig.find_first_of("-");
                        string id = sig.substr(0, ind);
                        string pass = sig.substr(ind + 1, ind1 - ind - 1);
                        string fullname = sig.substr(ind1 + 1, ind2 - ind1 - 1);
                        string day = sig.substr(ind2 + 1, ind3 - ind2 - 1);
                        string month = sig.substr(ind3 + 1, ind4 - ind3 - 1);
                        string year = sig.substr(ind4 + 1, sig.size() - ind4 - 1);
                        player->set_username(id);
                        player->set_password(pass);
                        player->set_fullname(fullname);
                        player->set_day(day);
                        player->set_month(month);
                        player->set_year(year);
                        return;
                    }
                    else if (sig == "No")
                    {
                        ui.DrawFailLogin();
                        setsignal("Login");
                        return;
                    }

                }
                else if (Checkid == false && Checkpass == false && !Create && Create1 && !quit && t == 13 && y==39)
                {
                    ui.readme();
                    while (1)
                    {
                        if (_kbhit()) {
                            char t = _getch();
                            if (t == 13) {
                                cout << _signal;
                                setsignal("Login");
                                return;
                            }
                        }
                    }
                }
                else if (Checkid == false && Checkpass == false && !Create && !Create1 && quit && t == 13 && y==39)
                {
                    string tmp = "END:";//-------------------
                

                    send(network->ClientSocket, tmp.c_str(), strlen(tmp.c_str()), 0);
                    shutdown(network->ClientSocket, SD_SEND);
                    closesocket(network->ClientSocket);
                    WSACleanup();
                    exit(0);
                }
                else if (t != 8 && x > 60 && x <= 117 && Checkpass)
                {
                    //type password
                    gotoXY(x, y);
                    Pass.push_back(t);
                    putchar('*');
                    x++;
                    px = x;
                }
                else if (t != 8 && x > 60 && x <117 && Checkid)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Id.push_back(t);
                    x++;
                    ix = x;
                }
                else if (t == 8 && x > 61 && x < 117 && Checkpass)
                {
                    //clear
                    gotoXY(x, y);
                    putchar(' ');
                    Pass.erase(Pass.length() - 1);
                    x--;
                    px = x;
                }
                else if (t == 8 && x > 61 && x <= 117 && Checkid)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Id.erase(Id.length() - 1);
                    x--;
                    ix = x;
                }
        }
    }
}

void ClientGame::Changeinfo()
{
    ui.setChangeInforUI();
    bool Checkday = false, Checkmonth = false, Checkyear = false, Checkfullname = true, Create = false;
    string  Fullname = "", Day = "", Month = "", Year = "";
    int x = 45 + 20, y = 23, pd = 50, pm = 82, py = 111, pfn = x;
    gotoXY(37, 23); cout << ">>" << endl;
    while (1)
    {
        if (y != 37) ui.cur(x, y);
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y == 23)
                {
                    Checkfullname = false;
                    Checkday = true;
                    y = 30;
                    x = pd;
                    gotoXY(37, 30);
                    cout << ">>" << endl;
                    gotoXY(37, 23);
                    cout << "  " << endl;

                }
                else if (y == 30 && Checkday)
                {
                    Checkday = false;
                    Checkmonth = true;
                    x = pm;
                    gotoXY(67, 30);
                    cout << ">>" << endl;
                    gotoXY(37, 30);
                    cout << "  " << endl;
                }
                else if (y == 30 && Checkmonth)
                {
                    Checkmonth = false;
                    Checkyear = true;
                    x = py;
                    gotoXY(97, 30);
                    cout << ">>" << endl;
                    gotoXY(67, 30);
                    cout << "  " << endl;
                }
                else if (y == 30 && Checkyear)
                {
                    Create = true;
                    Checkyear = false;
                    y = 37;
                    gotoXY(37, 37);
                    cout << ">>" << endl;
                    gotoXY(97, 30);
                    cout << "  " << endl;
                }
                else if (y == 37 && Create)
                {
                    Create = false;
                    gotoXY(97, 37);
                    cout << ">>" << endl;
                    gotoXY(37, 37);
                    cout << "  " << endl;
                }
                else if (y == 37 && !Create)
                {
                    Create = true;
                    Checkfullname = true;
                    x = pfn;
                    y = 23;
                    gotoXY(37, 23);
                    cout << ">>" << endl;
                    gotoXY(97, 37);
                    cout << "  " << endl;
                }

                continue;
            }
            if (!Checkday && !Checkmonth && !Checkyear && !Checkfullname && !Create && t == 13)
            {
                //return;
                setsignal("UiClient");
                return;
            }
            else
                //send to sv to check
                if (!Checkday && !Checkmonth && !Checkyear && !Checkfullname && Create && t == 13)
                {
                    string tmp = "Changeinfo:" + player->username() + "-" + player->password() + "+" + Fullname + "/" + Day + "," + Month + "." + Year;
                    send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                    int data;
                    string sig;
                    
                    if (getaccess())
                    {
                        tmp = Encryption::Encrypt(tmp);
                        tmp = "1" + tmp;
                     }

                    send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                            

                    while (1)
                    {
                        //wait for receive
                        data = network->Receive(network_data);
                        sig = string(network_data, 0, data);

                        sig = Encryption::Decrypt(sig);

                        if (data > 0)
                            break;
                    }
                    if (sig == "Yes")
                    {
                        ui.DrawChangeinfoSuccess();
                        setsignal("UiClient");
                        return;
                    }
                    else if (sig == "No")
                    {
                        ui.DrawFailChangeinfo();
                        setsignal("ChangeInfo");
                        return;
                    }
                }
                else if (t != 8 && x >= 50 && x < 52 && Checkday)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Day.push_back(t);
                    x++;
                    pd = x;
                }
                else if (t != 8 && x >= 82 && x < 84 && Checkmonth)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Month.push_back(t);
                    x++;
                    pm = x;
                }
                else if (t != 8 && x >= 111 && x < 115 && Checkyear)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Year.push_back(t);
                    x++;
                    py = x;
                }
                else if (t != 8 && x >= 65 && x < 115 && Checkfullname)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Fullname.push_back(t);
                    x++;
                    pfn = x;
                }


                else if (t == 8 && x > 50 && x <= 53 && Checkday)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Day.erase(Day.length() - 1);
                    x--;
                    pd = x;
                }
                else if (t == 8 && x > 82 && x <= 84 && Checkmonth)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Month.erase(Month.length() - 1);
                    x--;
                    pm = x;
                }
                else if (t == 8 && x > 111 && x <= 115 && Checkyear)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Year.erase(Year.length() - 1);
                    x--;
                    py = x;
                }
                else if (t == 8 && x > 65 && x <= 115 && Checkfullname)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Fullname.erase(Fullname.length() - 1);
                    x--;
                    pfn = x;
                }

        }
    }
}

void ClientGame::Register()
{
    ui.setRegisterUI();
    bool Checkid = true, Checkpass = false, Create = false, Checkcurpass = false;
    bool Checkday = false, Checkmonth = false, Checkyear = false, Checkfullname = false;
    string Id = "", CurPass = "", Pass = "", Fullname = "", Day = "", Month = "", Year = "";
    int x = 45 + 20, y = 18, px = x, ix = x, pcx = x, pd = 51, pm = 83, py = 112, pfn = 65;
    gotoXY(37, 18); cout << ">>" << endl;
    while (1)
    {
        if (y != 43) ui.cur(x, y);
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y == 18)
                {
                    Checkid = false;
                    Checkcurpass = true;
                    x = pcx; y = 23;
                    gotoXY(37, 23);
                    cout << ">>" << endl;
                    gotoXY(37, 18);
                    cout << "  " << endl;

                }
                else if (y == 23)
                {
                    Checkcurpass = false;
                    Checkpass = true;
                    x = px; y = 28;
                    gotoXY(37, 28);
                    cout << ">>" << endl;
                    gotoXY(37, 23);
                    cout << "  " << endl;
                }
                else if (y == 28)
                {
                    Checkpass = false;
                    Checkday = true;
                    x = pd;
                    y = 33;
                    gotoXY(37, 33);
                    cout << ">>" << endl;
                    gotoXY(37, 28);
                    cout << "  " << endl;
                }
                else if (y == 33 && Checkday)
                {
                    Checkmonth = true;
                    Checkday = false;
                    x = pm;
                    gotoXY(67, 33);
                    cout << ">>" << endl;
                    gotoXY(37, 33);
                    cout << "  " << endl;
                }
                else if (y == 33 && Checkmonth)
                {
                    Checkmonth = false;
                    Checkyear = true;
                    x = py;
                    gotoXY(97, 33);
                    cout << ">>" << endl;
                    gotoXY(67, 33);
                    cout << "  " << endl;
                }
                else if (y == 33 && Checkyear)
                {
                    Checkfullname = true;
                    Checkyear = false;
                    x = pfn;
                    y = 38;
                    gotoXY(37, 38);
                    cout << ">>" << endl;
                    gotoXY(97, 33);
                    cout << "  " << endl;
                }
                else if (y == 38)
                {
                    Checkfullname = false;
                    Create = true;
                    x = ix; y = 43;
                    gotoXY(37, 43);
                    cout << ">>" << endl;
                    gotoXY(37, 38);
                    cout << "  " << endl;
                }
                else if (y == 43 && Create)
                {
                    Create = false;
                    gotoXY(97, 43);
                    cout << ">>" << endl;
                    gotoXY(37, 43);
                    cout << "  " << endl;
                }
                else if (y == 43 && !Create)
                {
                    Checkid = true;
                    ix = x;
                    y = 18;
                    gotoXY(37, 18);
                    cout << ">>" << endl;
                    gotoXY(97, 43);
                    cout << "  " << endl;
                }

                continue;
            }
            if (Checkid == false && Checkpass == false && Checkcurpass == false && Create == false && t == 13 && y==43)
            {
                setsignal("Login");
                return;
            }
            else
                //send to sv to check
                if (Checkid == false && Checkpass == false && Checkcurpass == false && Create == true && t == 13 && y==43)
                {
                    int data;
                    string sig;
                    string tmp = "Register:" + Id + "," + Pass + "." + Fullname + "/" + Day + "+" + Month + "-" + Year;
                    if (getaccess())
                    {
                        tmp = Encryption::Encrypt(tmp);
                        tmp = "1" + tmp;
                    }

                    send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0); 
                    while (1)
                    {
                        //wait for receive

                        data = network->Receive(network_data);
                        sig = string(network_data, 0, data);

                        sig = Encryption::Decrypt(sig);


                        if (data > 0)
                            break;
                    }
                    if (sig == "Yes")
                    {
                        setsignal("Login");
                        return;
                    }
                    else if (sig == "No")
                    {
                        ui.DrawFailReg();
                        setsignal("Register");
                        return;
                    }
                }
                else if (t != 8 && x >= 65 && x < 117 && Checkpass)
                {
                    //type password
                    gotoXY(x, y);
                    Pass.push_back(t);
                    putchar('*');
                    x++;
                    px = x;
                }
                else if (t != 8 && x >= 65 && x < 117 && Checkcurpass)
                {
                    //type password
                    gotoXY(x, y);
                    CurPass.push_back(t);
                    putchar('*');
                    x++;
                    pcx = x;
                }
                else if (t != 8 && x >= 65 && x < 117 && Checkid)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Id.push_back(t);
                    x++;
                    ix = x;
                }
                else if (t != 8 && x > 50 && x < 53 && Checkday)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Day.push_back(t);
                    x++;
                    pd = x;
                }
                else if (t != 8 && x > 82 && x < 85 && Checkmonth)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Month.push_back(t);
                    x++;
                    pm = x;
                }
                else if (t != 8 && x >= 111 && x < 116 && Checkyear)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Year.push_back(t);
                    x++;
                    py = x;
                }
                else if (t != 8 && x >= 65 && x < 116 && Checkfullname)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Fullname.push_back(t);
                    x++;
                    pfn = x;
                }

                else if (t == 8 && x > 65 && x < 117 && Checkcurpass)
                {
                    //clear
                    gotoXY(x, y);
                    putchar(' ');
                    CurPass.erase(CurPass.length() - 1);
                    x--;
                    px = x;
                }
                else if (t == 8 && x > 65 && x < 117 && Checkpass)
                {
                    //clear
                    gotoXY(x, y);
                    putchar(' ');
                    Pass.erase(Pass.length() - 1);
                    x--;
                    pcx = x;
                }
                else if (t == 8 && x > 65 && x < 117 && Checkid)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Id.erase(Id.length() - 1);
                    x--;
                    ix = x;
                }
                else if (t == 8 && x > 50 && x <= 53 && Checkday)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Day.erase(Day.length() - 1);
                    x--;
                    pd = x;
                }
                else if (t == 8 && x > 81 && x <= 84 && Checkmonth)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Month.erase(Month.length() - 1);
                    x--;
                    pm = x;
                }
                else if (t == 8 && x > 111 && x <= 116 && Checkyear)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Year.erase(Year.length() - 1);
                    x--;
                    py = x;
                }
                else if (t == 8 && x > 65 && x <= 117 && Checkfullname)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Fullname.erase(Fullname.length() - 1);
                    x--;
                    pfn = x;
                }

        }
    }
}

void ClientGame::Changepass()
{
    ui.setChangePassUI();
    bool Checkid = true, Checkpass = false, Create = false, Checkcurpass = false;
    string Id = "", CurPass = "", Pass = "";
    int x = 45 + 20, y = 23, px = x, ix = x, pcx = x;
    gotoXY(37, 23); cout << ">>" << endl;
    while (1)
    {
        if (y != 39) ui.cur(x, y);
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y == 23)
                {
                    Checkid = false;
                    Checkcurpass = true;
                    x = pcx; y = 29;
                    gotoXY(37, 29);
                    cout << ">>" << endl;
                    gotoXY(37, 23);
                    cout << "  " << endl;

                }
                else if (y == 29)
                {
                    Checkcurpass = false;
                    Checkpass = true;
                    x = px; y = 34;
                    gotoXY(37, 34);
                    cout << ">>" << endl;
                    gotoXY(37, 29);
                    cout << "  " << endl;
                }
                else if (y == 34)
                {
                    Checkpass = false;
                    x = px; y = 39;
                    gotoXY(37, 39);
                    cout << ">>" << endl;
                    gotoXY(37, 34);
                    cout << "  " << endl;
                }
                else if (y == 39 && !Create)
                {
                    Create = true;
                    Checkpass = false;
                    gotoXY(97, 39);
                    cout << ">>" << endl;
                    gotoXY(37, 39);
                    cout << "  " << endl;
                }
                else if (y == 39 && Create)
                {
                    Checkid = true;
                    Create = false;
                    x = ix; y = 23;
                    gotoXY(37, 23);
                    cout << ">>" << endl;
                    gotoXY(97, 39);
                    cout << "  " << endl;
                }

                continue;
            }
            if (Checkid == false && Checkpass == false && Checkcurpass == false && Create == true && t == 13 && y==39)
            {
                setsignal("UiClient");
                //return;
                return;
            }
            else
                //send to sv to check
                if (Checkid == false && Checkpass == false && Checkcurpass == false && Create == false && t == 13 && y==39)
                {
                    string tmp = "Changepass:" + Id + "," + CurPass + "." + Pass;
                    tmp = Encryption::Encrypt(tmp);
                    send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                    int data;
                    string sig;
                    if (getaccess())
                    {
                        tmp = Encryption::Encrypt(tmp);
                        tmp = "1" + tmp;
                    }
                         
                    send(network->ClientSocket, tmp.c_str(), (int)strlen(tmp.c_str()), 0);
                    
                    while (1)
                    {
                        //wait for receive

                        data = network->Receive(network_data);
                        sig = string(network_data, 0, data);

                        sig = Encryption::Decrypt(sig);

                        if (data > 0)
                            break;
                    }
                    if (sig == "Yes")
                    {
                        ui.DrawChangePassSuccess();
                        setsignal("UiClient");
                        return;
                    }
                    else if (sig == "No")
                    {
                        ui.DrawFailChangepass();
                        setsignal("ChangePass");
                        return;
                    }
                }
                else if (t != 8 && x >= 65 && x <= 117 && Checkpass)
                {
                    //type password
                    gotoXY(x, y);
                    Pass.push_back(t);
                    putchar('*');
                    x++;
                    px = x;
                }
                else if (t != 8 && x >= 65 && x <= 117 && Checkcurpass)
                {
                    //type password
                    gotoXY(x, y);
                    CurPass.push_back(t);
                    putchar('*');
                    x++;
                    pcx = x;
                }
                else if (t != 8 && x >= 65 && x <= 117 && Checkid)
                {
                    //type id
                    gotoXY(x, y);
                    putchar(t);
                    Id.push_back(t);
                    x++;
                    ix = x;
                }
                else if (t == 8 && x > 65 && x <= 117 && Checkcurpass)
                {
                    //clear
                    gotoXY(x, y);
                    putchar(' ');
                    CurPass.erase(CurPass.length() - 1);
                    x--;
                    px = x;
                }
                else if (t == 8 && x > 65 && x <= 117 && Checkpass)
                {
                    //clear
                    gotoXY(x, y);
                    putchar(' ');
                    Pass.erase(Pass.length() - 1);
                    x--;
                    pcx = x;
                }
                else if (t == 8 && x > 65 && x <= 117 && Checkid)
                {
                    gotoXY(x, y);
                    putchar(' ');
                    Id.erase(Id.length() - 1);
                    x--;
                    ix = x;
                }
        }
    }
}

void ClientGame::Playgame() {
    //send request to sv to show list online player

    string req = "ListOnline";
    if (getaccess())
    {
        string req = Encryption::Encrypt("ListOnline");
        req = "1" + req;
    }
    send(network->ClientSocket, req.c_str(), (int)strlen(req.c_str()), 0);

    int data = 0;
    while (data<=0)
    {
        data = network->Receive(network_data);
    }
   
    string sig = string(network_data, 0, data);

    sig = Encryption::Decrypt(sig);

    Onlinelist = getOnlinelist(sig);    //error att
    ui.ShowOnlinePlayer(Onlinelist);
    int x1 = 40, y1 = 22;
    if (data == 0)
    {
        gotoXY(40, 44);
        cout << ">>";
    }
    else
    {
        gotoXY(40, 22);
        cout << ">>";
    }
    while (1)
    {
        if (_kbhit())
        {
            char t = _getch();
            if (t == 9)
            {
                if (y1 < Onlinelist.size() * 2 + 20)
                {
                    y1 += 2;
                    gotoXY(x1, y1);
                    cout << ">>";
                    gotoXY(x1, y1 - 2);
                    cout << "  ";
                }
                else if (y1 == 20 + Onlinelist.size() * 2)
                {
                    gotoXY(40, 44);
                    cout << ">>";
                    gotoXY(40, 20 + Onlinelist.size() * 2);
                    cout << "  ";
                }
            }
            else if (t == 13 && y1 == 44)
            {
                setsignal("UiClient");
                return;
            }
            else if (t == 13)
            {
                //send to sv

                int data1 = 0;
                string sig1;

                string re = "ConnectClient:" + Onlinelist[((y1 - 20) / 2)-1]->id();

                if (getaccess())
                {
                    string re = Encryption::Encrypt(re);
                    re = "1" + re;
                }

                send(network->ClientSocket, re.c_str(), strlen(re.c_str()), 0);
                while (1)
                {
                    data1 = network->Receive(network_data);
                    sig1 = string(network_data, 0, data1);

                    sig1 = Encryption::Decrypt(sig1);

                    if (data1 > 0)break;
                }

                if (sig1 == "Yes")
                {
                    break;
                }
                else
                {
                    setsignal("UiClient");
                    return;
                }
            }
        }
    }
    //input file map
    ui.ShowforReadFile();
    string smap;//map
    string map_send;
    string namef;
    x1 = 75; int ix1 = 75;
    while (1)
    {
        if (_kbhit())
        {
            char t = _getch();
            if (t == 13)
            {
                smap = FileSystem::ReadFileCSV(namef);
                map_send = smap;
                break;
            }
            else if (t != 8 && x1 >= 75 && x1 < 117)
            {
                //type id
                gotoXY(x1, 24);
                putchar(t);
                namef.push_back(t);
                x1++;
                ix1 = x1;
            }
            else if (t == 8 && x1 > 75 && x1 <= 117)
            {
                //clear
                gotoXY(x1, 24);
                putchar(' ');
                namef.erase(namef.length() - 1);
                x1--;
                ix1 = x1;
            }
        }
    }
    map_send = "File:" + map_send;
    if (getaccess())
    {
        map_send = Encryption::Encrypt(map_send);
        map_send = "1" + map_send;
    }
    send(network->ClientSocket, map_send.c_str(), (int)strlen(map_send.c_str()), 0);

    //
    int data_length1 = 0;
    while (sig!="StartGame")
    {
        data_length1=network->Receive(network_data);
        sig = string(network_data, 0, data_length1);

        sig = Encryption::Decrypt(sig);
    }
    //send to play game
    if (sig == "StartGame")
    {
        string tm = "Start:";
        if (getaccess())
        {
            tm = Encryption::Encrypt(tm);
            tm = "1" + tm;
        }

        send(network->ClientSocket, tm.c_str(), (int)strlen(tm.c_str()), 0);
       
        while (1)
        {
            ui.draw.DrawControler();
            ui.Showmap(smap);
            while (1) {
                data_length1 = network->Receive(network_data);
                sig = string(network_data, 0, data_length1);

                sig = Encryption::Decrypt(sig);

               /* cout << sig << endl;
                Sleep(1000);*/

                if (sig == "Wait!")
                {
                    gotoXY(105, 40);
                    cout << "                  ";
                    //Sleep(100);
                    gotoXY(105, 40);
                    cout << "Wait";
                    continue;
                }
                else if (sig == "Your turn!")
                {
                    gotoXY(105, 40);
                    cout << "Your turn";
                    string Id = "", CurPass = "";
                    int x = 65, y = 40, ix = x, pcx = x;
                    gotoXY(37, 40); cout << ">>" << endl;
                    while (1)
                    {
                        if (y != 44) ui.cur(x, y);
                        if (_kbhit())
                        {
                            char t = _getch();
                            if (t == 9)
                            {
                                if (y == 40)
                                {
                                    x = pcx; y = 42;
                                    gotoXY(37, 42);
                                    cout << ">>" << endl;
                                    gotoXY(37, 40);
                                    cout << "  " << endl;
                                }
                                else if (y == 42)
                                {
                                    y = 44;
                                    gotoXY(37, 44);
                                    cout << ">>" << endl;
                                    gotoXY(37, 42);
                                    cout << "  " << endl;
                                }
                                else if (y == 44)
                                {
                                    x = ix; y = 40;
                                    gotoXY(37, 40);
                                    cout << ">>" << endl;
                                    gotoXY(37, 44);
                                    cout << "  " << endl;
                                }
                                continue;
                            }
                            else if (t != 8 && x >= 65 && x <= 117 && y == 40)
                            {
                                //type password
                                gotoXY(x, y);
                                CurPass.push_back(t);
                                putchar(t);
                                x++;
                                pcx = x;
                            }
                            else if (CurPass != "" && Id != "" && t == 13 && y == 44)
                            {
                                //send to sv
                                if (stoi(Id) < 10)
                                    Id = "0" + Id;
                                if (stoi(CurPass) < 10)
                                    CurPass = "0" + CurPass;
                                string packet = "atk:" + Id + CurPass;
                                if (getaccess())
                                {
                                    packet = Encryption::Encrypt(packet);
                                    packet = "1" + packet;
                                }

                               send(network->ClientSocket, packet.c_str(), (int)strlen(packet.c_str()), 0);
                               break;
                               //
                               // data_length1 = network->Receive(network_data);
                               // sig = string(network_data, 0, data_length1);
                               //
                               // sig = Encryption::Decrypt(sig);
                               // smap = sig;
                               // break;
                            }  
                            else if (t != 8 && x >= 65 && x <= 117 && y == 42)
                            {
                                //type id
                                gotoXY(x, y);
                                putchar(t);
                                Id.push_back(t);
                                x++;
                                ix = x;
                            }
                            else if (t == 8 && x > 65 && x <= 117)
                            {
                                //clear
                                gotoXY(x, y);
                                putchar(' ');
                                CurPass.erase(CurPass.length() - 1);
                                x--;
                                pcx = x;
                            }
                            else if (t == 8 && x > 65 && x <= 117)
                            {
                                gotoXY(x, y);
                                putchar(' ');
                                Id.erase(Id.length() - 1);
                                x--;
                                ix = x;
                            }
                        }
                    }
                }
                else if (sig == "Win") {
                    gotoXY(105, 40);
                    cout << "You Win";
                    Sleep(500);
                    setsignal("UiClient");
                    return;
                }
                else if (sig == "Lose") {
                    gotoXY(105, 40);
                    cout << "You lose";
                    Sleep(500);
                    setsignal("UiClient");
                    return;
                }
                else if (sig != "Your turn!") {
                    smap = sig;
                    break;
                }
            }
        }
    }
}

vector<vector<int>> ClientGame::updatemap(string s)
{
    vector<vector<int>> list;           
    int pos = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            string temp = s.substr(pos, 1);
            int num = -1;
            try
            {
                num = stoi(temp);
            }

            catch (const std::exception&)
            {
                return list;
            }

            list[i][j] = num;
            pos++;
        }
    }
    return list;
}
