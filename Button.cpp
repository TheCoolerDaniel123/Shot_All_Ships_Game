#include "Files_n_libs.h"

extern Game* game;
extern RecordManager* RecordList;
extern NameInputManager* NameInput;

SDL_Texture* Button::Default_look = NULL;
SDL_Texture* Button::Lighted_look = NULL;

Button::Button(int type, int SourceH, int SourceW, int SourceX, int SourceY)
{
    ObjTex = Default_look;

    srcR.h = SourceH;
    srcR.w = SourceW;
    srcR.x = SourceX;
    srcR.y = SourceY;

    destR.h = game->GetHeight() / 10; //установка размера и расположения кнопок
    if(type == INF) destR.w = destR.h;
    else destR.w = game->GetWidth() / 5.5;

    switch(type)
    {
        case PLAY: destR.x = game->GetWidth() / 2 - destR.w - 5;
                   destR.y = game->GetHeight() - destR.h - 10;
                   break;
        case INF: destR.x = game->GetWidth() - game->GetWidth() / 5.5 - destR.w - 10;
                  destR.y = 5;
                  break;
        case RECS: destR.x = game->GetWidth() / 2 + 5;
                   destR.y = game->GetHeight() - destR.h - 10;
                   break;
        case NAME: destR.x = 5;
                   destR.y = 5;
                   break;
        case EXIT: destR.x = game->GetWidth() - destR.w - 5;
                   destR.y = 5;
                   break;
    }
    Type = type;
}

Button::~Button()
{
    if(Default_look) SDL_DestroyTexture(Default_look);
    Default_look = NULL;
    if(Lighted_look) SDL_DestroyTexture(Lighted_look);
    Lighted_look = NULL;
}

void Button::Press()
{
    switch(Type)
    {
        case PLAY: game->SetState(Game::SHOW_LVL); SDL_ShowCursor(false); game->SetShipStart(SDL_GetTicks()); break;
        case INF: game->SetState(Game::INFO); break;
        case RECS: game->SetState(Game::RECORDS); RecordList->ConvertCurRecs(); break;
        case NAME: game->SetState(Game::ENTERING_NAME);
                   break;
        case EXIT: if(game->GetState() == Game::MENU)
                   {
                        RecordList->StoreCurRecs();
                        game->SetRunning(false);
                   }
                   game->SetState(Game::MENU);
                   break;
    }
}

void Button::Update()
{
    if(game->MouseOn(&destR)) ObjTex = Lighted_look;
    else ObjTex = Default_look;
}
