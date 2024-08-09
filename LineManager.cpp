#include "Files_n_libs.h"

extern Game* game;

LineManager::LineManager(int lines_amount)
{
    Lines_amount = lines_amount;
    FreeLines = new queue<int>();
    MakeAllAvailable();
}

LineManager::~LineManager()
{
    delete FreeLines;
}

Ship* LineManager::Add_ship(int line, ShipInfo* CurStats, int IsRev) //добавление корабля на свободную линию
{
    if (line == -1)
        return NULL;
    int Xpos, Ypos;
    switch (line)
    {
        case 0: Ypos = 1; break;
        case 1: Ypos = 61; break;
        case 2: Ypos = 121; break;
        default: cout << "[Line Manager] Don't forget to add new line Ypos" << endl;
    }
    if(IsRev)
        Xpos = game->GetWidth() - 20;
    else
        Xpos = -80;
    Ship* ship;
    ship = new Ship(CurStats, Xpos, Ypos, line, IsRev); //непосредственное создание объекта
    UpdateLine(line, false);
    return ship;
}

int LineManager::FindFreeLine() //поиск линии для добавления корабля
{
    if(FreeLines->empty())
        return -1;
    return FreeLines->front();
}

void LineManager::UpdateLine(int line, bool flag)
{
    if(!flag)
        FreeLines->pop();
    else
        FreeLines->push(line);
}

void LineManager::MakeAllAvailable()
{
    int i;
    queue<int> empty;
    swap(*FreeLines, empty);
    for(i = 0; i < Lines_amount; i++)
        FreeLines->push(i);
}
