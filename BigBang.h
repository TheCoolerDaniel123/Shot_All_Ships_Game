#pragma once
#ifndef BIGBANG_H_INCLUDED
#define BIGBANG_H_INCLUDED

class BigBang: public GameObject
{
    public:
        BigBang(ShipInfo* CurStats, int X, int Y, bool rev, SDL_Rect* Explosion);
        ~BigBang(){}
        void Update() override;
        void Render() override;
        SDL_Texture* SetTex(Uint32 Cur_delay, bool Ship_or_Bang_flag);
        bool IsDeleted (){return deleted;}

        Uint32 GetStart(){return Drawning_start;}
        void SetStart(Uint32 value){Drawning_start = value;}

        static SDL_Texture** BangTexes;
    private:
        int StartIndex, Drawn_condition, HeigthToCut;
        Uint32 Drawning_start, Condition_delay, Ticks_to_wait, Drawning_length;
        bool deleted;
        SDL_Texture* Bang;
        ShipInfo* Info;
        SDL_Rect BangRect, BangSource;
};

#endif // BIGBANG_H_INCLUDED

