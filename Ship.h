#pragma once
#ifndef SHIP_H_INCLUDED
#define SHIP_H_INCLUDED

class Ship: public GameObject
{
    public:
        Ship(ShipInfo* CurStats, int X, int Y, int Line, int IsRev);
        ~Ship(){}
        void Update() override;
        void Render() override;
        bool IsDestroyed(){return destroyed;}
        int GetPoints(){return points;}
        bool GetRev(){return rev;}
        int GetLine(){return line;}
        ShipInfo* GetInfo(){return Info;}

        static SDL_Texture* Waves;
        static SDL_Texture* rWaves;
    protected:
        SDL_Texture* CurWaves;
        SDL_Rect WavesDRect, WavesSRect[3];
        int WhichWave;

        int line, points;
        bool rev, destroyed;
        ShipInfo* Info;
        int Wave_condition_delay;
        SDL_Rect Hitbox;
};

#endif // SHIP_H_INCLUDED
