#include "Files_n_libs.h"

extern Game* game;

Background::Background (const char* texture, int SourceH, int SourceW, int SourceX, int SourceY)
{
    ObjTex = IMG_LoadTexture(Game::renderer, texture);
    if(!ObjTex)
        cout << IMG_GetError() << endl;
    else
        cout << "Background's texture was made" << endl;
    destR.x = destR.y = 0;
    srcR.h = SourceH;
    srcR.w = SourceW;
    srcR.x = SourceX;
    srcR.y = SourceY;

    destR.h = game->GetHeight();
    destR.w = game->GetWidth();
}

Background::~Background()
{
    if(ObjTex) SDL_DestroyTexture(ObjTex);
}
