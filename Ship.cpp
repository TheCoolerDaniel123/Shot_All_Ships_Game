#include "Files_n_libs.h"

extern Submarine* submarine;
extern Game* game;

SDL_Texture* Ship::Waves = NULL;
SDL_Texture* Ship::rWaves = NULL;

Ship::Ship(ShipInfo* CurStats, int X, int Y, int Line, int IsRev)
{
    Info = CurStats;

    x = X;
    y = Y;

    rev = IsRev;
    ObjTex = Info->All_textures[IsRev];
    if(!ObjTex) cout << "[Ship Constructor] Texture troubles" << endl;

    if(IsRev) CurWaves = rWaves;
    else CurWaves = Waves;

    destroyed = false;
    Wave_condition_delay = 130;

    line = Line;

    switch (Info->Speed)
    {
        case 2: points = 1; break;
        case 4: points = 2; break;
        case 6: points = 3; break;
        default: cout << "[Ship constructor] Did you add a new speed? Set points for it please" << endl; points = 1;
    }

    srcR.h = Info->SourceHeigth;
    srcR.w = Info->SourceWidth;
    srcR.x = Info->SourceX;
    srcR.y = Info->SourceY;

    destR.h = srcR.h / (game->GetHeight() / Info->DestConstDiv);
    destR.w = srcR.w / (game->GetWidth() / Info->DestConstDiv) + Info->DestConstPlus;

    WavesSRect[0].x = WavesSRect[1].x = WavesSRect[2].x = 0;
    WavesSRect[0].y = 0; WavesSRect[0].h = 92; WavesSRect[0].w = 459;
    WavesSRect[1].y = 168; WavesSRect[1].h = 267 - 175; WavesSRect[1].w = 459;
    WavesSRect[2].y = 348; WavesSRect[2].h = 466 - 357; WavesSRect[2].w = 459;

    WavesDRect.h = game->GetHeight() / 35; WavesDRect.w = destR.w + 10;
    WavesDRect.y = y + destR.h - WavesDRect.h * 0.5;
    WhichWave = 0;
}

void Ship::Update()
{
    destR.x = x;
    destR.y = y;

    WavesDRect.x = x - 5;

    Hitbox.x = destR.x; Hitbox.y = destR.y + destR.h - 10; //обновление хитбокса
    Hitbox.w = destR.w; Hitbox.h = 10;

    if(submarine->GetShot() && SDL_HasIntersection(&Hitbox, submarine->GetHitbox())) //если в корабль попал снаряд
    {
        destroyed = true;
        //cout<<"Ship: begin = ("<<destR.x<<", "<<destR.y<<"); end = ("<<destR.x + destR.w<<", "<<destR.y + destR.h<<")\n";
        return;
    }
    if(x < game->GetWidth() - 20 && !rev) x += Info->Speed; //реализация поворота корабля
    else
    {
        if(!rev)
        {
            rev = true;
            ObjTex = Info->All_textures[1];
            CurWaves = rWaves;
        }
        else if(x <= -80)
             {
                 rev = false;
                 ObjTex = Info->All_textures[0];
                 CurWaves = Waves;
             }
        x -= Info->Speed;
    }
    WhichWave = SDL_GetTicks() / Wave_condition_delay % 3;
}


void Ship::Render()
{
    //if(1) SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &Hitbox); else//проверка хитбоксов
    SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &destR);
    SDL_RenderCopy(Game::renderer, CurWaves, &WavesSRect[WhichWave], &WavesDRect);
}
