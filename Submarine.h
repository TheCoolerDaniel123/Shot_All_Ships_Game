#pragma once
#ifndef SUBMARINE_H_INCLUDED
#define SUBMARINE_H_INCLUDED

#include "Torpedo.h"

class Submarine: public GameObject
{
    public:
        Submarine(int X, int Y);
        ~Submarine();
        void Update() override;
        void Render() override;
        bool Shoot();
        void SetMove(char side, int condition);

        bool GetShot(){return shot;}
        void SetShot(bool condition){shot = condition;}

        SDL_Rect* GetHitbox(){return MissileHitbox;}
    private:
        Torpedo* missile;
        SDL_Rect* MissileHitbox;
        int Condition_delay;
        bool moveLeft, moveRight;
        bool shot;

        SDL_Rect SUnderWaterRect[3];
        SDL_Rect UnderWaterDrect;
        SDL_Texture* CurWaves;
        int WhichWave;
};

#endif // SUBMARINE_H_INCLUDED
