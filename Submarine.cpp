#include "Files_n_libs.h"

extern Game* game;

Submarine::Submarine(int X, int Y)
{
    ObjTex = IMG_LoadTexture(Game::renderer,"images/Submarine.png");
    if(!ObjTex)
        cout << IMG_GetError() << endl;
    else
        cout << "Submarine's texture was made" << endl;
    x = X;
    y = Y;
    Condition_delay = 115; //задержка между сменой текстур волн
    missile = new Torpedo("images/Torpedo.png");
    shot = false;
    moveLeft = moveRight = false;

    srcR.h = 1024;
    srcR.w = 1024;
    srcR.x = 0;
    srcR.y = 0;

    destR.h = game->GetHeight() / (game->GetHeight() / 110);
    destR.w = game->GetWidth() / (game->GetWidth() / 110);
    destR.x = x;
    destR.y = y;

    SUnderWaterRect[0].x = SUnderWaterRect[1].x = SUnderWaterRect[2].x = 0;
    SUnderWaterRect[0].y = 0; SUnderWaterRect[0].h = 92; SUnderWaterRect[0].w = 459;
    SUnderWaterRect[1].y = 168; SUnderWaterRect[1].h = 267 - 175; SUnderWaterRect[1].w = 459;
    SUnderWaterRect[2].y = 348; SUnderWaterRect[2].h = 466 - 357; SUnderWaterRect[2].w = 459;
    WhichWave = 0;
}

Submarine::~Submarine()
{
    if(missile) delete missile;
    if(ObjTex) SDL_DestroyTexture(ObjTex);
}

void Submarine::Update()
{
    if(shot) //если снаряд на карте
    {
        missile->Update();
        if (missile->GetY() < -30) shot = false; //снаряд улетает за карту
        MissileHitbox = missile->GetHitbox(); //получение информации о хитбоксе снаряда
    }

    destR.x = x;
    destR.y = y;

    if(moveLeft || moveRight)
    {
        if(moveLeft) CurWaves = Ship::rWaves;
        else CurWaves = Ship::Waves;
        WhichWave = SDL_GetTicks() / Condition_delay % 3;
        UnderWaterDrect = destR;
        UnderWaterDrect.h = destR.h * 0.2;
        UnderWaterDrect.y += destR.h / 2;
    }

    if (moveLeft && x > -40) x -= 2; //движение подлодки при соответсвующих действиях пользователя
    else if (moveRight && x < game->GetWidth() - 80) x += 2;
}

bool Submarine::Shoot(void)
{

    if(shot || moveLeft || moveRight) return false; //невозможность встрела при движении и имеющемся снаряде
    //cout << "Submarine shooted" << endl;
    missile->SetX(x + 10);
    missile->SetY(y);
    shot = true; //флаг выстрела
    return true;
}

void Submarine::SetMove(char side, int condition)
{
    switch (condition) //установка флагов движения в зависимости от действий пользователя
    {
        case 1: if(side == 'l' && !moveRight) moveLeft = true;
                else if(!moveLeft) moveRight = true;
                break;
        case 0: moveLeft = false;
                moveRight = false;
                break;
    }
}


void Submarine::Render()
{
    if(shot) missile->Render();
    if(moveLeft || moveRight) SDL_RenderCopy(Game::renderer, CurWaves, &SUnderWaterRect[WhichWave], &UnderWaterDrect);
    else SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &destR);
}
