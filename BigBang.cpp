#include "Files_n_libs.h"

extern Game* game;

SDL_Texture** BigBang::BangTexes = new SDL_Texture*[2];

BigBang::BigBang(ShipInfo* CurStats, int X, int Y, bool rev, SDL_Rect* Explosion)
{
    x = X;
    y = Y;
    Info = CurStats; //параметры текстур тонущего корабля

    Drawning_start = SDL_GetTicks();
    Drawn_condition = 0;

    if (rev) StartIndex = Info->IndexDrawnLeft;
    else StartIndex = Info->IndexDrawnRight;

    ObjTex = Info->All_textures[StartIndex];
    Condition_delay = Ticks_to_wait = 150; //задержка между стадиями тонущего корабля
    Drawning_length = 900; //сколько тонет корабль по времени
    deleted = false;

    BangRect = *Explosion; //место отображения взрыва
    BangRect.h = BangRect.w = 30;
    BangSource.x = BangSource.y = 100;
    BangSource.h = BangSource.w = 1000;
    Bang = BangTexes[0];

    srcR.h = HeigthToCut = Info->SourceHeigth;
    srcR.w = Info->SourceWidth;
    srcR.x = Info->SourceX;
    srcR.y = Info->SourceY;

    destR.h = srcR.h / (game->GetHeight() / Info->DestConstDiv);
    destR.w = srcR.w / (game->GetWidth() / Info->DestConstDiv) + Info->DestConstPlus;
}

void BigBang::Update()
{
    y++;

    srcR.h = HeigthToCut -= Info->Drawning_speed; //уменьшение высоты текстуры (типа погружение в воду)
    destR.h = srcR.h / (game->GetHeight() / Info->DestConstDiv);

    destR.x = x;
    destR.y = y;
    Uint32 Time_after_start = SDL_GetTicks() - Drawning_start; //время для отображения соотвествующей текстуры
    if (Time_after_start <= Drawning_length) //если корабль еще тонет
    {
        if (Ticks_to_wait < Time_after_start) {Ticks_to_wait += Condition_delay; Drawn_condition++;}
        ObjTex = SetTex(Time_after_start, false);
        Bang = SetTex(Time_after_start, true);
    }
    else deleted = true; //если уже потонул, объект удаляется в основной функции апдейта
}

SDL_Texture* BigBang::SetTex(Uint32 Cur_delay, bool Explosion)
{
    if(!Explosion) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
    /*{
        if (Cur_delay <= Condition_delay) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 2) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 3) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 4) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 5) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 6) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 7) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 8) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        if (Cur_delay <= Condition_delay * 9) return Info->All_textures[StartIndex + (Drawn_condition % Info->DrawningTexAmount)];
        cout << "[BigBang SetTex] Unexpected delay " << Cur_delay << endl; return NULL;
    }*/
    else
    {
        if (Cur_delay <= /*Condition_delay*/100) return BangTexes[0];
        if (Cur_delay <= /*Condition_delay * 2*/200)
        {
            BangRect.h = BangRect.w = 40;
            //BangRect.x--; BangRect.y--;
            return BangTexes[1];
        }
        if (Cur_delay <= /*Condition_delay * 3*/300)
        {
            BangRect.h = BangRect.w = 35;
            //BangRect.x++; BangRect.y++;
            return BangTexes[1];
        }
        /*if (Cur_delay <= Condition_delay * 4)
        {
            BangRect.h = BangRect.w = 30;
            //BangRect.x++; BangRect.y++;
            return BangTexes[1];
        }*/
        return NULL;
    }
}

void BigBang::Render()
{
    SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &destR);
    if(Bang) SDL_RenderCopy(Game::renderer, Bang, &BangSource, &BangRect);
}
