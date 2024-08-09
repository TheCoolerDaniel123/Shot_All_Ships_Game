#include "Files_n_libs.h"

extern Game* game;

Torpedo::Torpedo(const char* texture)
{
    ObjTex = IMG_LoadTexture(Game::renderer, texture);
    if(!ObjTex) cout << IMG_GetError() << endl;
    else cout << "Torpedo's texture was made" << endl;

    srcR.h = 980;
    srcR.w = 330;
    srcR.x = 280;
    srcR.y = 0;

    destR.h = game->GetHeight() / (game->GetHeight() / 40);
    destR.w = game->GetWidth() / (game->GetWidth() / 15);
}

Torpedo::~Torpedo()
{
    if(ObjTex) SDL_DestroyTexture(ObjTex);
}

void Torpedo::Update()
{
    y -= 6;

    destR.x = x;
    destR.y = y;

    Hitbox = destR; //обновление хитбокса снаряда
    Hitbox.h = 10; Hitbox.w = 15;
    Hitbox.y += 10;
}

/*void Torpedo::Render()//проверка хитбоксов
{
    SDL_RenderCopy(Game::renderer, ObjTex, &srcR, &Hitbox);
}*/
