#pragma once
#ifndef RECORDMANAGER_H_INCLUDED
#define RECORDMANAGER_H_INCLUDED

struct GameData;

class RecordManager
{
    public:
        RecordManager(string filename);
        ~RecordManager();
        void LoadCurRecs();
        void ConvertCurRecs();
        void StoreCurRecs();
        void DeleteRecTexes();
        void AddData(GameData);
        void RenderRecs();
    private:
        string Filename;
        vector<GameData> CurRecData;
        UIText** CurRecords; //10
        bool Converted;
};

struct GameData
{
    string Name;
    int Points;
};

#endif // RECORDMANAGER_H_INCLUDED
