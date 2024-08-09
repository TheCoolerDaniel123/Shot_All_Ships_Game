#include "Files_n_libs.h"

ShipTypeManager::ShipTypeManager(string filename)
{
    //keys = new int[keys_amount] {11, 12, 13, 21, 22, 23, 31, 32, 33}; //ключи для доступа к информации о типе кораблей
    keys = new int[keys_amount] {10, 20, 30};
    Filename = filename;
}

ShipTypeManager::~ShipTypeManager()
{
    int i;
    for (i = 0; i < keys_amount; i++)
        if(ShipStats[keys[i]])
            delete ShipStats[keys[i]];
    if(keys) delete[] keys;
}

void ShipTypeManager::LoadAll()
{
    int i, tex_amount, n = 0;
    const int Drawning_speed = 3, Speed1 = 2, Speed2 = 4, Speed3 = 6;
    for(i = 0; i < keys_amount; i++) ShipStats[keys[i]] = NULL; //инициализация словаря
    ifstream stream;
    stream.open(Filename);
    if(!stream.is_open()) cout << "[ShipTypeManager LoadAll] Troubles with opening file (" << Filename << ")" << endl;
    ///подгрузка текстур и их размеров для работы с ними
    /**Ship 1.1**/
    stream >> tex_amount; stream.get();
    LoadShip(stream, keys[n++], Speed1, Drawning_speed, 490, 1490, 39, 89, 60, -10, 4, 2, 6, tex_amount);

    /**Ship 1.2**/
    stream >> tex_amount; stream.get();
    //LoadShip(stream, keys[n++], /*Speed1*/Speed2, Drawning_speed, /*745, 1832, 0, 0*/404, 1758, 93, 117, 55, -5, 5, 2, 7, tex_amount);
    LoadShip(stream, keys[n++], /*Speed1*/Speed2, Drawning_speed, 496, 1726, 0, 0, 52, 5, 4, 2, 6, tex_amount);

    /**Ship 1.3**/
    stream >> tex_amount; stream.get();
    LoadShip(stream, keys[n++], /*Speed1*/Speed3, Drawning_speed, 569, 732, 2, 0, 70, 15, 4, 2, 6, tex_amount);

    /**Ship 2.1**/
    /*stream >> tex_amount; stream.get();
    LoadShip(stream, keys[n++], Speed2, Drawning_speed, 1080, 1920, 2, 0, 40, 15, 4, 2, 6, tex_amount);*/

    stream.close();
}

void ShipTypeManager::LoadShip(ifstream &stream, int key, int spd, int spdD, int sH, int sW, int sX, int sY, int div, int pls, int N, int idR, int idL, int tex_amount)
{
    int i;
    string CurTexName;
    ShipStats.at(key) = new ShipInfo;
    ShipStats.at(key)->Speed = spd;
    ShipStats.at(key)->Drawning_speed = spdD;
    ShipStats.at(key)->SourceHeigth = sH;
    ShipStats.at(key)->SourceWidth = sW;
    ShipStats.at(key)->SourceX = sX;
    ShipStats.at(key)->SourceY = sY;
    ShipStats.at(key)->DestConstDiv = div;
    ShipStats.at(key)->DestConstPlus = pls;
    ShipStats.at(key)->DrawningTexAmount = N;
    ShipStats.at(key)->IndexDrawnRight = idR;
    ShipStats.at(key)->IndexDrawnLeft = idL; //cout << tex_amount << endl;
    for (i = 0; i < tex_amount; i++)
    {
        getline(stream, CurTexName);//cout << CurTexName << endl;
        ShipStats.at(key)->All_textures[i] = IMG_LoadTexture(Game::renderer, &CurTexName[0]);
    }
    cout << IMG_GetError() << endl;
}
