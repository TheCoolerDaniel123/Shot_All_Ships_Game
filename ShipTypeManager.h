#pragma once
#ifndef SHIPTYPEMANAGER_H_INCLUDED
#define SHIPTYPEMANAGER_H_INCLUDED


struct ShipInfo;

class ShipTypeManager
{
    public:
        ShipTypeManager(string filename);
        ~ShipTypeManager();
        void LoadAll();
        void LoadShip(ifstream&, int, int, int, int, int, int, int, int, int, int, int, int, int);
        ShipInfo* GetShipInfo(int key){return ShipStats.at(key);}
    private:
        map<int, ShipInfo*> ShipStats;
        const int keys_amount = 9;
        int* keys;
        string Filename;
};

struct ShipInfo
{
    ShipInfo(){}
    ~ShipInfo(){}
    int Speed, Drawning_speed;
    int SourceHeigth, SourceWidth;
    int SourceX, SourceY;
    int DestConstDiv, DestConstPlus;
    int DrawningTexAmount;
    int IndexDrawnRight, IndexDrawnLeft;
    SDL_Texture* All_textures[12];
 };

#endif // SHIPTYPEMANAGER_H_INCLUDED
